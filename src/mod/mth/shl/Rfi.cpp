// ---------------------------------------------------------------------------
// - Rfi.cpp                                                                 -
// - afnix:mth module - real function interface implementation               -
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

#include "Rfi.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // map this object as a functor

  t_real Rfi::operator () (const t_real x) const {
    return compute (x);
  }

  // compute the function derivate at point x

  t_real Rfi::derivate (const t_real x) const {
    t_real h = Math::d_seps;
    return ((compute (x + h) - compute (x)) / h);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_COMPUTE  = zone.intern ("compute");
  static const long QUARK_DERIVATE = zone.intern ("derivate");

  // return true if the given quark is defined

  bool Rfi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Rfi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_COMPUTE) {
        t_real x = argv->getreal (0);
        return new Real (compute (x));
      }
      if (quark == QUARK_DERIVATE) {
        t_real x = argv->getreal (0);
        return new Real (derivate (x));
      }
    }
    // call the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
