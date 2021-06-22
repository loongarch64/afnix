// ---------------------------------------------------------------------------
// - Predsvg.cpp                                                             -
// - afnix:svg service - predicates implementation                           -
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

#include "Cons.hpp"
#include "Boolean.hpp"
#include "Predsvg.hpp"
#include "Plot2d.hpp"
#include "SvgRoot.hpp"
#include "SvgRect.hpp"
#include "SvgLine.hpp"
#include "SvgGroup.hpp"
#include "SvgCircle.hpp"
#include "Transform.hpp"
#include "Exception.hpp"
#include "SvgEllipse.hpp"
#include "SvgPolygon.hpp"
#include "SvgPolyline.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
                                 const String& pname) {
    Object* car = nullptr;
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
                       pname);
    car = args->getcar ();
    return (car == nullptr) ? nullptr : car->eval (zobj,nset);
  }

  // pltdp: plot2d object predicate

  Object* svg_pltdp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "plot2d-p");
    bool result = (dynamic_cast <Plot2d*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // trfrp: svg transform object predicate
  
  Object* svg_trfrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "transform-p");
    bool result = (dynamic_cast <Transform*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rootp: svg root object predicate

  Object* svg_rootp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-root-p");
    bool result = (dynamic_cast <SvgRoot*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // fragp: svg fragment object predicate

  Object* svg_fragp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-fragment-p");
    bool result = (dynamic_cast <SvgFragment*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // stlgp: svg styling object predicate

  Object* svg_stlgp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-styling-p");
    bool result = (dynamic_cast <SvgStyling*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rectp: svg rectangle object predicate

  Object* svg_rectp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-rect-p");
    bool result = (dynamic_cast <SvgRect*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // circp: svg circle object predicate

  Object* svg_circp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-circle-p");
    bool result = (dynamic_cast <SvgCircle*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // elpsp: svg ellipse object predicate

  Object* svg_elpsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-ellipse-p");
    bool result = (dynamic_cast <SvgEllipse*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // linep: svg line object predicate

  Object* svg_linep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-line-p");
    bool result = (dynamic_cast <SvgLine*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // plinp: svg polyline object predicate

  Object* svg_plinp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-polyline-p");
    bool result = (dynamic_cast <SvgPolyline*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pgonp: svg polygon object predicate

  Object* svg_pgonp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-polygon-p");
    bool result = (dynamic_cast <SvgPolygon*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // grpp: svg group object predicate

  Object* svg_grpp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "svg-group-p");
    bool result = (dynamic_cast <SvgGroup*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
