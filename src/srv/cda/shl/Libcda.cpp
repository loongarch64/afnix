// ---------------------------------------------------------------------------
// - Libcda.cpp                                                              -
// - afnix:cda service - declaration & implementation                        -
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
#include "Spsds.hpp"
#include "Lnrds.hpp"
#include "Stmdl.hpp"
#include "Libcda.hpp"
#include "Sampler.hpp"
#include "Predcda.hpp"
#include "Function.hpp"

namespace afnix {

  // initialize the afnix:cda service

  Object* init_afnix_cda (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:cda nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("cda");

    // bind all symbols in the afnix:cda nameset
    gset->symcst ("Spsds",                  new Meta (Spsds::mknew));
    gset->symcst ("Lnrds",                  new Meta (Lnrds::mknew));
    gset->symcst ("Stmdl",                  new Meta (Stmdl::mknew));
    gset->symcst ("Sampler",                new Meta (Sampler::mknew));
    gset->symcst ("Localizer",              new Meta (Localizer::meval));

    // bind the predicates
    gset->symcst ("sampler-p",              new Function (cda_splp));
    gset->symcst ("localizer-p",            new Function (cda_locp));
    gset->symcst ("streamable-p",           new Function (cda_stmp));
    gset->symcst ("sps-streamer-p",         new Function (cda_spsp));
    gset->symcst ("linear-streamer-p",      new Function (cda_lnrp));
    gset->symcst ("streamable-localizer-p", new Function (cda_sdlp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_cda (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_cda (interp, argv);
  }
}
