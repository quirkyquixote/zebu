Zebu - an Abstract Syntax Tree implementation
=============================================

This library provides utilities to build and walk Abstract Syntax Trees; it was
originally designed to work with the kind of parsers generated by Yacc or GNU
Bison, hence the name and the jokeyish zz namespace.

Requirements
------------

Just ANSI C.

Installation
------------

zebu provices a simple Makefile; to build from scratch:

    make all
    make install

Usage
-----

The zebu.h file is fully documented, and gives a relatively good idea of the
API; the bullet points are:

The zz_tree struct manages a full tree, including resource allocation.
Individual nodes are of type zz_node.

To create a tree, you must provide a list of valid node types zz_node_type: the
index of the object in the table represents the "token", and the type includes
a string that is used to print the token in error messages and a field
indicating what kind of data may be attached to that kind of node.

Leaves in the AST may hold data of type int, unsigned int, double, char\*
(automatically allocated by the tree), or void\* (the referenced memory must be
managed by the user); inner nodes do not hold data, just more nodes.
