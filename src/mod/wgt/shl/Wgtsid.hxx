// ---------------------------------------------------------------------------
// - Wgtsid.hxx                                                              -
// - afnix:wgt module - serial id definition                                 -
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

#ifndef  AFNIX_WGTSID_HXX
#define  AFNIX_WGTSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                vvtt pppp pllu uuuu
  // wgt serial id [0000 0000 0010 0100][@0x0024U]
  static const t_word SRL_COND_SID = 0x0000U; // conditional id

  // the wgt dispatch id
  static const t_word SRL_DEOD_WGT = 0x0024U;
}

#endif
