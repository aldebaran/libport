#ifndef LIBPORT_STATISTICS_HH
# define LIBPORT_STATISTICS_HH

# include <vector>

namespace libport
{

  /// \c T must be a type convertible to double. When \c extra_precision
  /// is true, the mean is computed with a double division instead of a
  /// T/size_t division, and then converted back to T.
  template<typename T, bool extra_precision = false>
  class Statistics
  {
  public:
    Statistics(size_t capacity);
    void reserve(size_t capacity);
    size_t capacity() const;
    size_t size() const;
    bool empty() const;
    void add_sample(T value);
    size_t n_samples() const;
    T mean() const;
    double variance() const;
    double standard_deviation() const;
    T min() const;
    T max() const;
  private:
    size_t capacity_;
    size_t count_;
    size_t index_;
    std::vector<T> samples_;
    T sum_;
    T sum2_;
    mutable T min_;
    mutable T max_;
    mutable bool min_ok_;
    mutable bool max_ok_;
  };

} // namespace libport

# include <libport/statistics.hxx>

#endif // LIBPORT_STATISTICS_HH
