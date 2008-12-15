namespace libport
{
  /// Whether the C string \a Lhs and \a Rhs are equal.
  bool
  streq(const char* lhs, const char* rhs)
  {
    return !strcmp(lhs, rhs);
  }
}

