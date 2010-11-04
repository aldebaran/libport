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
 ** \file libport/file-library.hxx
 ** \brief implements inline function of libport/file-library.hh
 */

#ifndef LIBPORT_FILE_LIBRARY_HXX
# define LIBPORT_FILE_LIBRARY_HXX

# include <libport/foreach.hh>
# include <libport/range.hh>

namespace libport
{

  template <class ForwardRange>
  file_library::file_library(const ForwardRange& r, const char* sep)
  {
    push_cwd();
    push_back(r, sep);
  }

  template <class ForwardRange>
  file_library&
  file_library::push_back(const ForwardRange& r, const char* sep)
  {
    bool inserted = false;
    typename boost::range_iterator<const ForwardRange>::type
      first = boost::const_begin(r);
    if (first != boost::const_end(r))
    {
      if (first->empty())
        // Insert the following search path component.
        push_back(skip_first(r), sep);
      else
        foreach (const std::string& s, split(*first, sep))
        {
          if (!s.empty())
            push_back(s);
          else if (!inserted)
          {
            // Insert the following search path component.
            push_back(skip_first(r), sep);
            inserted = true;
          }
        }
    }
    return *this;
  }

  template <class Iterable>
  file_library&
  file_library::push_back(const std::string& user_path,
                          const Iterable& default_path,
                          const char* sep)
  {
    bool inserted = false;
    if (user_path.empty())
    {
      // Insert the following search path component.
      foreach (const std::string& s, default_path)
        if (!s.empty())
          push_back(s);
    }
    else
    {
      foreach (const std::string& s1, split(user_path, sep))
      {
        if (!s1.empty())
          push_back(s1);
        else if (!inserted)
        {
          foreach (const std::string& s2, default_path)
            if (!s2.empty())
              push_back(s2);
          inserted = true;
        }
      }
    }
    return *this;
  }

  inline file_library&
  file_library::append(const path& p)
  {
    return push_back(p);
  }

  inline file_library&
  file_library::prepend(const path& p)
  {
    return push_front(p);
  }

  inline const file_library::path_list_type&
  file_library::search_path_get() const
  {
    return search_path_;
  }

  inline file_library::path_list_type&
  file_library::search_path()
  {
    return search_path_;
  }

  inline std::ostream&
  operator<<(std::ostream& ostr, const file_library& l)
  {
    return l.dump(ostr);
  }
}

#endif // !LIBPORT_FILE_LIBRARY_HXX
