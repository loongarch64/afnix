// ---------------------------------------------------------------------------
// - t_central.cpp                                                           -
// - standard object library - central object control tester                 -
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

#include "Central.hpp"

int main (int, char**) {
  using namespace afnix;

  // -------------------------------------------------------------------------
  // - quark tester                                                          -
  // -------------------------------------------------------------------------

  // check for the nil quark
  if (Central::qmap (0).isnil () == false) return -1;

  long hq = Central::intern ("hello");
  long wq = Central::intern ("world");

  if (hq <= 0) return -1;
  if (wq <= 0) return -1;

  if (Central::qmap (hq) != "hello") return -1;
  if (Central::qmap (wq) != "world") return -1;

  // that's all folks
  return 0;
}
