
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/bn.h>

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

void RSA_decrypt_ascii(const BIGNUM* cy, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx)
{
   size_t output_size;
   BIGNUM* m = BN_new();
   /* m = cy^pk % n, where pk is the private key */
   BN_mod_exp(m, cy, pk, n, ctx);
   output_size = BN_num_bytes(m);
   output_size++;
   unsigned char str_out[output_size];
   str_out[output_size - 1] = '\0';
   BN_bn2bin(m, str_out); /* Should work on the target machine. */
   printf("The decoded string is: %s\n", str_out);
}

int main()
{
   BN_CTX* num_factory = BN_CTX_new();
   BIGNUM* n2 = BN_new(); BN_hex2bn(&n2, "57F4953D30DBD5BBB1A4CA66F9F8A014D1D77AE33E07A1959C2D6AC4F0C1EDD");
   BIGNUM* d2 = BN_new(); BN_hex2bn(&d2, "04D9C2FF79A74783969D14C8DE260B4B98E98F30A71E92AF2D03432F88047951");
   BIGNUM* cy2 = BN_new(); BN_hex2bn(&cy2, "019D18EA69AB6937E418B8AD5F9A2553B46CFE1A1064F06E4144A8EC67A6B3B0");
   RSA_decrypt_ascii(cy2, d2, n2, num_factory);
   return 0;
}




