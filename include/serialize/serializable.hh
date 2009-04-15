#ifndef LIBPORT_SERIALIZE_SERIALIZABLE_HH
# define LIBPORT_SERIALIZE_SERIALIZABLE_HH

# include <serialize/export.hh>
# include <serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API Serializable
    {
    public:
      virtual void serialize(OSerializer& s) const = 0;
      virtual ~Serializable() {}
    };
  }
}

#define UNSERIALIZE_SUPER(Next, Data, Elt) Elt(ser),
#define UNSERIALIZE_ATTR(Next, Data, Elt) BOOST_PP_CAT(Elt, _)(ser),
#define UNSERIALIZE_POD(Next, Data, Elt)        \
  ser.unserialize(#Elt, BOOST_PP_CAT(Elt, _));

#define SERIALIZE_SUPER(Next, Data, Elt) Elt::serialize(ser);
#define SERIALIZE_ATTR(Next, Data, Elt) ser.serialize(#Elt, BOOST_PP_CAT(Elt, _));

#define SERIALIZABLE(Name, Supers, Attrs, Pods)                   \
public:                                                           \
Name(ISerializer& ser)                                            \
: BOOST_PP_SEQ_FOR_EACH(UNSERIALIZE_SUPER, , Supers)              \
BOOST_PP_SEQ_FOR_EACH(UNSERIALIZE_ATTR , , Attrs)                 \
_dummy_(false)                                                    \
{                                                                 \
  BOOST_PP_SEQ_FOR_EACH(UNSERIALIZE_POD , , Pods);                \
}                                                                 \
                                                                  \
virtual void serialize(OSerializer& ser) const                    \
{                                                                 \
  BOOST_PP_SEQ_FOR_EACH(SERIALIZE_SUPER, , Supers);               \
  BOOST_PP_SEQ_FOR_EACH(SERIALIZE_ATTR, , Attrs);                 \
  BOOST_PP_SEQ_FOR_EACH(SERIALIZE_ATTR, , Pods);                  \
}                                                                 \
                                                                  \
private:                                                          \
bool _dummy_;                                                     \

#endif
