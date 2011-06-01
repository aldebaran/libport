# Copyright (C) 2011, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.

import sys
import gdb
import re

_extract_number = re.compile("(\d+)")
_range_re = re.compile('^(?P<lb>[][])(?P<l>\S+)\s+(?P<h>\S+)(?P<hb>[][])$')
_gdb_version = [ int(v.group()) for v in _extract_number.finditer(gdb.VERSION) ]

def _compare_versions(v1, v2):
    def reduce_cmp(x, y):
        if x != 0:
            return x
        else:
            return cmp(*y)
    return reduce(reduce_cmp, zip(v1, v2) + [(len(v1), len(v2))], 0)

def _in_version_range(ver, range):
    match = _range_re.match(range)
    if not match:
        return False
    l = match.group('l')
    if l != '...':
        rule = { ']' : [1], '[' : [0, 1] }
        lv = [ int(v.group()) for v in _extract_number.finditer(l) ]
        lbr = rule[match.group('lb')]
        if not (_compare_versions(ver, lv) in lbr):
            return False
    h = match.group('h')
    if h != '...':
        rule = { ']' : [-1, 0], '[' : [-1] }
        hv = [ int(v.group()) for v in _extract_number.finditer(h) ]
        hbr = rule[match.group('hb')]
        if not (_compare_versions(ver, hv) in hbr):
            return False
    return True

# TODO: Add a mean to declare range of versions, to avoid declaring multiple
# times the same variables.  This is critical for other decorator such as
# gdb_command and gdb_pretty_printer to avoid possibly complex operations.
def require_gdb_version(range):
    """Select a function implementation based on its gdb requierements."""

    def notimpl(value):
        locals = sys._getframe(1).f_locals
        # Keep the current version if it exists.
        if value.__name__ in locals:
            if ('_gdb_' + value.__name__) in locals:
                if locals['_gdb_' + value.__name__]['impl']:
                    return locals['_gdb_' + value.__name__]['fun']

        def subst(*args, **kwargs):
            print "Error: %s is not implemented for this version of gdb" % value.__name__
        subst.__name__ = value.__name__
        subst.__doc__  = value.__doc__
        subst.__dict__ = value.__dict__
        subst.__gdb_version_impl__ = True
        locals['_gdb_' + value.__name__] = {
            'versionRange' : range,
            'impl' : False
            }
        return subst

    def canimpl(value):
        locals = sys._getframe(1).f_locals
        # Keep the documentation of previous versions
        if value.__name__ in locals:
            value.__doc__ = locals[value.__name__].__doc__
        value.__gdb_version_impl__ = True
        locals['_gdb_' + value.__name__] = {
            'versionRange' : range,
            'impl' : True,
            # store the value to avoid issues with decorators which are not
            # projections.
            'fun' : value,
            }
        return value

    if _in_version_range(_gdb_version, range):
        return canimpl
    else:
        return notimpl


def is_gdb_compatible(value):
    if hasattr(value, '__gdb_version_impl__'):
        if value.__gdb_version_impl__:
            return True
    else:
        return True
    return False


# TODO: Implement it by using gdb.execute
@require_gdb_version(']... 7.2]')
def gdbBreakpoint():
    "Function to get gdb.Breakpoint when it exists."
    return object

@require_gdb_version(']7.2 ...[')
def gdbBreakpoint():
    return gdb.Breakpoint


# The content of this function is inspired from qtcreator sources.
# 2.2-ru/share/qtcreator/gdbmacros/dumper.py
@require_gdb_version(']... 7.1[')
def gdb_parse_and_eval(exp):
    """Evaluate an expression and return the gdb.Value or None which
    correspond to the result of the evaluation."""

    # Work around non-existing gdb.parse_and_eval as in released 7.0
    overwrite = gdb.parameter("logging overwrite")
    redirect = gdb.parameter("logging redirect")
    file = gdb.parameter("logging file")
    # Unfortunately there is no mean to check if the logging is enabled or
    # not.  TODO: So we look for the modification date of the logging file
    # to check if it is enabled.
    logging = False
    def restore():
        gdb.execute("set logging off")
        gdb.execute("set logging redirect off")
        gdb.execute("set logging overwrite off")
        gdb.execute("set logging file " + file)
        gdb.execute("set logging redirect " + ("on" if redirect else "off"))
        gdb.execute("set logging logging " + ("on" if logging else "off"))
        # re-enable it after the start of the logging.
        gdb.execute("set logging overwrite " + ("on" if overwrite else "off"))
    gdb.execute("set logging off")
    gdb.execute("set logging file .gdb.python.tmp")
    gdb.execute("set logging overwrite on")
    gdb.execute("set logging redirect on")
    gdb.execute("set logging on")
    try:
        gdb.execute("print %s" % exp)
    except:
        restore()
        return None
    restore()
    return gdb.history(0)

@require_gdb_version('[7.1 ...[')
def gdb_parse_and_eval(exp):
    return gdb.parse_and_eval(exp)



def cleanTypeOf(value):
    "clean the type of the value"
    type = value.type

    # @require_gdb_version('[7.2 ...[')
    if hasattr(value, 'dynamic_type'):
        type = value.dynamic_type

    if type.code == gdb.TYPE_CODE_REF:
        type = type.target()

    type = type.unqualified().strip_typedefs()

    return type


@require_gdb_version(']... 7.2]')
def dynamicType(value):
    """Get the dynamic type of a value."""

    # unfortunately we have no other choices yet.
    return value.type

@require_gdb_version(']7.2 ...[')
def dynamicType(value):
    return value.dynamic_type


@require_gdb_version('[7.2 ...[')
def castToDynamicType(value, name = None):
    """cast a value to its dynamic type, used the name if the dynamic_type
    is not available."""

    if name == None:
        return value
    dyntype = gdb.lookup_type(name)
    return value.cast(dyntype)

@require_gdb_version(']... 7.2[')
def castToDynamicType(value, name = None):
    dyntype = value.dynamic_type
    return value.cast(dyntype)


@require_gdb_version(']... 7.1[')
def gdbTypeField_is_base_class(field):
    try:
        gdb.lookup_type(field.name)
        return True
    except:
        return False

@require_gdb_version('[7.1 ...[')
def gdbTypeField_is_base_class(field):
    return field.is_base_class


def inherit(deriv, base):
    """Return True if the first type derives from the second type."""
    def fold(x, y):
        if (not x) and gdbTypeField_is_base_class(y):
            t = gdb.lookup_type(y.name)
            return reduce(fold, t.fields(), t.tag == base.tag)
        else:
            return x
    return reduce(fold, deriv.fields(), deriv.tag == base.tag)


def recFields(type):
    l = []
    def collect(x):
        if x.is_base_class:
            t = gdb.lookup_type(x.name)
            for y in t.fields():
                collect(y)
        else:
            l.append(x.name)
    for y in type.fields():
        collect(y)
    return l


def find_pretty_printer(value):
    "Find a pretty printer suitable for value"

    type = cleanTypeOf(value)
    if type.tag == None:
        return None

    # The list is reversed to benefit from inhertance where more specialized
    # cases are using inheritance to avoid the re-implementation of previous
    # functionalities.  So type matching is made from more specialized
    # classes to more general classes.
    for pretty_printer in reversed(list_pretty_printers):
        if pretty_printer.supports(type):
            return pretty_printer(value)

    return None

def register_printers(obj):
    "Register Pretty Printers."
    if obj == None:
        obj = gdb
    obj.pretty_printers.append(find_pretty_printer)

# insert the list of printers which are using another interface than the one
# defined inside the gdb manual.  This interface implies that each class
# need a 'supports' static function which returns a value which can be
# interpreted as True if the class can handle such type.
register_printers(gdb)



# Define annotation and their processing methods to avoid using post
# declarations of classes.
list_pretty_printers = []
def gdb_pretty_printer(pretty_printer):
    "Annotation to register a Pretty Printer into gdb"
    if is_gdb_compatible(pretty_printer):
        list_pretty_printers.append(pretty_printer)
    return pretty_printer

list_commands = []
def gdb_command(command):
    "Annotation to register a Command into gdb"

    if is_gdb_compatible(command):
        list_commands.append(command)
    return command

def gdb_register_commands():
    "This function register commands annotated with @gdb_commands"
    for cmd in list_commands:
        cmd()
    del list_commands[:]
