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

// C4355: 'this' : used in base member initializer list
//
// For instance: UVar::UVar() : rangemin(*this, ...)
//                                       ^^^^^^


// C4251: 'urbi::UList::array' : class 'std::vector<_Ty>' needs
// to have dll-interface to be used by clients of class 'urbi::UList'
//
// In practice, I (Akim) have not seen any problems with this.


// C4512: 'classname' : assignment operator could not be generated


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

// C4668: 'MACRO' is not defined as a preprocessor macro, replacing
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
                 4251                           \
                 4347 4355                      \
                 4512 4571                      \
                 4619 4625 4626 4668            \
                 4710 4711                      \
                 4800 4820 )

#endif
