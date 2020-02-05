/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" { target { ! ilp32 } } } } */

#include "test_sve_acle.h"

/*
** ld4_bf16_base:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_base, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0),
	   z0 = svld4 (p0, x0))

/*
** ld4_bf16_index:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, x1, lsl 1\]
**	ret
*/
TEST_LOAD (ld4_bf16_index, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + x1),
	   z0 = svld4 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_bf16_1:
**	incb	x0
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_1, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + svcnth ()),
	   z0 = svld4 (p0, x0 + svcnth ()))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_bf16_2:
**	incb	x0, all, mul #2
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_2, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + svcnth () * 2),
	   z0 = svld4 (p0, x0 + svcnth () * 2))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_bf16_3:
**	incb	x0, all, mul #3
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_3, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + svcnth () * 3),
	   z0 = svld4 (p0, x0 + svcnth () * 3))

/*
** ld4_bf16_4:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #4, mul vl\]
**	ret
*/
TEST_LOAD (ld4_bf16_4, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + svcnth () * 4),
	   z0 = svld4 (p0, x0 + svcnth () * 4))

/*
** ld4_bf16_28:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #28, mul vl\]
**	ret
*/
TEST_LOAD (ld4_bf16_28, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + svcnth () * 28),
	   z0 = svld4 (p0, x0 + svcnth () * 28))

/*
** ld4_bf16_32:
**	[^{]*
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x[0-9]+\]
**	ret
*/
TEST_LOAD (ld4_bf16_32, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 + svcnth () * 32),
	   z0 = svld4 (p0, x0 + svcnth () * 32))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_bf16_m1:
**	decb	x0
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_m1, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 - svcnth ()),
	   z0 = svld4 (p0, x0 - svcnth ()))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_bf16_m2:
**	decb	x0, all, mul #2
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_m2, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 - svcnth () * 2),
	   z0 = svld4 (p0, x0 - svcnth () * 2))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_bf16_m3:
**	decb	x0, all, mul #3
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_bf16_m3, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 - svcnth () * 3),
	   z0 = svld4 (p0, x0 - svcnth () * 3))

/*
** ld4_bf16_m4:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #-4, mul vl\]
**	ret
*/
TEST_LOAD (ld4_bf16_m4, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 - svcnth () * 4),
	   z0 = svld4 (p0, x0 - svcnth () * 4))

/*
** ld4_bf16_m32:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #-32, mul vl\]
**	ret
*/
TEST_LOAD (ld4_bf16_m32, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 - svcnth () * 32),
	   z0 = svld4 (p0, x0 - svcnth () * 32))

/*
** ld4_bf16_m36:
**	[^{]*
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x[0-9]+\]
**	ret
*/
TEST_LOAD (ld4_bf16_m36, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_bf16 (p0, x0 - svcnth () * 36),
	   z0 = svld4 (p0, x0 - svcnth () * 36))

/*
** ld4_vnum_bf16_0:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_0, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 0),
	   z0 = svld4_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_vnum_bf16_1:
**	incb	x0
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_1, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 1),
	   z0 = svld4_vnum (p0, x0, 1))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_vnum_bf16_2:
**	incb	x0, all, mul #2
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_2, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 2),
	   z0 = svld4_vnum (p0, x0, 2))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_vnum_bf16_3:
**	incb	x0, all, mul #3
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_3, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 3),
	   z0 = svld4_vnum (p0, x0, 3))

/*
** ld4_vnum_bf16_4:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #4, mul vl\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_4, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 4),
	   z0 = svld4_vnum (p0, x0, 4))

/*
** ld4_vnum_bf16_28:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #28, mul vl\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_28, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 28),
	   z0 = svld4_vnum (p0, x0, 28))

/*
** ld4_vnum_bf16_32:
**	[^{]*
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x[0-9]+\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_32, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, 32),
	   z0 = svld4_vnum (p0, x0, 32))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_vnum_bf16_m1:
**	decb	x0
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_m1, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, -1),
	   z0 = svld4_vnum (p0, x0, -1))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_vnum_bf16_m2:
**	decb	x0, all, mul #2
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_m2, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, -2),
	   z0 = svld4_vnum (p0, x0, -2))

/* Moving the constant into a register would also be OK.  */
/*
** ld4_vnum_bf16_m3:
**	decb	x0, all, mul #3
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_m3, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, -3),
	   z0 = svld4_vnum (p0, x0, -3))

/*
** ld4_vnum_bf16_m4:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #-4, mul vl\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_m4, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, -4),
	   z0 = svld4_vnum (p0, x0, -4))

/*
** ld4_vnum_bf16_m32:
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x0, #-32, mul vl\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_m32, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, -32),
	   z0 = svld4_vnum (p0, x0, -32))

/*
** ld4_vnum_bf16_m36:
**	[^{]*
**	ld4h	{z0\.h - z3\.h}, p0/z, \[x[0-9]+\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_m36, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, -36),
	   z0 = svld4_vnum (p0, x0, -36))

/* Using MUL to calculate an index would also be OK.  */
/*
** ld4_vnum_bf16_x1:
**	cntb	(x[0-9]+)
**	madd	(x[0-9]+), (x1, \1|\1, x1), x0
**	ld4h	{z0\.h - z3\.h}, p0/z, \[\2\]
**	ret
*/
TEST_LOAD (ld4_vnum_bf16_x1, svbfloat16x4_t, bfloat16_t,
	   z0 = svld4_vnum_bf16 (p0, x0, x1),
	   z0 = svld4_vnum (p0, x0, x1))
