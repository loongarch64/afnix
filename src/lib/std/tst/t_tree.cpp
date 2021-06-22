// ---------------------------------------------------------------------------
// - t_tree.cpp                                                              -
// - standard object library - binary tree class tester                      -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Tree.hpp"
#include "Integer.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a tree and check
  Tree* tree = new Tree;
  if (tree->repr () != "Tree") return 1;

  // check 0 length
  if (tree->length () != 0L) return 1;

  // add a node and check
  tree->add (0L, new Integer(0L));
  if (tree->length () != 1L) return 1;
  if (tree->exists (0L) == false) return 1;
  Integer* ival = dynamic_cast <Integer*> (tree->find (0));
  if ((ival == nullptr) || (ival->tolong() != 0L)) return 1;

  tree->add (1L, new Integer(1L));
  if (tree->length () != 2L) return 1;
  if (tree->exists (1L) == false) return 1;
  ival = dynamic_cast <Integer*> (tree->find (1L));
  if ((ival == nullptr) || (ival->tolong() != 1L)) return 1;

  tree->add (-1L, new Integer(-1L));
  if (tree->length () != 3L) return 1;
  if (tree->exists (-1L) == false) return 1;
  ival = dynamic_cast <Integer*> (tree->find (-1L));
  if ((ival == nullptr) || (ival->tolong() != -1L)) return 1;

  tree->add (4L, new Integer(4L));
  if (tree->length () != 4L) return 1;
  if (tree->exists (4L) == false) return 1;
  ival = dynamic_cast <Integer*> (tree->find (4L));
  if ((ival == nullptr) || (ival->tolong() != 4L)) return 1;

  tree->add (3L, new Integer(3L));
  if (tree->length () != 5L) return 1;
  if (tree->exists (3L) == false) return 1;
  ival = dynamic_cast <Integer*> (tree->find (3L));
  if ((ival == nullptr) || (ival->tolong() != 3L)) return 1;
  
  // delete everything
  delete tree;
  return 0;
}
