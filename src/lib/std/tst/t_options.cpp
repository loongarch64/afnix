// ---------------------------------------------------------------------------
// - t_options.cpp                                                           -
// - standard object library - options class tester                          -
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

#include "Options.hpp"

int main (int, char**) {
  using namespace afnix;
  
  const String UMSG          = "t_options [options] file";
  const String H_MESG        = "  -h         print this help message";
  const String V_MESG        = "  -v         print the version";
  const String F_DEBUG_MESG  = "  -f debug   enable debug  mode";
  const String F_ASSERT_MESG = "  -f assert  enable assert mode";
  const String O_FILE_MESG   = "  -o [file]  set output file name";

  // create a new options object
  Options opt;
  opt.setumsg (UMSG);

  // add uniq option
  opt.add (Options::UNIQ, 'h', H_MESG);
  opt.add (Options::UNIQ, 'v', V_MESG);
  // add list options
  opt.add ('f', "debug",  F_DEBUG_MESG);
  opt.add ('f', "assert", F_ASSERT_MESG);
  // add a string option
  opt.add (Options::SOPT, 'o', O_FILE_MESG);

  // ok - everything is fine
  return 0;
}
