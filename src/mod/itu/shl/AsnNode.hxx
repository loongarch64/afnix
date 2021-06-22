// ---------------------------------------------------------------------------
// - AsnNode.hxx                                                             -
// - afnix:itu module - asn global private definitions                       -
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

#ifndef AFNIX_ASNNODE_HXX
#define AFNIX_ASNNODE_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {

  // the identifier octet class mask
  static const t_byte ASN_MASK_CLS  = 0xC0;
  // the identifier octet constructed flag mask
  static const t_byte ASN_MASK_CFL  = 0x20;
  // the identifier octet tag mask
  static const t_byte ASN_MASK_TAG  = 0x1F;

  // the universal class encoding
  static const t_byte ASN_UNIV_CLS  = 0x00;
  // the application class encoding
  static const t_byte ASN_APPL_CLS  = 0x40;
  // the context specific class encoding
  static const t_byte ASN_CTXS_CLS  = 0x80;
  // the private class encoding
  static const t_byte ASN_PRIV_CLS  = 0xC0;

  // the constructed flag
  static const t_byte ASN_CONS_CLS  = 0x20;
  // the long tag number encoding
  static const t_byte ASN_LTAG_BIN  = 0x1F;
  // the indefinite length encoding
  static const t_byte ASN_ILEN_BIN  = 0x80;
}

#endif
