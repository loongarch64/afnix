// ---------------------------------------------------------------------------
// - AxdCalls.hpp                                                            -
// - afnix cross debugger - commands call definitions                        -
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

#ifndef  AFNIX_AXDCALLS_HPP
#define  AFNIX_AXDCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {
  Object* axd_lst (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_ifo (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_bfo (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_xit (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_run (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_ldf (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_bpt (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_cnt (Evaluable* zobj, Nameset* nset, Cons* args);
  Object* axd_nxt (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
