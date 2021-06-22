// ---------------------------------------------------------------------------
// - Lexicon.cpp                                                             -
// - afnix:txt module - lexicon object class implementation                  -
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

#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Lexicon.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the trie structure - all methods operate by looking at the next
  // link - this approach permits to operate with an empty root tree
  struct s_eirt {
    // the character reference
    t_quad d_cref;
    // the terminal flag
    bool   d_term;
    // the horizontal link
    s_eirt* p_hlnk;
    // the vertical link
    s_eirt* p_vlnk;
    // create a default trie element
    s_eirt (void) {
      d_cref = nilq;
      d_term = false;
      p_hlnk = nullptr;
      p_vlnk = nullptr;
    }
    // create a trie element by value
    s_eirt (const t_quad c) {
      d_cref = c;
      d_term = false;
      p_hlnk = nullptr;
      p_vlnk = nullptr;
    }
    // destroy this trie element
    ~s_eirt (void) {
      delete p_vlnk;
      delete p_hlnk;
    }
    // find the next trie element
    s_eirt* find (const t_quad c) const {
      // initialize with the v-link
      s_eirt* elem = p_vlnk;
      // iterate with the h-links
      while (elem != nullptr) {
	if (elem->d_cref == c) return elem;
	elem = elem->p_hlnk;
      }
      return nullptr;
    }
    // add the next element
    s_eirt* add (const t_quad c) {
      // check the v-link
      if (p_vlnk == nullptr) {
	p_vlnk = new s_eirt (c);
	return p_vlnk;
      }
      // check for first entry
      if (c < p_vlnk->d_cref) {
	s_eirt* elem = new s_eirt (c);
	elem->p_hlnk = p_vlnk;
	p_vlnk = elem;
	return elem;
      }
      // look horizontaly
      s_eirt* elem = p_vlnk;
      while (elem != nullptr){
	if (elem->d_cref == c) return elem;
	if (elem->p_hlnk == nullptr) {
	  elem->p_hlnk = new s_eirt (c);
	  return elem->p_hlnk;
	} else {
	  if (c < elem->p_hlnk->d_cref) {
	    s_eirt* helm = new s_eirt (c);
	    helm->p_hlnk = elem->p_hlnk;
	    elem->p_hlnk = helm;
	    return helm;
	  }
	}
	elem = elem->p_hlnk;
      }
      throw Exception ("internal-error", "end of trie list reached in add");
    }
  };
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default lexicon

  Lexicon::Lexicon (void) {
    d_llen = 0;
    p_eirt = new s_eirt;
  }

  // destroy this lexicon
  
  Lexicon::~Lexicon (void) {
    delete p_eirt;
  }
  
  // return the class name
  
  String Lexicon::repr (void) const {
    return "Lexicon";
  }
  
  // reset this lexicon

  void Lexicon::reset (void) {
    wrlock ();
    try {
      delete p_eirt;
      d_llen = 0;
      p_eirt = new s_eirt;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the lexicon is empty

  bool Lexicon::empty (void) const {
    rdlock ();
    try {
      bool result = (d_llen == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of elements

  long Lexicon::length (void) const {
    rdlock ();
    try {
      long result = d_llen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a name exists

  bool Lexicon::exists (const String& name) const {
    // do nothing with a nil name
    if (name.isnil () == true) return false;
    // lock and check
    rdlock ();
    try {
      // get the trie tree
      s_eirt* elem = p_eirt;
      // loop in the name
      long nlen = name.length ();
      for (long i = nlen-1; 0 <= i; i--) {
	t_quad c = name[i];
	elem = elem->find (c);
	if (elem == nullptr) break;
      }
      bool result = (elem == nullptr) ? false : elem->d_term;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a name in the lexicon

  void Lexicon::add (const String& name) {
    // do nothing with a nil name
    if (name.isnil () == true) return;
    // lock and bind
    wrlock ();
    try {
      // get the trie tree
      s_eirt* elem = p_eirt;
      // loop in the name
      long nlen = name.length ();
      for (long i = nlen-1; 0 <= i; i--) {
	t_quad c = name[i];
	elem = elem->add (c);
      }
      // mark the trie element
      if (elem->d_term == false) {
	d_llen++;
	elem->d_term = true;
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
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_EMPTY  = zone.intern ("empty-p");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_EXISTS = zone.intern ("exists-p");
  
  // create a new object in a generic way
  
  Object* Lexicon::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Lexicon;
    throw Exception ("argument-error", "too many arguments with lexicon");
  }
  
  // return true if the given quark is defined
  
  bool Lexicon::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Lexicon::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTY)  return new Boolean (empty  ());
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	String name = argv->getstring (0);
	add (name);
	return nullptr;
      }
      if (quark == QUARK_EXISTS) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
