// ---------------------------------------------------------------------------
// - Bcesid.hxx                                                              -
// - afnix:bce service - serial id definition                                -
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

#ifndef  AFNIX_BCESID_HXX
#define  AFNIX_BCESID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                vvtt pppp pllu uuuu
  // bce serial id [0000 0000 0100 0001][@0x0041]
  static const t_word SRL_RQST_SID = 0x0001U; // request id
  static const t_word SRL_BLOK_SID = 0x0010U; // block id
  static const t_word SRL_LGDR_SID = 0x0011U; // ledger id
  static const t_word SRL_BCHN_SID = 0x0012U; // chain id

  // the bce dispatch id
  static const t_word SRL_DEOD_BCE = 0x0041U;
}

#endif
