#ifndef LIBPORT_ATTRIBUTES_HH
# define LIBPORT_ATTRIBUTES_HH

# include <libport/traits.hh>

#define ATTRIBUTE(Type, Name)                   \
  private:                                      \
  Type Name ## _;

#define ATTRIBUTE_GETTER(Type, Name)                    \
  public:                                               \
  libport::traits::Arg<Type>::res Name ## _get() const  \
  { return Name ## _; }                                 \

#define ATTRIBUTE_SETTER(Type, Name)                            \
  public:                                                       \
  void Name ## _set(libport::traits::Arg<Type>::res val)        \
  { Name ## _ = val; }                                          \

#define ATTRIBUTE_R(Type, Name)                 \
  ATTRIBUTE(Type, Name);                        \
  ATTRIBUTE_GETTER(Type, Name);

#define ATTRIBUTE_W(Type, Name)                 \
  ATTRIBUTE(Type, Name);                        \
  ATTRIBUTE_SETTER(Type, Name);

#define ATTRIBUTE_RW(Type, Name)                \
  ATTRIBUTE(Type, Name);                        \
  ATTRIBUTE_GETTER(Type, Name);                 \
  ATTRIBUTE_SETTER(Type, Name);

#endif
