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

#include "config.h"
#include "urbi-crypto/exception.hh"
#include "urbi-crypto/xutility.hh"
#include "urbi-crypto/utility.hh"

#ifdef WIN32
# include <Windows.h>
# include <io.h>
# include "urbi-crypto/applink.c"
#endif

#include <iostream>

namespace crypto
{

  std::string*	Utility::temporary_directory = 0;


  unsigned int
  Utility::byte_sum (unsigned char *s, int size)
  {
    unsigned int ret = 0;
    for (int i = 0; i < size; ++i)
      ret += s[i];
    return ret;
  }


  void
  Utility::readFileInStream (const char*	filename,
			     std::stringstream&	out,
			     const char*	read_mode,
			     const char*	stripped)
  {
    FILE *file = xfopen(filename, read_mode);

    // get length of file:
    int cur_pos = crypto::xftell (file, filename);
    crypto::xfseek(file, 0, SEEK_END, filename);
    int file_size = crypto::xftell (file, filename) - cur_pos;
    crypto::xfseek(file, cur_pos, SEEK_SET, filename);

    char *tmp_msg = (char*) malloc (sizeof (char) * file_size);

    unsigned int byte_check1 = 0;
    //while (!feof (file))
    //{
      memset (tmp_msg, 0, file_size);
      int ret = xfread (tmp_msg, sizeof (char), file_size, file, filename);
      byte_check1 += byte_sum ((unsigned char *)tmp_msg, ret);

      int newSize = stripCharacters (tmp_msg, stripped, ret);
#ifdef REAL_DEBUG
      DEBUG_LOG() << "*** Size after stripping: " << newSize << " bytes" << std::endl;
#endif
      xwrite (out, tmp_msg, newSize);
      //}
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Read bytes sum: " << byte_check1 << std::endl;
    byte_check1 = byte_sum ((unsigned char *)out.str ().c_str (), out.str ().size ());
    DEBUG_LOG() << "*** Stream bytes sum: " << byte_check1 << std::endl;
#endif

    free (tmp_msg);
    xfclose(file, filename);
  }



#ifndef WIN32 /* Ugly workaround */

  std::string
  Utility::createTemporaryFile ()
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Linux. Create temporary file" << std::endl;
#endif

    std::string tmp_path("/tmp");

    /// Test if the user set a custom temporary path
    if (temporary_directory)
      tmp_path = *temporary_directory;
    tmp_path += "/";

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** The temporary folder is: " << tmp_path << std::endl;
#endif

    std::string filename = tmp_path;
    filename += "urbi-crypto.XXXXXX";

    char* tmpfile = strdup(filename.c_str ());

    int fd = mkstemp(tmpfile);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** The temporary file is: " << tmpfile << std::endl;
#endif

    if (-1 == fd)
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_MKSTEMP, tmpfile);

    if (-1 == close(fd))
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_CLOSEFILE, tmpfile);

    std::string res = tmpfile;
    free (tmpfile);
    return res;
  }

  void
  Utility::removeTemporaryFile (const std::string& filename)
  {
    if (-1 == unlink(filename.c_str ()))
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_UNLINK, filename);
  }

#else

  std::string
  Utility::createTemporaryFile ()
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Windows. Create temporary file" << std::endl;
#endif

    /// Get the path to the temporary folder
    char tmp_path[MAX_PATH];
    memset (tmp_path, 0, MAX_PATH);
    try
    {
      if (!temporary_directory)
      {
	GetTempPath(MAX_PATH, tmp_path);
      }
    }
    catch (...)
    {
      THROW_CRYPTO(E_GETTMP);
    }
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** The temporary folder is: " << tmp_path << std::endl;
#endif

    char tmpfile[MAX_PATH];
    memset (tmpfile, 0, MAX_PATH);
    try
    {
      if (!temporary_directory)
	GetTempFileName (tmp_path, "", 0, tmpfile);
      else
	GetTempFileName (temporary_directory->c_str (), "", 0, tmpfile);
    }
    catch (...)
    {
      THROW_CRYPTO(E_CREATETMP);
    }
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** The temporary file is: " << tmpfile << std::endl;
#endif
    return (tmpfile);
  }

  void Utility::removeTemporaryFile (const std::string& filename)
  {
    if (-1 == _unlink(filename.c_str ()))
      THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG (E_UNLINK, filename);
  }

#endif /* !WIN32 */

  int
  Utility::stripCharacters(char *msg, const char* stripped, int prev_size)
  {
    char *ptr_fst = msg, *ptr_snd = msg;
    if (!msg)
      return 0;
    int size = 0;
    while (prev_size > 0)
    {
      while (prev_size > 0)
      {
	int i;
	for (i = 0; stripped[i] && stripped[i] != *ptr_fst; ++i)
	  ;
	if (stripped[i] && prev_size > 0)
	{
	  ++ptr_fst;
	  --prev_size;
	}
	else
	  break;
      }
      if (prev_size <= 0)
	break;
      *ptr_snd = *ptr_fst;
      ++ptr_snd;
      ++ptr_fst;
      ++size;
      --prev_size;
    }
    /// If at least one characted was stripped we put a \0 at the end
    /// of the string
    if (ptr_snd != ptr_fst)
      *ptr_snd = 0;
    return size;
  }
}
