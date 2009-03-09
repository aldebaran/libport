#ifndef LIBPORT_MARKUP_OSTREAM_HH
# define LIBPORT_MARKUP_OSTREAM_HH

# include <boost/function.hpp>

# include <libport/export.hh>
# include <libport/io-stream.hh>

namespace libport
{
  class MarkupStreamBuffer;

  class LIBPORT_API MarkupOStream: public libport::IOStream<MarkupStreamBuffer>
  {
  public:
    typedef libport::IOStream<MarkupStreamBuffer> super_type;
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
