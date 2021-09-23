#ifndef BN_UTILITIES_H_INCLUDED
#define BN_UTILITIES_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/bn.h>

void printBN(char* msg, BIGNUM* val);
void printBN_hex(char* msg, BIGNUM* val);

BIGNUM* RSA_encrypt_ascii(const char* str_in, const BIGNUM* n, const BIGNUM* e, BN_CTX* ctx);
void RSA_decrypt_ascii(const BIGNUM* cy, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx);
BIGNUM* RSA_signiture_ascii(const char* str_in, const BIGNUM* pk, const BIGNUM* n, BN_CTX* ctx);

#endif // BN_UTILITIES_H_INCLUDED
