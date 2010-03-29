/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
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

/* ATTRIBUTE_{mode}(Type, Name, [Getter modifiers], [Setter modifiers], [Member modifier])
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

#define ATTRIBUTE(Type, ...)                  \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), NONE , NONE , LIBPORT_VARIADIC(__VA_ARGS__)); \

#define ATTRIBUTE_R(Type, ...)                  \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), IMPL , NONE , LIBPORT_VARIADIC(__VA_ARGS__)); \

#define ATTRIBUTE_r(Type, ...)                  \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), PROTO, NONE , LIBPORT_VARIADIC(__VA_ARGS__));   \

#define ATTRIBUTE_W(Type, ...)                  \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), NONE , IMPL , LIBPORT_VARIADIC(__VA_ARGS__));   \

#define ATTRIBUTE_w(Type, ...)                  \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), NONE , PROTO, LIBPORT_VARIADIC(__VA_ARGS__));   \

#define ATTRIBUTE_RW(Type, ...)                 \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), IMPL , IMPL , LIBPORT_VARIADIC(__VA_ARGS__));   \

#define ATTRIBUTE_rw(Type, ...)                 \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), PROTO, PROTO, LIBPORT_VARIADIC(__VA_ARGS__));   \

#define ATTRIBUTE_Rw(Type, ...)                 \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), IMPL , PROTO, LIBPORT_VARIADIC(__VA_ARGS__));   \

#define ATTRIBUTE_rW(Type, ...)                 \
  ATTRIBUTE_BOUNCE(Type, LIBPORT_LIST_HEAD(LIBPORT_LIST(__VA_ARGS__,)), PROTO, IMPL , LIBPORT_VARIADIC(__VA_ARGS__));   \


/*----------.
| Helpers.  |
`----------*/

#define ATTRIBUTE_MEMBER(Type, Name, Mods)      \
  private:                                      \
  Mods Type Name ## _;                          \

#define ATTRIBUTE_BOUNCE(Type, Name, Get, Set, Modifiers)               \
  ATTRIBUTE_GETTER_##Get(Type, Name, LIBPORT_LIST_NTH(0, Modifiers))    \
  ATTRIBUTE_SETTER_##Set(Type, Name, LIBPORT_LIST_NTH(1, Modifiers))    \
  ATTRIBUTE_MEMBER(Type, Name, LIBPORT_LIST_NTH(2, Modifiers))          \

/*---------.
| Getter.  |
`---------*/

#define ATTRIBUTE_GETTER(Type, Name, Mods)                       \
  public:                                                        \
  Mods libport::traits::Arg<Type>::res Name ## _get() const      \

#define ATTRIBUTE_GETTER_NONE(Type, Name, Mods)

#define ATTRIBUTE_GETTER_PROTO(Type, Name, Mods)        \
  ATTRIBUTE_GETTER(Type, Name, Mods);                   \

#define ATTRIBUTE_GETTER_IMPL(Type, Name, Mods) \
  ATTRIBUTE_GETTER(Type, Name, Mods)            \
  { return Name ## _; }                         \

/*---------.
| Setter.  |
`---------*/

#define ATTRIBUTE_SETTER(Type, Name, Mods)                      \
  public:                                                       \
  Mods void Name ## _set(libport::traits::Arg<Type>::res val)   \

#define ATTRIBUTE_SETTER_NONE(Type, Name, Mods)

#define ATTRIBUTE_SETTER_PROTO(Type, Name, Mods)        \
  ATTRIBUTE_SETTER(Type, Name, Mods);                   \

#define ATTRIBUTE_SETTER_IMPL(Type, Name, Mods) \
  ATTRIBUTE_SETTER(Type, Name, Mods)            \
  { Name ## _ = val; }                          \

#endif
