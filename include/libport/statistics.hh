/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_STATISTICS_HH
# define LIBPORT_STATISTICS_HH

# include <vector>

namespace libport
{

  /// \c T is the type of the samples, \c R is the type of the result
  /// and intermediate computations.
  /// \c T must be a type convertible to \c R and \c R must be a type
  /// compatible to double.
  template<typename T, typename R = double>
  class Statistics
  {
  public:
    Statistics(size_t capacity = 0);
    void resize(size_t capacity);
    size_t capacity() const;
    size_t size() const;
    bool empty() const;
    void add_sample(T value);
    size_t n_samples() const;
    R mean() const;
    R variance() const;
    R standard_deviation() const;
    T min() const;
    T max() const;
  private:
    /// Update or invalidate min and max values.
    void update_min_max(T value);
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
