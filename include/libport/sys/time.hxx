namespace std
{
  inline
  std::ostream&
  operator<< (std::ostream& o, const timeval& t)
  {
    return o
      << "timeval"
      << "{ tv_sec = " << t.tv_sec
      << ", tv_usec = " << t.tv_usec
      << " }";
  }
}

inline
bool
operator==(const timeval& lhs, const timeval& rhs)
{
  return lhs.tv_sec == rhs.tv_sec && lhs.tv_usec == rhs.tv_usec;
}

inline
bool
operator!=(const timeval& lhs, const timeval& rhs)
{
  return !(lhs == rhs);
}


inline
timeval&
operator-=(timeval& res, const timeval& rhs)
{
  res.tv_sec -= rhs.tv_sec;
  res.tv_usec -= rhs.tv_usec;
  return res;
}

inline
timeval
operator-(const timeval& lhs, const timeval& rhs)
{
  timeval res = lhs;
  return res -= rhs;
}
