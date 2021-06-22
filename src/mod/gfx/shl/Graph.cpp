// ---------------------------------------------------------------------------
// - Graph.cpp                                                               -
// - afnix:gfx module - graph base class implementation                      -
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

#include "Graph.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty graph

  Graph::Graph (void) {
    p_clo = nullptr;
  }

  // create a graph with a client object

  Graph::Graph (Object* clo) {
    Object::iref (p_clo = clo);
  }

  // delete this graph

  Graph::~Graph (void) {
    Object::dref (p_clo);
  }

  // return the graph class name

  String Graph::repr (void) const {
    return "Graph";
  }

  // release this object

  void Graph::release (void) {
    wrlock ();
    try {
      // protect us
      Object::iref (this);
      // release all edges
      long ne = getne ();
      for (long i = 0; i < ne; i++) {
	Edge* edge = dynamic_cast <Edge*> (d_edges.get (i));
	if (edge == nullptr) continue;
	edge->release ();
      }
      // release all vertices
      long nv = getnv ();
      for (long i = 0; i < nv; i++) {
	Vertex* vrtx = dynamic_cast <Vertex*> (d_vrtxs.get (i));
	if (vrtx == nullptr) continue;
	vrtx->release ();
      }
      Object::tref (this);
    } catch (...) {
      Object::tref (this);
      unlock ();
      throw;
    }
  }

  // reset the graph

  void Graph::reset (void) {
    wrlock ();
    try {
      ereset ();
      vreset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset all edges in this graph

  void Graph::ereset (void) {
    wrlock ();
    try {
      long ne = getne ();
      for (long i = 0; i < ne; i++) {
	Edge* edge = dynamic_cast <Edge*> (d_edges.get (i));
	if (edge == nullptr) continue;
	edge->reset ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset all vertices in this graph

  void Graph::vreset (void) {
    wrlock ();
    try {
      long nv = getnv ();
      for (long i = 0; i < nv; i++) {
	Vertex* vrtx = dynamic_cast <Vertex*> (d_vrtxs.get (i));
	if (vrtx == nullptr) continue;
	vrtx->reset ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the vertex exists in this graph
  
  bool Graph::exists (Vertex* vertex) const {
    rdlock ();
    bool result = d_vrtxs.exists (vertex);
    unlock ();
    return result;
  }

  // return true if the edge exists in this graph
  
  bool Graph::exists (Edge* edge) const {
    rdlock ();
    bool result = d_edges.exists (edge);
    unlock ();
    return result;
  }
 
  // add an edge to this graph

  void Graph::add (Edge* edge) {
    if (edge == nullptr) return;
    wrlock ();
    try {
      if (d_edges.exists (edge) == false) {
	// add the edge
	d_edges.add (edge);
	// collect all vertices and add them
	long nv = edge->cardinality ();
	for (long i = 0; i < nv; i++) add (edge->get (i));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vertex to this graph

  void Graph::add (Vertex* vertex) {
    if (vertex == nullptr) return
    wrlock ();
    try {
      if (d_vrtxs.exists (vertex) == false) {
	// add the vertx
	d_vrtxs.add (vertex);
	// collect all edges and add them
	long ne = vertex->degree ();
	for (long i = 0; i < ne; i++) add (vertex->get (i));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of edges

  long Graph::getne (void) const {
    rdlock ();
    long result = d_edges.length ();
    unlock ();
    return result;
  }

  // return the number of vertices

  long Graph::getnv (void) const {
    rdlock ();
    long result = d_vrtxs.length ();
    unlock ();
    return result;
  }


  // get an edge by index

  Edge* Graph::getedge (const long index) const {
    rdlock ();
    try {
      Edge* edge = dynamic_cast <Edge*> (d_edges.get (index));
      unlock ();
      return edge;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a vertex by index

  Vertex* Graph::getvrtx (const long index) const {
    rdlock ();
    try {
      Vertex* vertex = dynamic_cast <Vertex*> (d_vrtxs.get (index));
      unlock ();
      return vertex;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the graph client object

  void Graph::setclo (Object* clo) {
    wrlock ();
    if (p_clo != clo) {
      Object::dref (p_clo);
      Object::iref (p_clo = clo);
    }
    unlock ();
  }

  // get the graph client object

  Object* Graph::getclo (void) const {
    rdlock ();
    Object* clo = p_clo;
    unlock ();
    return clo;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the graph supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_ERESET  = zone.intern ("reset-edges");
  static const long QUARK_VRESET  = zone.intern ("reset-vertices");
  static const long QUARK_EXISTS  = zone.intern ("exists");
  static const long QUARK_NEDGES  = zone.intern ("number-of-edges");
  static const long QUARK_NVRTXS  = zone.intern ("number-of-vertices");
  static const long QUARK_GETCLO  = zone.intern ("get-client");
  static const long QUARK_SETCLO  = zone.intern ("set-client");
  static const long QUARK_GETEDGE = zone.intern ("get-edge");
  static const long QUARK_GETVRTX = zone.intern ("get-vertex");

  // create a new object in a generic way

  Object* Graph::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return new Graph;
    if (argc == 1) return new Graph (argv->get (0));
    throw Exception ("argument-error", "too many arguments to create graph");
  }

  // return true if the given quark is defined

  bool Graph::isquark (const long quark, const bool hflg) const {
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
  
  Object* Graph::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEDGES) return new Integer (getne ());
      if (quark == QUARK_NVRTXS) return new Integer (getnv ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_ERESET) {
	ereset ();
	return nullptr;
      }
      if (quark == QUARK_VRESET) {
	vreset ();
	return nullptr;
      }
      if (quark == QUARK_GETCLO) {
	rdlock ();
	Object* result = getclo ();
	zobj->post (result);
	unlock ();
	return result;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	Edge* edge = dynamic_cast <Edge*> (argv->get (0));
	if (edge != nullptr) {
	  add (edge);
	  zobj->post (edge);
	  return edge;
	}
	Vertex* vertex = dynamic_cast <Vertex*> (argv->get (0));
	if (vertex != nullptr) {
	  add (vertex);
	  zobj->post (vertex);
	  return vertex;
	}
	throw Exception ("type-error", "invalid object to add to graph");
      }
      if (quark == QUARK_EXISTS) {
	Object* obj = argv->get (0);
	// check for edge
	Edge* edge = dynamic_cast <Edge*> (obj);
	if (edge != nullptr) return new Boolean (exists (edge));
	// check for vertex
	Vertex* vertex = dynamic_cast <Vertex*> (obj);
	if (vertex != nullptr) return new Boolean (exists (vertex));
	// type error
	throw Exception ("type-error", "invalid object to check in graph",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETEDGE) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Edge* edge = getedge (index);
	  zobj->post (edge);
	  unlock ();
	  return edge;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETVRTX) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Vertex* vertex = getvrtx (index);
	  zobj->post (vertex);
	  unlock ();
	  return vertex;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SETCLO) {
	Object* result = argv->get (0);
	setclo (result);
	zobj->post (result);
	return result;
      }
    }
    // call the collectable method
    return Collectable::apply (zobj, nset, quark, argv);
  }
}
