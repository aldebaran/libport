#include <libport/xltdl.hh>


namespace libport
{
  namespace xlt
  {
    namespace details
    {
      void init()
      {
        static bool tail = false;
        if (!tail++)
        {
          lt_dlinit();
          lt_dladd_log_function((lt_dllog_function*) &ltdebug, 0);
        }
      }
    }
  }
}
