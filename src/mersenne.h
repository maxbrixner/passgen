#ifndef EXIFEXTRAS_H_INCLUDED
#define EXIFEXTRAS_H_INCLUDED

#include <stdio.h>

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

void init_genrand(unsigned long s);
unsigned long genrand_int32(void);

#endif
