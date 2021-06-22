// ---------------------------------------------------------------------------
// - Strfifo.cpp                                                             -
// - standard object library - string fifo class implementation              -
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

#include "Prime.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Strfifo.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Exception.hpp"
#include "QuarkZone.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the hash bucket
  struct s_hbkt {
    // the string value
    t_quad* p_data;
    // the reference count
    long    d_rcnt;
    // next bucket
    s_hbkt* p_next;
    // previous bucket
    s_hbkt* p_prev;
    // create a bucket
    s_hbkt (const String& key) {
      p_data = key.toquad ();
      d_rcnt = 0;
      p_next = nullptr;
      p_prev = nullptr;
    }
    // delete the bucket chain
    ~s_hbkt (void) {
      delete [] p_data;
      delete p_next;
    }
    // find a bucket by key given its root bucket
    s_hbkt* find (const String& key) {
      s_hbkt* hbkt = this;
      // loop until we have a match
      while (hbkt != nullptr) {
	if (key.equals (hbkt->p_data) == true) return hbkt;
	hbkt = hbkt->p_next;
      }
      // no bucket found
      return nullptr;
    }
  };

  // the hash table
  struct s_htbl {
    // the hash table size
    long d_hsiz;
    // the hash table length
    long d_hlen;
    // the hash table threshold
    long d_hthr;
    // the hash lines
    s_hbkt** p_hlns;
    // create an empty hash table
    s_htbl (void) {
      d_hsiz = Prime::mkthrp (0);
      d_hlen = 0;
      d_hthr = (d_hsiz * 7) / 10;
      p_hlns = new s_hbkt*[d_hsiz];
      for (long i = 0; i < d_hsiz; i++) p_hlns[i] = nullptr;
    }
    // destroy this table
    ~s_htbl (void) {
      if (p_hlns != nullptr) {
	for (long i = 0; i < d_hsiz; i++) delete p_hlns[i];
	delete [] p_hlns;
      }
    }
    // get the hash table length
    long length (void) const {
      return d_hlen;
    }
    // find a bucket by key
    s_hbkt* find (const String& key) const {
      // check for nil
      if (p_hlns == nullptr) return nullptr;
      // get the hash id
      long hid = key.hashid () % d_hsiz;
      // look for master bucket
      s_hbkt* hbkt = p_hlns[hid];
      // localize the bucket by key
      return (hbkt == nullptr) ? nullptr : hbkt->find (key);
    }
    // return true if a key exists
    bool exists (const String& key) const {
      return (find (key) == nullptr) ?  false : true;
    }
    // add a key in the table by flag
    s_hbkt* add (const String& key) {
      // create a new bucket
      s_hbkt* hbkt = new s_hbkt (key);
      // get the hash id
      long hid = key.hashid () % d_hsiz;
      // set the bucket links
      hbkt->p_next = p_hlns[hid];
      if (p_hlns[hid] != nullptr) p_hlns[hid]->p_prev = hbkt;
      p_hlns[hid] = hbkt;
	// check for resize
      if (++d_hlen > d_hthr) resize (Prime::mkthrp (d_hsiz + 1));
      // here it is
      return hbkt;
    }
    // remove a bucket in the hash table
    void remove (s_hbkt* hbkt) {
      // check for nil
      if (hbkt == nullptr) return;
      // adjust reference count
      if (--hbkt->d_rcnt > 0) return;
      // unlink the bucket
      s_hbkt* prev = hbkt->p_prev;
      if (prev == nullptr) {
	// rebuild the hash id
	long hid = String::hashid (hbkt->p_data) % d_hsiz;
	// relink the root bucket
	p_hlns[hid] = hbkt->p_next;
	if (p_hlns[hid] != nullptr) p_hlns[hid]->p_prev = nullptr;
      } else {
	prev->p_next = hbkt->p_next;
	if (hbkt->p_next != nullptr) hbkt->p_next->p_prev = prev;
      }
      // clean the bucket
      hbkt->p_prev = nullptr;
      hbkt->p_next = nullptr;
      delete hbkt;
    }
    // resize this hash table
    void resize (const long size) {
      // check for the size
      if (size < d_hsiz) return;
      // initialize the new table
      s_hbkt** hlns = new s_hbkt*[size];
      for (long i = 0; i < size; i++) hlns[i] = nullptr;
      // rebuild the table
      for (long i = 0; i < d_hsiz; i++) {
        s_hbkt* hbkt = p_hlns[i];
        while (hbkt != nullptr) {
	  // isolate the bucket
          s_hbkt* next = hbkt->p_next;
          hbkt->p_next = nullptr;
          hbkt->p_prev = nullptr;
	  // recompute the hash id
          long hid = String::hashid (hbkt->p_data) % size;
	  // replace the bucket in the table
          hbkt->p_next = hlns[hid];
	  if (hlns[hid] != nullptr) hlns[hid]->p_prev = hbkt;
          hlns[hid] = hbkt;
	  // move to the next bucket
          hbkt = next;
        }
      }
      // clean the old table
      delete [] p_hlns;
      // restore the new table
      d_hsiz = size;
      d_hthr = (d_hsiz * 7) / 10;
      p_hlns = hlns;
    }
  };

  // the fifo structure
  struct s_fifo {
    // the hash bucket
    s_hbkt* p_hbkt;
    // the next element
    s_fifo* p_next;
    // create a default fifo element
    s_fifo (s_hbkt* hbkt) {
      p_hbkt = hbkt;
      p_next = nullptr;
    }
    // destroy the fifo
    ~s_fifo (void) {
      s_fifo* next = p_next;
      while (next != nullptr) {
	s_fifo* elem = next->p_next;
	next->p_next = nullptr;
	delete next;
	next = elem;
      }
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default string fifo

  Strfifo::Strfifo (void) {
    d_uniq = false;
    p_htbl = new s_htbl;
    p_fifo = nullptr;
  }

  // create a string fifo with a uniq flag

  Strfifo::Strfifo (const bool uniq) {
    d_uniq = uniq;
    p_htbl = new s_htbl;
    p_fifo = nullptr;
  }


  // destroy this string fifo
  
  Strfifo::~Strfifo (void) {
    reset ();
  }

  // return the class name

  String Strfifo::repr (void) const {
    return "Strfifo";
  }

  // return the string fifo did

  t_word Strfifo::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the string fifo sid

  t_word Strfifo::getsid (void) const {
    return SRL_STRF_SID;
  }

  // serialize this string fifo

  void Strfifo::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the vector length
      long len = length ();
      Serial::wrlong (len, os);
      // write the uniq flag
      Serial::wrbool (d_uniq, os);
      // write the strings
      s_fifo* elem = p_fifo;
      while (elem != nullptr) {
	String key = elem->p_hbkt->p_data;
	key.wrstream (os);
	elem = elem->p_next;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this string fifo

  void Strfifo::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the string vector
      reset  ();
      // get the vector length
      long len = Serial::rdlong (is);
      // get the uniq flag
      d_uniq = Serial::rdbool (is);
      // read in each object
      for (long i = 0; i < len; i++) {
	String key;
	key.rdstream (is);
	add (key);
      }
      unlock ();
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // reset this string fifo

  void Strfifo::reset (void) {
    wrlock ();
    try {
      // delete the elements
      delete p_fifo;
      delete p_htbl;
      // reset to nil
      p_htbl = new s_htbl;
      p_fifo = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the uniq flag

  void Strfifo::setuniq (const bool uniq) {
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

  bool Strfifo::getuniq (void) const {
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

  // return true if the fifo is empty

  bool Strfifo::empty (void) const {
    rdlock ();
    try {
      bool result = (p_fifo == nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the vector is active

  bool Strfifo::active (void) const {
    rdlock ();
    try {
      bool result = (p_fifo != nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check that a string exists in this string fifo

  bool Strfifo::exists (const String& key) const {
    rdlock ();
    try {
      bool result = p_htbl->exists (key);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of elements in this string fifo

  long Strfifo::length (void) const {
    rdlock ();
    try {
      // set result
      long result  = 0;
      s_fifo* elem = p_fifo;
      // loop in list
      while (elem != nullptr) {
	result++;
	elem = elem->p_next;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new element in this string fifo
  
  bool Strfifo::add (const String& key) {
    wrlock ();
    try {
      // find the bucket in the hash table
      s_hbkt* hbkt = p_htbl->find (key);
      // check for existence
      if (hbkt != nullptr) {
	// check for uniq
	if (d_uniq == true) {
	  unlock ();
	  return false;
	}
      } else {
	hbkt = p_htbl->add (key);
      }
      // update the reference count
      hbkt->d_rcnt++;
      // create the fifo record
      s_fifo* elem = new s_fifo (hbkt);
      // update the links
      elem->p_next = p_fifo;
      p_fifo = elem;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pop a string from the fifo

  String Strfifo::pop (void) {
    wrlock ();
    try {
      // check for an element
      if (p_fifo == nullptr) {
	throw Exception ("strfifo-error", "pop request with empty fifo");
      }
      // unlink the fifo element
      s_fifo* elem = p_fifo;
      p_fifo = elem->p_next;
      elem->p_next = nullptr;
      // get the hash bucket
      s_hbkt* hbkt = elem->p_hbkt;
      // get the string result
      String result = hbkt->p_data;
      // remove the bucket
      p_htbl->remove (hbkt);
      // clean the element
      delete elem;
      // unlock and return
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
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_POP    = zone.intern ("pop");
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_EXISTP = zone.intern ("exists-p");
  static const long QUARK_EMPTYP = zone.intern ("empty-p");
  static const long QUARK_ACTIVP = zone.intern ("active-p");
  static const long QUARK_SETUNQ = zone.intern ("set-unique");
  static const long QUARK_GETUNQ = zone.intern ("get-unique");

  // create a new object in a generic way

  Object* Strfifo::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new Strfifo;
    // check 1 argument
    if (argc == 1) {
      bool uniq = argv->getbool (0);
      return new Strfifo (uniq);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many argument for string fifo");

  }

  // return true if the given quark is defined

  bool Strfifo::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Strfifo::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_POP)    return new String  (pop     ());
      if (quark == QUARK_LENGTH) return new Integer (length  ());
      if (quark == QUARK_EMPTYP) return new Boolean (empty   ());
      if (quark == QUARK_ACTIVP) return new Boolean (active  ());
      if (quark == QUARK_GETUNQ) return new Boolean (getuniq ());
      if (quark == QUARK_RESET) {
	reset  ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
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
      if (quark == QUARK_SETUNQ) {
	bool uniq = argv->getbool (0);
	setuniq (uniq);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
