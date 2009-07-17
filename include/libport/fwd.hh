#ifndef LIBPORT_FWD_HH
# define LIBPORT_FWD_HH

namespace libport
{
#if 0
  // We don't fwddecl hash, because it is too much of a PITA:

  // namespace ns
  // {
  //   struct foo;
  // }
  //
  // namespace impl
  // {
  //   struct foo {};
  // }
  //
  // namespace ns
  // {
  //   using impl::foo;
  // }

  // does not work, the compiler complains there are two definitions
  // of ns::foo.  The following works:

  // namespace impl
  // {
  //   template <typename T>
  //   struct foo;
  // }
  //
  // namespace ns
  // {
  //   using impl::foo;
  // }
  //
  // namespace impl
  // {
  //   template <typename T>
  //   struct foo {};
  // }
  //
  // namespace ns
  // {
  //   using impl::foo;
  // }

  // but the problem is that this definition will depend upon the
  // architecture to find what the "impl" namespace is.  So we
  // just don't fwddecl hash, unless someone feels like filling the
  // blanks.

  // hash.hh.
  // Warning: The VC++ implementation might have a different number of
  // parameters, in which case a #if will be needed to fwd decl both
  // flavors of hash_map.
  // Anyway, we want std::tr1::unodered_map here.
  template<class _Key, class _Tp, class _HashFcn, class _EqualKey, class _Alloc>
  class hash_map;
#endif

  // asio.hh;
  struct ConcreteSocket;
  struct Socket;

  // lockable.hh.
  class Lockable;

  // package-info.hh;
  class PackageInfo;

  // path.hh;
  struct Path;

  // ref-pt.hh
  template <class T> class RefPt;

  // select-const.hh.
  template <typename T> struct constify_traits;
  template <typename T> struct id_traits;

  // semaphore.hh.
  class Semaphore;

  // shared-ref.hh.
  template <typename T> class shared_ref;

  // symbol.hh.
  class Symbol;

  // xltdl.hh
  class xlt_advise;
  class xlt_handle;
}
namespace boost
{
  namespace asio
  {
    class io_service;
  }
}

#endif // !LIBPORT_FWD_HH
