/* SYSC 2006 Winter 2016 Lab 11. */

#include <stdio.h>
#include <math.h>

/* Return x raised to the power n for n >= 0. */
double power(double x, int n)
{
   if (n <= 0){ return 1; }
   return x * power(x, n - 1);
}

/* Return the number of digits in integer n, n >= 0. */
int num_digits(int n)
{
   n = n / 10;
   if (n){ return (1 + num_digits(n)); }
   return 1;
}

/* Return the count of the number of times target occurs in the first
 * n elements of array a.
 */
int occurrences(int a[], int n, int target)
{
   if (n < 0){ return 0; }
   return occurrences(a + 1, n - 1, target) + ((a[0] == target) ? 1:0);
}

/* Challenge exercise: return x raised to the power n for n >= 0. */
double power2(double x, int n)
{
   if (n <= 0){ return 1; }
   if (n % 2 == 0){
      double tempDouble = power2(x, n/2);
      return tempDouble * tempDouble;}
   return x * power2(x, n - 1);
}







