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


/* This solves for ax + by = gcd(a, b) using the extended euclidean algorithm.
// Because the values of e and carmichael @ n are known to be coprime,
// the gcd is known to be 1. This means that if a is set as car(n), b as e, and y as d,
// the solution ends up as car(n) * x + d * e = 1.
// If (% car(n)) is taken from both sides, (car(n) * x + d * e) % car(n) = 1 % car(n).
// This simplifies to our desired solution of the form (d * e + k * car(n)) % car(n) = 1. (k is int)
// Inputs: 2 BIGNUM* values, with a > b, and a BN_CTX for BIGNUM functions.
// Output: BIGNUM* value, points to begining of output value array.
// The output values is currently just the value of d assuming gcd was actually 1.
// If the function fails then the return value will be NULL.
*/
static BIGNUM* extended_euclidean_algorithm(const BIGNUM* a, const BIGNUM* b, BN_CTX* ctx)
{
   printf("In EEA!\n");
   /*
   // Note that res will be used for general calculations of r, s, t
   // while res2 will be used for the quotient.
   // swap_val is just a regular pointer to swap with.
   // extra_val is to hold the bumped off BIGNUMs because it looks better.
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
   size_t counter = 0; // Mostly for debugging, also avoid infinite loop.

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
   /* Assuming no errors to keep this simple since it already hurt my brain enough. */
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
   printf("End Init!\n");
   /* End init */

   /* Carmichael
   // Equals lcm(p - 1, q - 1),
   // or (p - 1) * (q - 1) / gcd(p - 1, q - 1)
   */
   BN_sub_word(p, 1);
   BN_sub_word(q, 1);
   BN_gcd(res, p, q, ctx);
   BN_mul(res2, p, q, ctx);
   BN_div(car, NULL, res2, res, ctx);

   /* Check if e is good, not strictly needed as the assignment value is good. */
   BN_gcd(res, e, car, ctx);
   assert(BN_is_one(res));
   assert(BN_cmp(car, e) == 1); // car > e

   /* Free some values that aren't used after this. */
   BN_free(p); BN_free(q); BN_free(n);

   /* Reuse n pointer because lazy */
   n = extended_euclidean_algorithm(car, e, ctx);
   if (n == NULL){
      printf("NULL!\n");
      return NULL;
   }

   BN_mul(res2, n, e, ctx);
   BN_mod(res, res2, car, ctx);
   BN_free(car); BN_free(e); BN_free(res2);

   if(BN_is_one(res) == 1){
      BN_free(res);
      return n;
   }
   else{
      BN_free(res); BN_free(n);
      return NULL;
   }
}




BIGNUM* RSA_encrypt_ascii(const char* str_in, const BIGNUM* n, const BIGNUM* e, BN_CTX* ctx)
{
   BIGNUM* cy = BN_new();
   BIGNUM* m = string_to_BN(str_in);
   /* n must be bigger than the message, otherwise you would need to chunk it.
   // That's too much work, so I'll just not do that.
   */
   if (BN_cmp(n, m)){
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
   unsigned char str_out[output_size];
   str_out[output_size - 1] = '\0';
   BN_bn2bin(m, str_out); // Could have issues with endianness? Should work on the target machine.
   printf("The decoded string is: %s\n", str_out);
}

BIGNUM* RSA_signiture_ascii(const char* str_in, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx)
{
   size_t output_size;
   BIGNUM* m = BN_new(); BIGNUM* res = BN_new();
   BN_bin2bn((const unsigned char*) str_in, strlen(str_in), m);
   BN_mod_exp(res, m, pk, n, ctx);
   BN_free(m);
   return res;
}

/* Returns 0 if signiture is the same, non-zero otherwise */
int RSA_verify_signiture_ascii(const char* str_in, const BIGNUM* s, const BIGNUM* e, const BIGNUM* n, BN_CTX* ctx)
{
   BIGNUM* m_val = BN_new();
   BIGNUM* str_val = string_to_BN(str_in);
   int ret_val;
   BN_mod_exp(m_val, s, e, n, ctx);
   printBN_hex("m_val is:", m_val);
   printBN_hex("s_val is:", str_val);
   ret_val = BN_cmp(m_val, str_val);
   BN_free(m_val); BN_free(str_val);
   return ret_val;
}





