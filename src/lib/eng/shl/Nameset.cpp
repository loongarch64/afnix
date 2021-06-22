// ---------------------------------------------------------------------------
// - Nameset.cpp                                                            -
// - afnix engine - nameset class implementation                            -
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

#include "Symbol.hpp"
#include "Nameset.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the nameset as a quark
  static const long QUARK_THIS   = String::intern (".");
  // the parent nameset as a quark
  static const long QUARK_PARENT = String::intern ("..");
  // the whom object as a quark
  static const long QUARK_WHOM   = String::intern ("whom");

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default nameset

  Nameset::Nameset (void) {
    p_pset = nullptr;
    p_whom = nullptr;
  }

  // destroy this nameset

  Nameset::~Nameset (void) {
    Object::dref (p_pset);
    Object::dref (p_whom);
  }

  // set the parent nameset 

  void Nameset::setparent (Nameset* nset) {
    wrlock ();
    try {
      // check for equality
      if (nset == this) {
	throw Exception ("nameset-error", "cannot bind itself as parent");
      }
      if (p_pset != nullptr) remove (QUARK_PARENT);
      Object::iref (nset); Object::dref (p_pset); p_pset = nset;
      if (nset != nullptr) symcst (QUARK_PARENT, nset);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the parent nameset

  Nameset* Nameset::getparent (void) const {
    rdlock ();
    try {
      Nameset* result = p_pset;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the nameset context  object

  void Nameset::setwhom (Object* object) {
    wrlock ();
    try {
      if (p_whom != nullptr) remove (QUARK_WHOM);
      Object::iref (object); Object::dref (p_whom); p_whom = object;
      if (object != nullptr) symcst (QUARK_WHOM, object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the nameset context

  Object* Nameset::getwhom (void) const {
    rdlock ();
    try {
      Object* result = p_whom;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a new object by name

  void Nameset::bind (const String& name, Object* object) {
    wrlock ();
    try {
      bind (name.toquark (), object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the name exists in this nameset
  bool Nameset::exists (const String& name) const {
    rdlock ();
    try {
      bool result = exists (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the name exists in this nameset
  
  bool Nameset::valid (const long quark) const {
    rdlock ();
    try {
      // check in the local nameset
      bool result = exists (quark);
      // check in the parent nameset
      if ((result == false) && (p_pset != nullptr)) {
	result = p_pset->valid (quark);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the name exists in this nameset
  
  bool Nameset::valid (const String& name) const {
    rdlock ();
    try {
      bool result = valid (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an object by name locally

  Object* Nameset::get (const String& name) const {
    rdlock ();
    try {
      Object* result = get (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an object by quark recursively

  Object* Nameset::find (const long quark) const {
    try {
      Object* result = get (quark);
      if ((result == nullptr) && (p_pset != nullptr)) {
	result = p_pset->find (quark);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an object by name recursively

  Object* Nameset::find (const String& name) const {
    try {
      Object* result = find (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove an object by name

  void Nameset::remove (const String& name) {
    wrlock ();
    try {
      remove (name.toquark ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }  
  }
  
  // create a nameset set by quark

  Nameset* Nameset::mknset (const long quark) {
    wrlock ();
    try {
      // look for an existing one
      if (exists (quark) == true) {
	Nameset* nset = getnset (quark);
	unlock ();
	return nset;
      }
      // create the result nameset and bind it
      Nameset* result = dup ();
      symcst (quark, result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a nameset set by name

  Nameset* Nameset::mknset (const String& name) {
    return mknset (name.toquark ());
  }

  // get a nameset by quark

  Nameset* Nameset::getnset (const long quark) const {
    rdlock ();
    if (exists (quark) == false) {
      unlock ();
      throw Exception ("nameset-error", "cannot find nameset",
		       String::qmap (quark));
    }
    // get the object and map it - most likelly the object is a symbol
    Object*  obj = find (quark);
    Symbol*  sym = dynamic_cast <Symbol*> (obj);
    // remap the object and check
    if (sym != nullptr) obj = sym->getobj ();
    Nameset* nset = dynamic_cast <Nameset*> (obj);
    if (nset == nullptr) {
      unlock ();
      throw Exception ("type-error", "object is not a nameset", 
		       String::qmap (quark));
    }
    unlock ();
    return nset;
  }

  // get a nameset by name

  Nameset* Nameset::getnset (const String& name) const {
    return getnset (name.toquark ());
  }

  // create a new const symbol by quark

  void Nameset::symcst (const long quark, Object* object) {
    wrlock ();
    try {
      Symbol* sym = new Symbol (quark, true, object);
      bind (quark, sym);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a new const symbol by name

  void Nameset::symcst (const String& name, Object* object) {
    wrlock ();
    try {
      Symbol* sym = new Symbol (name, true, object);
      bind (name, sym);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a new symbol by quark

  void Nameset::symdef (const long quark, Object* object) {
    wrlock ();
    try {
      Symbol* sym = new Symbol (quark, object);
      bind (quark, sym);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a new symbol by name

  void Nameset::symdef (const String& name, Object* object) {
    wrlock ();
    try {
      Symbol* sym = new Symbol (name, object);
      bind (name, sym);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a new symbol by quark and flag

  void Nameset::symset (const long quark, const bool cflag, Object* object) {
    wrlock ();
    try {
      if (cflag == true) {
	symcst (quark, object);
      } else {
	symdef (quark, object);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a new symbol by name and flag

  void Nameset::symset (const String& name, const bool cflag, Object* object) {
    wrlock ();
    try {
      if (cflag == true) {
	symcst (name, object);
      } else {
	symdef (name, object);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // set a constant object by quark

  Object* Nameset::cdef (Evaluable* zobj, Nameset* nset, const long quark,
			 Object* object) {
    wrlock ();
    try {
      // check for the localset
      if (quark == QUARK_THIS) {
	throw Exception ("nameset-error", "cannot bind localset symbol");
      }
      // get the object by quark
      Object* obj = get (quark);
      // bind the object if possible
      if (obj != nullptr) {
	obj->cdef (zobj, nset, object);
	zobj->post (object);
	unlock ();
	return object;
      }
      // create a symbol locally
      symcst (quark, object);
      zobj->post (object);
      unlock ();
      return object;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an object by quark

  Object* Nameset::vdef (Evaluable* zobj, Nameset* nset, const long quark,
			 Object* object) {
    wrlock ();
    try {
      // check for the localset
      if (quark == QUARK_THIS) {
	throw Exception ("nameset-error", "cannot bind localset symbol");
      }
      // get the object by quark
      Object* obj = get (quark);
      // bind the object if possible
      if (obj != nullptr) {
	obj->vdef (zobj, nset, object);
	zobj->post (object);
	unlock ();
	return object;
      }
      // create a symbol locally
      symdef (quark, object);
      zobj->post (object);
      unlock ();
      return object;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unreference an object by quark

  Object* Nameset::udef (Evaluable* zobj, Nameset* nset, const long quark) {
    wrlock ();
    try {
      // check for the localset
      if (quark == QUARK_THIS) {
	throw Exception ("nameset-error", 
			 "cannot unreference localset symbol");
      }
      // get the object by quark
      Object* obj = get (quark);
      // unreference the object if possible
      if (obj != nullptr) obj->udef (zobj, nset);
      // remove the symbol localy
      remove (quark);
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate an object in the current nameset by quark

  Object* Nameset::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    // check for localset
    if (quark == QUARK_THIS) return this;
    // lock and evaluate
    rdlock ();
    try {
      // get the object by quark
      Object* obj = get (quark);
      // evaluate the object
      if (obj != nullptr) {
	Object* result = obj->eval (zobj, nset);
	zobj->post (result);
	unlock ();
	return result;
      }
      // try in the parent
      if (p_pset != nullptr) {
	Object* result = p_pset->eval (zobj, p_pset, quark);
	zobj->post (result);
	unlock ();
	return result;
      }
      // not found
      throw Exception ("eval-error", "unbound symbol", String::qmap (quark));
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
