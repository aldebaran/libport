/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include "auto_pimpl.h"
#include <iostream>

// full declaration of implementation type
class pimpl_impl
{
  public:
  pimpl_impl ();
  ~pimpl_impl ();
  size_t value;
};

// facade type constructor and destructor
pimpl::pimpl() : impl(new pimpl_impl) { }
pimpl::~pimpl () { std::cerr << "Deleted" << std::endl; }

// facade type delagates functionality to implementation type
size_t pimpl::get_value () const { return impl->value; }

// implementation type constructor and destructor
pimpl_impl::pimpl_impl() : value(7) { }
pimpl_impl::~pimpl_impl () { }
