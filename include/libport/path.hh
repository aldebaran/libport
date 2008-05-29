/**
 ** \file libport/path.hh
 ** \brief path: represents an absolute path in the filesystem
 */

#ifndef LIBPORT_PATH_HH
# define LIBPORT_PATH_HH

# include <string>
# include <list>

namespace libport
{
  /** \brief Helper for handling paths
   **/
  class path
  {
  public:
    /// Exception thrown on invalid path
    class invalid_path
    {
    public:
      invalid_path(const std::string& msg)
        : _msg(msg)
      {}
      std::string what()
      {
        return _msg;
      }
    private:
      std::string _msg;
    };

    /// \name Constructors.
    /// \{

    /// Init object with \a path.
    /** @throw invalid_path if \a p isn't a valid path
     */
    path (const std::string& p);

    /** @throw invalid_path if \a p isn't a valid path
     */
    path (const char* p);
    /// \}

    /// \name Operations on path.
    /// \{
#ifdef SWIG
    %rename (assign) operator= (const path& rhs);
#endif
    path& operator= (const path& rhs);
    /** @throw invalid_path if \a rhs is absolute.
     */
    path& operator/= (const path& rhs);
    /** @throw invalid_path if \a rhs is absolute.
     */
    path operator/ (const std::string& rhs) const;
    bool operator== (const path& rhs) const;

    std::string basename () const;
    path dirname () const;
    bool exists () const;

    /// \}

    /// \name Printing and converting.
    /// \{
#ifdef SWIG
    %rename (__str__) operator std::string () const;
#endif
    std::string to_string () const;
    operator std::string () const;
    std::ostream& dump (std::ostream& ostr) const;

    /// \}

    /// Whether is absolute.
    bool absolute_get () const;

  private:
    /// path is represented with a list of directories.
    typedef std::list<std::string> path_type;

    /// Append a single directory \a dir.
    void append_dir (const std::string& dir);

    /// Init object with path \a p.
    void init (std::string p);

    /// Test whether the given path is absolute and set attributes adequately.
    void test_absolute (std::string& p);

    /// Represented path.
    path_type path_;

    /// "absolute" flag.
    bool absolute_;

#ifdef WIN32
    /// Path separator
    static const char separator_ = '\\';

    /// Name of the volume (C:) for absolute paths under windows.
    std::string volume_;
#else
    /// Path separator
    static const char separator_ = '/';
#endif
  };

  /// Dump \a p on \a o.
  std::ostream& operator<< (std::ostream& o, const path& p);
}

# include "libport/path.hxx"

#endif // !LIBPORT_PATH_HH
