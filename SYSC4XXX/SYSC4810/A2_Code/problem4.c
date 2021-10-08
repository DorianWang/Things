
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/bn.h>

#include "printBN.h"

BIGNUM* string_to_BN(const char* str_in)
{
   /* A character is 8 bits, 2 hex digits is 8 bits.
   // Could probably have used BN_bn2bin if the hex wasn't required?
   // Not too sure if endianess could affect this, but it works on the VM.
   */
   size_t i;
   const size_t length = strlen(str_in);
   char hex_str[length * 2 + 1]; /* The required size for the hex string. */
   BIGNUM* m = BN_new();
   for (i = 0; i < length; i++){
      sprintf(hex_str + i * 2, "%02hhX", str_in[i]);
   }
   BN_hex2bn(&m, hex_str);
   return m;
}

BIGNUM* RSA_encrypt_ascii(const char* str_in, const BIGNUM* n, const BIGNUM* e, BN_CTX* ctx)
{
   BIGNUM* cy = BN_new();
   BIGNUM* m = string_to_BN(str_in);
   /* n must be bigger than the message, otherwise you would need to chunk it.
   // That's too much work, so I'll just not do that.
   // This just returns m^e % n. I don't know a good way to do padding so this system
   // would be vulnerable to a chosen plaintext attack by just guessing combinations of
   // scan type + result using the public key.
   */
   if (BN_cmp(n, m) > 0){
      BN_mod_exp(cy, m, e, n, ctx);
   }
   else{
      assert(0); /* Halt and catch fire? */
   }
   BN_free(m);
   return cy;
}

int main()
{
   BN_CTX* num_factory = BN_CTX_new();
   BIGNUM* n2 = BN_new(); BN_hex2bn(&n2, "57F4953D30DBD5BBB1A4CA66F9F8A014D1D77AE33E07A1959C2D6AC4F0C1EDD");
   BIGNUM* e2 = BN_new(); BN_hex2bn(&e2, "24DB1");
   const char* message = "John Doe: Brain MRI -> Clear";
   BIGNUM* cyphertext = RSA_encrypt_ascii(message, n2, e2, num_factory);
   printBN("The cyphertext is:", cyphertext);
   return 0;
}



