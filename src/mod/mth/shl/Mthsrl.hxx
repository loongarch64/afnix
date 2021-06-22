// ---------------------------------------------------------------------------
// - Mthsid.hxx                                                              -
// - afnix:mth module - serial decoding definition                           -
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

#ifndef  AFNIX_MTHSRL_HXX
#define  AFNIX_MTHSRL_HXX

#include "Mthsid.hxx"
#include "Nblock.hpp"
#include "Narray.hpp"
#include "Rblock.hpp"
#include "Npoint.hpp"
#include "Rpoint.hpp"
#include "Ivector.hpp"
#include "Rmatrix.hpp"
#include "Permute.hpp"
#include "Utility.hpp"
#include "Rpoint1.hpp"
#include "Rpoint2.hpp"
#include "Rpoint3.hpp"
#include "Rpoint4.hpp"
#include "Rvector1.hpp"
#include "Rvector2.hpp"
#include "Rvector3.hpp"
#include "Rvector4.hpp"
#include "Rsamples.hpp"

namespace afnix {
  // the engine dispatch function
  static Serial* srl_deod_mth (const t_word sid) {
    switch (sid) {
    case SRL_IVEC_SID:
      return new Ivector;
      break;
    case SRL_NVEC_SID:
      return new Nvector;
      break;
    case SRL_RVEC_SID:
      return new Rvector;
      break;
    case SRL_NPNT_SID:
      return new Npoint;
      break;
    case SRL_RPNT_SID:
      return new Rpoint;
      break;
    case SRL_NBLK_SID:
      return new Nblock;
      break;
    case SRL_NARY_SID:
      return new Narray;
      break;
    case SRL_RBLK_SID:
      return new Rblock;
      break;
    case SRL_RMTX_SID:
      return new Rmatrix;
      break;
    case SRL_RSPL_SID:
      return new Rsamples;
      break;
    case SRL_PRMT_SID:
      return new Permute;
      break;
    case SRL_VEC1_SID:
      return new Rvector1;
      break;
    case SRL_VEC2_SID:
      return new Rvector2;
      break;
    case SRL_VEC3_SID:
      return new Rvector3;
      break;
    case SRL_VEC4_SID:
      return new Rvector4;
      break;
    case SRL_PNT1_SID:
      return new Rpoint1;
      break;
    case SRL_PNT2_SID:
      return new Rpoint2;
      break;
    case SRL_PNT3_SID:
      return new Rpoint3;
      break;
    case SRL_PNT4_SID:
      return new Rpoint4;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid mth module sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_MTH, srl_deod_mth);
}

#endif
