#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <openssl/bn.h>


#include "BN_utilities.h"
/*
// Many thanks to the nerds that post math stuff on the internet
*/

int main()
{
   BN_CTX* num_factory = BN_CTX_new();
   BIGNUM* res;

   BIGNUM* private_key = RSA_private_key_from_values("CF751B1FA7FD450223CD96CDABE96AC7",
                                                     "FD8F0F9D611E28DD688447C0E9A0CDBD",
                                                     "41AC7", num_factory);
   assert(private_key != NULL);
   printBN("The private key d would be:", private_key);

   BIGNUM* n2 = BN_new(); BN_hex2bn(&n2, "57F4953D30DBD5BBB1A4CA66F9F8A014D1D77AE33E07A1959C2D6AC4F0C1EDD");
   BIGNUM* e2 = BN_new(); BN_hex2bn(&e2, "24DB1");
   BIGNUM* d2 = BN_new(); BN_hex2bn(&d2, "04D9C2FF79A74783969D14C8DE260B4B98E98F30A71E92AF2D03432F88047951");
   BIGNUM* cy2 = BN_new(); BN_hex2bn(&cy2, "019D18EA69AB6937E418B8AD5F9A2553B46CFE1A1064F06E4144A8EC67A6B3B0");
   const char* message = "John Doe: Brain MRI -> Clear";
   BIGNUM* cyphertext = RSA_encrypt_ascii(message, n2, e2, num_factory);
   printBN("The cyphered value would be:", cyphertext);
   RSA_decrypt_ascii(cyphertext, d2, n2, num_factory);
   RSA_decrypt_ascii(cy2, d2, n2, num_factory);

   res = RSA_signiture_ascii("Ben Rice: Knee MRI -> MCL tear", d2, n2, num_factory);
   printBN("Signiture 1:", res);
   BN_free(res);
   res = RSA_signiture_ascii("Ben Rice: Knee MRI -> LCL tear", d2, n2, num_factory);
   printBN("Signiture 2:", res);
   BN_free(res);

   const char* message2 = "Lisa McKay: Brain CT -> Benign";
   BIGNUM* sig = BN_new(); BN_hex2bn(&sig, "54AAB2D38593EB85BE29B4D084B348F36E8099DDE34A8FE57CE5011F2AF20007");
   BIGNUM* e3 = BN_new(); BN_hex2bn(&e3, "A223F");
   BIGNUM* n3 = BN_new(); BN_hex2bn(&n3, "BBE2F8AE1FFB0B44C55B9AEB8E55FE300A34590FC19D06485C31563421281819");

   RSA_verify_signiture_ascii(message2, sig, e3, n3, num_factory);
   BN_add_word(sig, 1);
   RSA_verify_signiture_ascii(message2, sig, e3, n3, num_factory);



   return 0;
}





