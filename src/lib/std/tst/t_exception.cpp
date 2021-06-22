// ---------------------------------------------------------------------------
// - t_exception.cpp                                                         -
// - standard object library - exception class tester                        -
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

#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // create an exception class
  Exception e1 ("hello-world");
  Exception e2 ("hello", "world");
  Exception e3 ("hello", "world", "bonjour");

  // check for hello
  if (e1.geteid () != "hello-world") return 1;
  if (e2.geteid () != "hello")       return 1;
  if (e3.geteid () != "hello")       return 1;

  // check for reason
  if (e2.getval () != "world")         return 1;
  if (e3.getval () != "world bonjour") return 1;  

  // everything is fine
  return 0;
}
