/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/compiler.hh>
#include <libport/escape.hh>
#include <libport/format.hh>
#include <libport/foreach.hh>
#include <libport/input-arguments.hh>

namespace libport
{
  namespace opts
  {

    /*-------.
    | Data.  |
    `-------*/
    Data::~Data()
    {}

    /*-----------.
    | TextData.  |
    `-----------*/
    TextData::TextData(const std::string& s)
      : command_(s)
    {}

    void
    TextData::accept(visitor_type& e) const
    {
      e.operator()(*this);
    }

    std::ostream&
    TextData::dump(std::ostream& o) const
    {
      return o << "expression {{{" << command_ << "}}}";
    }

    /*-----------.
    | FileData.  |
    `-----------*/
    FileData::FileData(const std::string& s)
      : filename_(s)
    {}

    void
    FileData::accept(visitor_type& e) const
    {
      e.operator()(*this);
    }

    std::ostream&
    FileData::dump(std::ostream& o) const
    {
      return o << "file `" << filename_ << "'";
    }


    /*-----------.
    | DataList.  |
    `-----------*/

    DataList::DataList()
      : super_type()
      , has_exps_(false)
      , has_files_(false)
      , has_modules_(false)
      , cb_exp_(boost::bind(&DataList::add_exp,
                            &input_arguments, _1))
      , cb_file_(boost::bind(&DataList::add_file,
                             &input_arguments, _1))
      , cb_module_(boost::bind(&DataList::add_module,
                               &input_arguments, _1))
    {
      exp.set_callback(cb_exp_);
      file.set_callback(cb_file_);
      module.set_callback(cb_module_);
    }

    DataList::~DataList()
    {}


    void
    DataList::add_exp(const std::string& arg)
    {
      has_exps_ = true;
      push_back(new TextData(arg));
    }

    void
    DataList::add_file(const std::string& arg)
    {
      has_files_ = true;
      push_back(new FileData(arg == "-" ? "/dev/stdin" : arg));
    }

    void
    DataList::add_module(const std::string& arg)
    {
      has_modules_ = true;
      push_back(new TextData(libport::format("loadModule(\"%s\");",
                             libport::escape(arg))));
    }

    void
    DataList::clear()
    {
      foreach (Data* d, *this)
        delete d;
      super_type::clear();
    }


    /*--------------.
    | DataVisitor.  |
    `--------------*/

    DataVisitor::~DataVisitor()
    {}

    void
    DataVisitor::operator()(const DataList& l)
    {
      foreach (Data* d, l)
        operator()(*d);
    }

    void
    DataVisitor::operator()(const Data& d)
    {
      d.accept(*this);
    }

    /*---------------------.
    | Predefined options.  |
    `---------------------*/

    OptionValues exp("send SCRIPT to the server",
                     "expression", 'e', "SCRIPT");
    OptionValues file("send the contents of FILE to the server",
                      "file", 'f', "FILE");
    OptionValues module("load the MODULE shared library",
                        "module", 'm', "MODULE");

    DataList input_arguments;
  }
}
