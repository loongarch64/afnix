// ---------------------------------------------------------------------------
// - Mthsid.hxx                                                              -
// - afnix:mth module - serial id definition                                 -
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

#ifndef  AFNIX_MTHSID_HXX
#define  AFNIX_MTHSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                 vvtt pppp pllu uuuu
  // math serial id [0000 0000 0010 0000][0x0020]
  static const t_word SRL_IVEC_SID = 0x0000U; // i-vector   id
  static const t_word SRL_NVEC_SID = 0x0001U; // n-vector   id
  static const t_word SRL_RVEC_SID = 0x0002U; // r-vector   id
  static const t_word SRL_NPNT_SID = 0x0005U; // n-point    id
  static const t_word SRL_RPNT_SID = 0x0006U; // r-point    id
  static const t_word SRL_NBLK_SID = 0x0009U; // n-block    id
  static const t_word SRL_RBLK_SID = 0x000AU; // r-block    id
  static const t_word SRL_NARY_SID = 0x000BU; // n-array    id
  static const t_word SRL_RMTX_SID = 0x0012U; // r-matrix   id
  static const t_word SRL_RSPL_SID = 0x0016U; // r-samples  id
  static const t_word SRL_PRMT_SID = 0x0020U; // permute    id
  static const t_word SRL_VEC1_SID = 0x0031U; // r-vector 1 id
  static const t_word SRL_VEC2_SID = 0x0032U; // r-vector 2 id
  static const t_word SRL_VEC3_SID = 0x0033U; // r-vector 3 id
  static const t_word SRL_VEC4_SID = 0x0034U; // r-vector 4 id
  static const t_word SRL_PNT1_SID = 0x0035U; // r-point  1 id
  static const t_word SRL_PNT2_SID = 0x0036U; // r-point  2 id
  static const t_word SRL_PNT3_SID = 0x0037U; // r-point  3 id
  static const t_word SRL_PNT4_SID = 0x0038U; // r-point  4 id

  // the math dispatch id
  static const t_word SRL_DEOD_MTH = 0x0020U;
}

#endif
