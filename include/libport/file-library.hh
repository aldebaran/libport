/**
 ** \file libport/file-library.hh
 ** \brief Manage sets of inclusion paths.
 */

#ifndef LIBPORT_FILE_LIBRARY_HH
# define LIBPORT_FILE_LIBRARY_HH

# include <string>
# include <list>
# include <libport/export.hh>
# include <libport/path.hh>
# include <exception>

namespace libport
{
  /** \brief Manage search path.

      Store search path and all informations used for handling
      paths when processing import directives. */
  class LIBPORT_API file_library
  {
  public:

    typedef std::list<path> path_list_type;

    /// Exception thrown when a file cannot be located.
    class LIBPORT_API Not_found : public std::exception
    {};

    /// \name Constructor.
    /// \{
    /// An empty library.
    file_library ();
    /// Init the library with one path.
    file_library (const path& p);
    /// Init the library with a string to split at \a separator.
    file_library (const std::string& library, const char* separator);
    /// \}

    /// \name Managing inclusion paths.
    /// \{
    void push_back (const path& p);
    /// Split \a library on \a separator and put at the end of path list.
    void push_back (const std::string& library, const char* separator);
    void append (const path& p);

    void push_front (const path& p);
    /// Split \a library on \a separator and put at the beginning of path list.
    /// \warning "a:b:c" will end with "c" first, then "b", then "a",
    /// then what was already here.
    void push_front (const std::string& library, const char* separator);
    void prepend (const path& p);
    /// \}

    /// \name current directory.
    /// \{
    void push_current_directory (const path& p);
    void pop_current_directory ();
    path current_directory_get () const;
    /// \}

    /// \name Findind file
    /// \{
    /** \brief Search a file.

	Determine real path of \a file, by looking in search path if
	necessary.

	\return Directory containing \a file, or "" if not found. */
    path find_file (const path& file);

    /// \brief Check if \a file exists in directory \a dir.
    bool find_in_directory (const path& dir, const std::string& file) const;
    /// \}

    /// \name Printing.
    /// \{
    std::ostream& dump (std::ostream& ostr) const;
    /// \}

    /// \name Accessor.
    /// \{
    const path_list_type& search_path_get() const;
    /// \}

  private:
    /// Push the working directory on the stack.
    void push_cwd ();

    /** \brief Find file "\a relative_path / \a filename" using include path.
	\return Absolute path where the file lies or empty path
		if the file does not exist. */
    path find_in_search_path (const path& relative_path,
			      const std::string& filename) const;

    /** \brief Ensure that path is absolute by prepending current
	directory if necessary */
    path ensure_absolute_path (const path& p) const;

    /// Inclusion path list.
    path_list_type search_path_;

    /// Current directory stack.
    path_list_type current_directory_;
  };

  /// Print \a l on \a o.
  std::ostream& operator<< (std::ostream& o, const file_library& l);

}

# include <libport/file-library.hxx>

#endif // !LIBPORT_FILE_LIBRARY_HH
