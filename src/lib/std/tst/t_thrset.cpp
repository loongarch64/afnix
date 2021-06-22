// ---------------------------------------------------------------------------
// - t_thrset.cpp                                                            -
// - standard object library - thread set class tester                       -
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

#include "Thrset.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  static const long MSIZ = 10L;

  // create a default thread set
  Thrset tset (MSIZ);
  if (tset.repr () != "Thrset") return 1;

  // check the initial predicates
  if (tset.full  () == true)  return 1;
  if (tset.empty () == false) return 1;

  // check the initial size and length
  if (tset.getmsiz () != MSIZ) return 1;
  if (tset.length  () != 0)    return 1;

  // allocate thread
  for (long i = 0; i < MSIZ; i++) tset.request ();

  // check size and predicate
  if (tset.full   () == false) return 1;
  if (tset.length () != MSIZ)  return 1;

  // success
  return 0;
}
