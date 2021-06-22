// ---------------------------------------------------------------------------
// - t_cshl.cpp                                                              -
// - standard platform library - dynamic library tester module               -
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

#include "cshl.hpp"

int main (int, char**) {
  using namespace afnix;

  // if we are statically linked - we have nothing to do
  if (AFNIX_LINKTYPE_STATIC == true) return 0;

  // try to open our library
  void* handle = c_dlopen ("afnix-plt");
  if (!handle) return 1;

  // try to query a symbol in the afnix namespace
  void* sym = c_dlsym ("cshl_test_symbol", handle);
  if (!sym) return 1;

  // close the library
  c_dlclose (handle);
  return 0;
}
