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


# This class is inspired from
# https://github.com/ruediger/Boost-Pretty-Printer/ excepts that it shrink
# the typename to make it shorter.
@gdb_pretty_printer
class BoostOptional(object):
    "Pretty Printer for boost::optional (Boost.Optional)"

    regex = re.compile('^boost::optional<(.*)>$')
    @staticmethod
    def supports(type):
        return BoostOptional.regex.search(type.tag)

    def __init__(self, value):
        self.typename = cleanTypeOf(value).tag
        self.param = cleanTypeOf(value).template_argument(0)
        self.value = value

    class _iterator:
        def __init__(self, member, empty):
            self.member = member
            self.done = empty

        def __iter__(self):
            return self

        def next(self):
            if(self.done):
                raise StopIteration
            self.done = True
            return self.member.dereference()

    def __iter__(self):
        initialized = self.value['m_initialized']
        if(not initialized):
            return self._iterator('', True)
        else:
            match = BoostOptional.regex.search(self.typename)
            if match:
                try:
                    membertype = self.param.pointer()
                    member = self.value['m_storage']['dummy_']['data'].address.cast(membertype)
                    return self._iterator(member, False)
                except:
                    return self._iterator('', True)

    def to_string(self):
        initialized = self.value['m_initialized']
        if(not initialized):
            return "%s?()" % self.param
        else:
            return "%s?(%s)" % (self.param, self.__iter__().next())

@gdb_pretty_printer
class LibportIntrusivePtr(object):
    "Pretty Printer for libport::intrusive_ptr<T>"

    regex = re.compile('^libport::intrusive_ptr<(.*)>$')
    @staticmethod
    def supports(type):
        return LibportIntrusivePtr.regex.search(type.tag)

    def __init__(self, value):
        self.param = cleanTypeOf(value).template_argument(0)
        self.value = value

    def pointer(self):
        return self.value['pointee_']

    def to_string(self):
        # should used param.pointer().tag here, but this returns None :(
        val = self.pointer()
        vtype = val.dynamic_type
        if vtype != None and vtype.target() != None:
            return '(%s) %s' % (vtype, val)
        else:
            return '(%s) %s' % (self.param.pointer(), val)

@gdb_pretty_printer
class LibportSymbol(object):
    "Pretty Printer for libport::Symbol"

    regex = re.compile('^libport::Symbol$')
    @staticmethod
    def supports(type):
        return LibportSymbol.regex.search(type.tag)

    def __init__(self, value):
        self.value = value

    def to_string(self):
        strp = self.value['str_']
        if strp != 0:
            return strp.dereference()
        else:
            return '<empty>'

@gdb_pretty_printer
class LibportVector(object):
    "Pretty Printer for libport::Vector"

    regex = re.compile('^libport::Vector<(.*)>$')
    @staticmethod
    def supports(type):
        return LibportVector.regex.search(type.tag)

    def __init__(self, value):
        self.value = value

    class _iterator:
        def __init__(self, tab, size):
            self.tab = tab
            self.size = size
            self.i = 0

        def __iter__(self):
            return self

        def next(self):
            if(self.i < self.size):
                self.i += 1
                return self.tab[self.i - 1]
            else:
                raise StopIteration

    def __iter__(self):
        size = self.value['size_']
        data = self.value['data_']
        tab = None
        if size > 0:
            # Strangely we need to dereference the pointer before making the
            # cast to the array type.
            tab = data.dereference().cast(data.type.target().array(size - 1))
        return self._iterator(tab, size)

    def display_hint(self):
        return 'array'

    def to_string(self):
        tab = self.__iter__().tab
        if tab:
            return "%s" % tab
        else:
            return "{}"


gdb_register_commands()
