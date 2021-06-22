// ---------------------------------------------------------------------------
// - Libapi.cpp                                                              -
// - example:api module - declaration & implementation                       -
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

#include "Libapi.hpp"
#include "Apicalls.hpp"
#include "Function.hpp"

namespace example {
  // use the afnix namespace
  using namespace afnix;

  // initialize the api module

  Object* init_example_api (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the api nameset
    Nameset* api = interp->mknset ("api");

    // bind the add fucntion
    api->symcst ("add", new Function (api_add));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_example_api (afnix::Interp* interp, afnix::Vector* argv) {
    return example::init_example_api (interp, argv);
  }
}
