m4_pattern_forbid([^ACX_])dnl
m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_SDK
# --------
# Instantiate the files required by SDKs.
#
# *WARNING*: the name of the package is check to decide which
# precursor of param.mk is to be used.  This is somewhat fragile:
# if the name of the package changes, update this.  Maybe there is
# another criterium to use, but I don't know which currently.
AC_DEFUN([URBI_SDK],
[AC_CONFIG_FILES([sdk/umake:build-aux/umake.in], [chmod +x sdk/umake])

AC_CONFIG_FILES([sdk/Makefile])
# If we are in Liburbi-C++, then the precursor is in build-aux.
m4_pushdef([AC_param_mk_in],
	   [m4_case(AC_PACKAGE_NAME,
		    [liburbi-c++],      [build-aux/param-pc.mk.in],
		    [urbiengine-linux], [build-aux/param-pc.mk.in],
		    [sdk/param.mk])])
AC_CONFIG_FILES([sdk/param.mk:]AC_param_mk_in)
m4_popdef([AC_param_mk_in])
])

## Local Variables:
## mode: autoconf
## End:
