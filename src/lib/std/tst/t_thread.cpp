// ---------------------------------------------------------------------------
// - t_thread.cpp                                                            -
// - standard object library - thread class tester                           -
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

#include "Thread.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // check the thread functions
  if (Thread::ismaster () == false) return 1;

  // create a new thread of control
  Thread thr;
  if (thr.repr  () != "Thread") return 1;
  if (thr.isend () == true)     return 1;

  // wait for all
  Thread::wall ();

  // success
  return 0;
}
