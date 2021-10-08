How to compile:
Easiest way is to use the makefile:
make all
(make all_dec will make the BN printing decimal instead of hexadecimal)
(replace # with 3, 4, etc):
gcc problem#.c -lcrypto -O2
Adding the "-DDEC_PRINT" option will make BN printing decimal instead of hexadecimal
