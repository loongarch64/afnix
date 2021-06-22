// ---------------------------------------------------------------------------
// - Stdsid.hxx                                                              -
// - afnix:std - serial id definition                                        -
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

#ifndef  AFNIX_STDSID_HXX
#define  AFNIX_STDSID_HXX

#ifndef AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {

  // standard serial id [0000 0000 0000 0000][@0x0000U]
  static const t_word SRL_NILP_SID = 0x0000U; // nil pointer
  static const t_word SRL_BOOL_SID = 0x0001U; // boolean
  static const t_word SRL_BYTE_SID = 0x0002U; // byte
  static const t_word SRL_RSV1_SID = 0x0003U; // reserved
  static const t_word SRL_EOSC_SID = 0x0004U; // eos character
  static const t_word SRL_RSV2_SID = 0x0005U; // reserved
  static const t_word SRL_INTG_SID = 0x0006U; // integer
  static const t_word SRL_REAL_SID = 0x0007U; // real
  static const t_word SRL_CHAR_SID = 0x0008U; // character
  static const t_word SRL_STRG_SID = 0x0009U; // string
  static const t_word SRL_RELT_SID = 0x000AU; // relative
  static const t_word SRL_REGX_SID = 0x000BU; // regex
  static const t_word SRL_BUFR_SID = 0x000CU; // buffer
  static const t_word SRL_CONS_SID = 0x000DU; // cons
  static const t_word SRL_VECT_SID = 0x000EU; // vector
  static const t_word SRL_OSET_SID = 0x000FU; // set
  static const t_word SRL_NTBL_SID = 0x0010U; // name table
  static const t_word SRL_STRV_SID = 0x0011U; // string vector
  static const t_word SRL_PROP_SID = 0x0012U; // property
  static const t_word SRL_PLST_SID = 0x0013U; // plist
  static const t_word SRL_LIST_SID = 0x0014U; // list
  static const t_word SRL_STRF_SID = 0x0015U; // string fifo
  static const t_word SRL_PTBL_SID = 0x0016U; // print table
  static const t_word SRL_MESG_SID = 0x0017U; // message
  static const t_word SRL_STYL_SID = 0x0018U; // style
  static const t_word SRL_NUMR_SID = 0x0019U; // numeral
  static const t_word SRL_HASH_SID = 0x001AU; // hash table
  static const t_word SRL_LOCL_SID = 0x001BU; // locale
  static const t_word SRL_STRC_SID = 0x001CU; // structure
  static const t_word SRL_RPT2_SID = 0xFFFAU; // internal block point
  static const t_word SRL_RBLK_SID = 0xFFFBU; // internal block real
  static const t_word SRL_LBLK_SID = 0xFFFCU; // internal block long
  static const t_word SRL_OBLK_SID = 0xFFFDU; // internal block byte
  static const t_word SRL_BBLK_SID = 0xFFFEU; // internal block bool

  // the standard dispatch id
  static const t_word SRL_DEOD_STD = 0x0000U;

}

#endif
