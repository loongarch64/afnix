// ---------------------------------------------------------------------------
// - t_transient.cpp                                                         -
// - afnix nuts and bolts - transient tester                                 -
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

#include "transient.tcc"

int main (int, char**) {
  using namespace afnix;

  // create a new tansient
  t_transient<long> tl = new long[256];

  // push and test array data
  for (long k = 0L; k < 256; k++) tl[k] = k;
  for (long k = 0L; k < 256; k++) {
    if (tl[k] != k) return 1;
    if ((*tl)[k] != k) return 1;
  }

  // done
  return 0;
}
