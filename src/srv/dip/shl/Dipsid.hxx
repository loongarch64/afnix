// ---------------------------------------------------------------------------
// - Dipsid.hxx                                                              -
// - afnix:dip service - serial id definition                                -
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

#ifndef  AFNIX_DIPSID_HXX
#define  AFNIX_DIPSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                vvtt pppp pllu uuuu
  // dip serial id [0000 0000 0100 0010][@0x0042]
  static const t_word SRL_PIXL_SID = 0x0000U; // pixel id
  static const t_word SRL_PIXM_SID = 0x0001U; // pixmap id
  static const t_word SRL_MIXM_SID = 0x0002U; // mixmap id

  // the dip dispatch id
  static const t_word SRL_DEOD_DIP = 0x0042U;
}

#endif
