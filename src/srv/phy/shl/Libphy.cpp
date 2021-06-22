// ---------------------------------------------------------------------------
// - Libphy.cpp                                                              -
// - afnix:phy service - declaration & implementation                        -
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
#include "Unit.hpp"
#include "Libphy.hpp"
#include "Predphy.hpp"
#include "Physics.hpp"
#include "Periodic.hpp"
#include "PhyCalls.hpp"
#include "Function.hpp"

namespace afnix {

  // initialize the afnix:phy service

  Object* init_afnix_phy (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:phy nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("phy");

    // bind all symbols in the afnix:phy nameset
    gset->symcst ("Unit",                new Meta (Unit::mknew));
    gset->symcst ("Atom",                new Meta (Atom::mknew));
    gset->symcst ("Physics",             new Meta (Physics::mknew));
    gset->symcst ("Periodic",            new Meta (Periodic::mknew));

    // bind the predicates
    gset->symcst ("unit-p",              new Function (phy_unitp));
    gset->symcst ("atom-p",              new Function (phy_atomp));
    gset->symcst ("physics-p",           new Function (phy_physp));
    gset->symcst ("periodic-p",          new Function (phy_ptblp));

    // bind other functions
    gset->symcst ("to-unit",             new Function (phy_tounit));
    gset->symcst ("to-physics",          new Function (phy_tophys));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_phy (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_phy (interp, argv);
  }
}
