/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_LOCAL_DATA_HH
# define LIBPORT_LOCAL_DATA_HH

# include <boost/function.hpp>

namespace libport
{
  namespace localdata
  {
    template <typename T, typename Encapsulate>
    struct Wrapper;
  }

  template <typename T>
  class AbstractLocalData
  {
  public:
    virtual T* get() = 0;
    virtual void set(T* v) = 0;
  };

  template <typename T, typename Enc>
  class LocalData
    : public AbstractLocalData<T>
  {
  public:
    T* get();
    void set(T* v);

  private:
    typedef typename localdata::Wrapper<T, Enc> traits;
    typedef typename traits::container container;
    container container_;
  };

  template <typename T>
  class AbstractLocalSingleton
  {
  public:
    typedef boost::function0<T*> builder;
    virtual T& instance(builder b) = 0;
  };

  template <typename T, typename Enc>
  class LocalSingleton
    : public AbstractLocalSingleton<T>
  {
  public:
    typedef typename AbstractLocalSingleton<T>::builder builder;
    T& instance(builder b);

  private:
    LocalData<T, Enc> data_;
  };
}

# include <libport/local-data.hxx>

#endif
