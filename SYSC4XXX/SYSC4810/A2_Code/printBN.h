#ifndef PRINTBN_H_INCLUDED
#define PRINTBN_H_INCLUDED

#include <stdio.h>
#include <openssl/bn.h>

inline void printBN(char* msg, BIGNUM* val)
{
   char* number_str = BN_bn2dec(val);
   printf("%s %s\n", msg, number_str);
   OPENSSL_free(number_str);
}

inline void printBN_hex(char* msg, BIGNUM* val)
{
   char* number_str = BN_bn2hex(val);
   printf("%s %s\n", msg, number_str);
   OPENSSL_free(number_str);
}


#endif // PRINTBN_H_INCLUDED
