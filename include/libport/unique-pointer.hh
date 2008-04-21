#ifndef LIBPORT_UNIQUE_POINTER_HH
# define LIBPORT_UNIQUE_POINTER_HH

# include <set>

namespace libport
{
  /// Check whether a pointer was defined once only.
  class UniquePointer
  {
  public:
    UniquePointer();
    virtual ~UniquePointer();

    /// Register the pointer, pointer to the referenced entity.
    /// \return true iff it is the first time it is registered.
    template <typename T>
    bool operator() (T* t);

    template <typename T>
    bool operator() (T& t);

  private:
    typedef std::set<const void*> set_type;
    // The set of known pointers.
    set_type set_;
  };
}

# include <libport/unique-pointer.hxx>

#endif // !LIBPORT_UNIQUE_POINTER_HH
