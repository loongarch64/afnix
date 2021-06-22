// ---------------------------------------------------------------------------
// - t_cthr.cpp                                                              -
// - standard platform library - thread function tester                      -
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

#include "cthr.hpp"

// a simple global variable
static int status = 1;

// this procedure is run in a task
static void* tsk_test (void* args) {
  // check for nullptr
  if (args == nullptr) {
    status = -1;
    return nullptr;
  }
  // map the integer
  auto ival = reinterpret_cast<int*> (args);
  status = *ival;
  return nullptr;
}

// this procedure is ran in a thread and check for its self equality
static void* thr_test (void* args) {
  using namespace afnix;

  // make sure we are not the master
  if (c_thrmaster () == true) {
    status = -1;
    return nullptr;
  }
  // check for argument and execution
  if (args != 0) {
    status = -1;
    return nullptr;
  }
  void* thr = c_thrself ();
  if (thr == 0) {
    status = -1;
    return nullptr;
  }
  if (c_threqual (thr) == false) {
    status = -1;
    return nullptr;
  }
  if (c_thrgetgid (thr) != 0) {
    status = -1;
    return nullptr;
  }
  // set status and exit
  status = 0;
  return nullptr;
}

int main (int, char**) {
  using namespace afnix;

  // create a task to run
  int ival = 2;
  void* ptsk = c_tsknew (tsk_test, &ival);
  if (ptsk == nullptr) return -1;
  // wait for completion
  c_tskwait (ptsk);
  if (status != ival) return -1;
  c_tskdel (ptsk);

  // with no thread - check that getting self still work
  void* thr = c_thrself ();
  if (thr != 0) return -1;

  // check for equal here
  if (c_threqual (thr) == false) return -1;

  // check for master
  if (c_thrmaster () == false) return -1;

  // start a new thread and eventually wait
  s_targ targ;
  targ.p_func = thr_test;
  thr = c_thrnew (targ);
  c_thrwait (thr);

  // test and destroy
  if (status != 0) return -1;
  if (c_thrgetres (thr) != 0) return -1;
  c_thrdel (thr);

  // here it is
  return 0;
}
