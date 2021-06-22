// ---------------------------------------------------------------------------
// - Worder.cpp                                                              -
// - afnix:txt module - word analyzer object class implementation            -
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

#include "Worder.hpp"
#include "Vector.hpp"
#include "Lexicon.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty worder

  Worder::Worder (void) {
    d_words.reset ();
  }

  // copy constructs this worder

  Worder::Worder (const Worder& that) {
    that.rdlock ();
    try {
      d_words = that.d_words;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String Worder::repr (void) const {
    return "Worder";
  }
  
  // clone this object

  Object* Worder::clone (void) const {
    return new Worder (*this);
  }

  // reset this worder

  void Worder::reset (void) {
    wrlock ();
    try {
      d_words.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the worder is empty

  bool Worder::empty (void) const {
    rdlock ();
    try {
      bool result = d_words.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of elements

  long Worder::length (void) const {
    rdlock ();
    try {
      long result = d_words.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of unique words

  long Worder::uwlen (void) const {
    rdlock ();
    try {
      // create a working lexicon
      Lexicon wlex;
      // add the words in the lexicon
      long wlen = d_words.length ();
      for (long i = 0; i < wlen; i++) {
	wlex.add (d_words.get (i));
      }
      // here is the result
      long result = wlex.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a word exists

  bool Worder::exists (const String& word) const {
    rdlock ();
    try {
      bool result = d_words.exists (word);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a word in the worder

  void Worder::add (const String& word) {
    // do nothing with a nil word
    if (word.isnil () == true) return;
    // lock and bind
    wrlock ();
    try {
      d_words.add (word);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a word by index

  String Worder::get (const long index) const {
    rdlock ();
    try {
      String result = d_words.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if word matches a regex

  bool Worder::match (const Regex& re) const {
    rdlock ();
    try {
      long wlen = d_words.length ();
      for (long i = 0; i < wlen; i++) {
	String word = d_words.get (i);
	if (re == word) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the words matches the string regex
  
  bool Worder::match (const String& re) const {
    rdlock ();
    try {
      // split the string specification
      Strvec svr = Strvec::split (re);
      // get spec length and check for null
      long len = svr.length ();
      if (len == 0) {
	unlock ();
	return false;
      }
      // loop in the spec
      for (long i= 0; i < len; i++) {
	Regex sre = svr.get (i);
	if (match (sre) == false) {
	  unlock ();
	  return false;
	}
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // count the number of words that match a regex

  long Worder::count (const Regex& re) const {
    rdlock ();
    try {
      // initialize result
      long result = 0;
      // loop in the words
      long wlen = d_words.length ();
      for (long i = 0; i < wlen; i++) {
	String word = d_words.get (i);
	if (re == word) result++;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // count the number of words that match a string regex

  Plist Worder::count (const String& re) const {
    rdlock ();
    try {
      // prepare result
      Plist result;
      // split the string specification
      Strvec svr = Strvec::split (re);
      // get the spec length and loop
      long len = svr.length ();
      for (long i = 0; i < len; i++) {
	String srs = svr.get (i);
	Regex  sre = srs;
	result.set (srs, Integer (count (sre)));
      }
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
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_COUNT  = zone.intern ("count");
  static const long QUARK_EMPTY  = zone.intern ("empty-p");
  static const long QUARK_UWLEN  = zone.intern ("unique-length");
  static const long QUARK_MATCHP = zone.intern ("match-p");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_EXISTS = zone.intern ("exists-p");
  
  // create a new object in a generic way
  
  Object* Worder::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Worder;
    throw Exception ("argument-error", "too many arguments with worder");
  }
  
  // return true if the given quark is defined
  
  bool Worder::isquark (const long quark, const bool hflg) const {
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
  
  Object* Worder::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTY)  return new Boolean (empty  ());
      if (quark == QUARK_UWLEN)  return new Integer (uwlen  ());
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	String word = argv->getstring (0);
	add (word);
	return nullptr;
      }
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	return new String (get (index));
      }
      if (quark == QUARK_EXISTS) {
	String word = argv->getstring (0);
	return new Boolean (exists (word));
      }
      if (quark == QUARK_MATCHP) {
	Object* obj = argv->get (0);
	// check for a regex
	Regex* zobj = dynamic_cast <Regex*> (obj);
	if (zobj != nullptr) return new Boolean (match (*zobj));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new Boolean (match (*sobj));
	// invalid object
	throw Exception ("type-error", "invalid object with match-p",
			 Object::repr (obj));
      }
      if (quark == QUARK_COUNT) {
	Object* obj = argv->get (0);
	// check for a regex
	Regex* zobj = dynamic_cast <Regex*> (obj);
	if (zobj != nullptr) return new Integer (count (*zobj));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new Plist (count (*sobj));
	// invalid object
	throw Exception ("type-error", "invalid object with count",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
