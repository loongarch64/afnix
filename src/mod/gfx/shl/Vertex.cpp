// ---------------------------------------------------------------------------
// - Vertex.cpp                                                              -
// - afnix:gfx module - graph vertex class implementation                    -
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

  // create a new vertex

  Vertex::Vertex (void) {
    d_mrk = false;
    p_clo = nullptr;
  }

  // create a vertex with a client object

  Vertex::Vertex (Object* clo) {
    d_mrk = false;
    Object::iref (p_clo = clo);
  }

  // destroy this vertex

  Vertex::~Vertex (void) {
    Object::dref (p_clo);
  }

  // return the vertex class name

  String Vertex::repr (void) const {
    return "Vertex";
  }

 // release this object

  void Vertex::release (void) {
    wrlock ();
    try {
      // protect us
      Object::iref (this);
      // release in all edges
      long elen = d_eos.length ();
      for (long i = 0; i < elen; i++) {
	Edge* edge = dynamic_cast <Edge*> (d_eos.get (i));
	if (edge == nullptr) continue;
	edge->remove (this);
      }
      Object::tref (this);
      unlock ();
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // reset the vertex

  void Vertex::reset (void) {
    wrlock ();
    try {
      d_mrk = false;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the vertex degree

  long Vertex::degree (void) const {
    rdlock ();
    try {
      long result = d_eos.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an edge to this vertex

  void Vertex::add (Edge* edge) {
    if (edge == nullptr) return;
    wrlock ();
    try {
      if (d_eos.exists (edge) == false) {
	d_eos.add (edge);
	edge->add (this);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an edge by index

  Edge* Vertex::get (const long index) const {
    rdlock ();
    try {
      Edge* result = dynamic_cast <Edge*> (d_eos.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an edge by object

  bool Vertex::remove (Edge* edge) {
    wrlock ();
    try {
      // protect us
      Object::iref (edge);
      // remove the edge
      bool status = (edge == nullptr) ? false : d_eos.remove (edge);
      // unprotect and unlock
      Object::tref (edge);
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the vertex client object

  void Vertex::setclo (Object* clo) {
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

  // get the vertex client object

  Object* Vertex::getclo (void) const {
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

  // the vertex supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_DEGREE = zone.intern ("degree");
  static const long QUARK_GETCLO = zone.intern ("get-client");
  static const long QUARK_SETCLO = zone.intern ("set-client");

  // create a new object in a generic way

  Object* Vertex::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return new Vertex;
    if (argc == 1) return new Vertex (argv->get (0));
    throw Exception ("argument-error", "too many arguments to create vertex");
  }

  // return true if the given quark is defined

  bool Vertex::isquark (const long quark, const bool hflg) const {
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

  Object* Vertex::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_DEGREE) return new Integer (degree ());
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
	Edge* edge = dynamic_cast <Edge*> (argv->get (0));
	if (edge == nullptr) throw Exception ("type-error", "invalid object");
	add (edge);
	zobj->post (edge);
	return edge;
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
	  Edge* edge = get (index);
	  zobj->post (edge);
	  unlock ();
	  return edge;
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
