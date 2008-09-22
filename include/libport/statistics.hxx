#ifndef LIBPORT_STATISTICS_HXX
# define LIBPORT_STATISTICS_HXX

# include <cmath>

namespace libport
{

  template<typename T, typename R>
  Statistics<T, R>::Statistics(size_t capacity)
  {
    reserve(capacity);
  }

  template<typename T, typename R>
  inline void
  Statistics<T, R>::reserve(size_t capacity)
  {
    capacity_ = capacity;
    count_ = index_ = 0;
    sum_ = sum2_ = 0;
    min_ok_ = max_ok_ = false;
    samples_.reserve(capacity_);
  }

  template<typename T, typename R>
  inline void
  Statistics<T, R>::add_sample(T value)
  {
    if (capacity_)  // Running
    {
      // Do not increment the index for the first item.
      if (count_)
	index_ = (index_ + 1) % capacity_;

      // Update or invalidate min and max values.
      if (count_ < capacity_)
      {
	if (count_++) {
	  if (value < min_)
	    min_ = value;
	  if (value > max_)
	    max_ = value;
	}
	else
	{
	  min_ = max_ = value;
	  min_ok_ = max_ok_ = true;
	}
      }
      else
      {
	if (min_ok_)
	{
	  // Check that we are not removing the smallest sample.
	  if (value <= min_)
	    min_ = value;
	  else if (samples_[index_] == min_)
	    min_ok_ = false;
	}
	if (max_ok_)
	{
	  // Check that we are not removing the greatest sample.
	  if (value >= max_)
	    max_ = value;
	  else if (samples_[index_] == max_)
	    max_ok_ = false;
	}

	// Remove the old sample from the sums.
	const T old_value = samples_[index_];
	sum_ -= old_value;
	sum2_ -= old_value * old_value;
      }

      // Store the sample.
      samples_[index_] = value;
    }
    else  // Not running case.
    {
      // Update min and max values.
      if (count_++)
      {
	if (value < min_)
	  min_ = value;
	if (value > max_)
	  max_ = value;
      }
      else
      {
	min_ = max_ = value;
	min_ok_ = max_ok_ = true;
      }
    }

    // Update sums.
    sum_ += value;
    sum2_ += value * value;
  }

  template<typename T, typename R>
  inline size_t
  Statistics<T, R>::size() const
  {
    return count_;
  }

  template<typename T, typename R>
  inline size_t
  Statistics<T, R>::capacity() const
  {
    return capacity_;
  }

  template<typename T, typename R>
  inline bool
  Statistics<T, R>::empty() const
  {
    return !size();
  }

  template<typename T, typename R>
  inline R
  Statistics<T, R>::mean() const
  {
    return static_cast<R>(sum_) / static_cast<R>(count_);
  }

  template<typename T, typename R>
  inline R
  Statistics<T, R>::variance() const
  {
    return (static_cast<R>(sum2_) -
	    (static_cast<R>(sum_) * static_cast<R>(sum_))
	        / static_cast<R>(count_)) /
      static_cast<R>(count_);
  }

  template<typename T, typename R>
  inline R
  Statistics<T, R>::standard_deviation() const
  {
    return static_cast<R>(sqrt(static_cast<double>(variance())));
  }

  template<typename T, typename R>
  inline T
  Statistics<T, R>::min() const
  {
    if (!min_ok_)
    {
      min_ = samples_[0];
      for (unsigned int i = 1; i < capacity_; i++)
	if (samples_[i] < min_)
	  min_ = samples_[i];
      min_ok_ = true;
    }
    return min_;
  }

  template<typename T, typename R>
  inline T
  Statistics<T, R>::max() const
  {
    if (!max_ok_)
    {
      max_ = samples_[0];
      for (unsigned int i = 1; i < capacity_; i++)
	if (samples_[i] > max_)
	  max_ = samples_[i];
      max_ok_ = true;
    }
    return max_;
  }

} // namespace libport

#endif // LIBPORT_STATISTICS_HXX
