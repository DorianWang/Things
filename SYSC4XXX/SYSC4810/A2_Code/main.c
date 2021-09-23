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
   /* Init */
   BN_CTX* num_factory = BN_CTX_new();
   BIGNUM* p = BN_new();
   BIGNUM* q = BN_new();
   BIGNUM* n = BN_new();
   BIGNUM* e = BN_new();
   BIGNUM* car = BN_new();
   BIGNUM* res = BN_new();
   BIGNUM* res2 = BN_new();

   BN_hex2bn(&p, "CF751B1FA7FD450223CD96CDABE96AC7");
   BN_hex2bn(&q, "FD8F0F9D611E28DD688447C0E9A0CDBD");
   // BN_dec2bn(&p, "8191");
   // BN_dec2bn(&q, "131071");
   BN_mul(n, p, q, num_factory);
   printBN("The value of p:", p);
   printBN("The value of q:", q);

   printBN("The value of n:", n);
   BN_hex2bn(&e, "41AC7");
   // BN_dec2bn(&e, "7243");
   /* End init */

   printf("Time to die!\n");

   /* Carmichael
   // Equals lcm(p - 1, q - 1),
   // or (p - 1) * (q - 1) / gcd(p - 1, q - 1)
   // Assuming no errors to keep this simple and clear.
   */
   BN_sub_word(p, 1);
   BN_sub_word(q, 1);
   BN_gcd(res, p, q, num_factory);
   BN_mul(res2, p, q, num_factory);
   BN_div(car, NULL, res2, res, num_factory);
   printBN("The value of car:", car);

   /* Check if e is good, not strictly needed though */
   BN_gcd(res, e, car, num_factory);
   assert(BN_is_one(res));
   assert(BN_cmp(car, e) == 1); // car > e

   /* Free some variables that aren't used */
   BN_free(p); BN_free(q); BN_free(n);

   printf("Time to die!\n");

   /* Find d (private key) using extended euclidean algorithm
   // where (d * e) % car = 1, or d * e + car * x = 1
   // Time for lots of extra variables o_o
   // Note that res will be used for general calculations of r, s, t
   // while res2 will be used for the quotient.
   // swap_val is just a regular pointer to swap with.
   // extra_val is to hold the bumped off BIGNUMs to avoid too much memory allocation.
   */
   BIGNUM* swap_val = NULL; BIGNUM* extra_val = BN_new();
   BIGNUM* r_prev = BN_dup(car); BIGNUM* r_curr = BN_dup(e);
   BIGNUM* s_prev = BN_new(); BIGNUM* s_curr = BN_new();
   BIGNUM* t_prev = BN_new(); BIGNUM* t_curr = BN_new();
   BN_one(s_prev); BN_one(t_curr);
   size_t counter = 0; // Mostly for debugging, also avoid infinite loop.
   printBN("The value of r_prev is:", r_prev);
   printBN("The value of r_curr is:", r_curr);

   // Loop until the current remainder is 1
   while (!BN_is_one(r_curr) && counter++ < 20){
      /* previous remainders must be larger, r must trend to 0. */
      assert(BN_ucmp(r_prev, r_curr));

      /* Gets quotient, discards remainder. */
      BN_div(res2, NULL, r_prev, r_curr, num_factory);


      /* A complicated way of doing var(i) = var(i - 2) - var(i - 1) * quotient
      // All variables are swapped around so that curr is var(i) and prev is var(i-1)
      */
      BN_mul(res, res2, r_curr, num_factory);
      swap_val = r_prev;
      r_prev = r_curr;
      r_curr = extra_val;
      extra_val = swap_val;
      BN_sub(r_curr, extra_val, res);

      BN_mul(res, res2, s_curr, num_factory);
      swap_val = s_prev;
      s_prev = s_curr;
      s_curr = extra_val;
      extra_val = swap_val;
      BN_sub(s_curr, extra_val, res);

      BN_mul(res, res2, t_curr, num_factory);
      swap_val = t_prev;
      t_prev = t_curr;
      t_curr = extra_val;
      extra_val = swap_val;
      BN_sub(t_curr, extra_val, res);

      /*
      printf("The value of counter is %llu!\n", counter);
      printBN("The value of quot:", res2);
      printBN("The value of r_prev is:", r_prev);
      printBN("The value of r_curr is:", r_curr);
      printBN("The value of s_prev:", s_prev);
      printBN("The value of s_curr:", s_curr);
      printBN("The value of t_prev:", t_prev);
      printBN("The value of t_curr:", t_curr);
      getchar();
      */
   }

   /* When the remainder is 1 (equal to gcd), the coefficients of car and e are found.
   // d should equal to t_curr, but may be negative. If so, just bump it up.
   */

   if (BN_is_negative(t_curr)){
      BN_add(t_curr, t_curr, car);
   }

   BN_mul(res2, t_curr, e, num_factory);
   BN_mod(res, res2, car, num_factory);
   assert(BN_is_one(res));

   printBN_hex("The value of t_curr/d:", t_curr);

   BIGNUM* n2 = BN_new(); BN_hex2bn(&n2, "57F4953D30DBD5BBB1A4CA66F9F8A014D1D77AE33E07A1959C2D6AC4F0C1EDD");
   BIGNUM* e2 = BN_new(); BN_hex2bn(&e2, "24DB1");
   BIGNUM* d2 = BN_new(); BN_hex2bn(&d2, "04D9C2FF79A74783969D14C8DE260B4B98E98F30A71E92AF2D03432F88047951");
   BIGNUM* cy2 = BN_new(); BN_hex2bn(&cy2, "019D18EA69AB6937E418B8AD5F9A2553B46CFE1A1064F06E4144A8EC67A6B3B0");
   const char* message = "John Doe: Brain MRI -> Clear";
   BIGNUM* cyphertext = RSA_encrypt_ascii(message, n2, e2, num_factory);
   RSA_decrypt_ascii(cyphertext, d2, n2, num_factory);
   RSA_decrypt_ascii(cy2, d2, n2, num_factory);

   RSA_signiture_ascii("Ben Rice: Knee MRI -> MCL tear", d2, n2, num_factory);
   RSA_signiture_ascii("Ben Rice: Knee MRI -> LCL tear", d2, n2, num_factory);

   printf("Hello world!\n");
   return 0;
}





