/*! @file
  @brief
  mruby/c Math class

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#include "vm_config.h"
#include <math.h>

#include "value.h"
#include "class.h"


#if MRBC_USE_FLOAT && MRBC_USE_MATH

//================================================================
/*! convert mrbc_value to c double
*/
static double to_double( const mrbc_value *v )
{
  switch( v->tt ) {
  case MRBC_TT_FIXNUM:	return (double)v->i;
  case MRBC_TT_FLOAT:	return (double)v->d;
  default:		return 0;	// TypeError. raise?
  }
}



//================================================================
/*! (method) acos
*/
static void c_math_acos(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, acos( to_double(&v[1]) ));
}

//================================================================
/*! (method) acosh
*/
static void c_math_acosh(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, acosh( to_double(&v[1]) ));
}

//================================================================
/*! (method) asin
*/
static void c_math_asin(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, asin( to_double(&v[1]) ));
}

//================================================================
/*! (method) asinh
*/
static void c_math_asinh(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, asinh( to_double(&v[1]) ));
}

//================================================================
/*! (method) atan
*/
static void c_math_atan(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, atan( to_double(&v[1]) ));
}

//================================================================
/*! (method) atan2
*/
static void c_math_atan2(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, atan2( to_double(&v[1]), to_double(&v[2]) ));
}

//================================================================
/*! (method) atanh
*/
static void c_math_atanh(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, atanh( to_double(&v[1]) ));
}

//================================================================
/*! (method) cbrt
*/
static void c_math_cbrt(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, cbrt( to_double(&v[1]) ));
}

//================================================================
/*! (method) cos
*/
static void c_math_cos(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, cos( to_double(&v[1]) ));
}

//================================================================
/*! (method) cosh
*/
static void c_math_cosh(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, cosh( to_double(&v[1]) ));
}

//================================================================
/*! (method) erf
*/
static void c_math_erf(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, erf( to_double(&v[1]) ));
}

//================================================================
/*! (method) erfc
*/
static void c_math_erfc(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, erfc( to_double(&v[1]) ));
}

//================================================================
/*! (method) exp
*/
static void c_math_exp(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, exp( to_double(&v[1]) ));
}

//================================================================
/*! (method) hypot
*/
static void c_math_hypot(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, hypot( to_double(&v[1]), to_double(&v[2]) ));
}

//================================================================
/*! (method) ldexp
*/
static void c_math_ldexp(struct VM *vm, mrbc_value v[], int argc)
{
  int exp;
  switch( v[2].tt ) {
  case MRBC_TT_FIXNUM:	exp = v[2].i;		break;
  case MRBC_TT_FLOAT:	exp = (int)v[2].d;	break;
  default:		exp = 0;	// TypeError. raise?
  }

  v[0] = mrbc_float_value(vm, ldexp( to_double(&v[1]), exp ));
}

//================================================================
/*! (method) log
*/
static void c_math_log(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, log( to_double(&v[1]) ));
}

//================================================================
/*! (method) log10
*/
static void c_math_log10(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, log10( to_double(&v[1]) ));
}

//================================================================
/*! (method) log2
*/
static void c_math_log2(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, log2( to_double(&v[1]) ));
}

//================================================================
/*! (method) sin
*/
static void c_math_sin(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, sin( to_double(&v[1]) ));
}

//================================================================
/*! (method) sinh
*/
static void c_math_sinh(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, sinh( to_double(&v[1]) ));
}

//================================================================
/*! (method) sqrt
*/
static void c_math_sqrt(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, sqrt( to_double(&v[1]) ));
}

//================================================================
/*! (method) tan
*/
static void c_math_tan(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, tan( to_double(&v[1]) ));
}

//================================================================
/*! (method) tanh
*/
static void c_math_tanh(struct VM *vm, mrbc_value v[], int argc)
{
  v[0] = mrbc_float_value(vm, tanh( to_double(&v[1]) ));
}


/* MRBC_AUTOGEN_METHOD_TABLE

  CLASS("Math")
  FILE("method_table_math.h")
  FUNC("mrbc_init_class_math")

  METHOD( "acos",	c_math_acos )
  METHOD( "acosh",	c_math_acosh )
  METHOD( "asin",	c_math_asin )
  METHOD( "asinh",	c_math_asinh )
  METHOD( "atan",	c_math_atan )
  METHOD( "atan2",	c_math_atan2 )
  METHOD( "atanh",	c_math_atanh )
  METHOD( "cbrt",	c_math_cbrt )
  METHOD( "cos",	c_math_cos )
  METHOD( "cosh",	c_math_cosh )
  METHOD( "erf",	c_math_erf )
  METHOD( "erfc",	c_math_erfc )
  METHOD( "exp",	c_math_exp )
  METHOD( "hypot",	c_math_hypot )
  METHOD( "ldexp",	c_math_ldexp )
  METHOD( "log",	c_math_log )
  METHOD( "log10",	c_math_log10 )
  METHOD( "log2",	c_math_log2 )
  METHOD( "sin",	c_math_sin )
  METHOD( "sinh",	c_math_sinh )
  METHOD( "sqrt",	c_math_sqrt )
  METHOD( "tan",	c_math_tan )
  METHOD( "tanh",	c_math_tanh )
*/
#include "method_table_math.h"

#endif  // MRBC_USE_FLOAT && MRBC_USE_MATH
