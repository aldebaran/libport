/**
 *
 * Copyright (C) Gostai S.A.S., 2006-2008.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#ifndef URBI_CRYPTO_MANGLE_DEFINES_HH
# define URBI_CRYPTO_MANGLE_DEFINES_HH

/// When we release a product, using the key system,
/// we don't want anyone to be able to read the symbols in our
/// binaries and to hack them easily. The best solution is
/// to 'strip' the binary. But how do we do for libraries ?
/// They cannot be stripped. That is why we mangle the symbols
/// directly in the code. This is a broken solution why might
/// generate problems...
# define setPublicKey	l899
# define setOutStream   l900
# define setInStream	l901
# define SignSHA1	l902
# define Crypto		l903
# define check		l904

#endif // !URBI_CRYPTO_MANGLE_DEFINES_HH
