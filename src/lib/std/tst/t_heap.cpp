// ---------------------------------------------------------------------------
// - t_heap.cpp                                                              -
// - standard object library - heap class tester                             -
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

#include "Heap.hpp"
#include "Integer.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new heap
  Heap heap;
  
  // check the size and flag
  if (heap.empty  () == false) return 1;
  if (heap.length () != 0)     return 1;

  // add a series of integer in reverse order
  for (long i = 9; i >= 0; i--) {
    heap.add (i, new Integer (i));
  }
  if (heap.length () != 10) return 1;

  // check heap value order
  for (long i = 0; i < 10; i++) {
    Integer* iobj = dynamic_cast <Integer*> (heap.pop ());
    if (iobj == nullptr) return 1;
    if (iobj->tolong() != i) return 1;
    Object::dref (iobj);
  }
  // check the size and flag
  if (heap.empty  () == false) return 1;
  if (heap.length () != 0)     return 1;

  // add a double serie
  for (long i = 9; i >= 0; i--) {
    heap.add (i, new Integer (i));
  }
  for (long i = 9; i >= 0; i--) {
    heap.add (i, new Integer (i));
  }
  if (heap.length () != 20) return 1;

  // check  for double value
  for (long i = 0; i < 10; i++) {
    // check first value
    Integer* iobj = dynamic_cast <Integer*> (heap.pop ());
    if (iobj == nullptr) return 1;
    if (iobj->tolong() != i) return 1;
    Object::dref (iobj);
    // check second value
    iobj = dynamic_cast <Integer*> (heap.pop ());
    if (iobj == nullptr) return 1;
    if (iobj->tolong() != i) return 1;
    Object::dref (iobj);
  }
  // check the size and flag
  if (heap.empty  () == false) return 1;
  if (heap.length () != 0)     return 1;

  // reset the heap and check
  heap.reset ();
  if (heap.length () != 0) return 1;
  
  // check key flag
  if (heap.ismink () == true) return 1;
  if (heap.ismaxk () == true) return 1;

  // set a min key
  heap.setmink (10LL);
  if (heap.ismink  () == false) return 1;
  if (heap.getmink () != 10LL)  return 1;
  Integer* omin = new Integer (9);
  heap.add (9, omin);
  if (heap.length () != 0)      return 1;
  Object::cref (omin);

  // set a max key
  heap.setmaxk (100LL);
  if (heap.ismaxk  () == false) return 1;
  if (heap.getmaxk () != 100LL) return 1;
  Integer* omax = new Integer (110);
  heap.add (110, omax);
  if (heap.length () != 0)      return 1;
  Object::dref (omax);
  
  // reset the key and check
  heap.rstmink ();
  heap.rstmaxk ();
  if (heap.ismink () == true) return 1;
  if (heap.ismaxk () == true) return 1;

  // ok - everything is fine
  return 0;
}
