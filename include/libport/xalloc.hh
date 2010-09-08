/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/xalloc.hh
 ** \brief Declaration of libport::xalloc.
 **/

#ifndef LIBPORT_XALLOC_HH
# define LIBPORT_XALLOC_HH

# include <iostream>
# include <functional>
# include <libport/export.hh>

namespace libport
{
  /// Defines the operator() for the classes get_type, set_type and swap_type.
  struct LIBPORT_API iomanipulator
    : public std::unary_function<void, std::ostream>
  {
    virtual ~iomanipulator ();
    virtual void operator() (std::ostream& o) const = 0;
  };

  /** \brief Launch the iomanipulator action on o.
   **
   ** Call the operator() of g (so set, get or swap the
   ** xalloced data with data_).
   */
  std::ostream& operator<< (std::ostream& o, const iomanipulator& g);


  /** \brief Allocate slots in std::ostreams.
   **
   ** Used to store flags in streams so that calling print
   ** with additional parameters is no longer required (for
   ** example in ast/libast.hh).
   **
   ** Set, get or swap data.
   ** The idea is to build the right inner class with set, get or
   ** swap methods, then use this class as a parameter of operator<<.
   */
  template <class StoredType>
  class xalloc
  {
  private:
    /** \brief Handle the data to put in the xalloced place.
     **
     ** This inner class is used only with the set method.
     */
    class set_type : public iomanipulator
    {
    public:
      /// Set data_ to data.
      set_type(const xalloc& slot, StoredType& data);
      /// Set the data in the xalloced place.
      void operator()(std::ostream& ostr) const;
    private:
      /// The xalloced data.
      const xalloc& slot_;
      /// Data to put in the stream.
      StoredType& data_;
    };

    /** \brief Handle the data to get from the xalloced place.
     **
     ** This inner class is used only with the get method.
     */
    class get_type : public iomanipulator
    {
    public:
      /// Set data_ to data.
      get_type(const xalloc& slot, StoredType& data);

      /// Get the data from the xalloced place.
      void operator()(std::ostream& ostr) const;
    private:
      /// The xalloced data.
      const xalloc& slot_;
      /// Variable in which we return the xalloc data.
      StoredType& data_;
    };

    /** \brief Swap the data stored in the stream for a given one.
     **
     ** This inner class is used only with the swap method.
     */
    class swap_type : public iomanipulator
    {
    public:
      /// Set data_ to data.
      swap_type(const xalloc& slot, StoredType& data);

      /// Swap the data from the xalloced place for a given one.
      void operator()(std::ostream& ostr) const;
    private:
      /// The xalloced data.
      const xalloc& slot_;
      /// Variable in which we store the data to be swapped.
      StoredType& data_;
    };

    /// The index of the slot.
    const long int index_;

  public:
    /// Allocates the slot.
    xalloc();
    /// The xalloc index.
    long int index() const;
    /// The stored data as an lvalue.

    StoredType& operator()(std::ostream& ostr) const;

    /// A setter.
    /// \param data where to find the value when the setter is executed.
    set_type set(StoredType& data) const;

    /// A getter.
    /// \param data where to store the value when the getter is executed.
    get_type get(StoredType& data) const;

    /// A swapper.
    /// \param data value to exchange when the swapper is executed.
    swap_type swap(StoredType& data) const;
  };

}

# include "libport/xalloc.hxx"

#endif // !LIBPORT_XALLOC_HH
