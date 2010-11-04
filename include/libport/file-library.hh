/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
  /** \brief Manage search paths.

      Store a search path and all informations used for handling
      paths when processing import directives. */
  class LIBPORT_API file_library
  {
  public:
    typedef file_library self_type;

    typedef std::list<path> path_list_type;

    /// Exception thrown when a file cannot be located.
    class LIBPORT_API Not_found : public std::exception
    {};

    /// \name Constructor.
    /// \{
    /// An empty library.
    file_library();
    /// Init the library with one path.
    file_library(const path& p);
    /// Init the library with a string to split at \a separator.
    file_library(const std::string& library, const char* separator);

    /// Handle a list of search-paths.
    /// \param r  the collection of search-paths.
    ///           each occurrence of an additional colon marks the
    ///           place where the following search-paths will be inserted.
    ///           If there is none, the remaining components are ignored.
    /// \param sep  the separator to split search-paths (typically colon).
    template <class ForwardRange>
      file_library(const ForwardRange& r, const char* sep);
    /// \}

    /// \name Managing inclusion paths.
    /// \{
    self_type& push_back(const path& p);
    /// Split \a library on \a separator and put at the end of path list.
    self_type& push_back(const std::string& library, const char* separator);

    /// Handle a list of search-paths.
    /// \param r  the collection of search-paths.
    ///           each occurrence of an additional colon marks the
    ///           place where the following search-paths will be inserted.
    ///           If there is none, the remaining components are ignored.
    /// \param sep  the separator to split search-paths (typically colon).
    template <class ForwardRange>
      self_type& push_back(const ForwardRange& r, const char* sep);

    template <class Iterable>
      self_type& push_back(const std::string& user_path,
                           const Iterable& default_path,
                           const char* sep);

    self_type& append(const path& p);

    self_type& push_front(const path& p);

    /// Split \a library on \a separator and put at the beginning of path list.
    /// \warning "a:b:c" will end with "c" first, then "b", then "a",
    /// then what was already here.
    self_type& push_front(const std::string& library, const char* separator);
    self_type& prepend(const path& p);
    /// \}

    /// \name current directory.
    /// \{
    void push_current_directory(const path& p);
    void pop_current_directory();
    path current_directory_get() const;
    /// \}


    typedef std::list<std::string> strings_type;
    /// Split \a lib at each occurrence of \a sep, return the list
    /// of components.
    /// Behaves especially on Windows when splitting on ":", in
    /// order to preserve drive prefixes (i.e., "c:foo:d:bar" is
    /// split in "c:foo", "d:bar").
    static strings_type split(const std::string& lib, const char* sep);


    /// \name Finding files.
    /// \{
    /** \brief Search a file.
     *
     * Determine real path of \a file, by looking in search path if
     * necessary.
     *
     * \return Directory containing \a file.
     * \throw  Not_found    file cannot be found,
     *                      in which case errno is set to ENOENT. */
    path find_file(const path& file) const;

    /// \brief Check if \a file exists in directory \a dir.
    bool find_in_directory(const path& dir, const std::string& file) const;
    /// \}

    /// \name Printing.
    /// \{
    std::ostream& dump(std::ostream& ostr) const;
    /// \}

    /// \name Accessor.
    /// \{
    const path_list_type& search_path_get() const;
    path_list_type& search_path();
    /// \}

  private:
    /// Push the working directory on the stack.
    void push_cwd();

    /** \brief Find file "\a relative_path / \a filename" using include path.
	\return Absolute    path where the file lies.
        \throw  Not_found   file not found. */
    path find_in_search_path(const path& relative_path,
                             const std::string& filename) const;

    /** \brief Ensure that path is absolute by prepending current
	directory if necessary */
    path ensure_absolute_path(const path& p) const;

    /// Inclusion path list.
    path_list_type search_path_;

    /// Current directory stack.
    path_list_type current_directory_;
  };

  /// Print \a l on \a o.
  std::ostream& operator<<(std::ostream& o, const file_library& l);

}

# include <libport/file-library.hxx>

#endif // !LIBPORT_FILE_LIBRARY_HH
