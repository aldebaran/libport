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

#ifndef UTILITY_HH_
# define UTILITY_HH_

# include <sstream>

# define READ_SIZE	1024

namespace crypto
{

  /// This class contains utility functions that are declared static.
  /// The functions deals with files (loading, creating, ...)
  class Utility
  {
  public:
    /// Takes a file and store it in a 'stringstream'. You can specify
    /// the 'read_mode' used to open the file with 'fopen'. You can specify
    /// special characters in 'stripped' that will be stripped (removed)
    /// from the loaded file. This is usefull for compatibility between
    /// windows and linux. It is actually a hack that have no reason to
    /// exist if files where opened in the correct mode.
    static void		readFileInStream (const char*		filename,
					  std::stringstream&	out,
					  const char*		read_mode,
					  const char*		stripped);

    /// This function return a filename that can be used to open a temporary
    /// file. The temporary file isn't actually created.
    static std::string	createTemporaryFile ();

    /// Remove a temporary file
    static void		removeTemporaryFile (const std::string& filename);

    /// Strip characters from a file. The characters to strip are contained
    /// in the string 'stripped'. You have to provide the size of the file
    /// in order for the function to be able to process binary files
    static int		stripCharacters(char*		msg,
					const char*	stripped,
					int		prev_size);

  public:

    /// Hold the name of a temporary directory where we can create
    /// temporary files. By default this pointer is NULL.
    static std::string*	temporary_directory;


  private:
    /// Calculate the sum of the bytes of a string. It's a sort of checksum
    static unsigned int	byte_sum (unsigned char *s, int size);
  };
}

#endif
