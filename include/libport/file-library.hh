/**
 ** \file libport/file-library.hh
 ** \brief Manage sets of inclusion paths.
 */

#ifndef LIBPORT_FILE_LIBRARY_HH
# define LIBPORT_FILE_LIBRARY_HH

# include <string>
# include <list>
# include "libport/path.hh"
# include <boost/exception.hpp>

namespace libport
{
  /** \brief Manage search path.

      Store search path and all informations used for handling
      paths when processing import directives. */
  class file_library
  {
  public:

    /// Exception thrown when a file is not localisable
    class Not_found : public boost::exception
    {};

    /// \name Constructor.
    /// \{
    /// An empty library.
    file_library ();
    /// Init the library with one path.
    file_library (path p);
    /// \}

    /// \name Managing inclusion paths.
    /// \{
    void append_dir (path p);
    void push_back (path p);
    void prepend_dir (path p);
    void push_front (path p);
    /// \}

    /// \name current directory.
    /// \{
    void push_current_directory (path p);
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

  private:
    /// Push the working directory on the stack.
    void push_cwd ();

    /** \brief Find file "\a relative_path / \a filename" using include path.
	\return Absolute path where the file lies or empty path
		if the file does not exist. */
    path find_in_search_path (const path& relative_path,
			      const std::string& filename) const;

    /** \brief Split the string with character ':', and insert each
	resultant string as a new search path. */
    void append_dir_list (std::string path_list);

    /** \brief Ensure that path is absolute by prepending current
	directory if necessary */
    path ensure_absolute_path (path p) const;

    typedef std::list<path> path_list_type;

    /// Inclusion path list.
    path_list_type search_path_;

    /// Current directory stack.
    path_list_type current_directory_;
  };

  std::ostream&
  operator<< (std::ostream& ostr, const file_library& l);

}

# include "libport/file-library.hxx"

#endif // !LIBPORT_FILE_LIBRARY_HH
