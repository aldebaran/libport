/**
 ** \file  libport/contract.hh
 ** \brief Replacement for cassert.
 **/

#ifndef LIBPORT_CONTRACT_HH
# define LIBPORT_CONTRACT_HH

#include <libport/compiler.hh>




 ATTRIBUTE_NORETURN void  __Terminate (const char*, int, const char*);

# define die(reason)		  __Terminate (__FILE__, __LINE__, reason)
# define unreached()		  die ("unreachable code reached")

# ifdef NDEBUG

#  define assertion(expr)         ((void) 0)
#  define iassertion(expr)	  (expr)
#  define precondition(expr)      ((void) 0)
#  define postcondition(expr)     ((void) 0)

# else // NDEBUG

ATTRIBUTE_NORETURN
void __FailedCondition (const char* condType,
		       const char* condText,
		       const char* fileName,
		       int fileLine);

#  define __TestCondition(condType,expr)				\
  ((void) ((expr) ? 0 : (__FailedCondition ( #condType, #expr,		\
					   __FILE__, __LINE__ ), 0)))

template<typename T>
inline T&
__iassert (T& expr, const char* expr_str, const char* file, int line)
{
  if (expr)
    return expr;
  __FailedCondition ("Assertion", expr_str, file, line);
}

#  define iassertion(expr)						\
  __iassert ((expr), #expr, __FILE__, __LINE__ )

#  define assertion(expr)         __TestCondition (Assertion,expr)
#  define precondition(expr)      __TestCondition (Precondition,expr)
#  define postcondition(expr)     __TestCondition (Postcondition,expr)

# endif // ! NDEBUG

#endif // !LIBPORT_CONTRACT_HH
