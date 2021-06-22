// ---------------------------------------------------------------------------
// - t_style.cpp                                                             -
// - standard object library - style class tester                            -
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

#include "Style.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default style
  Style style;
  if (style.getwght () != Style::WGHT_N) return 1;
  if (style.getalgn () != Style::ALGN_L) return 1;
  if (style.gettsiz () != 0) return 1;
  if (style.getfill () != ' ') return 1;
  if (style.getpsiz () != 0) return 1;
  if (style.getsflg () != false) return 1;

  // check mutators
  style.setwght (Style::WGHT_B);
  if (style.getwght () != Style::WGHT_B) return 1;
  style.setalgn (Style::ALGN_R);
  if (style.getalgn () != Style::ALGN_R) return 1;
  style.settsiz (10L);
  if (style.gettsiz () != 10L) return 1;
  style.setfill ('+');
  if (style.getfill () != '+') return 1;
  style.setpsiz (5L);
  if (style.getpsiz () != 5L) return 1;
  style.setsflg (true);
  if (style.getsflg () == false) return 1;

  // ok - everything is fine
  return 0;
}
