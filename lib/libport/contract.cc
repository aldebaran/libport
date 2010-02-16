/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/** \file libport/contract.cc
 ** \brief Implementation for contract.hh
 */

#include <iostream>
#include <libport/cstdlib>

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
  abort();
}

#endif // !NDEBUG

void __Terminate (const char* fileName, int fileLine, const char* reason)
{
  std::cerr << fileName << ":" << fileLine
	    << ": Program abortion (" << reason << ')'
	    << std::endl;
  abort();
}
