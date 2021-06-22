// ---------------------------------------------------------------------------
// - t_array.cpp                                                             -
// - afnix nuts and bolts - array tester                                     -
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

#include "array.tcc"

int main (int, char**) {
  using namespace afnix;

  // create a new array
  t_array<long> al;
  if (al.length() != 0L) return 1;

  // push and test array data
  for (long k = 0L; k < 256; k++) al.push (k);
  if (al.length () != 256L) return 1;
  for (long k = 0L; k < 256; k++) {
    if (al.get (k) != k) return 1;
  }
  // set and test the array
  for (long k = 0L; k < 256; k++) al.set (255L-k,k);
  for (long k = 0L; k < 256; k++) {
    if (al.get (k) != (255L-k)) return 1;
  }
  
  // reset the array
  al.reset ();
  if (al.length() != 0L) return 1;

  // done
  return 0;
}
