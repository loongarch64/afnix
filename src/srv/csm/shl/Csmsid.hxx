// ---------------------------------------------------------------------------
// - Csmsid.hxx                                                              -
// - afnix:csm service - serial id definition                                -
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

#ifndef  AFNIX_CSMSID_HXX
#define  AFNIX_CSMSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  //                vvtt pppp pllu uuuu
  // csm serial id [0000 0000 0100 0000][@0x0040]
  static const t_word SRL_PART_SID = 0x0001U; // part id
  static const t_word SRL_BLOB_SID = 0x0002U; // blob id
  static const t_word SRL_BLOC_SID = 0x0003U; // bloc id
  static const t_word SRL_CBLB_SID = 0x0004U; // carrier id
  static const t_word SRL_DBLB_SID = 0x0005U; // delegate id
  static const t_word SRL_TCKR_SID = 0x0006U; // tracker id
  static const t_word SRL_COLN_SID = 0x0007U; // collection id
  static const t_word SRL_DOMN_SID = 0x0008U; // domain id
  static const t_word SRL_WOIS_SID = 0x0009U; // domain whois id
  static const t_word SRL_WAIS_SID = 0x000AU; // blob whatis id
  static const t_word SRL_CRED_SID = 0x0020U; // credential id
  static const t_word SRL_IDTY_SID = 0x0021U; // identity id
  static const t_word SRL_AUTH_SID = 0x0022U; // authority id
  static const t_word SRL_PCPL_SID = 0x0023U; // principal id
  static const t_word SRL_XACT_SID = 0x0030U; // act id
  static const t_word SRL_VISA_SID = 0x0031U; // visa id
  static const t_word SRL_SESS_SID = 0x0040U; // session id

  // the csm dispatch id
  static const t_word SRL_DEOD_CSM = 0x0040U;
}

#endif
