# Copyright (C) 2011, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.

import re
import gdb

from libport.tools import *


@gdb_pretty_printer
class SchedJob(object):
    "Pretty Printer for sched::Job"

    regex = re.compile('^sched::Job$')
    @staticmethod
    def supports(type):
        return SchedJob.regex.search(type.tag)

    def __init__(self, value):
        self.value = value

    # Damn, the std::list provided by the stl is not available for iterating
    # over the content of it in python.  We would need to check for it by
    # iterating over the list of pretty-printers, then asking for the
    # children method.

    extract_state = re.compile('^sched::(?P<state>\w+)$')
    def to_string(self):
        child = ""
        parent = self.value['parent_']
        if parent:
            child = " (child of %s)" % parent
        state = self.extract_state.match(str(self.value['state_']))
        if state:
            state = state.group('state')
        else:
            state = "unknown"
        return "Job(%s%s is %s)" % (self.value.address, child, state)
