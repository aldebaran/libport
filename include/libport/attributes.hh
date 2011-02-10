/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_ATTRIBUTES_HH
# define LIBPORT_ATTRIBUTES_HH

# include <libport/preproc.hh>
# include <libport/traits.hh>

/* ATTRIBUTE_{mode}(Type, Name, [Getter modifiers], [Setter modifiers], [Xetter modifier], [Member modifier])
 *
 * mode:
 * - nothing: generate only the membur.
 * - R: generate a getter.
 * - W: generate a setter.
 * - RW: generate both.
 * R and W can be used lowercase, in which case only the prototype of
 * the getter/setter is added. You can then provide your own
 * implementation.
 *
 * Type: type of the attribute.
 * Name: name of the attribute.
 * Getter/Setter modifiers: modifiers for the getter/setter (virtual, static, ...).
 * Member modifier: modifier for the member (mutable, static, ...).
 */


#define LIBPORT_VARIADIC(...) LIBPORT_LIST_TAIL(LIBPORT_LIST(__VA_ARGS__,))

#define ATTRIBUTE(Type, ...)                                            \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   NONE, NONE, NONE, LIBPORT_VARIADIC(__VA_ARGS__))     \

#define ATTRIBUTE_R(Type, ...)                                          \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   IMPL, NONE, NONE, LIBPORT_VARIADIC(__VA_ARGS__))     \

#define ATTRIBUTE_r(Type, ...)                                          \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   PROTO, NONE, NONE, LIBPORT_VARIADIC(__VA_ARGS__))    \

#define ATTRIBUTE_W(Type, ...)                                          \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   NONE, IMPL, NONE, LIBPORT_VARIADIC(__VA_ARGS__))     \

#define ATTRIBUTE_w(Type, ...)                                          \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   NONE, PROTO, NONE, LIBPORT_VARIADIC(__VA_ARGS__))    \

#define ATTRIBUTE_RW(Type, ...)                                         \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   IMPL, IMPL, NONE, LIBPORT_VARIADIC(__VA_ARGS__))     \

#define ATTRIBUTE_rw(Type, ...)                                         \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   PROTO, PROTO, NONE, LIBPORT_VARIADIC(__VA_ARGS__))   \

#define ATTRIBUTE_Rw(Type, ...)                                         \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   IMPL, PROTO, NONE, LIBPORT_VARIADIC(__VA_ARGS__))    \

#define ATTRIBUTE_rW(Type, ...)                                         \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   PROTO, IMPL, NONE, LIBPORT_VARIADIC(__VA_ARGS__))    \

#define ATTRIBUTE_RX(Type, ...)                                         \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   IMPL, NONE, IMPL, LIBPORT_VARIADIC(__VA_ARGS__))     \

#define ATTRIBUTE_rX(Type, ...)                                         \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   PROTO, NONE, IMPL, LIBPORT_VARIADIC(__VA_ARGS__))    \

#define ATTRIBUTE_RWX(Type, ...)                                        \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   IMPL, IMPL, IMPL, LIBPORT_VARIADIC(__VA_ARGS__))     \

#define ATTRIBUTE_rwX(Type, ...)                                        \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   PROTO, PROTO, IMPL, LIBPORT_VARIADIC(__VA_ARGS__))   \

#define ATTRIBUTE_RwX(Type, ...)                                        \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   IMPL, PROTO, IMPL, LIBPORT_VARIADIC(__VA_ARGS__))    \

#define ATTRIBUTE_rWX(Type, ...)                                        \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), \
                   PROTO, IMPL, IMPL, LIBPORT_VARIADIC(__VA_ARGS__))    \


/*----------.
| Helpers.  |
`----------*/

#define ATTRIBUTE_MEMBER(Type, Name, Mods)      \
  private:                                      \
  Mods Type Name ## _;                          \

#define ATTRIBUTE_BOUNCE(Type, Name, Get, Set, Xet, Modifiers)          \
  ATTRIBUTE_MEMBER(Type, Name, LIBPORT_LIST_NTH(3, Modifiers))          \
  ATTRIBUTE_GETTER_##Get(Type, Name, LIBPORT_LIST_NTH(0, Modifiers))    \
  ATTRIBUTE_SETTER_##Set(Type, Name, LIBPORT_LIST_NTH(1, Modifiers))    \
  ATTRIBUTE_XETTER_##Xet(Type, Name, LIBPORT_LIST_NTH(2, Modifiers))    \

/*---------.
| Getter.  |
`---------*/

#define ATTRIBUTE_GETTER(Type, Name, Mods)                       \
  ;public:                                                       \
  Mods libport::traits::Arg<Type>::res Name ## _get() const      \

#define ATTRIBUTE_GETTER_NONE(Type, Name, Mods)

#define ATTRIBUTE_GETTER_PROTO(Type, Name, Mods)        \
  ATTRIBUTE_GETTER(Type, Name, Mods)                    \

#define ATTRIBUTE_GETTER_IMPL(Type, Name, Mods) \
  ATTRIBUTE_GETTER(Type, Name, Mods)            \
  { return Name ## _; }                         \

/*---------.
| Setter.  |
`---------*/

#define ATTRIBUTE_SETTER(Type, Name, Mods)                      \
  ;public:                                                      \
  Mods void Name ## _set(libport::traits::Arg<Type>::res val)   \

#define ATTRIBUTE_SETTER_NONE(Type, Name, Mods)

#define ATTRIBUTE_SETTER_PROTO(Type, Name, Mods)        \
  ATTRIBUTE_SETTER(Type, Name, Mods)                    \

#define ATTRIBUTE_SETTER_IMPL(Type, Name, Mods) \
  ATTRIBUTE_SETTER(Type, Name, Mods)            \
  { Name ## _ = val; }                          \

/*---------------.
| Write getter.  |
`---------------*/

#define ATTRIBUTE_XETTER_NONE(Type, Name, Mods)

#define ATTRIBUTE_XETTER_PROTO(Type, Name, Mods)                 \
  ;public:                                                       \
  Mods Type& Name ## _get()                                      \

#define ATTRIBUTE_XETTER_IMPL(Type, Name, Mods) \
  ATTRIBUTE_XETTER_PROTO(Type, Name, Mods)      \
  { return Name ## _; }                         \

#endif
