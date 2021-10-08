
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <openssl/bn.h>

#include "printBN.h"

#define MAX_EEA_ITERATIONS ((size_t) 20)

/* This program uses the RSA algorithm and extended euclidean algorithm from wikipedia.
// Many thanks to the people that post complicated maths stuff.
*/

/* This solves for ax + by = gcd(a, b) using the extended euclidean algorithm.
// Because the values of e and carmichael @ n are known to be coprime,
// the gcd is known to be 1. This means that if a is set as car(n), b as e, and y as d,
// the solution ends up as car(n) * x + d * e = 1.
// If (% car(n)) is taken from both sides, (car(n) * x + d * e) % car(n) = 1 % car(n).
// This simplifies to our desired solution of the form (d * e + k * car(n)) % car(n) = 1. (k is int)
// Inputs: 2 BIGNUM* values, with a > b, and BN_CTX for BIGNUM functions.
// Output: BIGNUM* value, points value of d, assuming gcd(a, b) is actually 1.
// If the function fails then the return value will be NULL.
// This was a lot more fun than just calling BN_mod_inverse(), but I would use that for actual work.
*/
BIGNUM* extended_euclidean_algorithm(const BIGNUM* a, const BIGNUM* b, BN_CTX* ctx)
{
   /*
   // Note that res will be used for general calculations of r, s, t
   // while res2 will be used for the quotient.
   // swap_val is just a regular pointer to swap with.
   // extra_val is to hold the bumped off BIGNUMs so there will be fewer FREEs.
   */
   BIGNUM* swap_val = NULL; BIGNUM* extra_val = BN_new();
   BIGNUM* res = BN_new(); BIGNUM* res2 = BN_new();
   BIGNUM* r_prev = BN_dup(a); BIGNUM* r_curr = BN_dup(b);
   /* s value is not used currently, but it could be for other stuff.
   // BIGNUM* s_prev = BN_new(); BIGNUM* s_curr = BN_new();
   // BN_one(s_prev);
   */
   BIGNUM* t_prev = BN_new(); BIGNUM* t_curr = BN_new();
   BN_one(t_curr);
   size_t counter = 0; /* Mostly for debugging, also avoid infinite loop. */

   /* Loop until the current remainder is 0 or until max iterations hit. */
   while (!BN_is_zero(r_curr)){
      counter++;
      /* previous remainders must be larger, r must trend to 0. */
      if (BN_ucmp(r_prev, r_curr) < 1 || counter > MAX_EEA_ITERATIONS){
         BN_free(extra_val);
         BN_free(res); BN_free(res2);
         BN_free(r_prev); BN_free(r_curr);
         /* BN_free(s_prev); BN_free(s_curr); */
         BN_free(t_prev); BN_free(t_curr);
         return NULL;
      }

      /* Gets quotient, discards remainder. */
      BN_div(res2, NULL, r_prev, r_curr, ctx);


      /* A complicated way of doing var(i) = var(i - 2) - var(i - 1) * quotient
      // All variables are swapped around so that curr is var(i) and prev is var(i-1)
      */
      BN_mul(res, res2, r_curr, ctx);
      swap_val = r_prev;
      r_prev = r_curr;
      r_curr = extra_val;
      extra_val = swap_val;
      BN_sub(r_curr, extra_val, res);

      /*
      BN_mul(res, res2, s_curr, ctx);
      swap_val = s_prev;
      s_prev = s_curr;
      s_curr = extra_val;
      extra_val = swap_val;
      BN_sub(s_curr, extra_val, res);
      */

      BN_mul(res, res2, t_curr, ctx);
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

   BN_free(extra_val);
   BN_free(res); BN_free(res2);
   BN_free(r_prev); BN_free(r_curr);
   /* BN_free(s_prev); BN_free(s_curr); */
   BN_free(t_curr);

   /* When the remainder is 1 (equal to gcd), the coefficients of car and e are found.
   // d should equal to t_prev, but may be negative. If so, just bump it up.
   */
   if (BN_is_negative(t_prev)){
      BN_add(t_prev, t_prev, a);
   }

   return t_prev;
}


BIGNUM* RSA_private_key_from_values(const char* p_str, const char* q_str, const char* e_str, BN_CTX* ctx)
{
   /*Assuming no errors to keep this simple,
   since it would be really messy with the return checks.*/
   /* Init */
   BIGNUM* p = BN_new();
   BIGNUM* q = BN_new();
   BIGNUM* n = BN_new();
   BIGNUM* e = BN_new();
   BIGNUM* car = BN_new();
   BIGNUM* res = BN_new();
   BIGNUM* res2 = BN_new();

   BN_hex2bn(&p, p_str);
   BN_hex2bn(&q, q_str);
   BN_mul(n, p, q, ctx);
   BN_hex2bn(&e, e_str);
   /*printf("End Init!\n");*/
   /* End init */

   /* Carmichael totient
   // Equals lcm(p - 1, q - 1),
   // or (p - 1) * (q - 1) / gcd(p - 1, q - 1)
   */
   BN_sub_word(p, 1);
   BN_sub_word(q, 1);
   BN_gcd(res, p, q, ctx);
   BN_mul(res2, p, q, ctx);
   BN_div(car, NULL, res2, res, ctx);

   /* Check if e is good, not strictly needed as the given value is good. */
   BN_gcd(res, e, car, ctx);
   assert(BN_is_one(res));
   assert(BN_cmp(car, e) >= 1); /* car > e, seems docs I used were out of date*/

   /* Free some values that aren't used after this. */
   BN_free(p); BN_free(q); BN_free(n);

   /* Reuse n pointer because lazy */
   n = extended_euclidean_algorithm(car, e, ctx);
   if (n == NULL){
      return NULL;
   }

   BN_mul(res2, n, e, ctx);
   BN_mod(res, res2, car, ctx);
   BN_free(car); BN_free(e); BN_free(res2);

   /* Checks that d * e % car(n) = 1 */
   if(BN_is_one(res) == 1){
      BN_free(res);
      return n;
   }
   else{
      BN_free(res); BN_free(n);
      return NULL;
   }
}


int main()
{
   BN_CTX* num_factory = BN_CTX_new();

   BIGNUM* private_key = RSA_private_key_from_values("CF751B1FA7FD450223CD96CDABE96AC7",
                                                     "FD8F0F9D611E28DD688447C0E9A0CDBD",
                                                     "41AC7", num_factory);
   assert(private_key != NULL);
   printBN("The private key d is:", private_key);
   return 0;
}




