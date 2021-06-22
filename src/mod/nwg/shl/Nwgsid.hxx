// ---------------------------------------------------------------------------
// - Nwgsid.hxx                                                              -
// - afnix:nwg module - serial id definition                                 -
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

#ifndef  AFNIX_NWGSID_HXX
#define  AFNIX_NWGSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                vvtt pppp pllu uuuu
  // nwg serial id [0000 0000 0010 0001][@0x0021U]
  static const t_word SRL_UUID_SID = 0x0000U; // uuid id
  static const t_word SRL_HLNK_SID = 0x0001U; // hyperlink id

  // the nwg dispatch id
  static const t_word SRL_DEOD_NWG = 0x0021U;
}

#endif
