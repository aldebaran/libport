/* lt__debug.h -- debugging and logging

   Copyright (C) 2009 Free Software Foundation, Inc.
   Written by Akim Demaille, 2009

   NOTE: The canonical source of this file is maintained with the
   GNU Libtool package.  Report bugs to bug-libtool@gnu.org.

GNU Libltdl is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

As a special exception to the GNU Lesser General Public License,
if you distribute this file as part of a program or library that
is built using GNU Libtool, you may include this file under the
same distribution terms that you use for the rest of that program.

GNU Libltdl is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with GNU Libltdl; see the file COPYING.LIB.  If not, a
copy can be downloaded from http://www.gnu.org/licenses/lgpl.html,
or obtained by writing to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#if !defined(LT__DEBUG_H)
#define LT__DEBUG_H 1

#include "lt_system.h"

LT_BEGIN_C_DECLS

/* Register a message to log functions.  */
int lt_dllog (unsigned level, const char* ormat, ...);

/* --- DEBUGGING --- */

#if defined LT_DEBUG_LOADERS
# define LT_LOG(Args)                           \
  lt_dllog Args
#else
# define LT_LOG(Args)
#endif

#define LT_LOG0(Level, Format)                  \
  LT_LOG((Level, Format))
#define LT_LOG1(Level, Format, Arg1)            \
  LT_LOG((Level, Format, Arg1))
#define LT_LOG2(Level, Format, Arg1, Arg2)      \
  LT_LOG((Level, Format, Arg1, Arg2))
#define LT_LOG3(Level, Format, Arg1, Arg2, Arg3)        \
  LT_LOG((Level, Format, Arg1, Arg2, Arg3))

#define LT_NONNULL(Arg)                         \
  ((Arg) ? (Arg) : "(null)")
#define LT_NAME(Arg)                            \
  ((Arg) && (Arg)->name ? (Arg)->name : "(null)")
LT_END_C_DECLS

#endif /*!defined(LT__DEBUG_H)*/
