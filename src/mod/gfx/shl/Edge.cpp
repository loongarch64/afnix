// ---------------------------------------------------------------------------
// - Edge.cpp                                                                -
// - afnix:gfx module - graph edge class implementation                      -
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

#include "Edge.hpp"
#include "Vertex.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new edge

  Edge::Edge (void) {
    d_mrk = false;
    p_clo = nullptr;
  }

  // create a edge with a client object

  Edge::Edge (Object* clo) {
    d_mrk = false;
    Object::iref (p_clo = clo);
  }

  // destroy this edge

  Edge::~Edge (void) {
    Object::dref (p_clo);
  }

  // return the edge class name

  String Edge::repr (void) const {
    return "Edge";
  }

  // release this object

  void Edge::release (void) {
    wrlock ();
    try {
      // protect us
      Object::iref (this);
      // release in all vertices
      long vlen = d_vos.length ();
      for (long i = 0; i < vlen; i++) {
	Vertex* vrtx = dynamic_cast <Vertex*> (d_vos.get (i));
	if (vrtx == nullptr) continue;
	vrtx->remove (this);
      }
      Object::tref (this);
      unlock ();
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // reset the edge

  void Edge::reset (void) {
    wrlock ();
    try {
      d_mrk = false;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the edge cardinality

  long Edge::cardinality (void) const {
    rdlock ();
    try {
      long result = d_vos.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an vertex to this edge

  void Edge::add (Vertex* vertex) {
    if (vertex == nullptr) return;
    wrlock ();
    try {
      if (d_vos.exists (vertex) == false) {
	d_vos.add (vertex);
	vertex->add (this);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an vertex by index

  Vertex* Edge::get (const long index) const {
    rdlock ();
    try {
      Vertex* result = dynamic_cast <Vertex*> (d_vos.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove a vertex by object

  bool Edge::remove (Vertex* vrtx) {
    wrlock ();
    try {
      // protect us
      Object::iref (vrtx);
      // remove the vertex
      bool status = (vrtx == nullptr) ? false : d_vos.remove (vrtx);
      // unprotect and unlock
      Object::tref (vrtx);
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the edge client object

  void Edge::setclo (Object* clo) {
    wrlock ();
    try {
      if (p_clo != clo) {
	Object::dref (p_clo);
	Object::iref (p_clo = clo);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the edge client object

  Object* Edge::getclo (void) const {
    rdlock ();
    try {
      Object* clo = p_clo;
      unlock ();
      return clo;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the edge supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_CARD   = zone.intern ("cardinality");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_GETCLO = zone.intern ("get-client");
  static const long QUARK_SETCLO = zone.intern ("set-client");

  // create a new object in a generic way

  Object* Edge::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return new Edge;
    if (argc == 1) return new Edge (argv->get (0));
    throw Exception ("argument-error", "too many arguments to create edge");
  }

  // return true if the given quark is defined

  bool Edge::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Collectable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Edge::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_CARD) return new Integer (cardinality ());
      if (quark == QUARK_GETCLO) {
	rdlock ();
	Object* result = getclo ();
	zobj->post (result);
	unlock ();
	return result;
      }
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	Vertex* vertex = dynamic_cast <Vertex*> (argv->get (0));
	if (vertex == nullptr) throw Exception ("type-error", "invalid object");
	add (vertex);
	zobj->post (vertex);
	return vertex;
      }
      if (quark == QUARK_SETCLO) {
	Object* result = argv->get (0);
	setclo (result);
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Vertex* vertex = get (index);
	  zobj->post (vertex);
	  unlock ();
	  return vertex;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the collectable method
    return Collectable::apply (zobj, nset, quark, argv);
  }
}
