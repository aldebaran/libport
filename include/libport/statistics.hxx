#ifndef LIBPORT_STATISTICS_HXX
# define LIBPORT_STATISTICS_HXX

# include <cmath>

namespace libport
{

  template<typename T>
  Statistics<T>::Statistics()
  {
    samples_ = sum_ = sum2_ = 0;

  }

  template<typename T>
  inline void
  Statistics<T>::add_sample(T value)
  {
    if (samples_++)
    {
      if (value < min_)
	min_ = value;
      if (value > max_)
	max_ = value;
    }
    else
      min_ = max_ = value;
    sum_ += value;
    sum2_ += value * value;
  }

  template<typename T>
  inline unsigned
  Statistics<T>::samples() const
  {
    return samples_;
  }

  template<typename T>
  inline T
  Statistics<T>::mean() const
  {
    return sum_ / samples_;
  }

  template<typename T>
  inline T
  Statistics<T>::variance() const
  {
    T m = mean();
    return sum2_ / samples_ - m * m;
  }

  template<typename T>
  inline T
  Statistics<T>::standard_deviation() const
  {
    return static_cast<T>(sqrt(variance()));
  }

  template<typename T>
  inline T
  Statistics<T>::min() const
  {
    return min_;
  }

  template<typename T>
  inline T
  Statistics<T>::max() const
  {
    return max_;
  }

} // namespace libport

#endif // LIBPORT_STATISTICS_HXX
