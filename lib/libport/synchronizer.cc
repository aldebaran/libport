/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/synchronizer.hh>

namespace libport
{

  // This file exists to satisfy MSVC: any symbol declared as
  // dllexport *must* pbe present in the lib, it cannot be made only
  // of inline functions.  And anyway, it is safer to do it this way,
  // as the symbol will really be defined only once, instead of
  // several times.  Otherwise we will have several typeid for this
  // class, which results in risks of failing dynamic_casts (if the
  // typeids are taken from two libs), or failing "catch"es and so
  // forth.
  Synchronizer::~Synchronizer()
  {}

}
