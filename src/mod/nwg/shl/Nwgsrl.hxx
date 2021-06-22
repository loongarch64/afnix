// ---------------------------------------------------------------------------
// - Nwgsrl.hxx                                                              -
// - afnix:nwg module - serial decoding definition                           -
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

#ifndef  AFNIX_NWGSRL_HXX
#define  AFNIX_NWGSRL_HXX

#include "Uuid.hpp"
#include "Nwgsid.hxx"
#include "Utility.hpp"
#include "Hyperlink.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // the nwg dispatch function
  static Serial* srl_deod_nwg (const t_word sid) {
    switch (sid) {
    case SRL_UUID_SID:
      return new Uuid;
      break;
    case SRL_HLNK_SID:
      return new Hyperlink;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid nwg sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_NWG, srl_deod_nwg);
}

#endif
