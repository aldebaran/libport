namespace libport
{

  inline
  Backtrace::const_iterator
  Backtrace::begin() const
  {
    return backtrace_.begin();
  }

  inline
  Backtrace::const_iterator
  Backtrace::end() const
  {
    return backtrace_.end();
  }

  inline
  std::ostream&
  operator<<(std::ostream& o, const Backtrace& b)
  {
    return b.dump(o);
  }


}
