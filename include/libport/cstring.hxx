namespace libport
{
  inline
  bool
  streq(const char* lhs, const char* rhs)
  {
    return !strcmp(lhs, rhs);
  }

  inline
  bool
  strneq(const char* lhs, const char* rhs, size_t n)
  {
    return !strncmp(lhs, rhs, n);
  }
}
