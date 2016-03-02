/* main.c - SYSC 2006 Winter 2016 Lab 6 Test Harness */

#include <stdlib.h>  // exit, malloc, free
#include <stdio.h>   // printf
#include "sput.h"

#include "fraction.h"

/* By default, Pelles C generates "warning #2154: Unreachable code"
   and "warning #2130: Result of comparison is constant" when the 
   macros in sput.h are used. The following pragma directive disables the
   generation of these warnings.
 */
#pragma warn(disable: 2130 2154)

/* Test print_fraction. */
void test_print_fraction(void)
{
	printf("\nTesting print_fraction\n\n");

	fraction_t fr;

	fr.num = 1;
	fr.den = 3;
	printf("Expected output: 1/3\n");
    printf("Actual output:   ");
	print_fraction(&fr);
	printf("\n\n");

	fr.num = -1;
	fr.den = 3;
	printf("Expected output: -1/3\n");
    printf("Actual output:   ");
	print_fraction(&fr);
	printf("\n\n");
}



static void test_gcd(void)
{
    sput_fail_unless(gcd(42, 30) == 6, "gcd(42, 30) ==> 6");
    sput_fail_unless(gcd(30, 42) == 6, "gcd(30, 42) ==> 6");
    sput_fail_unless(gcd(-42, 30) == 6, "gcd(-42, 30) ==> 6");
    sput_fail_unless(gcd(42, -30) == 6, "gcd(42, -30) ==> 6");
    sput_fail_unless(gcd(-42, -30) == 6, "gcd(-42, -30) ==> 6");
    sput_fail_unless(gcd(144, -55) == 1, "gcd(144, -55) == 1");
}

static void test_reduce(void)
{
    fraction_t fr;
    int numerator, denominator;

    // Test reduce(1/3)
	fr.num = 1;  // Initialize the function to reduce
	fr.den = 3;
    reduce(&fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 1,
                     "reduce(1/3) ==> 1/3, "
                     "check numerator");
    sput_fail_unless(denominator == 3,
                     "reduce(1/3) ==> 1/3, "
                     "check denominator");

    // Test reduce(6/8)
	fr.num = 6;
	fr.den = 8;
    reduce(&fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 3,
                     "reduce(6/8) ==> 3/4, "
                     "check numerator");
    sput_fail_unless(denominator == 4,
                     "reduce(6/8) ==> 3/4, "
                     "check denominator");

    // Test reduce(-2/3)

	fr.num = -2;
	fr.den = 3;
    reduce(&fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == -2,
                     "reduce(-2/3) ==> -2/3, "
                     "check numerator");
    sput_fail_unless(denominator == 3,
                     "reduce(-2/3) ==> -2/3, "
                     "check denominator");

    // Test reduce(4/-5)
	fr.num = 4;
	fr.den = -5;
    reduce(&fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == -4,
                     "reduce(4/-5) ==> -4/5, "
                     "check numerator");
    sput_fail_unless(denominator == 5,
                     "reduce(4/-5) ==> -4/5, "
                     "check denominator");

    // Test reduce(-6/-8)
	fr.num = -6;
	fr.den = -8;
    reduce(&fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 3,
                     "reduce(-6/-8) ==> 3/4, "
                     "check numerator");
    sput_fail_unless(denominator == 4,
                     "reduce(-6/-8) ==> 3/4, "
                     "check denominator");

    // Test reduce(0/7)
	fr.num = 0;
	fr.den = 7;
    reduce(&fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 0,
                     "reduce(0/7) ==> 0/1, "
                     "check numerator");
    sput_fail_unless(denominator == 1,
                     "reduce(0/7) ==> 0/1, "
                     "check denominator");
}

void test_make_fraction(void)
{
    fraction_t fr;
    int numerator, denominator;

    // Test make_fraction(1, 3)

	fr.num = 0;
	fr.den = 0;
    make_fraction(1, 3, &fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 1,
                     "make_fraction(1, 3, &fr) ==> 1/3, "
                     "check numerator");
    sput_fail_unless(denominator == 3 ,
                     "make_fraction(1, 3, &fr) ==> 1/3, "
                     "check denominator");

    // Test make_fraction(6, 8)

	fr.num = 0;
	fr.den = 0;
    make_fraction(6, 8, &fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 3,
                     "make_fraction(6, 8, &fr) ==> 3/4, "
                     "check numerator");
    sput_fail_unless(denominator == 4,
                     "make_fraction(6, 8, &fr) ==> 3/4, "
                     "check denominator");

    // Test make_fraction(-2, 3)

	fr.num = 0;
	fr.den = 0;
    make_fraction(-2, 3, &fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == -2,
                     "make_fraction(-2, 3, &fr) ==> -2/3, "
                     "check numerator");
    sput_fail_unless(denominator == 3,
                     "make_fraction(-2, 3, &fr) ==> -2/3, "
                     "check denominator");

    // Test make_fraction(4, -5)

	fr.num = 0;
	fr.den = 0;
    make_fraction(4, -5, &fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == -4,
                     "make_fraction(4, -5, &fr) ==> -4/5, "
                     "check numerator");
    sput_fail_unless(denominator == 5,
                     "make_fraction(4, -5, &fr) ==> -4/5, "
                     "check denominator");

    // Test make_fraction(-6, -8)

	fr.num = 0;
	fr.den = 0;
    make_fraction(-6, -8, &fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 3,
                     "make_fraction(-6, -8, &fr) ==> 3/4, "
                     "check numerator");
    sput_fail_unless(denominator == 4,
                     "make_fraction(-6, -8, &fr) ==> 3/4, "
                     "check denominator");

    // Test make_fraction(0, 7)

	fr.num = 1;
	fr.den = 0;
    make_fraction(0, 7, &fr);
    numerator = fr.num;
    denominator = fr.den;
    sput_fail_unless(numerator == 0,
                     "make_fraction(0, 7, &fr) ==> 0/1, "
                     "check numerator");
    sput_fail_unless(denominator == 1,
                     "make_fraction(0, 7, &fr) ==> 0/1, "
                     "check denominator");
}

void test_add_fractions(void)
{
    fraction_t fr1, fr2, sum;
    int numerator, denominator;

    // Test add_fractions: 2/3 + 1/2
	//fr1.num = 2;
	//fr1.den = 3;
    //fr2.num = 1;
    //fr2.den = 2;

    make_fraction(2, 3, &fr1);
    make_fraction(1, 2, &fr2);

	sum.num = 0;
	sum.den = 0;
    add_fractions(&fr1, &fr2, &sum);
    numerator = sum.num;
    denominator = sum.den;
    sput_fail_unless(numerator == 7,
                     "add_fractions: 2/3 + 1/2 ==> 7/6, "
                     "check numerator"); 
    sput_fail_unless(denominator == 6,
                     "add_fractions: 2/3 + 1/2 ==> 7/6, "
                     "check denominator");   

    // Test add_fractions: 1/2 + 2/1
	//fr1.num = 1;
	//fr1.den = 2;
    //fr2.num = 2;
    //fr2.den = 1;

    make_fraction(1, 2, &fr1);
    make_fraction(2, 1, &fr2);

	sum.num = 0;
	sum.den = 0;
    add_fractions(&fr1, &fr2, &sum);
    numerator = sum.num;
    denominator = sum.den;
    sput_fail_unless(numerator == 5,
                     "add_fractions: 1/2 + 2/1 ==> 5/2, "
                     "check numerator"); 
    sput_fail_unless(denominator == 2,
                     "add_fractions: 1/2 + 2/1 ==> 5/2, "
                     " check denominator");
}


void test_multiply_fractions(void)
{

    fraction_t fr1, fr2, product;
    int numerator, denominator;

    // Test multiply_fractions: 2/3 * 1/2
	//fr1.num = 2;
	//fr1.den = 3;
    //fr2.num = 1;
    //fr2.den = 2;

    make_fraction(2, 3, &fr1);
    make_fraction(1, 2, &fr2);

	product.num = 0;
	product.den = 0;
    multiply_fractions(&fr1, &fr2, &product);
    numerator = product.num;
    denominator = product.den;
    sput_fail_unless(numerator == 1,
                     "multiply_fractions: 2/3 * 1/2 ==> 1/3, "
                     "check numerator");  
    sput_fail_unless(denominator == 3,
                     "multiply_fractions: 2/3 * 1/2 ==> 1/3, "
                     "check denominator"); 

    // Test multiply_fractions: 1/2 * 2/1
	//fr1.num = 1;
	//fr1.den = 2;
    //fr2.num = 2;
    //fr2.den = 1;

    make_fraction(1, 2, &fr1);
    make_fraction(2, 1, &fr2);

	product.num = 0;
	product.den = 0;
    multiply_fractions(&fr1, &fr2, &product);
    numerator = product.num;
    denominator = product.den;
    sput_fail_unless(numerator == 1,
                     "multiply_fractions: 1/2 * 2/1 ==> 1/1, "
                     "check numerator");  
    sput_fail_unless(denominator == 1,
                     "multiply_fractions: 1/2 * 2/1 ==> 1/1, "
                     "check denominator"); 
}

int main(void)
{
    sput_start_testing();

    test_print_fraction();

    sput_enter_suite("Exercise 2: gcd()");
    sput_run_test(test_gcd);

    sput_enter_suite("Exercise 3: reduce()");
    sput_run_test(test_reduce);

    sput_enter_suite("Exercise 4: make_fraction()");
    sput_run_test(test_make_fraction);

    sput_enter_suite("Exercise 5: add_fractions()");
    sput_run_test(test_add_fractions);

    sput_enter_suite("Exercise 6: multiply_fractions()");
    sput_run_test(test_multiply_fractions); 

    sput_finish_testing();
    return sput_get_return_value();
}



