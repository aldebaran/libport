/*!
 *******************************************************************************

 File: verify.cc\n
 Verify a digital signature with the SHA1 algorithm.

 This file is part of
 %URBI Kernel, version __kernelversion__\n
 (c) Jean-Christophe Baillie, 2004-2005.

 This software is provided "as is" without warranty of any kind,
 either expressed or implied, including but not limited to the
 implied warranties of fitness for a particular purpose.

 For more information, comments, bug reports: http://www.urbiforge.net

 **************************************************************************** */
#include <iostream>
#include "urbi-crypto/debug-log.hh"
#include "urbi-crypto/signsha1.hh"

int main(int argc, char**argv)
{
  crypto::SignSHA1 verify;

  crypto::createDebugLog ();

  if (argc < 4)
    {
      std::cerr << "Some flags are missing." << std::endl;
      std::cout << argv[0] << " <publickey> <signature> <file>" << std::endl;
      exit(1);
    }

  verify.setPublicKey(argv[1]);
  verify.setOutStream(argv[2], "rb", "");
  verify.setInStream(argv[3], "rt", "");
  if (verify.check())
    std::cout << "SUCCESS" << std::endl;
  else
    std::cout << "FAILED" << std::endl;
  return 0;
}
