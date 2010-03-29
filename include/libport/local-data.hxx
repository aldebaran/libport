/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_LOCAL_DATA_HXX
# define LIBPORT_LOCAL_DATA_HXX

# include <boost/type_traits/is_base_of.hpp>
# include <boost/mpl/if.hpp>

namespace libport
{

  namespace localdata
  {
    /// \brief Handle data with only one encapsulation.
    template <typename T>
    struct DataType
    {
      typedef typename T::target target;
      typedef typename T::type type;
      typedef typename T::container container;
      static target* get(container& c, bool create = false);
      static void set(container& c, target* e);
    };


    template <typename T>
    typename DataType<T>::target*
    DataType<T>::get(container& c, bool)
    {
      return c.get();
    }

    template <typename T>
    void
    DataType<T>::set(container& c, target* e)
    {
      c.reset(e);
    }

    /// \brief Handle data with multiple local data encapsulation.
    template <typename T>
    struct LocalDataType
    {
      typedef typename T::target target;
      typedef typename T::type type;
      typedef typename T::container container;
      static target* get(container& c, bool create = false);
      static void set(container& c, target* e);
    private:
      static type* get_type(container& c, bool create);
    };

    template <typename T>
    typename LocalDataType<T>::type*
    LocalDataType<T>::get_type(container& c, bool create)
    {
      type* e = c.get();

      if (!e)
      {
        if (create)
        {
          e = new type;
          c.reset(e);
        }
        else
          return 0;
      }
      return e;
    }

    template <typename T>
    typename LocalDataType<T>::target*
    LocalDataType<T>::get(container& c, bool create)
    {
      type* e = get_type(c, create);

      if (e)
        return e->get();
      return 0;
    }

    template <typename T>
    void
    LocalDataType<T>::set(container& c, target* e)
    {
      get_type(c, true)->set(e);
    }

    // parametric typedef.
    template <typename T>
    struct WrapperFuns
      : boost::mpl::if_<
          boost::is_base_of<
            AbstractLocalData<typename T::target>,
            typename T::type
          >,
          LocalDataType<T>,
          DataType<T>
        >::type
    {
    };
  }

  template <typename T>
  AbstractLocalData<T>::~AbstractLocalData()
  {
  }


  template <typename T, typename Enc>
  T*
  LocalData<T, Enc>::get()
  {
    return localdata::WrapperFuns<traits>::get(container_);
  }

  template <typename T, typename Enc>
  void
  LocalData<T, Enc>::set(T* v)
  {
    localdata::WrapperFuns<traits>::set(container_, v);
  }


  template <typename T, typename Enc>
  T&
  LocalSingleton<T, Enc>::instance()
  {
    T* e = data_.get();
    if (!e)
    {
      e = new T();
      data_.set(e);
    }
    return *e;
  }

  template <typename T, typename Enc>
  LocalSingleton<T, Enc>::operator T&()
  {
    return instance();
  }

  template <typename T, typename Enc>
  LocalSingleton<T, Enc>&
  LocalSingleton<T, Enc>::operator =(T& v)
  {
    instance() = v;
    return *this;
  }
}

#endif
