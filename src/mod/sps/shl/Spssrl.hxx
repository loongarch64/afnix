// ---------------------------------------------------------------------------
// - Spssrl.hxx                                                              -
// - afnix:sps module - serial decoding definition                           -
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

#ifndef  AFNIX_SPSSRL_HXX
#define  AFNIX_SPSSRL_HXX

#include "Sheet.hpp"
#include "Bundle.hpp"
#include "Spssid.hxx"
#include "Utility.hpp"

namespace afnix {
  
  // the sps dispatch function
  static Serial* srl_deod_sps (const t_word sid) {
    switch (sid) {
    case SRL_BNDL_SID:
      return new Bundle;
      break;
    case SRL_CELL_SID:
      return new Cell;
      break;
    case SRL_RECD_SID:
      return new Record;
      break;
    case SRL_SHTT_SID:
      return new Sheet;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid Sps sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_SPS, srl_deod_sps);
}

#endif
