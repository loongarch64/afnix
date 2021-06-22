// ---------------------------------------------------------------------------
// - t_pathname.cpp                                                          -
// - standard object library - path name class tester                        -
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

#include "System.hpp"
#include "Pathname.hpp"

int main (int, char**) {
  using namespace afnix;

  // create the path components
  String fnam = "axi";
  String dnam = System::join (System::join ("usr"), "bin");
  String rnam = System::getrdir ();   
  // create the path object
  Pathname path (fnam, dnam);

  // check the file name
  if (path.getfnam () != fnam) return 1;
  // check the directory name
  if (path.getdnam () != dnam) return 1;
  
  // check the directory components
  if (path.length  ()  != 2) return 1;
  if (path.getpath (0) != System::join ("usr")) return 1;
  if (path.getpath (1) != "bin") return 1;

  // create a simple root pathname
  Pathname prnm (rnam);
  // check the directory name
  if (prnm.getdnam () != rnam) return 1;

  // create a combined pathname
  Pathname cpnm ("", dnam);
  // check the directory name
  if (cpnm.getdnam () != dnam) return 1;

  // check normalize corner case
  if (Pathname::normalize ("/usr/bin")     != "/usr/bin") return 1;
  if (Pathname::normalize ("/")            != "/") return 1;
  if (Pathname::normalize ("//")           != "/") return 1;
  if (Pathname::normalize ("usr//bin")     != "usr/bin") return 1;
  if (Pathname::normalize ("//usr/.././/") != "/") return 1;
  if (Pathname::normalize ("//..//..///.") != "/") return 1;
  if (Pathname::normalize ("////////////") != "/") return 1;
  // finally - no failure
  return 0;
}
