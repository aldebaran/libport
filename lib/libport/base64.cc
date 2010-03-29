/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/base64.hh>

namespace libport
{
  std::string base64(const std::string& input)
  {
    const char encoding[64] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
      'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
    };

    std::string res;
    size_t size = input.size();
    for (unsigned i = 0; i < size; i += 3)
    {
      // Whether there are at least two characters left
      bool b2 = size > (i + 1);
      // Whether there are at least three characters left
      bool b3 = size > (i + 2);
      unsigned char chunk[3];

      chunk[0] =      input[i    ]      ;
      chunk[1] = b2 ? input[i + 1] : 0x0;
      chunk[2] = b3 ? input[i + 2] : 0x0;
      res += encoding[chunk[0] >> 2];
      res += encoding[((chunk[0] & 0x03) << 4) | ((chunk[1] & 0xf0) >> 4)];
      res += b2 ? encoding[((chunk[1] & 0x0f) << 2) | ((chunk[2] & 0xc0) >> 6)] : '=';
      res += b3 ? encoding[(chunk[2] & 0x3f)] : '=';
    }
    return res;
  }
}
