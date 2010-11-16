/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_OPTION_PARSER_HH
# define LIBPORT_OPTION_PARSER_HH

// See http://lists.boost.org/Archives/boost/2009/05/151305.php, and
// in particular
// http://lists.boost.org/Archives/boost/2009/05/151318.php:
//
//    Of course, one could just turn off strict aliasing for gcc,
//    using the -fno-strict-aliasing option. Doing so would put you in
//    good company; many large projects (including the Linux kernel,
//    BSD, rtems, Python, bjam(!), others that I don't recall off the
//    top of my head) turn off strict aliasing, with such reasons as
//    the following cited
//
//    - the strict aliasing rules and their implications are arcane,
//
//    - the strict aliasing rules forbid idioms common in low-level
//    system programming type code,
//
//    - the methods for dealing with strict aliasing in such low-level
//    system code make the code harder to understand,
//
//    - the benefits derived from enabling strict aliasing are not
//    worth the headaches
//
//    - tools (including compilers that support strict aliasing) don't
//    (and perhaps can't) provide good diagnostics about
//    violations. gcc, for example, specifically documents that all
//    levels for enabled strict aliasing warnings can produce both
//    false positives and false negatives.

// Bottom line is: Boost.Optional is not strict-aliasing safe, so
//
// ## --------------------------------------------------------------- ##
// ## users of this file must be compiled with -fno-strict-aliasing.  ##
// ## --------------------------------------------------------------- ##
//
// Unfortunately we cannot use the pragma/attribute
// `optimize("no-strict-aliasing")':
// https://bugzilla.redhat.com/show_bug.cgi?id=486176.

# include <exception>

# include <boost/optional.hpp>
# include <boost/function.hpp>

# include <libport/export.hh>
# include <libport/program-name.hh>

namespace libport
{
  typedef cli_args_type args_type;


  /*--------.
  | Error.  |
  `--------*/

  class LIBPORT_API Error: public std::exception
  {
  public:
    typedef std::vector<std::string> errors_type;
    Error(const std::string& msg);
    Error(const errors_type& errors);
    virtual ~Error() throw ();
    const errors_type& errors() const;
    virtual const char* what() const throw ();

  private:
    errors_type errors_;
  };


  /*---------.
  | Option.  |
  `---------*/

  class LIBPORT_API Option
  {
  public:
    Option(const std::string& doc);
    virtual ~Option();
    virtual bool test(cli_args_type& args) = 0;
    virtual void init() = 0;
    typedef boost::function0<void> callback_type;
    typedef boost::optional<callback_type> callback_opt_type;
    void set_callback(const callback_type& callback);
    void usage(std::ostream& output) const;
    void doc(std::ostream& output) const;
    void callback() const;

  protected:
    virtual void usage_(std::ostream& output) const = 0;
    virtual void doc_(std::ostream& output) const = 0;
    std::string documentation_;
    callback_opt_type callback_;
  };


  /*--------------.
  | OptionNamed.  |
  `--------------*/

  class LIBPORT_API OptionNamed: public Option
  {
  public:
    OptionNamed(const std::string& doc,
                const std::string& name_long,
               char name_short = '\0');
    bool test_name(const std::string& arg) const;

  protected:
    virtual void usage_(std::ostream& output) const;
    virtual void doc_(std::ostream& output) const;
    std::string name_long_;
    char name_short_;
  };

  /*-------------.
  | OptionFlag.  |
  `-------------*/

  class LIBPORT_API OptionFlag: public OptionNamed
  {
  public:
    OptionFlag(const std::string& doc,
               const std::string& name_long,
               char name_short = '\0');
    virtual bool test(cli_args_type& args);
    virtual void init();
    bool get() const;

  protected:
    virtual void usage_(std::ostream& output) const;
    virtual void doc_(std::ostream& output) const;

  private:
    bool value_;
  };

  /*---------------.
  | OptionValued.  |
  `---------------*/

  class LIBPORT_API OptionValued: public OptionNamed
  {
  public:
    typedef boost::optional<std::string> ostring;
    using Option::set_callback;
    typedef boost::function1<void, const std::string&> callback_type;
    OptionValued(const std::string& doc,
                 const std::string& name_long,
                 const std::string& formal,
                 char name_short = '\0',
                 const callback_type& cb = callback_type());
    void set_callback(const callback_type& callback);
    ostring test_option(cli_args_type& args);

  protected:
    virtual void usage_(std::ostream& output) const;
    virtual void doc_(std::ostream& output) const;
    callback_type callback1_;

  private:
    std::string formal_;
  };

  /*--------------.
  | OptionValue.  |
  `--------------*/

  class LIBPORT_API OptionValue: public OptionValued
  {
  public:
    OptionValue(const std::string& doc,
                const std::string& name_long,
                char name_short = '\0',
		const std::string& formal = "",
                const callback_type& cb = callback_type());
    virtual bool test(cli_args_type& args);
    virtual void init();
    std::string value(const boost::optional<std::string>& def
                      = boost::optional<std::string>()) const;
    std::string value(const char* def) const;

    template <typename T> T get(const boost::optional<T>& def) const;

    // Used to be implemented as a default argument to the previous
    // function, but MSVC dies with:
    //
    // error C2440: 'default argument' : cannot convert from
    // 'boost::optional<T>' to 'const boost::optional<T> &'
    //  with
    //  [
    //      T=int
    //  ]
    //  Reason: cannot convert from 'boost::optional<T>'
    //   to 'const boost::optional<T>'
    //  with
    //  [
    //      T=int
    //  ]
    //  No user-defined-conversion operator available that can
    //  perform this conversion, or the operator cannot be called.
    template <typename T> T get() const;

    bool filled() const;

  private:
    bool filled_;
    std::string value_;
  };


  /*---------------.
  | OptionValues.  |
  `---------------*/

  class LIBPORT_API OptionValues: public OptionValued
  {
  public:
    typedef std::vector<std::string> values_type;
    OptionValues(const std::string& doc,
		 const std::string& name_long,
		 char name_short = '\0',
		 const std::string& formal = "",
                 const callback_type& cb = callback_type());
    virtual bool test(cli_args_type& args);
    virtual void init();
    const values_type& get() const;

  private:
    values_type values_;
  };

  /*-------------.
  | OptionsEnd.  |
  `-------------*/

  class LIBPORT_API OptionsEnd: public Option
  {
  public:
    typedef std::vector<std::string> values_type;
    OptionsEnd(bool end_on_nonoption = false);
    virtual bool test(cli_args_type& args);
    virtual void init();
    const values_type& get() const;
    virtual void usage_(std::ostream& output) const;
    virtual void doc_(std::ostream& output) const;
    bool found_separator() const;
  private:
    values_type values_;
    bool end_on_nonoption_;
    bool found_separator_;
  };


  /*---------------.
  | OptionParser.  |
  `---------------*/

  class LIBPORT_API OptionParser
  {
  public:
    cli_args_type operator () (const cli_args_type& args);
    cli_args_type operator () (int argc, const char** argv);
    cli_args_type operator () (int argc, char** argv);
    OptionParser& operator << (Option& opt);
    OptionParser& operator << (const std::string& doc);
    void usage(std::ostream& ouput) const;
    void options_doc(std::ostream& ouput) const;

  private:
    std::vector<Option*> options_;
    std::vector<std::string> doc_;
    Error::errors_type errors_;
  };

  /// Bounce to options_doc.
  std::ostream& operator<<(std::ostream& o, const OptionParser& p);

  /*---------------------.
  | Predefined options.  |
  `---------------------*/

  namespace opts
  {
    // -f, --file=FILE
    // load file.
    extern LIBPORT_API OptionValues files;

    // -h, --help
    // display this message and exit successfully.
    extern LIBPORT_API OptionFlag help;

    // -H, --host=HOST
    // address to connect to (client).
    extern LIBPORT_API OptionValue host;

    // -H, --host=HOST
    // address to listen to (server).
    extern LIBPORT_API OptionValue host_l;

    // -P, --port=PORT
    // port to connect to (client).
    extern LIBPORT_API OptionValue port;

    // --port-file=FILE
    // file containing the port to listen to (client).
    extern LIBPORT_API OptionValue port_file;

    // -w, --port-file=FILE
    // write port number to the specified file (server).
    extern LIBPORT_API OptionValue port_file_l;

    // -P, --port=PORT
    // port to listen to, 0 for automatic selection (server).
    extern LIBPORT_API OptionValue port_l;

    // -v, --verbose
    // be more verbose.
    extern LIBPORT_API OptionFlag verbose;

    // --version
    // display version information.
    extern LIBPORT_API OptionFlag version;
  }
}

# include <libport/option-parser.hxx>

#endif
