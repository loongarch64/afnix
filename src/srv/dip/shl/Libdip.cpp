// ---------------------------------------------------------------------------
// - Libdip.cpp                                                              -
// - afnix:dip service - declaration & implementation                        -
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
#include "Pixmap.hpp"
#include "Mixmap.hpp"
#include "Dipsrl.hxx"
#include "Libdip.hpp"
#include "Preddip.hpp"
#include "Function.hpp"
#include "DipCalls.hpp"

namespace afnix {

  // initialize the afnix:dip service

  Object* init_afnix_dip (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid dip serial dispatcher");
    }
    
    // create the afnix:dip nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("dip");

    // bind all symbols in the afnix:dip nameset
    gset->symcst ("Pixel",           new Meta (Pixel::meval, Pixel::mknew));
    gset->symcst ("Pixmap",          new Meta (Pixmap::mknew));
    gset->symcst ("Mixmap",          new Meta (Mixmap::mknew));
    
    // bind the predicates
    gset->symcst ("pixel-p",         new Function (dip_pixp));
    gset->symcst ("image-p",         new Function (dip_imgp));
    gset->symcst ("slice-p",         new Function (dip_slcp));
    gset->symcst ("pixmap-p",        new Function (dip_pxmp));
    gset->symcst ("mixmap-p",        new Function (dip_mxmp));
    gset->symcst ("tranche-p",       new Function (dip_trnp));

    // bind other functions
    gset->symcst ("netpbm-read",     new Function (dip_rdpbm));
    gset->symcst ("netpbm-write",    new Function (dip_wrpbm));
    
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_dip (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_dip (interp, argv);
  }
}
