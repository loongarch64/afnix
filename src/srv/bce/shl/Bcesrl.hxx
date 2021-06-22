// ---------------------------------------------------------------------------
// - Bcesrl.hxx                                                              -
// - afnix:bce service - serial decoding definition                          -
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

#ifndef  AFNIX_BCESRL_HXX
#define  AFNIX_BCESRL_HXX

#include "Chain.hpp"
#include "Bcesid.hxx"
#include "Utility.hpp"
#include "Request.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // the bce dispatch function
  static Serial* srl_deod_bce (const t_word sid) {
    switch (sid) {
    case SRL_RQST_SID:
      return new Request;
      break;
    case SRL_BLOK_SID:
      return new Block;
      break;
    case SRL_LGDR_SID:
      return new Ledger;
      break;
    case SRL_BCHN_SID:
      return new Chain;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid bce sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_BCE, srl_deod_bce);
}

#endif
