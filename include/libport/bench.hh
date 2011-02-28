/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_BENCH_HH
# define LIBPORT_BENCH_HH

# include <libport/compiler.hh>
# include <libport/statistics.hh>

namespace libport
{

/** Bench duration of the current block. Trigger display by executing DISPLAY
 * when either N_SAMPLES_TRIGGER samples were acquired, or DURATION_TRIGGER
 * microseconds elapsed since last display. Execute DISPLAY to display the
 * stats in libport::Statistics object 's.stats'.
 */
#define LIBPORT_BENCH_BLOCK_(N_SAMPLES_TRIGGER, DURATION_TRIGGER, DISPLAY)    \
  class LIBPORT_CAT(BenchBlock, __LINE__)                                    \
  {                                                                          \
  public:                                                                    \
    struct StatType                                                           \
    {                                                                        \
      ::libport::Statistics< ::libport::utime_t, ::libport::ufloat> stats;   \
      ::libport::utime_t lastDisplay;                                        \
      StatType() : lastDisplay(::libport::utime()) {}                        \
    };                                                                       \
    LIBPORT_CAT(BenchBlock, __LINE__ ) (StatType& s, const char* pf, int line)\
    : start_(::libport::utime())                                             \
    , s(s), pf(pf), line(line)                                               \
    {}                                                                       \
    LIBPORT_CAT(~BenchBlock, __LINE__ )()                                    \
    {                                                                        \
      ::libport::utime_t now = ::libport::utime();                           \
      s.stats.add_sample(now - start_);                                      \
      if ( (N_SAMPLES_TRIGGER && (int)s.stats.n_samples() >= N_SAMPLES_TRIGGER)\
          || (DURATION_TRIGGER && now-s.lastDisplay > DURATION_TRIGGER)      \
          )                                                                  \
      {                                                                      \
        DISPLAY                                                              \
        s.stats.resize(0);                                                   \
        s.lastDisplay = now;                                                 \
      }                                                                      \
    }                                                                        \
  private:                                                                   \
    ::libport::utime_t start_;                                               \
    StatType& s;                                                             \
    const char* pf;                                                          \
    int line;                                                                \
  };                                                                         \
  static LIBPORT_CAT(BenchBlock, __LINE__ )::StatType                        \
    LIBPORT_CAT(libport_bench_stats, __LINE__);                              \
  LIBPORT_CAT(BenchBlock, __LINE__ )                                         \
    LIBPORT_CAT( libport_bench_block,__LINE__)                               \
      (LIBPORT_CAT(libport_bench_stats,__LINE__),                            \
                        __PRETTY_FUNCTION__, __LINE__ )

/** Bench duration of the current block using LIBPORT_BENCH_BLOCK_, display
 * stats to stderr
 */
#define LIBPORT_BENCH_BLOCK_STDERR(N_SAMPLES_TRIGGER, DURATION_TRIGGER)  \
  LIBPORT_BENCH_BLOCK_(N_SAMPLES_TRIGGER, DURATION_TRIGGER,              \
    std::cerr << pf << ":" << line << " duration stats: "                \
              << s.stats.mean() <<" " << s.stats.min()                   \
              << " " << s.stats.max() << " "  << s.stats.variance()      \
              << std::endl;)

/** Bench duration of the current block using LIBPORT_BENCH_BLOCK_, display
 * stats using GD_FINFO_TRACE.
 */
#define LIBPORT_BENCH_BLOCK_GD(N_SAMPLES_TRIGGER, DURATION_TRIGGER)      \
  LIBPORT_BENCH_BLOCK_(N_SAMPLES_TRIGGER, DURATION_TRIGGER,              \
    GD_FINFO_TRACE("%s:%s duration stats: %s %s %s %s",   pf, line,       \
                   s.stats.mean(), s.stats.min(), s.stats.max(),          \
                   s.vstats.ariance());)
}

#endif
