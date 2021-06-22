// ---------------------------------------------------------------------------
// - t_locale.cpp                                                            -
// - standard object library - byte class test module                        -
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

#include "Date.hpp"
#include "Locale.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;
  
  // create a default locale and check
  Locale ld;
  if (ld.repr () != "Locale") return 1;
  
  if (ld.getname().isnil() == false) return 1;
  if (ld.getinfo().isnil() == false) return 1;

  if (ld.toreal ("1.0") != 1.0) return 1;
  if (ld.toreal ("1 000.0") != 1000.0) return 1;

  // set the locale and check
  ld.setname ("locale");
  if (ld.getname () != "locale") return 1;
  ld.setinfo ("default");
  if (ld.getinfo () != "default") return 1;
  ld.setlidx (1L);
  if (ld.getlidx () != 1L) return 1;
  
  ld.reset ();
  ld.setprm("N=afnix&I=locale&L=0&D=,&S= &C=YYMMDD&M=-");
  if (ld.getname () != "afnix") return 1;
  if (ld.getinfo () != "locale") return 1;
  if (ld.getlidx () != 0L) return 1;
  if (ld.getdpnt () != ',') return 1;
  if (ld.getdsep () != ' ') return 1;
  if (ld.getdfmt () != "YYMMDD") return 1;
  if (ld.getdmrk () != '-') return 1;

  if (ld.toreal ("1,0") != 1.0) return 1;
  if (ld.toreal ("1 000,0") != 1000.0) return 1;

  // check one date format
  Date date (2000, 12, 31);
  t_long time = date.gettime (true);
  if (ld.todate ("00-12-31") != time) return 1;

  // check second date format
  ld.setprm("N=afnix&I=locale&D=,&S= &C=YYYYMMDD&M=/");
  if (ld.getprm ()  != "N=afnix&I=locale&D=,&M=/") return 1;
  if (ld.getdfmt () != "YYYYMMDD") return 1;
  if (ld.getdmrk () != '/') return 1;
  if (ld.todate ("2000/12/31") != time) return 1;

  // check third date format
  ld.setprm("N=afnix&I=locale&D=,&C=DDMMYY&M=/");
  if (ld.getprm () != "N=afnix&I=locale&D=,&C=DDMMYY&M=/") return 1;
  if (ld.getdfmt () != "DDMMYY") return 1;
  if (ld.getdmrk () != '/') return 1;
  if (ld.todate ("31/12/00") != time) return 1;

  // check fourth date format
  ld.setprm("N=afnix&I=locale&D=,&C=DDMMYYYY&M=:");
  if (ld.getprm () != "N=afnix&I=locale&D=,&C=DDMMYYYY&M=:") return 1;
  if (ld.getdfmt () != "DDMMYYYY") return 1;
  if (ld.getdmrk () != ':') return 1;
  if (ld.todate ("31:12:2000") != time) return 1;
  return 0;
}
