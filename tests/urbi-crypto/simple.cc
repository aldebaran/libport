/*! \file simple.cc
 *******************************************************************************

 File: simple.cc\n
 The simpliest test case.

 This file is part of
 %URBI Kernel, version __kernelversion__\n
 (c) Jean-Christophe Baillie, 2004-2005.

 This software is provided "as is" without warranty of any kind,
 either expressed or implied, including but not limited to the
 implied warranties of fitness for a particular purpose.

 For more information, comments, bug reports: http://www.urbiforge.net

 **************************************************************************** */

#include "urbi-crypto/openssl.hh"

int main()
{
  crypto::OpenSSL& ssl = crypto::OpenSSL::getInstance();

  ssl.displayError();
  return 0;
}
