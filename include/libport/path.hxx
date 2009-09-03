/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/**
 ** \file libport/path.hxx
 ** \brief Inline implementation of libport::path.
 */

#ifndef LIBPORT_PATH_HXX
# define LIBPORT_PATH_HXX

# include <libport/path.hh>

namespace libport
{

  /*---------------------.
  | Path::invalid_path.  |
  `---------------------*/

  inline
  path::invalid_path::invalid_path(const std::string& msg)
    : msg_(msg)
  {}

  inline
  const char*
  path::invalid_path::what() const throw ()
  {
    return msg_.c_str();
  }

  inline
  path::invalid_path::~invalid_path() throw ()
  {}


  /*-------.
  | path.  |
  `-------*/

  inline std::ostream&
  operator<<(std::ostream& o, const path& p)
  {
    return p.dump(o);
  }

  inline
  std::string
  path::volume_get() const
  {
    return WIN32_IF(volume_, "");
  }

  inline bool
  path::absolute_get() const
  {
    return absolute_;
  }
}

#endif // !LIBPORT_PATH_HXX
