// ---------------------------------------------------------------------------
// - Resume.cpp                                                              -
// - afnix cross debugger - resume exception class implementation            -
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

#include "Resume.hpp"
#include "String.hpp"

namespace afnix {

  // create a default resume

  Resume::Resume (void) {
  }
  
  // copy move this resume

  Resume::Resume (Resume&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this resume into this one

  Resume& Resume::operator = (Resume&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }
  
  // return the class name

  String Resume::repr (void) const {
    return "Resume";
  }
}
