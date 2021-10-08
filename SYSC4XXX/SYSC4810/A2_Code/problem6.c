
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/bn.h>

#include "printBN.h"


BIGNUM* RSA_signiture_ascii(const char* str_in, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx)
{
   /* Converts the message into a number m, then returns m to the power of the private key % n */
   size_t output_size;
   BIGNUM* m = BN_new(); BIGNUM* res = BN_new();
   BN_bin2bn((const unsigned char*) str_in, strlen(str_in), m);
   BN_mod_exp(res, m, pk, n, ctx);
   BN_free(m);
   return res;
}

int main()
{
   BN_CTX* num_factory = BN_CTX_new();
   BIGNUM* res;
   BIGNUM* n2 = BN_new(); BN_hex2bn(&n2, "57F4953D30DBD5BBB1A4CA66F9F8A014D1D77AE33E07A1959C2D6AC4F0C1EDD");
   BIGNUM* d2 = BN_new(); BN_hex2bn(&d2, "04D9C2FF79A74783969D14C8DE260B4B98E98F30A71E92AF2D03432F88047951");


   res = RSA_signiture_ascii("Ben Rice: Knee MRI -> MCL tear", d2, n2, num_factory);
   printf("Message 1: %s\n", "Ben Rice: Knee MRI -> MCL tear");
   printf("Message 2: %s\n", "Ben Rice: Knee MRI -> LCL tear");
   printBN("Signiture 1:", res);
   BN_free(res);
   res = RSA_signiture_ascii("Ben Rice: Knee MRI -> LCL tear", d2, n2, num_factory);
   printBN("Signiture 2:", res);
   BN_free(res);

   return 0;
}






