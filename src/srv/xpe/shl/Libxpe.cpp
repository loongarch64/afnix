// ---------------------------------------------------------------------------
// - Libxpe.cpp                                                              -
// - afnix:xpe service - declaration & implementation                        -
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
#include "Libxpe.hpp"
#include "Predxpe.hpp"
#include "Function.hpp"
#include "XmlPlist.hpp"
#include "XmlInclude.hpp"
#include "XmlProcessor.hpp"
#include "XmlPrintTable.hpp"

namespace afnix {

  // initialize the afnix:xpe module

  Object* init_afnix_xpe (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:xpe nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("xpe");

    // bind all symbols in the afnix:xpe nameset
    gset->symcst ("XmlContent",        new Meta (XmlContent::mknew));
    gset->symcst ("XmlInclude",        new Meta (XmlInclude::mknew));
    gset->symcst ("XmlProcessor",      new Meta (XmlProcessor::mknew));

    gset->symcst ("XmlPlist",          new Meta (XmlPlist::mknew));
    gset->symcst ("XmlPrintTable",     new Meta (XmlPrintTable::mknew));

    // bind the predicates
    gset->symcst ("xml-content-p",     new Function (xpe_xcntp));
    gset->symcst ("xml-feature-p",     new Function (xpe_xftrp));
    gset->symcst ("xml-include-p",     new Function (xpe_xincp));
    gset->symcst ("xml-processor-p",   new Function (xpe_procp));

    gset->symcst ("xml-plist-p",       new Function (xpe_xplstp));
    gset->symcst ("xml-print-table-p", new Function (xpe_xptblp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_xpe (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_xpe (interp, argv);
  }
}
