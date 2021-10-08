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

   BIGNUM* v1 = BN_new(); BN_hex2bn(&v1, "0239274D6A98200211E9DC33B144A8E2762E8EF9766C04DD5D101BBF24911F37");
   BIGNUM* v2 = BN_new(); BN_hex2bn(&v2, "03225C97EE592D714AE940132B0154550C0A9F062F8D823BF029C59DAA78657C");
   BIGNUM* v3 = BN_new(); BN_hex2bn(&v3, "05357D560B0BA2C2EC0A691A1B216578834E4B63D965D01C85B90508037E5289");
   BIGNUM* v4 = BN_new(); BN_hex2bn(&v4, "589AA5406A6EC70E277A1DAAA38F05A7229A13E00B8EBCB27510DECB065045");
   BIGNUM* v5 = BN_new(); BN_hex2bn(&v5, "029B985CB8A13E7BD01B2AC1412B21BC534E49A8722B0C1903C9327F648B4E5C");

   BN_sub(v1, v2, v1); BN_sub(v2, v3, v2); BN_sub(v3, v4, v3); BN_sub(v4, v5, v4);
   printBN("v1:", v1); printBN("v2:", v2); printBN("v3:", v3); printBN("v4:", v4);



   return 0;
}





