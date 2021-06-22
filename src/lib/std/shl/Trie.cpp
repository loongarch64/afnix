// ---------------------------------------------------------------------------
// - Trie.cpp                                                                -
// - standard object library - trie object class implementation              -
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

#include "Trie.hpp"
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

  // the trie structure - all methods operate by looking at the next
  // link - this approach permits to operate with an empty root tree
  struct s_trie {
    // the character reference
    t_quad d_cref;
    // the reference index
    long   d_ridx;
    // the counter flag
    long   d_rcnt;
    // the object reference
    Object* p_zobj;
    // the previous link
    s_trie* p_prev;
    // the next link
    s_trie* p_next;
    // the down element
    s_trie* p_down;
    // create a default trie element
    s_trie (void) {
      d_cref = nilq;
      d_ridx = -1;
      d_rcnt = 0L;
      p_zobj = nullptr;
      p_prev = nullptr;
      p_next = nullptr;
      p_down = nullptr;
    }
    // create a default trie element
    s_trie (const t_quad c) {
      d_cref = c;
      d_ridx = -1LL;
      d_rcnt = 0L;
      p_zobj = nullptr;
      p_prev = nullptr;
      p_next = nullptr;
      p_down = nullptr;
    }
    // destroy this trie element
    ~s_trie (void) {
      Object::dref (p_zobj);
      delete p_down;
      delete p_next;
    }
    // get the number of elements
    long length (void) const {
      // initialize result
      long result = (d_rcnt > 0) ? 1 : 0;
      // look at the next elements
      if (p_next != nullptr) result += p_next->length ();
      // look at the down elements
      if (p_down != nullptr) result += p_down->length ();
      // here it is
      return result;
    }    
    // count the total number of elements
    long count (void) const {
      // initialize result
      long result = d_rcnt;
      // look at the next elements
      if (p_next != nullptr) result += p_next->count ();
      // look at the down elements
      if (p_down != nullptr) result += p_down->count ();
      // here it is
      return result;
    }
    // find a trie element
    s_trie* find (const t_quad c) const {
      // initialize with the next element
      s_trie* elem = p_down;
      // iterate with the right links
      while (elem != nullptr) {
	if (elem->d_cref == c) return elem;
	elem = elem->p_next;
      }
      return nullptr;
    }
    // set or create a trie element
    s_trie* add (const t_quad c) {
      // check the next element
      if (p_down == nullptr) {
	p_down = new s_trie (c);
	return p_down;
      }
      // check for first entry
      if (c < p_down->d_cref) {
	s_trie* elem = new s_trie (c);
	elem->p_next = p_down;
	p_down->p_prev = elem;
	p_down = elem;
	return elem;
      }
      // look into next line
      s_trie* elem = p_down;
      while (elem != nullptr){
	if (elem->d_cref == c) return elem;
	if (elem->p_next == nullptr) {
	  elem->p_next = new s_trie (c);
	  elem->p_next->p_prev = elem;
	  return elem->p_next;
	} else {
	  if (c < elem->p_next->d_cref) {
	    s_trie* helm = new s_trie (c);
	    helm->p_next = elem->p_next;
	    helm->p_prev = elem;
	    elem->p_next = helm;
	    helm->p_next->p_prev = helm;
	    return helm;
	  }
	}
	elem = elem->p_next;
      }
      throw Exception ("internal-error", "end of trie list reached in add");
    }
    // update the names string vector
    void tonames (Strvec* sv, const String& pnam) const {
      // update local name
      if ((d_cref > 0) && (d_rcnt > 0)) {
	String name = pnam + d_cref;
	sv->add (name);
      }
      // look at the next elements
      if (p_next != nullptr) p_next->tonames (sv, pnam);
      // look at the down elements
      if (p_down != nullptr) {
	String name = pnam;
	if (d_cref > 0) name += d_cref;
	p_down->tonames (sv, name);
      }
    }
  };
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default trie

  Trie::Trie (void) {
    d_tlen = 0;
    p_tree = new s_trie;
  }

  // destroy tthis trie
  
  Trie::~Trie (void) {
    delete p_tree;
  }
  
  // return the class name
  
  String Trie::repr (void) const {
    return "Trie";
  }
  
  // reset this trie

  void Trie::reset (void) {
    wrlock ();
    try {
      delete p_tree;
      p_tree = new s_trie;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of elements

  long Trie::length (void) const {
    rdlock ();
    try {
      long result = d_tlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // return the trie total count

  long Trie::count (void) const {
    rdlock ();
    try {
      long result = p_tree->count ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // get the trie count by name

  long Trie::count (const String& name) const {
    // do nothing with a nil name
    if (name.isnil () == true) return 0L;
    // lock and check
    rdlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0L; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->find (c);
	if (elem == nullptr) break;
      }
      long result = (elem == nullptr) ? 0L : elem->d_rcnt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the trie reference index by name

  long Trie::index (const String& name) const {
    // do nothing with a nil name
    if (name.isnil () == true) return 0L;
    // lock and check
    rdlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0L; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->find (c);
	if (elem == nullptr) break;
      }
      long result = (elem == nullptr) ? -1 : elem->d_ridx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a name exists in the trie

  bool Trie::exists (const String& name) const {
    // do nothing with a nil name
    if (name.isnil () == true) return false;
    // lock and check
    rdlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->find (c);
	if (elem == nullptr) break;
      }
      bool result = (elem == nullptr) ? false : (elem->d_rcnt > 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a binding by name and object

  void Trie::add (const String& name, Object* wobj) {
    // do nothing with a nil name
    if (name.isnil () == true) return;
    // lock and bind
    wrlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->add (c);
      }
      // set the trie element
      elem->d_rcnt++;
      if (elem->d_rcnt == 1) elem->d_ridx = d_tlen++;
      Object::iref (wobj);
      Object::dref (elem->p_zobj);
      elem->p_zobj = wobj;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // mark a string in this trie

  void Trie::mark (const String& name) {
    // do nothing with a nil name
    if (name.isnil () == true) return;
    // lock and bind
    wrlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->add (c);
      }
      // mark the trie element
      elem->d_rcnt++;
      if (elem->d_rcnt == 1) elem->d_ridx = d_tlen++;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by name

  Object* Trie::get (const String& name) const {
    // do nothing with a nil name
    if (name.isnil () == true) return nullptr;
    // lock and check
    rdlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->find (c);
	if (elem == nullptr) break;
      }
      // get the element result
      Object* result = nullptr;
      if (elem != nullptr) result = elem->p_zobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by name of throw an exception

  Object* Trie::lookup (const String& name) const {
    // do nothing with a nil name
    if (name.isnil () == true) {
      throw Exception ("lookup-error", "cannot lookup with nil name");
    }
    // lock and check
    rdlock ();
    try {
      // get the trie tree
      s_trie* elem = p_tree;
      // loop in the name
      long wlen = name.length ();
      for (long i = 0; i < wlen; i++) {
	t_quad c = name[i];
	elem = elem->find (c);
	if (elem == nullptr) break;
      }
      // get the element result
      if ((elem == nullptr) || (elem->d_rcnt == 0L)) {
	throw Exception ("lookup-error", "cannot find name", name);
      }
      Object* result = elem->p_zobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a vector of trie names

  Strvec* Trie::tonames (void) const {
    rdlock ();
    Strvec* result = nullptr;
    try {
      // create a result vector
      result = new Strvec;
      // udpate in the trie
      p_tree->tonames (result, "");
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }	
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_MARK    = zone.intern ("mark");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_COUNT   = zone.intern ("count");
  static const long QUARK_INDEX   = zone.intern ("index");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_LOOKUP  = zone.intern ("lookup");
  static const long QUARK_EXISTS  = zone.intern ("exists-p");
  static const long QUARK_TONAMES = zone.intern ("to-names");
  
  // create a new object in a generic way
  
  Object* Trie::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Trie;
    throw Exception ("argument-error", "too many arguments with trie");
  }
  
  // return true if the given quark is defined
  
  bool Trie::isquark (const long quark, const bool hflg) const {
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
  
  Object* Trie::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length ());
      if (quark == QUARK_TONAMES) return tonames ();
      if (quark == QUARK_COUNT)   return new Integer (count ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTS) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_COUNT) {
	String name = argv->getstring (0);
	return new Integer (count (name));
      }
      if (quark == QUARK_INDEX) {
	String name = argv->getstring (0);
	return new Integer (index (name));
      }
      if (quark == QUARK_MARK) {
	String name = argv->getstring (0);
	mark (name);
	return nullptr;
      }
      if (quark == QUARK_GET) {
	String name = argv->getstring (0);
	rdlock ();
	try {
	  Object* result = get (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LOOKUP) {
	String name = argv->getstring (0);
	rdlock ();
	try {
	  Object* result = lookup (name);
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
	String  name = argv->getstring (0);
	Object* wobj = argv->get (1);
	add (name, wobj);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
