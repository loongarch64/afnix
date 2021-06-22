// ---------------------------------------------------------------------------
// - Libsvg.cpp                                                              -
// - afnix:svg service - declaration & implementation                        -
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
#include "Libsvg.hpp"
#include "Plot2d.hpp"
#include "Predsvg.hpp"
#include "SvgRoot.hpp"
#include "SvgRect.hpp"
#include "SvgLine.hpp"
#include "SvgGroup.hpp"
#include "Function.hpp"
#include "Transform.hpp"
#include "SvgCircle.hpp"
#include "SvgEllipse.hpp"
#include "SvgPolygon.hpp"
#include "SvgPolyline.hpp"

namespace afnix {

  // initialize the afnix:svg service

  Object* init_afnix_svg (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:svg nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("svg");

    // bind all symbols in the afnix:svg nameset
    gset->symcst ("Plot2d",          new Meta (Plot2d::mknew));
    gset->symcst ("Transform",       new Meta (Transform::mknew));

    gset->symcst ("SvgRoot",         new Meta (SvgRoot::mknew));
    gset->symcst ("SvgRect",         new Meta (SvgRect::mknew));
    gset->symcst ("SvgLine",         new Meta (SvgLine::mknew));
    gset->symcst ("SvgGroup",        new Meta (SvgGroup::mknew));
    gset->symcst ("SvgCircle",       new Meta (SvgCircle::mknew));
    gset->symcst ("SvgEllipse",      new Meta (SvgEllipse::mknew));
    gset->symcst ("SvgFragment",     new Meta (SvgFragment::mknew));
    gset->symcst ("SvgPolygon",      new Meta (SvgPolygon::mknew));
    gset->symcst ("SvgPolyline",     new Meta (SvgPolyline::mknew));

    // bind the predicates
    gset->symcst ("plot2d-p",        new Function (svg_pltdp));
    gset->symcst ("transform-p",     new Function (svg_trfrp));

    gset->symcst ("svg-root-p",      new Function (svg_rootp));
    gset->symcst ("svg-rect-p",      new Function (svg_rectp));
    gset->symcst ("svg-line-p",      new Function (svg_linep));
    gset->symcst ("svg-circle-p",    new Function (svg_circp));
    gset->symcst ("svg-group-p",     new Function (svg_grpp));
    gset->symcst ("svg-ellipse-p",   new Function (svg_elpsp));
    gset->symcst ("svg-styling-p",   new Function (svg_stlgp));
    gset->symcst ("svg-fragment-p",  new Function (svg_fragp));
    gset->symcst ("svg-polygon-p",   new Function (svg_pgonp));
    gset->symcst ("svg-polyline-p",  new Function (svg_plinp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_svg (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_svg (interp, argv);
  }
}
