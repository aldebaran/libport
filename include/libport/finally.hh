#ifndef LIBPORT_FINALLY_HH
# define LIBPORT_FINALLY_HH

# include <boost/function.hpp>
// This one is not directly required by finally.hh, but is required
// by 90% of its users.
# include <boost/bind.hpp>
# include <list>

# include "foreach.hh"

namespace libport
{
  /// RAII helper that can register actions to execute at its destruction.
  class Finally
  {
    public:

      typedef boost::function0<void> Action;

      /// Build a Finally object
      Finally();
      /// Build a Finally object, and register \a a to be executed add destruction.
      Finally(const Action& a);
      /// Execute all registered actions and destroy the Finally object.
      ~Finally();

      /// Register \a a to be executed add destruction.
      Finally& operator <<(const Action& a);

    private:
      std::list<Action> actions_;

  };
}

# include "finally.hxx"

#endif
