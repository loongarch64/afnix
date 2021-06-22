// ---------------------------------------------------------------------------
// - Spssid.hxx                                                              -
// - afnix:sps module - serial id definition                                 -
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

#ifndef  AFNIX_SPSSID_HXX
#define  AFNIX_SPSSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                vvtt pppp pllu uuuu
  // sps serial id [0000 0000 0010 0011][@0x0023U]
  static const t_word SRL_CELL_SID = 0x0000U; // cell id
  static const t_word SRL_RECD_SID = 0x0001U; // record id
  static const t_word SRL_SHTT_SID = 0x0002U; // sheet id
  static const t_word SRL_BNDL_SID = 0x0003U; // bundle id

  // the sps dispatch id
  static const t_word SRL_DEOD_SPS = 0x0023U;

}

#endif
