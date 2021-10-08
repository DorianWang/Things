
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/bn.h>

#include "printBN.h"

static BIGNUM* string_to_BN(const char* str_in)
{
   /* A character is 8 bits, 2 hex digits is 8 bits.
   // Could probably have used BN_bn2bin if the hex wasn't required?
   */
   size_t i;
   const size_t length = strlen(str_in);
   char hex_str[length * 2 + 1]; // The required size for the hex string.
   BIGNUM* m = BN_new();
   for (i = 0; i < length; i++){
      sprintf(hex_str + i * 2, "%02hhX", str_in[i]);
   }
   BN_hex2bn(&m, hex_str);
   return m;
}

/* Returns 0 if signiture is the same, non-zero otherwise */
int RSA_verify_signiture_ascii(const char* str_in, const BIGNUM* s, const BIGNUM* e, const BIGNUM* n, BN_CTX* ctx)
{
   BIGNUM* m_val = BN_new();
   BIGNUM* str_val = string_to_BN(str_in);
   int ret_val;
   BN_mod_exp(m_val, s, e, n, ctx);
   printBN_hex("message val is  :", m_val);
   printBN_hex("signiture val is:", str_val);
   ret_val = BN_cmp(m_val, str_val);
   BN_free(m_val); BN_free(str_val);
   return ret_val;
}

int main()
{
   BN_CTX* num_factory = BN_CTX_new();
   const char* message2 = "Lisa McKay: Brain CT -> Benign";
   BIGNUM* sig = BN_new(); BN_hex2bn(&sig, "54AAB2D38593EB85BE29B4D084B348F36E8099DDE34A8FE57CE5011F2AF20007");
   BIGNUM* e3 = BN_new(); BN_hex2bn(&e3, "A223F");
   BIGNUM* n3 = BN_new(); BN_hex2bn(&n3, "BBE2F8AE1FFB0B44C55B9AEB8E55FE300A34590FC19D06485C31563421281819");

   RSA_verify_signiture_ascii(message2, sig, e3, n3, num_factory);
   BN_add_word(sig, 1);
   RSA_verify_signiture_ascii(message2, sig, e3, n3, num_factory);
}








