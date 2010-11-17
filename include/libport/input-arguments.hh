/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_INPUT_ARGUMENTS_HH
# define LIBPORT_INPUT_ARGUMENTS_HH

# include <list>
# include <libport/fwd.hh>
# include <libport/option-parser.hh>

namespace libport
{

  namespace opts
  {

    struct LIBPORT_API Data
    {
      typedef DataVisitor visitor_type;
      virtual ~Data();
      virtual void accept(visitor_type& e) const = 0;
      /// Print.
      virtual std::ostream& dump(std::ostream& o) const = 0;
    };

    std::ostream& operator<< (std::ostream& o, const Data& d);

    struct LIBPORT_API TextData: Data
    {
      TextData(const std::string& s);

      virtual void accept(visitor_type& e) const;
      virtual std::ostream& dump(std::ostream& o) const;

      std::string command_;
    };


    struct LIBPORT_API FileData: Data
    {
      FileData(const std::string& s);

      virtual void accept(visitor_type& e) const;
      virtual std::ostream& dump(std::ostream& o) const;

      std::string filename_;
    };

    struct LIBPORT_API DataVisitor
    {
      virtual ~DataVisitor();
      virtual void operator()(const Data& d);
      virtual void operator()(const DataList& l);
      virtual void operator()(const TextData& d) = 0;
      virtual void operator()(const FileData& d) = 0;
    };

    /*-----------.
    | DataList.  |
    `-----------*/

    struct LIBPORT_API DataList : std::list<Data*>
    {
      typedef std::list<Data*> super_type;
      DataList();
      virtual ~DataList();
      void clear();
      void add_exp(const std::string& arg);
      void add_file(const std::string& arg);
      void add_module(const std::string& arg);
      bool has_exps() const;
      bool has_files() const;
      bool has_modules() const;

      typedef boost::function1<void, const std::string&> callback_type;
      bool has_exps_;
      bool has_files_;
      bool has_modules_;
      callback_type cb_exp_;
      callback_type cb_file_;
      callback_type cb_module_;
    };

  /*---------------------.
  | Predefined options.  |
  `---------------------*/

    extern LIBPORT_API OptionValues exp;
    extern LIBPORT_API OptionValues file;
    extern LIBPORT_API OptionValues module;
    extern LIBPORT_API DataList input_arguments;
  }
}

# include <libport/input-arguments.hxx>

#endif
