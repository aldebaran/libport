/// \file libport/finally.hh

#ifndef LIBPORT_FINALLY_HH
# define LIBPORT_FINALLY_HH

# include <boost/function.hpp>
// Not required by libport/finally.hh, but by 90% of its users.
# include <boost/bind.hpp>
# include <vector>

# include <libport/foreach.hh>
# include <libport/preproc.hh>

# define FINALLY_ATTR_DECLARE(Attr)             \
  LIBPORT_FIRST(Attr) LIBPORT_SECOND(Attr);
# define FINALLY_ATTR_ARG(Attr)                                 \
  LIBPORT_FIRST(Attr) LIBPORT_CAT(LIBPORT_SECOND(Attr), _)
# define FINALLY_ATTR_INIT(Attr)                                \
  LIBPORT_SECOND(Attr)(LIBPORT_CAT(LIBPORT_SECOND(Attr), _))
# define FINALLY_ATTR_FETCH(Attr)               \
  LIBPORT_SECOND(Attr)

# define FINALLY(Vars, Action)                                          \
    struct Finally                                                      \
    {                                                                   \
      ATTRIBUTE_ALWAYS_INLINE                                           \
        Finally(LIBPORT_SEPARATE(LIBPORT_MAP(FINALLY_ATTR_ARG, Vars)))  \
        : LIBPORT_SEPARATE(LIBPORT_MAP(FINALLY_ATTR_INIT, Vars))        \
      {}                                                                \
                                                                        \
      ATTRIBUTE_ALWAYS_INLINE                                           \
      ~Finally()                                                        \
      {                                                                 \
        Action;                                                         \
      }                                                                 \
                                                                        \
      LIBPORT_APPLY(FINALLY_ATTR_DECLARE, Vars);                        \
    };                                                                  \
    Finally finally                                                     \
    (LIBPORT_SEPARATE(LIBPORT_MAP(FINALLY_ATTR_FETCH, Vars)));          \

namespace libport
{
  /// RAII helper that can register actions to execute at its destruction.
  class Finally
  {
  public:

    typedef boost::function0<void> action_type;

    /// Build a Finally object.
    Finally();
    /// Build a finally object and desactive the original one.
    Finally(const Finally& f);
    /// Build a Finally object, and register \a a to be executed add
    /// destruction.
    Finally(const action_type& a);
    /// Build a Finally object which can store up to \a n objects
    /// without reallocation.
    Finally(unsigned int n);
    /// Execute all registered actions and destroy the Finally object.
    ~Finally();

    /// Register \a a to be executed add destruction.
    Finally& operator <<(const action_type& a);

  private:

    // Implementation note: using a vector here is as efficient as a
    // list if a single action is stored. When multiple actions are
    // stored, the vector implementation is more efficient.
    mutable std::vector<action_type> actions_;
  };

  /// Save a variable and restore it at the end of the scope.
  template<typename T>
  Finally::action_type restore(T&);

  /// Set a variable and restore it to its previous value at the
  /// end of the scope.
  template<typename T>
  Finally::action_type scoped_set(T&, T);

  /// Push \a value at the end of \a container, and pop it at the end
  /// of the scope
  template<typename Value, typename Container>
  Finally::action_type scoped_push(const Value& value, Container& container);
}

# include <libport/finally.hxx>

#endif
