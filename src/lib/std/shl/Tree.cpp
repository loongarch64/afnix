// ---------------------------------------------------------------------------
// - Tree.cpp                                                                -
// - standard object library - tree object class implementation              -
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

#include "Tree.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the tree structure
  struct s_node {
    // the node index
    long d_nidx;
    // the node object
    Object* p_nobj;
    // the left node
    s_node* p_lnod;
    // the right node
    s_node* p_rnod;
    // create a default node
    s_node (void) {
      d_nidx = 0L;
      p_nobj = nullptr;
      p_lnod = nullptr;
      p_rnod = nullptr;
    }
    // create a node by index and object
    s_node (const long nidx, Object* nobj) {
      d_nidx = nidx;
      Object::iref (p_nobj = nobj);
      p_lnod = nullptr;
      p_rnod = nullptr;
    }
    // destroy this node
    ~s_node (void) {
      Object::dref (p_nobj);
      if (p_lnod != nullptr) delete p_lnod;
      if (p_rnod != nullptr) delete p_rnod;
    }
    // compute the length of this node
    long length (void) const {
      long result = 1L;
      if (p_lnod != nullptr) result += p_lnod->length();
      if (p_rnod != nullptr) result += p_rnod->length();
      return result;
    }
    // check of a node exists by index
    bool exists (const long nidx) const {
      if (nidx < d_nidx) {
	return (p_lnod == nullptr) ? false : p_lnod->exists(nidx);
      } else if (nidx > d_nidx) {
	return (p_rnod == nullptr) ? false : p_rnod->exists(nidx);
      }
      return (nidx == d_nidx) ? true : false;
    }
    // find an object by nodex index
    Object* find (const long nidx) const {
      if (nidx < d_nidx) {
	return (p_lnod == nullptr) ? nullptr : p_lnod->find(nidx);
      } else if (nidx > d_nidx) {
	return (p_rnod == nullptr) ? nullptr : p_rnod->find(nidx);
      }
      return (nidx == d_nidx) ? p_nobj : nullptr;
    }
    // add an object by index
    void add (const long nidx, Object* nobj) {
      if (nidx < d_nidx) {
	if (p_lnod == nullptr) {
	  p_lnod = new s_node (nidx, nobj);
	} else {
	  p_lnod->add (nidx, nobj);
	}
      } else if (nidx > d_nidx) {
	if (p_rnod == nullptr) {
	  p_rnod = new s_node (nidx, nobj);
	} else {
	  p_rnod->add (nidx, nobj);
	}
      } else {
	Object::iref (nobj);
	Object::dref (p_nobj);
	p_nobj = nobj;
      }
    }
  };
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tree

  Tree::Tree (void) {
    p_root = nullptr;
  }

  // destroy this tree
  
  Tree::~Tree (void) {
    delete p_root;
  }
  
  // return the class name
  
  String Tree::repr (void) const {
    return "Tree";
  }
  
  // reset this tree

  void Tree::reset (void) {
    wrlock ();
    try {
      delete p_root;
      p_root = new s_node;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of elements

  long Tree::length (void) const {
    rdlock ();
    try {
      long result = (p_root == nullptr) ? 0L : p_root->length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	
  }
  
  // check if a node index exists

  bool Tree::exists (const long nidx) const {
    rdlock ();
    try {
      bool result = (p_root == nullptr) ? false : p_root->exists (nidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // add an object by node index

  void Tree::add (const long nidx, Object* nobj) {
    wrlock ();
    try {
      if (p_root == nullptr) {
	p_root = new s_node (nidx, nobj);
      } else {
	p_root->add (nidx, nobj);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // find an object by node index

  Object* Tree::find (const long nidx) const {
    rdlock ();
    try {
      Object* result = (p_root == nullptr) ? nullptr : p_root->find (nidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_FIND    = zone.intern ("find");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EXISTS  = zone.intern ("exists-p");
  
  // create a new object in a generic way
  
  Object* Tree::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Tree;
    throw Exception ("argument-error", "too many arguments with tree");
  }
  
  // return true if the given quark is defined
  
  bool Tree::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Tree::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTS) {
	long nidx = argv->getlong (0);
	return new Boolean (exists (nidx));
      }
      if (quark == QUARK_FIND) {
	long nidx = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = find (nidx);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	long nidx = argv->getlong (0);
	Object* nobj = argv->get (1);
	add (nidx, nobj);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
