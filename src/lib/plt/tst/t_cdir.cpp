// ---------------------------------------------------------------------------
// - t_cdir.cpp                                                              -
// - standard platform library - simple directory tester module              -
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

#include "cdir.hpp"
#include "cstr.hpp"

int main (int argc, char**) {
  using namespace afnix;

  // check simple path
  char buf[512];
  char pth[512];
  c_strcpy (buf, "afnix");
  char* name = c_xname (buf);
  if (c_strcmp (name, "afnix") == false) return 1;
  delete [] name; name = nullptr;

  // check root name
  buf[0] = c_getdsep ();
  buf[1] = nilc;
  c_strcat (buf, "home");
  c_strcpy (pth, buf);
  name  = c_xname (buf);
  if (c_strcmp (name, "home") == false) return 1;
  delete [] name;

  // check multiple name
  long pos = c_strlen (buf);
  buf[pos] = c_getdsep ();
  buf[pos+1] = nilc;
  c_strcat (buf, "afnix");
  name = c_xname (buf);
  if (c_strcmp (name, "afnix") == false) return 1;
  delete [] name;
  name = c_xbase (buf);
  if (c_strcmp (name, "afnix") == false) return 1;
  delete [] name;
  
  // check directory base
  name = c_xdir (buf);
  if (c_strcmp (name, pth) == false) return 1;
  delete [] name;

  // check drectory trim
  buf[0] = c_getdsep ();
  buf[1] = nilc;
  c_strcat (buf, "home");
  pos = c_strlen (buf);
  buf[pos] = c_getdsep ();
  buf[pos+1] = c_getdsep ();
  buf[pos+2] = nilc;
  c_strcat (buf, "afnix");
  
  name = c_xdir (buf);
  if (c_strcmp (name, pth) == false) return 1;
  delete [] name;

  // check directory strip
  pos = c_strlen (buf);
  buf[pos] = c_getdsep ();
  buf[pos+1] = nilc;
  name = c_xdir (buf);
  if (c_strcmp (name, pth) == false) return 1;
  delete [] name;

  name = c_xname (buf);
  if (c_strcmp (name, "") == false) return 1;
  delete [] name;
  name = c_xbase (buf);
  if (c_strcmp (name, "afnix") == false) return 1;
  delete [] name;
  
  // check for extension removal
  name = c_rmext ("t_csio.cpp");
  if (c_strcmp (name, "t_csio") == false) return 1;
  delete [] name;
  name = c_rmext ("t_csio");
  if (c_strcmp (name, "t_csio") == false) return 1;
  delete [] name;

  // check for extension extraction
  buf[0] = '.';
  buf[1] = c_getdsep ();
  buf[2] = nilc;
  c_strcat (buf, "t_cdir.cpp");
  name = c_xext (buf);
  if (c_strcmp (name, "cpp") == false) return 1;
  if (c_xext ("t_cdir") != nullptr)  return 1;
  delete [] name;

  // check for data directory name
  if (c_getddir (nullptr) != nullptr) return 1;
  
  // ok this is enough
  return 0;
}
