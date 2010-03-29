/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <vector>

#include <boost/function.hpp>

#include <libport/cassert>
#include <libport/foreach.hh>
#include <libport/markup-ostream.hh>

namespace libport
{
  class MarkupStreamBuffer: public libport::StreamBuffer
  {
  public:
    MarkupStreamBuffer(std::ostream& output)
      : output_(output)
      , in_table_(false)
      , table_()
    {}

    void col()
    {
      sync();
      table_.back().push_back("");
    }

    static void box(std::ostream& tgt, const std::string& data, size_t size)
    {
      aver(data.size() <= size);
      tgt << data;
      for (size_t i = data.size(); i < size; ++i)
        tgt << ' ';
    }

    void etable()
    {
      sync();
      passert(in_table_, "not in table mode");
      typedef std::vector<size_t> widths_type;
      widths_type widths;
      foreach (const row_type& row, table_)
      {
        widths_type::iterator width = widths.begin();
        foreach (const std::string& cell, row)
        {
          if (width == widths.end())
          {
            widths.push_back(0);
            width = widths.end() - 1;
          }
          *width = std::max(*width, cell.size());
          ++width;
        }
      }

      foreach (const row_type& row, table_)
      {
        row_type::const_iterator cell = row.begin();
        foreach (size_t width, widths)
        {
          if (cell == row.end())
            break;
          if (cell != row.end() - 1)
          {
            box(output_, *cell, width);
            output_ << ' ';
          }
          else
            output_ << *cell;
          ++cell;
        }
        output_ << std::endl;
      }
      table_.clear();
      in_table_ = false;
    }

    void row()
    {
      sync();
      table_.push_back(row_type());
    }

    void table()
    {
      sync();
      passert(!in_table_, "already in table mode");
      in_table_ = true;
    }

  protected:
    typedef std::vector<std::string> row_type;
    typedef std::vector<row_type> table_type;

    virtual size_t read(char*, size_t)
    {
      pabort("cannot read from MarkupOStream");
    }

    virtual void write(char* buffer, size_t size)
    {
      std::string data(buffer, size);
      if (in_table_)
      {
        if (table_.empty())
          table_.push_back(row_type());
        if (table_.back().empty())
          table_.back().push_back("");
        table_.back().back() += data;
      }
      else
        output_ << data;
    }

  private:
    std::ostream& output_;
    bool in_table_;
    table_type table_;
  };

  MarkupOStream::MarkupOStream(std::ostream& output)
    : libport::IOStream(buffer_ = new MarkupStreamBuffer(output))
  {}

#define MOD(Name)                                                       \
  std::ostream& Name(std::ostream& where)                               \
  {                                                                     \
    assert_exp(dynamic_cast<MarkupOStream*>(&where))->Name();           \
    return where;                                                       \
  }                                                                     \
                                                                        \
  void MarkupOStream::Name()                                            \
  {                                                                     \
    buffer_->Name();                                                    \
  }                                                                     \

  MOD(col);
  MOD(etable);
  MOD(row);
  MOD(table);
#undef MOD
}
