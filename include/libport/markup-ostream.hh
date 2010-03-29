/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_MARKUP_OSTREAM_HH
# define LIBPORT_MARKUP_OSTREAM_HH

# include <boost/function.hpp>

# include <libport/export.hh>
# include <libport/io-stream.hh>

namespace libport
{
  class MarkupStreamBuffer;

  class LIBPORT_API MarkupOStream: public libport::IOStream
  {
  public:
    typedef libport::IOStream super_type;
    MarkupOStream(std::ostream& output);

    void col();
    void etable();
    void row();
    void table();

  private:
    MarkupStreamBuffer* buffer_;
  };

  LIBPORT_API std::ostream& col(std::ostream& where);
  LIBPORT_API std::ostream& etable(std::ostream& where);
  LIBPORT_API std::ostream& row(std::ostream& where);
  LIBPORT_API std::ostream& table(std::ostream& where);
}

#endif
