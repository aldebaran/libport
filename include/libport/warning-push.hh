/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifdef _MSC_VER

# pragma warning( push )

// C4003: not enough actual parameters for macro
//
// When calling FOO() or even FOO(/* Something */).

// C4061: enumerator 'destroy_functor_tag' in switch of enum
//    'boost::detail::function::functor_manager_operation_type' is not
//    explicitly handled by a case label
// Although the switch has a "default:" case.  Disable this, since anyway
// GCC does the check properly.

// C4099: 'symbol' : type name first seen using 'class' now seen using
//                           'struct'
// (which prints lots of useless warnings when forward declarations are
// specified with "class" while the real definition uses "struct")

// C4121: 'symbol' : alignment of a member was sensitive to packing
// "A structure member is aligned on a memory offset whose value is not a
//  multiple of the member's size."
// (Quoting http://msdn2.microsoft.com/en-US/library/kabt0ka3.aspx)
// I see this as a spurious warning because the Visual's compiler align members
// on word boundaries by default.

// C4127: conditional expression is constant
// eg in: while (true) ...
//

// C4251: 'urbi::UList::array' : class 'std::vector<_Ty>' needs
// to have dll-interface to be used by clients of class 'urbi::UList'
//
// In practice, I (Akim) have not seen any problems with this.


// C4275: http://msdn.microsoft.com/en-us/library/3tdb471s(VS.80).aspx
//
// include/sched/scheduler.hh:43: warning C4275:
// non dll-interface class 'boost::noncopyable_::noncopyable'
// used as base for dll-interface class 'sched::Scheduler'
//
// As far as I can see, this has never been a problem.

// C4290: http://msdn.microsoft.com/en-us/library/sa28fef8.aspx
//
// A function is declared using exception specification, which Visual
// C++ accepts but does not implement. Code with exception
// specifications that are ignored during compilation may need to be
// recompiled and linked to be reused in future versions supporting
// exception specifications.

// C4347: behavior change: 'non_template_func()' is called instead of
// 'templat_func()'.  According to the MSDN: "In Visual Studio .NET,
// if you had a template function and a nontemplate function with the
// same name as the template function, the compiler incorrectly
// treated the nontemplate function as a specialization of the
// template function.".  So this is basically a backward compatibility
// warning that we can ignore.

// C4350
// sched/job.hh:438:
// warning C4350: behavior change:
// 'std::auto_ptr<_Ty>::auto_ptr(std::auto_ptr_ref<_Ty>) throw()'
// called instead of
// 'std::auto_ptr<_Ty>::auto_ptr(std::auto_ptr<_Ty> &) throw()'

// C4355: 'this' : used in base member initializer list
//
// For instance: UVar::UVar() : rangemin(*this, ...)
//                                       ^^^^^^


// C4503: 'identifier' : decorated name length exceeded, name was
// truncated
//
// boost/bind/bind.hpp:118: warning C4503:
// 'boost::asio::detail::handler_ptr<Alloc_Traits>::handler_ptr' :
// decorated name length exceeded, name was truncated
//        with
//        [
//            Alloc_Traits=alloc_traits
//        ]
//
// See http://msdn.microsoft.com/en-us/library/074af4b6(VS.80).aspx,
// it is might understanding that it affects only human readable
// messages, but not correctness.

// C4512: 'classname' : assignment operator could not be generated

// C4514: 'strchr' : unreferenced inline function has been removed

// C4571: Informational: catch(...) semantics changed since Visual
//                C++ 7.1; structured exceptions (SEH) are no longer caught.
// http://msdn2.microsoft.com/en-us/library/55s8esw4.aspx:
// "When compiling with /EHs, a catch(...) block will not catch a structured
//  exception (divide by zero, null pointer, for example); a catch(...) block
//  will only catch explicitly-thrown, C++ exceptions."
// Reminder: SEH are Windows' exceptions (with __try, __catch etc..) we don't
// care about them so we just drop that warning.
//
// #pragma warning : there is no warning number 'number'
// this often occurs in Boost headers, and we really don't care.

// When a base class hides its copy constructor (private):
// C4625: 'classname' : copy constructor could not be generated because
//                              a base class copy constructor is inaccessible
//
// C4626: 'derived class' : assignment operator could not be generated
//                 because a base class assignment operator is inaccessible
// "An assignment operator was not accessible in a base class and was therefore
//  not generated for a derived class."

// vcxx8/VC/include/xhash:657:
// warning C4628: digraphs not supported with -Ze.
// Character sequence '<:' not interpreted as alternate token for '['

// C4640: http://msdn.microsoft.com/en-us/library/4f5c8560(VS.80).aspx
// include/libport/hierarchy.hh:224:
// warning C4640: 'res' :
// construction of local static object is not thread-safe
//
//         static std::map<TypeInfo, Id> res;


// C4668, level 4: http://msdn.microsoft.com/en-us/library/4dt9kyhy(VS.80).aspx
//        'MACRO' is not defined as a preprocessor macro, replacing
//                 with '0' for '#if/#elif'

// C4710: 'method_inline' : function not inlined

// C4711: 'method' selected for inline expansion

// C4800: 'type' : forcing value to bool 'true' or 'false' (performance
//                warning)
// "This warning is generated when a value that is not bool is assigned or
//  coerced into type bool."
// http://msdn2.microsoft.com/en-us/library/b6801kcy.aspx

// C4820: 'classname' : 'N' bytes padding added after data member 'foo'

# pragma warning(disable:                       \
                 4003 4061 4099                 \
                 4121 4127                      \
                 4251 4275 4290                 \
                 4347 4350 4355                 \
                 4503 4512 4514 4571            \
                 4619 4625 4626 4628 4640 4668  \
                 4710 4711                      \
                 4800 4820)

#endif
