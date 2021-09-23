#include "BN_utilities.h"

void printBN(char* msg, BIGNUM* val)
{
   char* number_str = BN_bn2dec(val);
   printf("%s %s\n", msg, number_str);
   OPENSSL_free(number_str);
}


void printBN_hex(char* msg, BIGNUM* val)
{
   char* number_str = BN_bn2hex(val);
   printf("%s %s\n", msg, number_str);
   OPENSSL_free(number_str);
}


BIGNUM* RSA_encrypt_ascii(const char* str_in, const BIGNUM* n, const BIGNUM* e, BN_CTX* ctx)
{
   /* A character is 8 bits, 2 hex digits is 8 bits.
   // Could probably have used BN_bn2bin if the hex wasn't required?
   */
   size_t i;
   const size_t length = strlen(str_in);
   char hex_str[length * 2 + 1]; // The required size for the hex string.
   BIGNUM* m = BN_new();
   BIGNUM* cy = BN_new();

   for (i = 0; i < length; i++){
      sprintf(hex_str + i * 2, "%02hhX", str_in[i]);
   }
   printf("%s\n", hex_str);
   BN_hex2bn(&m, hex_str);
   if (BN_cmp(n, m)){ // n must be bigger than the message, otherwise you would need to chunk it.
      BN_mod_exp(cy, m, e, n, ctx);
   }

   BN_free(m);
   return cy;
}


void RSA_decrypt_ascii(const BIGNUM* cy, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx)
{
   size_t output_size;
   BIGNUM* m = BN_new();
   BN_mod_exp(m, cy, pk, n, ctx);
   output_size = BN_num_bytes(m);
   output_size++;
   char str_out[output_size];
   str_out[output_size - 1] = '\0';
   BN_bn2bin(m, str_out); // Could have issues with endianness? Should work on the target machine.
   printf("The decoded string is: %s\n", str_out);
}

BIGNUM* RSA_signiture_ascii(const char* str_in, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx)
{
   size_t output_size;
   BIGNUM* m = BN_new(); BIGNUM* res = BN_new();
   BN_bin2bn(str_in, strlen(str_in), m);
   BN_mod_exp(res, m, pk, n, ctx);
   BN_free(m);
   output_size = BN_num_bytes(res);
   output_size++;
   char str_out[output_size];
   str_out[output_size - 1] = '\0';
   BN_bn2bin(res, str_out); // Could have issues with endianness? Should work on the target machine.
   printf("The string signiture is: %s\n", str_out);

   return res;
}







