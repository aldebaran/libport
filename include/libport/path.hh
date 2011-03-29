/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/path.hh
 ** \brief Declaration of libport::path.
 */

#ifndef LIBPORT_PATH_HH
# define LIBPORT_PATH_HH

# include <string>
# include <list>
# include <libport/export.hh>
# include <libport/detect-win32.h>

# define BOOST_FILESYSTEM_VERSION 2
# include <boost/filesystem.hpp>

namespace libport
{
  /** \brief Paths in filesystems, i.e., file names.
   **/
  class LIBPORT_API path
  {
  public:
    /// Exception thrown on invalid path
    class LIBPORT_API invalid_path : public std::exception
    {
    public:
      invalid_path(const std::string& msg);
      const char* what() const throw ();
      virtual ~invalid_path() throw ();
    private:
      std::string msg_;
    };

    /// The "native" type we wrap.
    typedef boost::filesystem::path value_type;

    /// \name Constructors.
    /// \{

    /// Init object with \a path.
    /** @throw invalid_path if \a p isn't a valid path
     */
    path(const std::string& p);

    /** @throw invalid_path if \a p isn't a valid path
     */
    path(const char* p);

    /** @throw invalid_path if \a p isn't a valid path
     */
    path(const value_type& p);
    /// \}

    /// \name Operations on path.
    /// \{
#ifdef SWIG
    %rename (assign) operator=(const path& rhs);
#endif
    path& operator=(const path& rhs);
    /** @throw invalid_path if \a rhs is absolute.
     */
    path& operator/=(const path& rhs);
    /** @throw invalid_path if \a rhs is absolute.
     */
    path operator/(const path& rhs) const;
    bool operator==(const path& rhs) const;

    std::string basename() const;
    /// The extension of the file, or "" if not applicable.
    std::string extension() const;
    /// Return the volume.  Empty, unless on windows, in which
    /// case it looks like "c:".
    std::string volume_get() const;
    path dirname() const;
    bool exists() const;
    /// Remove the file.
    /// \return true iff no error.
    void remove() const;
    bool create() const;
    void rename(const std::string& dst);
    /// \}

    /// Return the current working directory.
    static path cwd();

    /// Whether the path is the root of a drive.
    bool is_root() const;

    /// Return a path which is the parent directory.
    /// If path is relative, current directory is
    /// returned when asking for parent of 1-depth entity.
    path parent() const;

    /// Return last write time WITHOUT local time offset.
    std::time_t last_write_time() const;

    static path temporary_file();

    /// \name Printing and converting.
    /// \{
#ifdef SWIG
    %rename (__str__) operator std::string() const;
#endif
    /// path is represented with a list of directories.
    typedef std::list<std::string> path_type;
    std::string to_string() const;
    operator std::string() const;
    path_type components() const;
    std::ostream& dump(std::ostream& ostr) const;
    /// \}

    /// Whether is absolute.
    bool absolute_get() const;

    /// Get boost::filesystem::path object.
    const value_type& value_get() const;

    /// Get boost::filesystem::path object.
    value_type& value_get();

  private:

    /// Clean a clean path string from value_.
    std::string clean() const;

    /// Init object with path \a p.
    void init(const std::string& p);

    /// Boost Object.
    value_type value_;

    /// Path separator.
    static const char separator_ = WIN32_IF('\\', '/');
  };

  LIBPORT_API
  std::string format_boost_fs_error(const char*);

  /// Dump \a p on \a o.
  std::ostream& operator<<(std::ostream& o, const path& p);
}

# include <libport/path.hxx>

#endif // !LIBPORT_PATH_HH
