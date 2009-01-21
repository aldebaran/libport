/**
 ** \file libport/path.hh
 ** \brief path: represents an absolute path in the filesystem
 */

#ifndef LIBPORT_PATH_HH
# define LIBPORT_PATH_HH

# include <string>
# include <list>
# include <libport/export.hh>
# include <libport/detect-win32.h>
namespace libport
{
  /** \brief Helper for handling paths
   **/
  class LIBPORT_API path
  {
  public:
    /// Exception thrown on invalid path
    class LIBPORT_API invalid_path : public std::exception
    {
    public:
      invalid_path(const std::string& msg)
        : _msg(msg)
      {}
      const char* what() const throw ()
      {
        return _msg.c_str();
      }
      virtual ~invalid_path() throw () {};
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
    path operator/ (const path& rhs) const;
    bool operator== (const path& rhs) const;

    std::string basename() const;
    /// Return the volume.  Empty, unless on windows, in which
    /// case it looks like "c:".
    std::string volume_get() const;
    void volume_set(const std::string& v);
    path dirname() const;
    bool exists() const;
    bool create() const;
    /// \}

    /// \name Printing and converting.
    /// \{
#ifdef SWIG
    %rename (__str__) operator std::string () const;
#endif
    /// path is represented with a list of directories.
    typedef std::list<std::string> path_type;
    std::string to_string () const;
    operator std::string () const;
    const path_type& components() const;
    std::ostream& dump (std::ostream& ostr) const;
    /// \}

    /// Whether is absolute.
    bool absolute_get () const;

  private:

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

# include <libport/path.hxx>

#endif // !LIBPORT_PATH_HH
