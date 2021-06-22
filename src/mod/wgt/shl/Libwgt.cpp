// ---------------------------------------------------------------------------
// - Libwgt.cpp                                                              -
// - afnix:wgt module - declaration & implementation                         -
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

#include "Meta.hpp"
#include "Wgtsrl.hxx"
#include "Libwgt.hpp"
#include "Predwgt.hpp"
#include "Function.hpp"
#include "Conditional.hpp"

namespace afnix {

  // initialize the afnix:wgt module

  Object* init_afnix_wgt (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid wgt serial dispatcher");
    }
    // create the afnix:wgt nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("wgt");

    // bind all symbols in the afnix:wgt nameset
    gset->symcst ("Conditional",        new Meta (Conditional::mknew));

    // bind the predicates
    gset->symcst ("conditional-p",      new Function (wgt_condp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_wgt (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_wgt (interp, argv);
  }
}
