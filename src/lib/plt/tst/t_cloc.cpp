// ---------------------------------------------------------------------------
// - t_cloc.cpp                                                              -
// - standard platform library - localization function tester                -
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

#include "cloc.hpp"

int main (int, char**) {
  using namespace afnix;

  // check for the codeset
  if (c_mapcset (nullptr)                != LOC_ASC_00) return 1;
  if (c_mapcset ("C")                 != LOC_ASC_00) return 1;
  if (c_mapcset ("POSIX")             != LOC_ASC_00) return 1;
  if (c_mapcset ("en")                != LOC_ASC_00) return 1;
  if (c_mapcset ("en_US")             != LOC_ASC_00) return 1;
  if (c_mapcset ("en_US.iso-8859-1")  != LOC_ISO_01) return 1;
  if (c_mapcset ("en_US.iso-8859-2")  != LOC_ISO_02) return 1;
  if (c_mapcset ("en_US.iso-8859-3")  != LOC_ISO_03) return 1;
  if (c_mapcset ("en_US.iso-8859-4")  != LOC_ISO_04) return 1;
  if (c_mapcset ("en_US.iso-8859-5")  != LOC_ISO_05) return 1;
  if (c_mapcset ("en_US.iso-8859-6")  != LOC_ISO_06) return 1;
  if (c_mapcset ("en_US.iso-8859-7")  != LOC_ISO_07) return 1;
  if (c_mapcset ("en_US.iso-8859-8")  != LOC_ISO_08) return 1;
  if (c_mapcset ("en_US.iso-8859-9")  != LOC_ISO_09) return 1;
  if (c_mapcset ("en_US.iso-8859-10") != LOC_ISO_10) return 1;
  if (c_mapcset ("en_US.iso-8859-11") != LOC_ISO_11) return 1;
  if (c_mapcset ("en_US.iso-8859-13") != LOC_ISO_13) return 1;
  if (c_mapcset ("en_US.iso-8859-14") != LOC_ISO_14) return 1;
  if (c_mapcset ("en_US.iso-8859-15") != LOC_ISO_15) return 1;
  if (c_mapcset ("en_US.iso-8859-16") != LOC_ISO_16) return 1;
  if (c_mapcset ("en_US.utf8")        != LOC_UTF_08) return 1;

  // everything is fine
  return 0;
}
