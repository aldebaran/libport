/** \file libport/contract.cc
 ** \brief Implementation for contract.hh
 */

#include <iostream>
#include <cstdlib>

#include <libport/contract.hh>

#ifndef NDEBUG

void __FailedCondition (const char* condType,
		       const char* condText,
		       const char* fileName,
		       int fileLine)
{
  std::cerr << fileName << ':'
	    << fileLine
	    << ": "
	    << condType << " `"
	    << condText << "' failed." << std::endl;
  abort ();
}

#endif // !NDEBUG

void __Terminate (const char* fileName, int fileLine, const char* reason)
{
  std::cerr << fileName << ":" << fileLine
	    << ": Program abortion (" << reason << ')'
	    << std::endl;
  abort ();
}
