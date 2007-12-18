/**
 ** \file  libport/contract.hh
 ** \brief Replacement for cassert.
 **/

#ifndef LIBPORT_CONTRACT_HH
# define LIBPORT_CONTRACT_HH

#include "libport/compiler.hh"




 __attribute__((noreturn)) void  __Terminate (const char*, int, const char*);

# define die(reason)		  __Terminate (__FILE__, __LINE__, reason)
# define unreached()		  die ("unreachable code reached")

# ifdef NDEBUG

#  define assertion(expr)         ((void) 0)
//#  define invariant(expr)         ((void) 0)
#  define precondition(expr)      ((void) 0)
#  define postcondition(expr)     ((void) 0)

# else // NDEBUG

__attribute__((noreturn))
void __FailedCondition (const char* condType,
		       const char* condText,
		       const char* fileName,
		       int fileLine);

#  define __TestCondition(condType,expr)				\
  ((void) ((expr) ? 0 : (__FailedCondition ( #condType, #expr,		\
					   __FILE__, __LINE__ ), 0)))

#  define assertion(expr)         __TestCondition (Assertion,expr)
//#  define invariant(expr)         __TestCondition (Invariant,expr)
#  define precondition(expr)      __TestCondition (Precondition,expr)
#  define postcondition(expr)     __TestCondition (Postcondition,expr)

# endif // ! NDEBUG

#endif // !LIBPORT_CONTRACT_HH
