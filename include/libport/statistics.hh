#ifndef LIBPORT_STATISTICS_HH
# define LIBPORT_STATISTICS_HH

namespace libport
{

  template<typename T>
  class Statistics
  {
  public:
    Statistics();
    void add_sample(T value);
    unsigned samples() const;
    T mean() const;
    T variance() const;
    T standard_deviation() const;
    T min() const;
    T max() const;
  private:
    unsigned samples_;
    T sum_;
    T sum2_;
    T min_;
    T max_;
  };

} // namespace libport

# include <libport/statistics.hxx>

#endif // LIBPORT_STATISTICS_HH
