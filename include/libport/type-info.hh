/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_TYPE_INFO_HH
# define LIBPORT_TYPE_INFO_HH

# include <string>

namespace libport
{
  class TypeInfo
  {
  public:
    TypeInfo(const ::std::type_info& info);
    template <typename T>
    explicit TypeInfo(const T& obj);

    bool operator <(const TypeInfo& other) const;
    bool operator==(const TypeInfo& other) const;

    std::string name() const;

  private:
    const ::std::type_info* info_;
  };

  std::ostream& operator << (std::ostream& out, const TypeInfo& info);
}

# include <libport/type-info.hxx>

#endif
