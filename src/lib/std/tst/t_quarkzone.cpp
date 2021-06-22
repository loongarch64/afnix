// ---------------------------------------------------------------------------
// - t_quarkzone.cpp                                                         -
// - standard object library - quark zone class tester                       -
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

#include "QuarkZone.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a quark zone and check default
  QuarkZone* zone = new QuarkZone (2);
  if (zone->length () != 0) return 1;

  // intern some string and check
  long hq = zone->intern ("hello");
  long wq = zone->intern ("world");

  // check the zone before resizing
  if (zone->length() != 2) return 1;
  if (zone->exists (hq) == false) return 1;
  if (zone->exists (wq) == false) return 1;

  // check quark values
  if (zone->get (0)!= hq) return 1;
  if (zone->get (1)!= wq) return 1;

  // check quark string
  if (zone->tostring (0)!= "hello") return 1;
  if (zone->tostring (1)!= "world") return 1;

  // add a new quar by resizing
  long yq = zone->intern ("2000");
  if (zone->length() != 3)  return 1;
  if (zone->get   (2)!= yq) return 1;

  // this is it
  delete zone;
  return 0;
}
  
