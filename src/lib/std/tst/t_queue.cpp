// ---------------------------------------------------------------------------
// - t_queue.cpp                                                             -
// - standard object library - queue class tester                            -
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

#include "Queue.hpp"
#include "String.hpp"

int main (int, char**) {
  using namespace afnix;

  // queue our favorite message
  Queue queue;
  queue.enqueue (new String ("hello"));
  queue.enqueue (new String ("world"));

  // check length and dequeue
  if (queue.empty  () == true) return 1;
  if (queue.length () != 2) return 1;

  String* str = dynamic_cast <String*> (queue.dequeue ());
  if (str == nullptr) return 1;
  if (*str != "hello") return 1;
  Object::dref (str);
  str = dynamic_cast <String*> (queue.dequeue ());
  if (str == nullptr) return 1;
  if (*str != "world") return 1;
  Object::dref (str);

  // check length
  if (queue.empty  () == false) return 1;
  if (queue.length () != 0) return 1;

  // done
  return 0;
}
