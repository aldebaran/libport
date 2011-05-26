# Copyright (C) 2011, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.

import gdb


def cleanTypeOf(value):
    "clean the type of the value"
    type = value.type

    # dynamic_type is introduced after the version 7.2 (which is not yet
    # released)
    if hasattr(value, 'dynamic_type'):
        type = value.dynamic_type

    if type.code == gdb.TYPE_CODE_REF:
        type = type.target()

    type = type.unqualified().strip_typedefs()

    return type

def castToDynamicType(value, name = None):
    """cast a value to its dynamic type, used the name if the dynamic_type
    is not available."""

    if hasattr(value, 'dynamic_type'):
        dyntype = value.dynamic_type
    else:
        dyntype = gdb.lookup_type(name)
    return value.cast(dyntype)

def inherit(deriv, base):
    def fold(x, y):
        if (not x) and y.is_base_class:
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
    list_pretty_printers.append(pretty_printer)
    return pretty_printer

list_commands = []
def gdb_command(command):
    "Annotation to register a Command into gdb"
    list_commands.append(command)
    # globals is local to the current module :(
    #globals()[command.__name__] = command
    #command()
    return command

def gdb_register_commands():
    "This function register commands annotated with @gdb_commands"
    for cmd in list_commands:
        cmd()
    del list_commands[:]
