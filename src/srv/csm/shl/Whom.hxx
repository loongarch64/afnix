// ---------------------------------------------------------------------------
// - Whom.hxx                                                              -
// - afnix:csm service - part/blob plist name definitions                    -
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

#ifndef  AFNIX_WHOM_HXX
#define  AFNIX_WHOM_HXX

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {
  
  // the part plist name/info
  static const String PN_PRT_NAME = "PN-PRT-NAME";
  static const String PI_PRT_NAME = "PART NAME";
  static const String PN_PRT_INFO = "PN-PRT-INFO";
  static const String PI_PRT_INFO = "PART INFO";
  static const String PN_PRT_UUID = "PN-PRT-UUID";
  static const String PI_PRT_UUID = "PART UUID";

  // the domain plist name/info
  static const String PN_DMN_XRID = "PN-DMN-XRID";
  static const String PI_DMN_XRID = "DOMAIN RID";
  static const String PN_DMN_CTIM = "PN-DMN-CTIM";
  static const String PI_DMN_CTIM = "DOMAIN CREATION TIME";
  static const String PN_DMN_MTIM = "PN-DMN-MTIM";
  static const String PI_DMN_MTIM = "DOMAIN MODIFICATION TIME";

  // the blob plist name/info
  static const String PN_BLB_TYPE = "PN-BLB-TYPE";
  static const String PI_BLB_TYPE = "BLOB TYPE";
  static const String PN_BLB_XRID = "PN-BLB-XRID";
  static const String PI_BLB_XRID = "BLOB RID";
  static const String PN_BLB_CTIM = "PN-BLB-CTIM";
  static const String PI_BLB_CTIM = "BLOB CREATION TIME";
  static const String PN_BLB_MTIM = "PN-BLB-MTIM";
  static const String PI_BLB_MTIM = "BLOB MODIFICATION TIME";
  static const String PN_BLB_TFLG = "PN-BLB-TFLG";
  static const String PI_BLB_TFLG = "BLOB TRANSIENT FLAG";
}

#endif
