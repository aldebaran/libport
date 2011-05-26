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
    "Pretty Printer for boost::optional"

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

# This class is inspired from
# http://stackoverflow.com/questions/2804641/pretty-printing-boostunordered-map-on-gdb
@gdb_pretty_printer
class BoostUnorderedMap:
    "Pretty Printer for boost::unordered_map"

    regex = re.compile('^boost::unordered_map<(.*)>$')
    @staticmethod
    def supports(type):
        return BoostUnorderedMap.regex.search(type.tag)

    class _iterator:
        def __init__ (self, value, parent):
            self.buckets = value['table_']['buckets_']
            self.bucket_count = value['table_']['bucket_count_']
            self.current_bucket = 0
            self.pair_pointer = parent.pair_pointer
            self.base_pointer = parent.base_pointer
            self.node_pointer = parent.node_pointer
            self.node = self.buckets[self.current_bucket]['next_']

        def __iter__(self):
            return self

        def next(self):
            while not self.node:
                self.current_bucket += 1
                if self.current_bucket >= self.bucket_count:
                    raise StopIteration
                self.node = self.buckets[self.current_bucket]['next_']

            iterator = self.node
            iterator = iterator.cast(self.node_pointer)
            iterator = iterator.cast(self.base_pointer)
            iterator = iterator.cast(self.pair_pointer)
            iterator = iterator.dereference()
            self.node = self.node['next_']

            return (iterator['first'], iterator['second'])

    def __init__(self, value):
        self.value = value
        type = cleanTypeOf(value)
        type_1 = type.template_argument(0)
        type_2 = type.template_argument(1)
        pair = "std::pair<%s const, %s>" % (type_1, type_2)
        self.pair_pointer = gdb.lookup_type(pair).pointer()
        self.base_pointer = gdb.lookup_type("boost::unordered_detail::value_base< %s >" % pair).pointer()
        self.node_pointer = gdb.lookup_type("boost::unordered_detail::hash_node<std::allocator< %s >, boost::unordered_detail::ungrouped>" % pair).pointer()

    def __iter__(self):
        return self._iterator(self.value, self)

    def children(self):
        return self.__iter__()

    def display_hint(self):
        return 'map'

    def to_string(self):
        return "{%s}" % ", ".join([ "%s: %s" % (k, v) for k, v in self.__iter__() ])


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
