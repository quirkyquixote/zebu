
=============================================

This library provides utilities to build and walk Abstract Syntax Trees; it was
originally designed to work with the kind of parsers generated by Yacc or GNU
Bison, hence the name and the jokeyish zz namespace.

Requirements
------------

Just C.

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

Node "tokens" are const strings; specifically, all nodes of the same type use
the same string as token, that doubles as the token name when printing messages
or formatting trees.

All nodes in the AST may hold data of type int, unsigned int, double, char\*
(automatically allocated by the tree), or void\* (the referenced memory must be
managed by the user).

Trees can be given a node size larger than sizeof(struct zz_node): the extra
bytes may be used to store user-defined fields.
