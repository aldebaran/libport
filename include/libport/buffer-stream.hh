/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_BUFFER_STREAM_HH
# define LIBPORT_BUFFER_STREAM_HH

# include <iostream>
# include <streambuf>

# include <libport/export.hh>

namespace libport
{
  /// Input stream similar to stringstream, without copying its data source.
  class LIBPORT_API BufferStream: public std::iostream
  {
  public:
    BufferStream(const char* data, size_t length);
    virtual ~BufferStream();
  };
}

#endif
