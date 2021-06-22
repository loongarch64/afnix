// ---------------------------------------------------------------------------
// - Libbce.cpp                                                              -
// - afnix:bce service - declaration & implementation                        -
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
#include "Chain.hpp"
#include "Bcesrl.hxx"
#include "Libbce.hpp"
#include "Predbce.hpp"
#include "Request.hpp"
#include "Function.hpp"

namespace afnix {

  // initialize the afnix:bce service

  Object* init_afnix_bce (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid bce serial dispatcher");
    }
    // create the afnix:bce nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("bce");

    // bind all symbols in the afnix:bce nameset
    gset->symcst ("Block",            new Meta (Block::mknew));
    gset->symcst ("Chain",            new Meta (Chain::mknew));
    gset->symcst ("Ledger",           new Meta (Ledger::mknew));
    gset->symcst ("Request",          new Meta (Request::mknew));

    // bind the predicates
    gset->symcst ("block-p",          new Function (bce_blokp));
    gset->symcst ("chain-p",          new Function (bce_bchnp));
    gset->symcst ("ledger-p",         new Function (bce_lgdrp));
    gset->symcst ("request-p",        new Function (bce_rqstp));
    gset->symcst ("transaction-p",    new Function (bce_tranp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_bce (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_bce (interp, argv);
  }
}
