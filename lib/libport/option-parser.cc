/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <boost/algorithm/string.hpp>

#include <libport/cassert>
#include <libport/containers.hh>
#include <libport/foreach.hh>
#include <libport/format.hh>
#include <libport/markup-ostream.hh>
#include <libport/option-parser.hh>

namespace libport
{

  /*--------.
  | Error.  |
  `--------*/

  Error::Error(const std::string& msg)
    : errors_()
  {
    errors_.push_back(msg);
  }

  Error::Error(const errors_type& errors)
    : errors_(errors)
  {}

  Error::~Error() throw ()
  {}

  const Error::errors_type&
  Error::errors() const
  {
    return errors_;
  }

  const char*
  Error::what() const throw ()
  {
    std::string res;
    foreach (const std::string& error, errors_)
      res += error + "\n";
    // Remove the last \n.
    res.resize(res.size() - 1);
    // FIXME: Returning pointer to destructed value.
    return res.c_str();
  }

  /*---------.
  | Option.  |
  `---------*/

  Option::Option(const std::string& doc)
    : documentation_(doc)
    , callback_()
  {}

  Option::~Option()
  {}

  void
  Option::set_callback(const callback_type& callback)
  {
    callback_ = callback;
  }

  void
  Option::usage(std::ostream& output) const
  {
    if (!documentation_.empty())
      usage_(output);
  }

  void
  Option::doc(std::ostream& output) const
  {
    if (!documentation_.empty())
      doc_(output);
  }

  void
  Option::callback() const
  {
    if (callback_)
      (*callback_)();
  }

  /*--------------.
  | OptionNamed.  |
  `--------------*/

  OptionNamed::OptionNamed(const std::string& doc,
                           const std::string& name_long,
                           char name_short)
    : Option(doc)
    , name_long_(name_long)
    , name_short_(name_short)
  {}

  bool
  OptionNamed::test_name(const std::string& arg) const
  {
    return arg == "--" + name_long_
      || (name_short_ && arg == std::string("-") + name_short_);
  }

  void
  OptionNamed::usage_(std::ostream& output) const
  {
    output << "--" << name_long_;
  }

  void
  OptionNamed::doc_(std::ostream& output) const
  {
    if (name_short_)
      output << "  -" << name_short_ << ", ";
    else
      output << "      ";
    output << "--" << name_long_;
  }

  /*-------------.
  | OptionFlag.  |
  `-------------*/

  OptionFlag::OptionFlag(const std::string& doc,
                         const std::string& name_long,
                         char name_short)
    : OptionNamed(doc, name_long, name_short)
    , value_(false)
  {}

  void
  OptionFlag::init()
  {
    value_ = false;
  }

  bool
  OptionFlag::test(cli_args_type& args)
  {
    std::string arg = args[0];
    bool res = false;

    if (test_name(arg))
    {
      args.erase(args.begin());
      res = true;
    }
    else if (name_short_ && arg[0] == '-' && arg[1] != '-')
    {
      size_t pos = arg.find(name_short_);
      if (pos != std::string::npos)
      {
        args[0] = arg.substr(0, pos) + arg.substr(pos + 1, std::string::npos);
        res = true;
      }
    }

    value_ = value_ || res;
    return res;
  }

  bool
  OptionFlag::get() const
  {
    return value_;
  }

  void
  OptionFlag::usage_(std::ostream& output) const
  {
    output << " [";
    OptionNamed::usage_(output);
    output << "]";
  }

  void
  OptionFlag::doc_(std::ostream& output) const
  {
    OptionNamed::doc_(output);
    output << " " << col << documentation_;
  }


  /*---------------.
  | OptionValued.  |
  `---------------*/

  OptionValued::OptionValued(const std::string& doc,
                             const std::string& name_long,
                             const std::string& formal,
			     char name_short,
                             const callback_type& cb)
    : OptionNamed(doc, name_long, name_short)
    , callback1_()
    , formal_(formal.empty() ?  boost::to_upper_copy(name_long) : formal)
  {
    set_callback(cb);
  }

  void
  OptionValued::set_callback(const callback_type& callback)
  {
    callback1_ = callback;
  }

  void
  OptionValued::usage_(std::ostream& output) const
  {
    output << " [";
    OptionNamed::usage_(output);
    output << "=" << formal_ << "]";
  }

  void
  OptionValued::doc_(std::ostream& output) const
  {
    OptionNamed::doc_(output);
    output << "=" << formal_ << " " << col << documentation_;
  }

  OptionValued::ostring
  OptionValued::test_option(cli_args_type& args)
  {
    size_t pos = args[0].find("=");
    // s/=.*//.
    std::string name =
      pos != std::string::npos ? args[0].substr(0, pos) : args[0];

    ostring res;
    if (test_name(name))
    {
      if (pos != std::string::npos)
        res = args[0].substr(pos + 1, std::string::npos);
      else
      {
        if (args.size() < 2)
          throw Error(format("option `--%s' requires an argument", name_long_));
        args.erase(args.begin());
        res = args[0];
      }
      args.erase(args.begin());
    }

    if (res && callback1_)
      callback1_(res.get());
    return res;
  }


  /*--------------.
  | OptionValue.  |
  `--------------*/

  OptionValue::OptionValue(const std::string& doc,
                           const std::string& name_long,
                           char name_short,
			   const std::string& formal,
                           const callback_type& cb)
    : OptionValued(doc, name_long, formal, name_short, cb)
    , filled_(false)
    , value_()
  {}

  void
  OptionValue::init()
  {
    filled_ = false;
    value_ = "";
  }

  bool
  OptionValue::test(cli_args_type& args)
  {
    ostring res = test_option(args);

    if (res)
    {
      filled_ = true;
      value_ = res.get();
      return true;
    }
    return false;
  }

  bool
  OptionValue::filled() const
  {
    return filled_;
  }

  std::string
  OptionValue::value(const boost::optional<std::string>& def) const
  {
    if (!filled_)
    {
      aver(def);
      return def.get();
    }
    return value_;
  }

  std::string
  OptionValue::value(const char* def) const
  {
    return value(std::string(def));
  }


  /*---------------.
  | OptionValues.  |
  `---------------*/

  OptionValues::OptionValues(const std::string& doc,
                             const std::string& name_long,
                             char name_short,
			     const std::string& formal,
                             const callback_type& cb)
    : OptionValued(doc, name_long, formal, name_short, cb)
    , values_()
  {}

  void
  OptionValues::init()
  {
    values_.clear();
  }

  bool
  OptionValues::test(cli_args_type& args)
  {
    ostring res = test_option(args);

    if (res)
    {
      values_.push_back(res.get());
      return true;
    }
    return false;
  }

  const OptionValues::values_type&
  OptionValues::get() const
  {
    return values_;
  }

  /*-------------.
  | OptionsEnd.  |
  `-------------*/

  OptionsEnd::OptionsEnd(bool end_on_nonoption)
    : Option("")
    , end_on_nonoption_(end_on_nonoption)
  {}

  bool
  OptionsEnd::test(cli_args_type& args)
  {
    bool isMinusMinus = (args[0] == "--");

    if (isMinusMinus ||
        (end_on_nonoption_ && !args[0].empty() && args[0][0] != '-'))
    {
      found_separator_ = isMinusMinus;
      values_.insert(values_.end(), args.begin() + (isMinusMinus?1:0),
                     args.end());
      args.clear();
      return true;
    }
    else
      return false;
  }

  bool
  OptionsEnd::found_separator() const
  {
    return found_separator_;
  }

  void
  OptionsEnd::init()
  {
    values_.clear();
    found_separator_ = false;
  }

  const OptionsEnd::values_type&
  OptionsEnd::get() const
  {
    return values_;
  }

  void OptionsEnd::usage_(std::ostream&) const
  {}

  void OptionsEnd::doc_(std::ostream&) const
  {}

  /*---------------.
  | OptionParser.  |
  `---------------*/

  cli_args_type
  OptionParser::operator () (int argc, const char** argv)
  {
    cli_args_type args;

    foreach (const char* arg, std::make_pair(argv + 1, argv + argc))
      args << arg;
    return operator () (args);
  }

  cli_args_type
  OptionParser::operator () (int argc, char** argv)
  {
    return operator () (argc, const_cast<const char**>(argv));
  }

  cli_args_type
  OptionParser::operator() (const cli_args_type& _args)
  {
    cli_args_type args(_args);
    cli_args_type remainder;

    foreach (Option* opt, options_)
      opt->init();

    while (!args.empty())
    {
      foreach (Option* opt, options_)
        try
        {
          if (opt->test(args))
	  {
	    opt->callback();
            goto end;
	  }
        }
        catch (Error& e)
        {
          errors_.push_back(e.errors().front());
        }
      remainder.push_back(args.front());
      args.erase(args.begin());
      end: ;
    }

    if (!errors_.empty())
      throw Error(errors_);

    return remainder;
  }

  OptionParser&
  OptionParser::operator << (Option& opt)
  {
    options_.push_back(&opt);
    doc_.push_back("");
    return *this;
  }

  OptionParser&
  OptionParser::operator << (const std::string& doc)
  {
    doc_.push_back(doc);
    return *this;
  }

  void
  OptionParser::usage(std::ostream& output) const
  {
    output << program_name();
    foreach (Option* opt, options_)
      opt->usage(output);
  }

  void
  OptionParser::options_doc(std::ostream& output) const
  {
    MarkupOStream stream(output);

    stream << table;
    std::vector<Option*>::const_iterator it = options_.begin();
    foreach (const std::string& str, doc_)
    {
      stream << row;
      if (str.empty())
	(*it++)->doc(stream);
      else
	stream << etable << row << str << table;
    }

//     foreach (Option* opt, options_)
//     {
//       stream << row;
//       opt->doc(stream);
//     }
    stream << etable;
  }

  namespace opts
  {
    OptionFlag
      help("output this message and exit successfully", "help",    'h'),
      verbose("be more verbose",                         "verbose", 'v'),
      version("output version information and exit successfully", "version");

    OptionValue
    // Client.
      host("address to connect to",    "host",    'H'),
      port("port to connect to",       "port",    'P'),
      port_file("read port number in FILE",
                "port-file", 0, "FILE"),
    // Server.
      host_l("address to listen to",   "host",    'H'),
      port_l("port to listen to, 0 for automatic selection", "port",  'P'),
      port_file_l("write port number to FILE",
                  "port-file", 'w', "FILE");

    OptionValues
      files("load file",     "file",    'f');
  }
}
