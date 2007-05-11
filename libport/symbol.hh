/**
 ** \file libport/symbol.hh
 ** \brief Definition of libport::Symbol.
 */

#ifndef LIBPORT_SYMBOL_HH
# define LIBPORT_SYMBOL_HH

# include <set>
# include <string>
# include <iosfwd>

namespace libport
{

  /** \brief Define class Symbol.
   **
   ** Map any string to a unique reference.
   ** This allows to avoid an "strcmp ()" style comparison of strings:
   ** reference comparison is much faster.
   */
  class Symbol
  {
    /// The type "set of strings".
    typedef std::set<std::string> string_set_type;
    /// The type for the size of string map.
    typedef string_set_type::size_type string_size_type;

    /** \name Ctor & Dtor.
     ** \{ */
  public:
    /** \brief Construct a Symbol.
     ** \param s referenced string */
    Symbol (const std::string& s);
    /** \brief Construct a Symbol.
     ** \param s referenced string */
    Symbol (const char* s);
    /** \} */

    /** \name Accessors.
     ** \{ */
  public:
    /// Return the string referenced by this Symbol.
    const std::string& name_get () const;
    operator const std::string& () const;

    /// Return the number of referenced strings.
    static string_size_type string_map_size ();
    /** \} */

    /** \name Operators.
     ** \{ */
  public:
    /** \brief Assign a Symbol to this Symbol.
     ** \param rhs Symbol to copy. */
    Symbol& operator= (const Symbol& rhs);

    /** \brief Compare two Symbol for equality.
     ** \param rhs Symbol to compare with. */
    bool operator== (const Symbol& rhs) const;
    /** \brief Compare two Symbol for inequality.
     ** \param rhs Symbol to compare with. */
    bool operator!= (const Symbol& rhs) const;
    /** \brief Compare two Symbol lexicographically for order.
     ** \param rhs Symbol to compare with. */
    bool operator< (const Symbol& rhs) const;
    /** \} */

    /** \name Factory methods.
     ** \{ */
    /** \brief Create a new unique symbol. */
    static Symbol fresh ();
    /** \brief Create a new unique symbol, forged from \a s. */
    static Symbol fresh (const Symbol& s);
    /** \} */

  private:
    /// Return the set of strings.
    static string_set_type& string_set_instance ();

    /// Pointer to the unique referenced string.
    const std::string* str_;

    /// Counter of unique symbols.
    static unsigned counter_;
  };

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param the a reference to the symbol to redirect
   */
  std::ostream& operator<< (std::ostream& ostr, const Symbol& the);
}

# include "libport/symbol.hxx"

#endif // !LIBPORT_SYMBOL_HH
