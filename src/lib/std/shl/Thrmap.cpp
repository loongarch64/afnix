// ---------------------------------------------------------------------------
// - Thrmap.cpp                                                              -
// - standard object library - object thread mapper class implementation     -
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

#include "Thrmap.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the thread specific map
  struct s_thrmap {
    // the thread id
    void*   p_tid;
    // the specific object
    Object* p_obj;
    // next specific in list
    s_thrmap* p_next;
    // create a new map entry
    s_thrmap (Object* object) {
      p_tid  = c_thrself ();
      p_obj  = Object::iref (object);
      p_next = nullptr;
    }
    // destroy this map
    ~s_thrmap (void) {
    Object::dref (p_obj);
    delete p_next;
    }
    // set an object in this map
    void set (Object* object) {
      Object::iref (object);
      Object::dref (p_obj);
      p_obj = object;
    }
  };

  // this procedure return the map associated with the thread
  static inline s_thrmap* get_tmap (s_thrmap* tmap) {
    // get the thread id
    void* tid = c_thrself ();
    // loop in the map
    while (tmap != nullptr) {
      if (tmap->p_tid == tid) return tmap;
      tmap = tmap->p_next;
    }
    // not found
    return nullptr;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new thread map

  Thrmap::Thrmap (void) {
    p_mobj = nullptr;
    p_tmap = nullptr;
  }

  // destroy this thread map

  Thrmap::~Thrmap (void) {
  Object::dref (p_mobj);
  delete p_tmap;
  }

  // return the class name

  String Thrmap::repr (void) const {
    return "Thrmap";
  }

  // set a thread specific object

  void Thrmap::set (Object* object) {
    // check if we are the master thread
    if (c_thrmaster () == true) {
      Object::iref (object);
      Object::dref (p_mobj);
      p_mobj = object;
      return;
    }
    // here we are in a thread - so we lock
    wrlock ();
    try {
      s_thrmap* tmap = get_tmap (p_tmap);
      if (tmap == nullptr) {
	tmap = new s_thrmap (object);
	if (p_tmap == nullptr) {
	  p_tmap = tmap;
	} else {
	  tmap->p_next = p_tmap;
	  p_tmap = tmap;
	}
      } else {
	tmap->set (object);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a thread specific object

  Object* Thrmap::get (void) const {
    // check if we are the master thread
    if (c_thrmaster () == true) return p_mobj;
    // here we are in a thread - so we lock
    rdlock ();
    try {
      s_thrmap* tmap = get_tmap (p_tmap);
      Object* result = (tmap == nullptr) ? nullptr : tmap->p_obj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
