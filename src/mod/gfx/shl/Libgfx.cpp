// ---------------------------------------------------------------------------
// - Libgfx.cpp                                                              -
// - afnix:gfx module - declaration & implementation                         -
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
#include "Graph.hpp"
#include "Libgfx.hpp"
#include "Predgfx.hpp"
#include "Function.hpp"

namespace afnix {

  // initialize the afnix:gfx module

  Object* init_afnix_gfx (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:gfx nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("gfx");

    // bind all symbols in the afnix:gfx nameset
    gset->symcst ("Edge",            new Meta (Edge::mknew));
    gset->symcst ("Graph",           new Meta (Graph::mknew));
    gset->symcst ("Vertex",          new Meta (Vertex::mknew));

    // bind the predicates
    gset->symcst ("edge-p",          new Function (gfx_edgep));
    gset->symcst ("graph-p",         new Function (gfx_grafp));
    gset->symcst ("vertex-p",        new Function (gfx_vrtxp));
    
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_gfx (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_gfx (interp, argv);
  }
}
