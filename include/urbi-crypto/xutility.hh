/*
 * Copyright (C) Gostai S.A.S., 2006-2008.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#ifndef URBI_CRYPTO_XUTILITY_HH
# define URBI_CRYPTO_XUTILITY_HH

# include <cstdio>
# include "urbi-crypto/exception.hh"
# include "urbi-crypto/debug-log.hh"

//# define REAL_DEBUG

/// Defines for the error messages

/// C I/O errors
# define E_OPENFILE	"Error opening file"
# define E_READFILE	"Error reading file"
# define E_WRITEFILE	"Error writing to file"
# define E_CLOSEFILE	"Error closing file"
# define E_SEEKFILE	"Error seeking in file"
# define E_TELLFILE	"Error calling ftell"

# define E_MKSTEMP	"Error creating temporary file"
# define E_UNLINK	"Error unlinking file"

/// C++ I/O errors
# define E_OSTRTELLP	"Error telling position with tellp, got -1."
# define E_OSTRWRITE	"Error writing to ostream"
# define E_ISTRTELLG	"Error telling position with tellg, got -1."
# define E_ISTRSEEKG	"Error seeking in istream"
# define E_ISTRREAD	"Error reading in stream"
# define E_ISTRREADNB	"Error reading in stream: gcount doesn't give good count"

/// Other errors
# define E_NOPRIVATEKEY	"No specified private key."
# define E_NOPUBLICKEY	"No specified public key."
# define E_SIGNINIT	"Sign initialization failed: "
# define E_SIGNUPDATE	"Sign update failed: "
# define E_SIGNFINAL	"Sign final checking failed: "
# define E_VERIFYINIT	"Verify initialisation failed: "
# define E_VERIFYUPDATE "Verify update failed: "
# define E_VERIFYFINAL	"Verify final failed: "
# define E_PKEYTORSA	"Problem converting private key to RSA: "
# define E_PRIVDECRYPT	"Failed to decrypt message: "
# define E_READPRIVKEY	"Error while reading private key: "
# define E_READPUBKEY	"Error while reading public key: "

# define E_CREATETMP	"Error creating a temporary file"
# define E_GETTMP	"Error obtaining the temporary files directory path"


namespace crypto
{

  ////////////////////////////////////////////////////////////////////////////
  ///                                                                      ///
  ///                            C I/O functions                           ///
  ///                                                                      ///
  ////////////////////////////////////////////////////////////////////////////

  /// Implementation of fopen with exception throwing and logging
  inline FILE* xfopen (const char *path, const char *mode)
  {
    FILE* file = 0;

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Opening file " << path
    		<< " with fopen in mode " << mode
    		<< std::endl;
#endif

    if (0 == (file = fopen(path, mode)))
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_OPENFILE, path);
    return file;
  }

  /// Implementation of fread with exception throwing and logging
  /// 'filename' is the name of the file processed. It is used in the
  /// exception message in case of error.
  inline size_t xfread (void*		ptr,
			size_t		size,
			size_t		nmemb,
			FILE*		stream,
			std::string	filename)
  {
    size_t res;
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Reading " << nmemb << " bytes in file " << filename << std::endl;
#endif
    res = fread (ptr, size, nmemb, stream);
    if (nmemb != res)
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_READFILE, filename);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Read " << res << " bytes in file " << filename << std::endl;
#endif
    return res;
  }

  /// Implementation of fwrite with exception throwing and logging
  /// 'filename' is the name of the file processed. It is used in the
  /// exception message in case of error.
  inline size_t xfwrite (const void*	ptr,
			 size_t		size,
			 size_t		nmemb,
			 FILE*		stream,
			 std::string	filename)
  {
    size_t res;
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Writing " << nmemb << " bytes in file"
	       << filename << std::endl;
#endif
    res = fwrite (ptr, size, nmemb, stream);
    if (nmemb != res)
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_WRITEFILE, filename);
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Written " << res << " bytes in file "
		<< filename << std::endl;
#endif
    return res;
  }

  /// Implementation of fclose with exception throwing and logging
  /// 'filename' is the name of the file processed. It is used in the
  /// exception message in case of error.
  inline int xfclose (FILE *stream, std::string filename)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Closing file " << filename << std::endl;
#endif
    if (EOF == fclose(stream))
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_CLOSEFILE, filename);
    return 0;
  }


  inline int xfseek (FILE *stream, long offset, int whence, std::string filename)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Seeking in file " << filename << std::endl;
#endif
    int res;
    res = fseek (stream, offset, whence);
    if (-1 == res)
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG(E_SEEKFILE, filename);
    return res;
  }

  inline long xftell (FILE *stream, std::string filename)
  {
    long res = ftell (stream);
    if (-1 == res)
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG(E_TELLFILE, filename);
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** File " << filename << " at position: " << res << std::endl;
#endif
    return res;
  }

  ////////////////////////////////////////////////////////////////////////////
  ///                                                                      ///
  ///                        C++ I/O Stream functions                      ///
  ///                                                                      ///
  ////////////////////////////////////////////////////////////////////////////


  /// Implementation of ostream tellp with exception throwing and logging
  inline std::streampos xtellp (std::ostream& ostr)
  {
    std::streampos res = ostr.tellp ();
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Ostream at position: " << res << std::endl;
#endif
    if (-1 == res)
      THROW_CRYPTO (E_OSTRTELLP);
    return res;
  }

  /// Implementation of ostream write with exception throwing and logging
  inline std::ostream& xwrite (std::ostream& ostr, const char* s,
			       std::streamsize n)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Writing " << n << " bytes in stream" << std::endl;
    std::streampos bytes_written = xtellp (ostr);
#endif

    std::ostream& res = ostr.write (s, n);
    if (ostr.bad ())
      THROW_CRYPTO (E_OSTRWRITE);

#ifdef REAL_DEBUG
    bytes_written = xtellp (ostr) - bytes_written;
    DEBUG_LOG() << "*** Written " << bytes_written << " bytes in stream"
		<< std::endl;
#endif
    return res;
  }

  inline std::streampos xtellg (std::istream& istr)
  {
    std::streampos res = istr.tellg ();

    if (-1 == res)
      THROW_CRYPTO (E_ISTRTELLG);
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Istream at position: " << res << std::endl;
#endif
    return res;
  }

  inline std::istream& xseekg (std::istream& istr, std::streamoff off,
			       std::ios_base::seekdir dir)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Calling seekg in istream" << std::endl;
#endif
    std::istream& res = istr.seekg (off, dir);

    if (istr.fail ())
      THROW_CRYPTO(E_ISTRSEEKG);
    return res;
  }

  inline std::istream& xread (std::istream& istr, char* s, std::streamsize n)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Reading " << n << " bytes in stream" << std::endl;
#endif
    std::istream& res = istr.read (s, n);
    if (istr.fail ())
      THROW_CRYPTO (E_ISTRREAD);

    int ret = istr.gcount ();
    if (ret != n)
      THROW_CRYPTO (E_ISTRREADNB)

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Read " << ret << " bytes in stream" << std::endl;
#endif
    return res;
  }

}

#endif // !URBI_CRYPTO_XUTILITY_HH
