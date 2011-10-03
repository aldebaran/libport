/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{
  template <typename T>
  inline
  const StreamWrapper&
  StreamWrapper::operator<<(const T& e) const
  {
    stream_ << e;
    return *this;
  }

  std::string
  inline
  StreamWrapper::str() const
  {
    return stream_.str();
  }
}
