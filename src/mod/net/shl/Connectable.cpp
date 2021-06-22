// ---------------------------------------------------------------------------
// - Connectable.cpp                                                         -
// - afnix:net module - connectable socket implementation                    -
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

#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Connectable.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this connectable

  Connectable::Connectable (Connectable&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this connectable into this one

  Connectable& Connectable::operator = (Connectable&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }
}
