/* lt__debug.c -- debugging and logging
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
copy can be downloaded from  http://www.gnu.org/licenses/lgpl.html,
or obtained by writing to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "lt__private.h"

static lt__log_list* lt_logs = 0;

static
lt__log_list*
lt__log_new (lt_dllog_function   *function,
             void                *data,
             lt__log_list        *next)
{
  lt__log_list* res = MALLOC (lt__log_list, 1);
  res->function = function;
  res->data = data;
  res->next = next;
  return res;
}

static
void
lt__log_delete (lt__log_list* l)
{
  assert (!l->next);
  FREE (l);
}


int
lt_dladd_log_function (lt_dllog_function *fun, void *data)
{
  lt_logs = lt__log_new (fun, data, lt_logs);
  return 0;
}

/* Look in and remove from *LIST.  */
static
int
lt__dlremove_log_function (lt__log_list** list,
                           lt_dllog_function *fun)
{
  if (!list || !*list)
    return 1;
  else if ((*list)->function == fun)
    {
      lt__log_list* found = *list;
      *list = found->next;
      found->next = 0;
      lt__log_delete (found);
      return 0;
    }
  else
    return lt__dlremove_log_function (&(*list)->next, fun);
}

int
lt_dlremove_log_function (lt_dllog_function *fun)
{
  return lt__dlremove_log_function (&lt_logs, fun);
}

int
lt_dllog (unsigned level, const char* format, ...)
{
  unsigned errors = 0;
  if (lt_logs)
    {
      va_list args;
      va_start(args, format);
      {
        lt__log_list *l;
        for (l = lt_logs; l; l = l->next)
          {
            va_list as;
#if _MSC_VER
            // FIXME: Make this nice looking.
            as = args;
#else
            va_copy(as, args);
#endif
            errors += (*l->function) (l->data, level, format, as);
            va_end(as);
          }
      }
      va_end(args);
    }
  return errors;
}
