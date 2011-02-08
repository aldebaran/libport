/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/symbol.hh
 ** \brief Definition of libport::Symbol.
 */

#ifndef LIBPORT_SYMBOL_HH
# define LIBPORT_SYMBOL_HH

# include <string>
# include <iosfwd>

#include <libport/config.h>

# ifdef WITH_BOOST_SERIALIZATION
#  include <boost/serialization/serialization.hpp>
#  include <boost/serialization/tracking.hpp>
# endif // WITH_BOOST_SERIALIZATION

# include <boost/unordered_set.hpp>

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
    typedef boost::unordered_set<std::string> string_set_type;
    /// The type for the size of string map.
    typedef string_set_type::size_type string_size_type;

    /** \name Ctor & Dtor.
     ** \{ */
  public:
    explicit Symbol();
    /** \brief Construct a Symbol.
     ** \param s referenced string */
    explicit Symbol (const std::string& s);
    explicit Symbol (const char* s);
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
    /** \brief Create a new unique symbol, forged from \a s. */
    static std::string fresh_string (const std::string& s = "a");
    /** \brief Create a new unique symbol, forged from \a s. */
    static Symbol fresh (const Symbol& s);
    /** \brief Return (and cache) an empty symbol. */
    static Symbol make_empty();
    /** \} */

  private:
    /// Return the set of strings.
    static string_set_type& string_set_instance ();

    /// Pointer to the unique referenced string.
    const std::string* str_;

# ifdef WITH_BOOST_SERIALIZATION
    /// Serializer.
    friend class boost::serialization::access;
    template <typename Archive>
    void save(Archive& ar, const unsigned int version) const;
    template <typename Archive>
    void load(Archive& ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER()
# endif // WITH_BOOST_SERIALIZATION
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

# ifdef WITH_BOOST_SERIALIZATION
BOOST_CLASS_TRACKING(libport::Symbol, boost::serialization::track_never)
# endif // WITH_BOOST_SERIALIZATION

# include <libport/symbol.hxx>

#endif // !LIBPORT_SYMBOL_HH
