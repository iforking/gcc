/* { dg-do compile } */
/* { dg-require-effective-target vect_int } */
/* { dg-require-effective-target sse { target i386*-*-* } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-msse" { target i386*-*-* } } */

typedef int v4si
  __attribute__ ((vector_size (16)));

int fn1 (v4si p)
{
  return p[0];
}

