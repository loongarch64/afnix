// ---------------------------------------------------------------------------
// - t_outstr.cpp                                                            -
// - standard object library - output string class tester                    -
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

#include "OutputString.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a default output string
  OutputString os;
  
  // fill it and check
  os.write ("hello");
  if (os.length   () != 5)       return 1;
  if (os.tostring () != "hello") return 1;

  // flush and test
  os.flush ();
  if (os.length () != 0) return 1;

  // everything is fine
  return 0;
}
