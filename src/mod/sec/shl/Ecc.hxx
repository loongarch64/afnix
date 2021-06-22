// ---------------------------------------------------------------------------
// - Ecc.hxx                                                                 -
// - afnix:sec module - elliptic curve cryptography private definitions      -
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

#ifndef  AFNIX_ECC_HXX
#define  AFNIX_ECC_HXX

#ifndef  AFNIX_ECP_HPP
#include "Ecp.hpp"
#endif

#ifndef  AFNIX_INTEGER_HPP
#include "Integer.hpp"
#endif

#ifndef  AFNIX_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace afnix {

  // this procedure extracts a relatif from an object

  static inline Relatif ecc_to_relatif (Object* obj) {
    // check for an integer
    auto iobj = dynamic_cast<Integer*>(obj);
    if (iobj != nullptr) return Relatif(iobj->tolong ());    
    // check for a relatif
    auto robj = dynamic_cast<Relatif*>(obj);
    if (robj != nullptr) return *robj;
    // invalid object
    throw Exception ("type-error", "invalid object as a relatif",
		     Object::repr (obj));
  }
  
  // this procedure extracts a point from an object
  
  static inline Ecp ecc_to_ecp (Object* obj) {
    // check for a point
    auto pobj = dynamic_cast<Ecp*>(obj);
    if (pobj != nullptr) return *pobj;
    // invalid object
    throw Exception ("type-error", "invalid object as an elliptic point",
		     Object::repr (obj));
  }
}

#endif
