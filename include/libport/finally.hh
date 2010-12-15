/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/finally.hh

#ifndef LIBPORT_FINALLY_HH
# define LIBPORT_FINALLY_HH

# include <libport/system-warning-push.hh>
# include <boost/function.hpp>
# include <boost/typeof/typeof.hpp>
# include <libport/system-warning-pop.hh>

// Not required by libport/finally.hh, but by 90% of its users.
# include <libport/bind.hh>
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

// MSVC 8.0 has a problem with structures declared in inline functions.
//
//    $ cat foo.hh
//    inline
//    void foo()
//    {
//      struct Foo { Foo() {} };
//      Foo foo;
//    }
//
//    $ cat foo-int.cc
//    #include <foo.hh>
//    void foo_int()
//    {
//      foo();
//    }
//    $ cat foo-float.cc
//    #include <foo.hh>
//    void foo_float ()
//    {
//      foo();
//    }
//
// cl.exe -O2 -I.  -c -o foo-float.o foo-float.cc
// cl.exe -O2 -I.  -c -o foo-int.o foo-int.cc
// cl.exe -o foo.dll foo-float.o foo-int.o -link -dll
// foo-int.o : error LNK2005:
//    "public: __thiscall `void __cdecl foo(void)'::`2'::Foo::Foo(void)"
//    (??0Foo@?1??foo@@YAXXZ@QAE@XZ) already defined in foo-float.o
// foo.dll : fatal error LNK1169: one or more multiply defined symbols found
//
// The problem is that it believes the structure was declared several
// times.  One way out is to remove the "inline", which comes with a
// penalty.  Another way out is to pull the structure out of the
// function.  This is why you are encouraged to use FINALLY, but if
// you use it in an inline function, you need to use FINALLY_DEFINE
// outside the function, and FINALLY_USE afterwards.

# define FINALLY_DEFINE(Name, Vars, Action)                             \
  struct Finally ## Name                                                \
  {                                                                     \
    ATTRIBUTE_ALWAYS_INLINE                                             \
    Finally ## Name(LIBPORT_SEPARATE(LIBPORT_MAP(FINALLY_ATTR_ARG, Vars))) \
      : LIBPORT_SEPARATE(LIBPORT_MAP(FINALLY_ATTR_INIT, Vars))          \
    {}                                                                  \
                                                                        \
    ATTRIBUTE_ALWAYS_INLINE                                             \
    ~Finally ## Name()                                                  \
    {                                                                   \
      Action;                                                           \
    }                                                                   \
                                                                        \
    LIBPORT_APPLY(FINALLY_ATTR_DECLARE, Vars);                          \
  }

# define FINALLY_USE(Name, Vars, Action)                                \
  Finally ## Name finally ## Name                                       \
  (LIBPORT_SEPARATE(LIBPORT_MAP(FINALLY_ATTR_FETCH, Vars)))
# define FINALLY(Vars, Action)                                          \
  FINALLY_DEFINE(Libport, Vars, Action);                               \
  FINALLY_USE(Libport, Vars, Action)

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
    Finally& operator<<(const action_type& a);

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

// Set Var to Val. Restore the previous value at scope end.
#define LIBPORT_SCOPE_SET(Var, Val)                                     \
  typedef BOOST_TYPEOF(Var) BOOST_PP_CAT(LibportResetType, __LINE__);   \
  LIBPORT_SCOPE_SET_DECLARE(__LINE__,                                   \
                            BOOST_PP_CAT(LibportResetType, __LINE__));  \
  LIBPORT_SCOPE_SET_USE(__LINE__, Var, Val)                             \

#define LIBPORT_SCOPE_INCREMENT(Var) LIBPORT_SCOPE_SET(Var, Var + 1)
#define LIBPORT_SCOPE_DECREMENT(Var) LIBPORT_SCOPE_SET(Var, Var - 1)

// Splitted versions of SCOPE_SET, to workaround Visual 2005 and
// inline functions. See comment on FINALLY.
#define LIBPORT_SCOPE_SET_DECLARE(Name, Type)                           \
  struct BOOST_PP_CAT(_LibportReset, Name)                              \
  {                                                                     \
    inline BOOST_PP_CAT(_LibportReset, Name)(Type& var, Type val)       \
      : var_(var)                                                       \
      , val_(val)                                                       \
    {}                                                                  \
                                                                        \
    inline ~BOOST_PP_CAT(_LibportReset, Name)()                         \
    {                                                                   \
      var_ = val_;                                                      \
    }                                                                   \
                                                                        \
    Type& var_;                                                         \
    Type  val_;                                                         \
  };                                                                    \

#define LIBPORT_SCOPE_SET_USE(Name, Var, Val)                           \
  BOOST_PP_CAT(_LibportReset,Name)                                      \
  BOOST_PP_CAT(_libport_reset_, __LINE__)(Var, Var);                    \
  Var = Val;                                                            \

# include <libport/finally.hxx>

#endif
