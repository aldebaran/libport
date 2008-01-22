/*!
 *******************************************************************************

 File: sign.cc\n
 Sign a file with the SHA1 algorithm.

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
  crypto::SignSHA1 signature;

  crypto::createDebugLog ();

  if (argc < 3)
    {
      std::cerr << "Some flags are missing." << std::endl;
      std::cout << argv[0] << " <privatekey> <file> <signature>" << std::endl;
      exit(1);
    }

  signature.setPrivateKey(argv[1]);
  signature.setInStream(argv[2], "rt", "");
  //signature.save(argv[3]);
  signature.sign ();
  std::ofstream os;
  os.open (argv[3]);
  os << signature.getOutStream ();
  os.close ();
  //std::cout << "Verification:" << signature.check() << std::endl;
  std::cout << "Success! The signature has been saved." << std::endl;
  return 0;
}
