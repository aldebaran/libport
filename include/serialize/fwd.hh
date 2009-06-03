#ifndef LIBPORT_SERIALIZE_FWD_HH
# define LIBPORT_SERIALIZE_FWD_HH

namespace libport
{
  namespace serialize
  {
    enum pointer_status
    {
      null,
      cached,
      serialized,
    };
  }
}

#endif
