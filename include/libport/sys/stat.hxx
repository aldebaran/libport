#ifdef WIN32

int
mkdir(const char *path, mode_t)
{
  return _mkdir(path);
}

#endif
