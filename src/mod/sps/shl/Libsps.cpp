// ---------------------------------------------------------------------------
// - Libsps.cpp                                                              -
// - afnix:sps module - declaration & implementation                         -
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

#include "Csv.hpp"
#include "Meta.hpp"
#include "Spssrl.hxx"
#include "Libsps.hpp"
#include "Predsps.hpp"
#include "Function.hpp"
#include "SpsTransit.hpp"

namespace afnix {

  // initialize the the afnix:sps module

  Object* init_afnix_sps (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid sps serial dispatcher");
    }
    // create the afnix:sps nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* nset = aset->mknset   ("sps");
 
    // bind all classes in the afnix:net nameset
    nset->symcst ("Csv",               new Meta (Csv::mknew));
    nset->symcst ("Cell",              new Meta (Cell::mknew));
    nset->symcst ("Xref",              new Meta (Xref::mknew));
    nset->symcst ("Index",             new Meta (Index::mknew));
    nset->symcst ("Sheet",             new Meta (Sheet::mknew));
    nset->symcst ("Folio",             new Meta (Folio::mknew));
    nset->symcst ("Bundle",            new Meta (Bundle::mknew));
    nset->symcst ("Record",            new Meta (Record::mknew));
    nset->symcst ("SpsTransit",        new Meta (SpsTransit::mknew));

    // bind all predicates in the afnix:sps nameset
    nset->symcst ("csv-p",             new Function (sps_csvp));
    nset->symcst ("cell-p",            new Function (sps_celp));
    nset->symcst ("xref-p",            new Function (sps_xrfp));
    nset->symcst ("index-p",           new Function (sps_idxp));
    nset->symcst ("sheet-p",           new Function (sps_shtp));
    nset->symcst ("folio-p",           new Function (sps_folp));
    nset->symcst ("bundle-p",          new Function (sps_bndp));
    nset->symcst ("record-p",          new Function (sps_rcdp));
    nset->symcst ("transit-p",         new Function (sps_tsit));
       
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_sps (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_sps (interp, argv);
  }
}
