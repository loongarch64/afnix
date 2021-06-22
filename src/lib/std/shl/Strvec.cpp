// ---------------------------------------------------------------------------
// - Strvec.cpp                                                              -
// - standard object library - string vector class implementation            -
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

#include "Stdsid.hxx"
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Exception.hpp"
#include "QuarkZone.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // check that a character is in the c-string. 
  static bool match_break_sequence (const t_quad c, const t_quad* str) {
    long size = Unicode::strlen (str);
    // loop and compare
    for (long i = 0; i < size; i++)
      if (c == str[i]) return true;
    return false;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // split this string with a sequence of characters

  Strvec Strvec::split (const String& name, const String& sbrk, 
			const bool    uniq, const bool    zlen) {
    Strvec result (uniq); result.setzlen (zlen);
    Buffer buffer;
    
    // first thing first - do we have a nil string
    if (name.length () == 0) return result;
    
    // get a unicode string representation
    t_quad* data = name.toquad ();
    t_quad* cptr = data;
    // fix the break sequence in case it is nil
    const t_quad* cbrk = (sbrk.length () == 0) ? Unicode::strdup (" \t\n") : 
      sbrk.toquad ();
    // loop and accumulate - if a character match the break sequence
    // the buffer is inserted into the vector
    t_quad c = nilq;
    buffer.reset ();
    while ((c = *data++) != nilc) {
      if (match_break_sequence (c, cbrk) == true) {
	result.add (buffer.tostring());
	buffer.reset ();
	continue;
      }
      buffer.add (c);
    }
    // check if the buffer is not empty
    if (buffer.length () != 0) result.add (buffer.tostring());
    // clean the break sequence and return
    delete [] cbrk;
    delete [] cptr;
    return result;
  }

  // split this string with a break sequence and unique flag
  
  Strvec Strvec::split (const String& name, const String& sbrk, 
			const bool    uniq) {
    return Strvec::split (name, sbrk, uniq, true);
  }
  
  // split this string with a break sequence
  
  Strvec Strvec::split (const String& name, const String& sbrk) {
    return Strvec::split (name, sbrk, false, true);
  }
  
  // split this string with a default break sequence
  
  Strvec Strvec::split (const String& name) {
    return Strvec::split (name, "", false, false);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty string vector

  Strvec::Strvec (void) {
    d_size   = 0;
    d_uniq   = false;
    d_zlen   = true;
    d_length = 0;
    p_vector = nullptr;
  }

  // create a string vector with an original size

  Strvec::Strvec (const long size) {
    if (size < 0) throw Exception ("size-error","negative string vector size");
    d_size   = size;
    d_uniq   = false;
    d_zlen   = true;
    d_length = 0;
    p_vector = new String[d_size];
  }

  // create a string vector with a uniq flag

  Strvec::Strvec (const bool uniq) {
    d_size   = 0;
    d_uniq   = uniq;
    d_zlen   = true;
    d_length = 0;
    p_vector = nullptr;
  }

  // create a string vector with an original size and flag

  Strvec::Strvec (const long size, const bool uniq) {
    if (size < 0) throw Exception ("size-error","negative string vector size");
    d_size   = size;
    d_uniq   = uniq;
    d_zlen   = true;
    d_length = 0;
    p_vector = new String[d_size];
  }

  // copy constructor for this string vector

  Strvec::Strvec (const Strvec& that) {
    that.rdlock ();
    try {
      // copy arguments
      d_size   = that.d_length;
      d_uniq   = that.d_uniq;
      d_zlen   = that.d_zlen;
      d_length = that.d_length;
      p_vector = nullptr;
      // create a new vector of strings and copy them
      if ((d_length > 0) && (that.p_vector != nullptr)) {
	p_vector = new String[d_length];
	for (long i = 0; i < d_length; i++) p_vector[i] = that.p_vector[i];
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this string vector
  
  Strvec::~Strvec (void) {
    delete [] p_vector;
  }

  // return the class name

  String Strvec::repr (void) const {
    return "Strvec";
  }

  // return the string vector did

  t_word Strvec::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the string vector sid

  t_word Strvec::getsid (void) const {
    return SRL_STRV_SID;
  }

  // serialize this string vector

  void Strvec::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the vector length
      Serial::wrlong (d_length, os);
      // write the uniq flag
      Serial::wrbool (d_uniq, os);
      // write the strings
      for (long i = 0; i < d_length; i++) {
	p_vector[i].wrstream (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this string vector

  void Strvec::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the string vector
      reset  ();
      // get the vector length
      long len = Serial::rdlong (is);
      // get the uniq flag
      bool uniq = Serial::rdbool (is);
      // force uniq to false for reading
      d_uniq = false;
      // read in each object
      for (long i = 0; i < len; i++) {
	String data;
	data.rdstream (is);
	add (data);
      }
      // fix the uniq flag
      d_uniq = uniq;
      unlock ();
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // assign a string vector to this one
  
  Strvec& Strvec::operator = (const Strvec& that) {
    // check againt equal equal
    if (this == &that) return *this;
    // lock everything
    wrlock ();
    that.rdlock ();
    try {
      // delete old value
      delete [] p_vector;
      // copy arguments
      d_size   = that.d_length;
      d_uniq   = that.d_uniq;
      d_zlen   = that.d_zlen;
      d_length = that.d_length;
      p_vector = nullptr;
      // create a new string vector of strings and copy them
      if ((d_length > 0) && (that.p_vector != nullptr)) {
	p_vector = new String[d_size];
	for (long i = 0; i < d_length; i++) p_vector[i] = that.p_vector[i];
      }
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // reset this vector

  void Strvec::reset (void) {
    wrlock ();
    try {
      delete [] p_vector;
      d_size   = 0;
      d_length = 0;
      p_vector = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the uniq flag

  void Strvec::setuniq (const bool uniq) {
    wrlock ();
    try {
      d_uniq = uniq;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uniq flag

  bool Strvec::getuniq (void) const {
    rdlock ();
    try {
      bool result = d_uniq;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // set the zero length flag

  void Strvec::setzlen (const bool zlen) {
    wrlock ();
    try {
      d_zlen = zlen;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the zero length flag

  bool Strvec::getzlen (void) const {
    rdlock ();
    try {
      bool result = d_zlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the vector is empty

  bool Strvec::empty (void) const {
    rdlock ();
    try {
      bool result = (d_length == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the vector is active

  bool Strvec::active (void) const {
    rdlock ();
    try {
      bool result = (d_length != 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check that a string exists in this vector

  bool Strvec::exists (const String& name) const {
    rdlock ();
    try {
      if (d_length == 0) {
	unlock ();
	return false;
      }
      for (long i = 0; i < d_length; i++) {
	if (p_vector[i] == name) {
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

  // get the number of element in this string vector

  long Strvec::length (void) const {
    rdlock ();
    try {
      long result = d_length;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new element in this string vector
  
  void Strvec::add (const String& s) {
    wrlock ();
    try {
      // check if we add zero length
      if ((d_zlen == false) && (s.isnil () == true)) {
	unlock ();
	return;
      }
      // check if we have to resize the vector
      if (d_length + 1 >= d_size) {
	long size = (d_size <= 0) ? 1 : d_size * 2;
	String* vector = new String[size];
	for (long i = 0; i < d_length; i++) vector[i] = p_vector[i];
	delete [] p_vector;
	d_size   = size;
	p_vector = vector;
      }
      // set the string in this vector
      if ((d_uniq == false) || (exists (s) == false)) {
	p_vector[d_length++] = s;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an string at a certain position in this vector

  void Strvec::set (const long index, const String& s) {
    wrlock ();
    try {
      // check that we are bounded
      if (index >= d_length) 
	throw Exception ("index-error","in string vector set");
      p_vector[index] = s;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pop the first string from this vector

  String Strvec::pop (void) {
    wrlock ();
    try {
      // check that the vector is not empty
      if (d_length == 0) {
	throw Exception ("vector-error", "pop request with empty vector");
      }
      // save first string
      String result = p_vector[0];
      // compress the vector
      for (long i = 1; i < d_length; i++) {
	p_vector[i-1] = p_vector[i];
      }
      // clear last element
      p_vector[--d_length] = "";
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pop the last string from this vector

  String Strvec::rml (void) {
    wrlock ();
    try {
      // check that the vector is not empty
      if (d_length == 0) {
	throw Exception ("vector-error", "rml request with empty vector");
      }
      // save last string
      String result = p_vector[d_length-1];
      // clear last element
      p_vector[--d_length] = "";
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an string at a certain position

  String Strvec::get (const long index) const {
    rdlock ();
    try {
      // check that we are bounded
      if ((index < 0) || (index >= d_length)) { 
	throw Exception ("index-error","in string vector set");
      }
      String result = p_vector[index];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the first string in this vector

  String Strvec::first (void) const {
    rdlock ();
    try {
      String result = get (0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the last string in this vector

  String Strvec::last (void) const {
    rdlock ();
    try {
      String result = get (d_length-1);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the index of a key or -1

  long Strvec::find (const String& key) const {
    rdlock ();
    try {
      for (long i = 0; i < d_length; i++) {
	if (p_vector[i] == key) {
	  unlock ();
	  return i;
	}
      }
      unlock ();
      return -1;
    } catch (...) {
      unlock () ;
      throw;
    }
  }

  // return the index of a key in this string vector

  long Strvec::lookup (const String& key) const {
    rdlock ();
    try {
      for (long i = 0; i < d_length; i++) {
	if (p_vector[i] == key) {
	  unlock ();
	  return i;
	}
      }
      throw Exception ("key-error", "key not found", key);
    } catch (...) {
      unlock () ;
      throw;
    }
  }
  
  // remove an entry by index and repack the vector

  void Strvec::remove (const long index) {
    wrlock ();
    try {
      if ((index < 0) || (index >= d_length)) {
	throw Exception ("index-error","index is out of range");
      }
      // repack the vector
      long mark = d_length - 1;
      for (long i = index; i < mark; i++) {
	p_vector[i] = p_vector[i+1];
      }
      p_vector[mark] = "";
      d_length = mark;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an entry by value if it exists

  void Strvec::remove (const String& key) {
    wrlock ();
    try {
      // get the key index
      long index = find (key);
      // remove the entry
      if (index != -1) remove (index);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // return the maximum string length in this vector

  long Strvec::maxlen (void) const {
    rdlock ();
    try {
      long result = 0;
      for (long i = 0; i < d_length; i++) {
	long len = p_vector[i].length ();
	if (len > result) result = len;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock () ;
      throw;
    }
  }

  // return the minimum string length in this vector

  long Strvec::minlen (void) const {
    rdlock ();
    try {
      long result = Utility::maxlong ();
      for (long i = 0; i < d_length; i++) {
	long len = p_vector[i].length ();
	if (len < result) result = len;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock () ;
      throw;
    }
  }

  // concatenate the string vector without a separator

  String Strvec::concat (void) const {
    rdlock ();
    try {
      String result = concat (nilq);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // concatenate the string vector with a separator

  String Strvec::concat (const t_quad sc) const {
    rdlock ();
    try {
      String result = d_length > 0 ? p_vector[0] : "";
      for (long i = 1; i < d_length; i++) {
	// add the separator
	if (sc != nilq) result += sc;
	// add the string
	result += p_vector[i];
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an array of quarks for this vector

  long* Strvec::toquarks (void) const {
    rdlock ();
    long* result = nullptr;
    try {
      if (d_length == 0) {
	unlock ();
	return result;
      }
      long* result = new long[d_length];
      for (long i = 0; i < d_length; i++) result[i] = p_vector[i].toquark ();
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock () ;
      throw;
    }
  }

  // return a vector of strings

  Vector* Strvec::tovector (void) const {
    rdlock ();
    Vector* result = nullptr;
    try {
      if (d_length == 0) {
	unlock ();
	return result;
      }
      result = new Vector;
      for (long i = 0; i < d_length; i++) {
	result->add (new String (p_vector[i]));
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock () ;
      throw;
    } 
  }

  // return a new vector iterator

  Iterator* Strvec::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Strvecit (this);
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
  static const long QUARK_ZONE_LENGTH = 22;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_POP    = zone.intern ("pop");
  static const long QUARK_RML    = zone.intern ("pop-last");
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_SET    = zone.intern ("set");
  static const long QUARK_FIND   = zone.intern ("find");
  static const long QUARK_LAST   = zone.intern ("last");
  static const long QUARK_FIRST  = zone.intern ("first");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_REMOVE = zone.intern ("remove");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_CONCAT = zone.intern ("concat");
  static const long QUARK_MINLEN = zone.intern ("min-length");
  static const long QUARK_MAXLEN = zone.intern ("max-length");
  static const long QUARK_EXISTP = zone.intern ("exists-p");
  static const long QUARK_EMPTYP = zone.intern ("empty-p");
  static const long QUARK_ACTIVP = zone.intern ("active-p");
  static const long QUARK_LOOKUP = zone.intern ("lookup");
  static const long QUARK_SETUNQ = zone.intern ("set-unique");
  static const long QUARK_GETUNQ = zone.intern ("get-unique");
  static const long QUARK_SETZLN = zone.intern ("set-zero-length");
  static const long QUARK_GETZLN = zone.intern ("get-zero-length");

  // create a new object in a generic way

  Object* Strvec::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new Strvec;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj !=nullptr) {
        long size = iobj->tolong ();
        return new Strvec (size);
      }
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj !=nullptr) {
        bool uniq = bobj->tobool ();
        return new Strvec (uniq);
      }
      throw Exception ("type-error", "invalid object with heap",
                       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      long size = argv->getlong (0);
      bool uniq = argv->getbool (1);
      return new Strvec (size, uniq);
    }
    // check for 3 arguments
    if (argc == 3) {
      String sval = argv->getstring (0);
      String sbrk = argv->getstring (1);
      bool   uniq = argv->getbool   (2);
      return new Strvec (Strvec::split (sval, sbrk, uniq));
    }
    throw Exception ("argument-error", "too many argument for string vector");

  }

  // return true if the given quark is defined

  bool Strvec::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Serial::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Strvec::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_POP)    return new String  (pop     ());
      if (quark == QUARK_RML)    return new String  (rml     ());
      if (quark == QUARK_LAST)   return new String  (last    ());
      if (quark == QUARK_FIRST)  return new String  (first   ());
      if (quark == QUARK_CONCAT) return new String  (concat  ());
      if (quark == QUARK_LENGTH) return new Integer (length  ());
      if (quark == QUARK_MAXLEN) return new Integer (maxlen  ());
      if (quark == QUARK_MINLEN) return new Integer (minlen  ());
      if (quark == QUARK_EMPTYP) return new Boolean (empty   ());
      if (quark == QUARK_ACTIVP) return new Boolean (active  ());
      if (quark == QUARK_GETUNQ) return new Boolean (getuniq ());
      if (quark == QUARK_GETZLN) return new Boolean (getzlen ());
      if (quark == QUARK_RESET) {
	reset  ();
	return nullptr;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	return new String (get (index));
      }
      if (quark == QUARK_ADD) {
	String s = argv->getstring (0);
	add (s);
	return nullptr;
      }
      if (quark == QUARK_EXISTP) {
	String key = argv->getstring (0);
	bool result = exists (key);
	return new Boolean (result);
      }
      if (quark == QUARK_FIND) {
	String key = argv->getstring (0);
	long index = find (key);
	return new Integer (index);
      }
      if (quark == QUARK_LOOKUP) {
	String key = argv->getstring (0);
	long index = lookup (key);
	return new Integer (index);
      }
      if (quark == QUARK_REMOVE) {
	Object* obj = argv->get (0);
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long index = iobj->tolong ();
	  remove (index);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  remove (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with remove",
			 Object::repr (obj));
      }
      if (quark == QUARK_SETUNQ) {
	bool uniq = argv->getbool (0);
	setuniq (uniq);
	return nullptr;
      }
      if (quark == QUARK_SETZLN) {
	bool zlen = argv->getbool (0);
	setzlen (zlen);
	return nullptr;
      }
      if (quark == QUARK_CONCAT) {
	t_quad sc = argv->getchar (0);
	return new String (concat (sc));
      }
    }

    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	long index = argv->getlong (0);
	String   s = argv->getstring (1);
	set (index, s);
	return nullptr;
      }
    }
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new string vector iterator

  Strvecit::Strvecit (Strvec* vobj) {
    Object::iref (p_vobj = vobj);
    begin ();
  }

  // destroy this vector iterator

  Strvecit::~Strvecit (void) {
    Object::dref (p_vobj);
  }

  // return the class name

  String Strvecit::repr (void) const {
    return "Strvecit";
  }

  // reset the iterator to the begining

  void Strvecit::begin (void) {
    wrlock ();
    try {
      d_vidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Strvecit::end (void) {
    wrlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      if (p_vobj != nullptr) {
	d_vidx = (p_vobj->d_length == 0) ? 0 : p_vobj->d_length;
      } else {
	d_vidx = 0;
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the next object

  void Strvecit::next (void) {
    wrlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      if (p_vobj != nullptr) {
	if (++d_vidx >= p_vobj->d_length) d_vidx = p_vobj->d_length;
      } else {
	d_vidx = 0;
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the previous object

  void Strvecit::prev (void) {
    wrlock ();
    try {
      if (--d_vidx < 0) d_vidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Strvecit::getobj (void) const {
    rdlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      Object* result = nullptr;
      if ((p_vobj != nullptr) && (d_vidx < p_vobj->d_length)) {
	result = new String (p_vobj->get (d_vidx));
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool Strvecit::isend (void) const {
    rdlock ();
    if (p_vobj != nullptr) p_vobj->rdlock ();
    try {
      bool result = false;
      if (p_vobj != nullptr) {
	result = (d_vidx >= p_vobj->d_length);
      }
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_vobj != nullptr) p_vobj->unlock ();
      unlock ();
      throw;
    }
  }
}
