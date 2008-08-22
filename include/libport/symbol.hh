/**
 ** \file libport/symbol.hh
 ** \brief Definition of libport::Symbol.
 */

#ifndef LIBPORT_SYMBOL_HH
# define LIBPORT_SYMBOL_HH

# include <set>
# include <string>
# include <iosfwd>

# include <boost/serialization/serialization.hpp>
# include <boost/serialization/tracking.hpp>
# include <boost/functional/hash.hpp>

# include <libport/export.hh>

namespace libport
{

  /** \brief Define class Symbol.
   **
   ** Map any string to a unique reference.
   ** This allows to avoid an "strcmp ()" style comparison of strings:
   ** reference comparison is much faster.
   */
  class LIBPORT_API Symbol
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
    explicit Symbol (const std::string& s = "");
    /** \} */

    /** \name Accessors.
     ** \{ */
  public:
    /// Return the string referenced by this Symbol.
    const std::string& name_get () const;
    operator const std::string& () const;

    /// Print the referenced string escaped and between single quotes.
    std::ostream& print_escaped (std::ostream&) const;

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

    /// Whether the name is empty.
    bool empty() const;

    /** \name Factory methods.
     ** \{ */
    /** \brief Create a new unique symbol. */
    static Symbol fresh ();
    /** \brief Create a new unique symbol, forged from \a s. */
    static Symbol fresh (const std::string& s);
    /** \brief Create a new unique symbol, forged from \a s. */
    static Symbol fresh (const Symbol& s);
    /** \} */

  private:
    /// Return the set of strings.
    static string_set_type& string_set_instance ();

    /// Pointer to the unique referenced string.
    const std::string* str_;

    /// Serializer.
    friend class boost::serialization::access;
    template <typename Archive>
    void save(Archive& ar, const unsigned int version) const;
    template <typename Archive>
    void load(Archive& ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param the a reference to the symbol to redirect
   */
  std::ostream& operator<< (std::ostream& ostr, const Symbol& the);

  /// Make boost recognize symbols as hashable values
  std::size_t
  hash_value(libport::Symbol s);
}

BOOST_CLASS_TRACKING(libport::Symbol, boost::serialization::track_never)

# include <libport/symbol.hxx>

#endif // !LIBPORT_SYMBOL_HH
