/**
 ** \file  libport/contract.hh
 ** \brief Replacement for cassert.
 **/

#ifndef LIBPORT_CONTRACT_HH
# define LIBPORT_CONTRACT_HH


// Use GCC magic bits to specify we cannot return from these functions

#ifndef __attribute__
# if !defined __GNUC__
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef ATTRIBUTE_NORETURN
# define ATTRIBUTE_NORETURN __attribute__ ((__noreturn__))
#endif


void __Terminate (const char*, int, const char*) ATTRIBUTE_NORETURN;

# define die(reason)		  __Terminate (__FILE__, __LINE__, reason)
# define unreached()		  die ("unreachable code reached")

# ifdef NDEBUG

#  define assertion(expr)         ((void) 0)
//#  define invariant(expr)         ((void) 0)
#  define precondition(expr)      ((void) 0)
#  define postcondition(expr)     ((void) 0)

# else // NDEBUG

void __FailedCondition (const char* condType,
		       const char* condText,
		       const char* fileName,
		       int fileLine) ATTRIBUTE_NORETURN;

#  define __TestCondition(condType,expr)				\
  ((void) ((expr) ? 0 : (__FailedCondition ( #condType, #expr,		\
					   __FILE__, __LINE__ ), 0)))

#  define assertion(expr)         __TestCondition (Assertion,expr)
//#  define invariant(expr)         __TestCondition (Invariant,expr)
#  define precondition(expr)      __TestCondition (Precondition,expr)
#  define postcondition(expr)     __TestCondition (Postcondition,expr)

# endif // ! NDEBUG

#endif // !LIBPORT_CONTRACT_HH
