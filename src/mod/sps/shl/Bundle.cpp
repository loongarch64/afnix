// ---------------------------------------------------------------------------
// - Bundle.cpp                                                              -
// - afnix:sps module - literal bundle class implementation                  -
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

#include "Real.hpp"
#include "Bundle.hpp"
#include "Vector.hpp"
#include "Spssid.hxx"
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

  // the literal bundle type serial code
  static const t_byte BNDL_BOOL_TYPE = 0x00U;
  static const t_byte BNDL_INTG_TYPE = 0x01U;
  static const t_byte BNDL_REAL_TYPE = 0x02U;
  static const t_byte BNDL_OTHR_TYPE = 0x03U;

  // map a literal bundle as a serial byte
  static inline t_byte bndl_to_byte (const Bundle::t_bndl type) {
    t_byte result = 0x00U;
    switch (type) {
    case Bundle::BNDL_BOOL:
      result = BNDL_BOOL_TYPE;
      break;
    case Bundle::BNDL_INTG:
      result = BNDL_INTG_TYPE;
      break;
    case Bundle::BNDL_REAL:
      result = BNDL_REAL_TYPE;
      break;
    case Bundle::BNDL_OTHR:
      result = BNDL_OTHR_TYPE;
      break;
    }
    return result;
  }

  // map a serial byte to a literal bundle type
  static inline Bundle::t_bndl bndl_to_type (const t_byte code) {
    Bundle::t_bndl result = Bundle::BNDL_OTHR;
    switch (code) {
    case BNDL_BOOL_TYPE:
      result = Bundle::BNDL_BOOL;
      break;
    case BNDL_INTG_TYPE:
      result = Bundle::BNDL_INTG;
      break;
    case BNDL_REAL_TYPE:
      result = Bundle::BNDL_REAL;
      break;
    case BNDL_OTHR_TYPE:
      result = Bundle::BNDL_BOOL;
      break;
    default:
      throw Exception ("bundle-error", "cannot convert serial code to type");
      break;
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty literal bundle

  Bundle::Bundle (void) {
    d_size = 0L;
    d_slen = 0L;
    d_lidx = -1L;
    d_type = BNDL_OTHR;
    p_lstk = nullptr;
  }

  // create a literal bundle by size

  Bundle::Bundle (const long size) {
    if (size < 0L) {
      throw Exception ("size-error","negative literal bundle size");
    }
    d_size = size;
    d_slen = 0;
    d_lidx = -1L;
    d_type = BNDL_OTHR;
    p_lstk = new Literal*[d_size];
    for (long k = 0; k < d_size; k++) p_lstk[k] = nullptr;
  }

  // copy constructor for this literal bundle

  Bundle::Bundle (const Bundle& that) {
    that.rdlock ();
    try {
      // copy arguments
      d_size = that.d_slen;
      d_slen = that.d_slen;
      d_lidx = that.d_lidx;
      d_type = that.d_type;
      switch (d_type) {
      case BNDL_BOOL:
	p_bstk = nullptr;
	if (d_slen > 0L) {
	  p_bstk = new bool[d_size];
	  for (long k = 0L; k < d_slen; k++) p_bstk[k] = that.p_bstk[k];
	}
	break;
      case BNDL_INTG:
	p_istk = nullptr;
	if (d_slen > 0L) {
	  p_istk = new long[d_size];
	  for (long k = 0L; k < d_slen; k++) p_istk[k] = that.p_istk[k];
	}
	break;
      case BNDL_REAL:
	p_rstk = nullptr;
	if (d_slen > 0L) {
	  p_rstk = new t_real[d_size];
	  for (long k = 0L; k < d_slen; k++) p_rstk[k] = that.p_rstk[k];
	}
	break;
      case BNDL_OTHR:
	p_lstk = nullptr;
	if (d_slen > 0L) {
	  p_lstk = new Literal*[d_size];
	  for (long k = 0L; k < d_slen; k++) {
	    Literal* lobj = that.p_lstk[k] == nullptr ? nullptr :
	      dynamic_cast <Literal*> (that.p_lstk[k]->clone());
	    Object::iref (p_lstk[k] = lobj);
	  }
	}
	break;	
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this literal bundle
  
  Bundle::~Bundle (void) {
    clear ();
  }

  // return the class name

  String Bundle::repr (void) const {
    return "Bundle";
  }

  // return a clone of this object

  Object* Bundle::clone (void) const {
    return new Bundle (*this);
  }

  // clear this literal bundle

  void Bundle::clear (void) {
    wrlock ();
    try {
      // clean the literals first
      if ((d_type == BNDL_OTHR) && (d_slen > 0)) {
	for (long k = 0; k < d_slen; k++) Object::dref (p_lstk[k]);
      }
      // clean the array
      switch (d_type) {
      case BNDL_BOOL:
	delete [] p_lstk;
	p_lstk = nullptr;
	break;
      case BNDL_INTG:
	delete [] p_istk;
	p_istk = nullptr;
	break;
      case BNDL_REAL:
	delete [] p_rstk;
	p_rstk = nullptr;
	break;
      case BNDL_OTHR:
	delete [] p_lstk;
	p_lstk = nullptr;
	break;
      }
      d_size = 0L;
      d_slen = 0L;
      d_lidx = -1L;
      d_type = BNDL_OTHR;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this bundle

  String Bundle::toliteral (void) const {
    rdlock ();
    try {
      String result =  "";
      if (d_slen == 0) {
	unlock ();
	return result;
      }
      long lidx = (d_lidx == -1L) ? d_slen - 1L : d_lidx;
      if (d_type == BNDL_BOOL) {
	Boolean bval = p_bstk[lidx];
	result = bval.toliteral ();
      }
      if (d_type == BNDL_INTG) {
	Integer ival = p_istk[lidx];
	result = ival.toliteral ();
      }
      if (d_type == BNDL_REAL) {
	Real rval = p_rstk[lidx];
	result = rval.toliteral ();
      }
      if (d_type == BNDL_OTHR) {
	Literal* lobj = p_lstk[lidx];
	if (lobj != nullptr) result = lobj->toliteral ();
      }	  
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this bundle

  String Bundle::tostring (void) const {
    rdlock ();
    try {
      String result =  "";
      if (d_slen == 0) {
	unlock ();
	return result;
      }
      long lidx = (d_lidx == -1L) ? d_slen - 1L : d_lidx;
      if (d_type == BNDL_BOOL) {
	Boolean bval = p_bstk[lidx];
	result = bval.tostring ();
      }
      if (d_type == BNDL_INTG) {
	Integer ival = p_istk[lidx];
	result = ival.tostring ();
      }
      if (d_type == BNDL_REAL) {
	Real rval = p_rstk[lidx];
	result = rval.tostring ();
      }
      if (d_type == BNDL_OTHR) {
	Literal* lobj = p_lstk[lidx];
	if (lobj != nullptr) result = lobj->tostring ();
      }	  
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a styled representation of this bundle

  String Bundle::format (const Style& lstl) const {
    rdlock ();
    try {
      String result =  "";
      if (d_slen == 0) {
	unlock ();
	return result;
      }
      long lidx = (d_lidx == -1L) ? d_slen - 1L : d_lidx;
      if (d_type == BNDL_INTG) {
	long ival = p_istk[lidx];
	result = lstl.format (ival);
      }
      if (d_type == BNDL_REAL) {
	t_real rval = p_rstk[lidx];
	result = lstl.format (rval);
      } else {
	result = tostring ();
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the serial did

  t_word Bundle::getdid (void) const {
    return SRL_DEOD_SPS;
  }

  // return the serial sid

  t_word Bundle::getsid (void) const {
    return SRL_BNDL_SID;
  }
  
  // serialize this literal bundle

  void Bundle::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the bundle length
      Serial::wrlong (d_slen, os);
      // write the bundle index
      Serial::wrlong (d_lidx, os);
      // write the bundle code
      os.write ((char) bndl_to_byte (d_type));
      // map the bundle data
      if (d_type == BNDL_BOOL) Serial::wrbool (d_slen, p_bstk, os);
      if (d_type == BNDL_INTG) Serial::wrlong (d_slen, p_istk, os);
      if (d_type == BNDL_REAL) Serial::wrreal (d_slen, p_rstk, os);
      if ((d_slen > 0) && (d_type == BNDL_OTHR)) {
	for (long k = 0; k < d_slen; k++) {
	  Literal* lobj = p_lstk[k];
	  if (lobj == nullptr) {
	    Serial::wrnilid (os);
	  } else {
	    lobj->serialize (os);
	  }
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this literal bundle

  void Bundle::rdstream (InputStream& is) {
    wrlock ();
    try {
      // clear the bundle 
      clear ();
      // get the bundle length
      d_size = d_slen = Serial::rdlong (is);
      // get the bundle index
      d_lidx = Serial::rdlong (is);
      // get the bundle type
      d_type = bndl_to_type ((t_byte) is.read ());
      // deserialize data block
      if (d_type == BNDL_BOOL) {
	p_bstk = (d_slen == 0L) ? nullptr : Serial::rdbool (is, d_size);
      }
      if (d_type == BNDL_INTG) {
	p_istk = (d_slen == 0L) ? nullptr : Serial::rdlong (is, d_size);
      }
      if (d_type == BNDL_REAL) {
	p_rstk = (d_slen == 0L) ? nullptr : Serial::rdreal (is, d_size);
      }
      if (d_type == BNDL_OTHR) {
	p_lstk = new Literal*[d_size];
	for (long k = 0; k < d_slen; k++) {
	  Object*  sobj = Serial::deserialize (is);
	  Literal* lobj = dynamic_cast <Literal*> (sobj);
	  if ((sobj != nullptr) && (lobj == nullptr)) {
	    throw Exception ("labundle-error", "invalid deserialized object");
	    Object::iref (p_lstk[k] = lobj);
	  }
	}
      }
      unlock ();
    } catch (...) {
      clear  ();
      unlock ();
      throw;
    }
  }

  // assign a literal bundle  to this one
  
  Bundle& Bundle::operator = (const Bundle& that) {
    // check againt equal equal
    if (this == &that) return *this;
    // lock everything
    wrlock ();
    that.rdlock ();
    try {
      // clear the bundle
      clear ();
      // copy arguments
      d_size = that.d_size;
      d_slen = that.d_slen;
      d_lidx = that.d_lidx;
      d_type = that.d_type;
      switch (d_type) {
      case BNDL_BOOL:
	p_bstk = nullptr;
	if (d_slen > 0L) {
	  p_bstk = new bool[d_size];
	  for (long k = 0L; k < d_slen; k++) p_bstk[k] = that.p_bstk[k];
	}
	break;
      case BNDL_INTG:
	p_istk = nullptr;
	if (d_slen > 0L) {
	  p_istk = new long[d_size];
	  for (long k = 0L; k < d_slen; k++) p_istk[k] = that.p_istk[k];
	}
	break;
      case BNDL_REAL:
	p_rstk = nullptr;
	if (d_slen > 0L) {
	  p_rstk = new t_real[d_size];
	  for (long k = 0L; k < d_slen; k++) p_rstk[k] = that.p_rstk[k];
	}
	break;
      case BNDL_OTHR:
	p_lstk = nullptr;
	if (d_slen > 0L) {
	  p_lstk = new Literal*[d_size];
	  for (long k = 0L; k < d_slen; k++) {
	    Literal* lobj = that.p_lstk[k] == nullptr ? nullptr :
	      dynamic_cast <Literal*> (that.p_lstk[k]->clone());
	    Object::iref (p_lstk[k] = lobj);
	  }
	}
	break;	
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

  // return true if the literal bundle is empty

  bool Bundle::empty (void) const {
    rdlock ();
    try {
      bool result = (d_slen == 0L);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of element in this literal bundle

  long Bundle::length (void) const {
    rdlock ();
    try {
      long result = d_slen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize the literal bundle

  void Bundle::resize (const long size) {
    wrlock ();
    try {
      // check for valid size
      if (size < 0L) {
	throw Exception ("bundle-error", "invalid negative size in resize");
      }
      // check for null size
      if (d_size == 0L) {
	d_size = (size == 0L) ? 256L : size;
	d_slen = 0L;
	d_lidx = -1L;
	switch (d_type) {
	case BNDL_BOOL:
	  p_bstk = new bool[d_size];
	  for (long k = 0L; k < d_size; k++) p_bstk[k] = false;
	  break;
	case BNDL_INTG:
	  p_istk = new long[d_size];
	  for (long k = 0L; k < d_size; k++) p_istk[k] = 0L;
	  break;
	case BNDL_REAL:
	  p_rstk = new t_real[d_size];
	  for (long k = 0L; k < d_size; k++) p_rstk[k] = 0.0;
	  break;
	case BNDL_OTHR:
	  p_lstk = new Literal*[d_size];
	  for (long k = 0L; k < d_size; k++) p_lstk[k] = nullptr;
	  break;
	}
	unlock ();
	return;
      }
      if (size <= d_size) {
	unlock ();
	return;
      }
      if (d_type == BNDL_BOOL) {
	bool* bstk = new bool[size];
	for (long k = 0;      k < d_slen; k++) bstk[k] = p_bstk[k];
	for (long k = d_slen; k < size;   k++) bstk[k] = false;
	d_size = size;
	delete [] p_bstk; p_bstk = bstk;
      }
      if (d_type == BNDL_INTG) {
	long* istk = new long[size];
	for (long k = 0;      k < d_slen; k++) istk[k] = p_istk[k];
	for (long k = d_slen; k < size;   k++) istk[k] = 0L;
	d_size = size;
	delete [] p_istk; p_istk = istk;
      }
      if (d_type == BNDL_REAL) {
	t_real* rstk = new t_real[size];
	for (long k = 0;      k < d_slen; k++) rstk[k] = p_rstk[k];
	for (long k = d_slen; k < size;   k++) rstk[k] = 0.0;
	d_size = size;
	delete [] p_rstk; p_rstk = rstk;
      }
      if (d_type == BNDL_OTHR) {
	Literal** lstk = new Literal*[size];
	for (long k = 0;      k < d_slen; k++) lstk[k] = p_lstk[k];
	for (long k = d_slen; k < size;   k++) lstk[k] = nullptr;
	d_size = size;
	delete [] p_lstk; p_lstk = lstk;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the literal index

  void Bundle::setlidx (const long lidx) {
    wrlock ();
    try {
      if ((lidx < 0L) || (lidx >= d_slen)) {
	d_lidx = -1L;
      } else {
	d_lidx = lidx;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the literal index

  long Bundle::getlidx (void) const {
    rdlock ();
    try {
      long result = (d_lidx == -1L) ? (d_slen - 1L) : d_lidx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a boolean in this bundle
  
  void Bundle::add (const bool bval) {
    wrlock ();
    try {
      // retype if needed
      if (d_slen == 0L) d_type = BNDL_BOOL;
      // check for valid type
      if (d_type != BNDL_BOOL) {
	throw Exception ("bundle-error", "invalid boolean type to add");
      }
      // check if we have to resize the bundle
      if (d_slen + 1L >= d_size) resize (2 * d_size);
      p_bstk[d_slen++] = bval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a boolean in this bundle
  
  void Bundle::add (const long ival) {
    wrlock ();
    try {
      // retype if needed
      if (d_slen == 0L) d_type = BNDL_INTG;
      // check for valid type
      if (d_type != BNDL_INTG) {
	throw Exception ("bundle-error", "invalid integer type to add");
      }
      // check if we have to resize the bundle
      if (d_slen + 1L >= d_size) resize (2 * d_size);
      p_istk[d_slen++] = ival;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a real in this bundle
  
  void Bundle::add (const t_real rval) {
    wrlock ();
    try {
      // retype if needed
      if (d_slen == 0L) d_type = BNDL_REAL;
      // check for valid type
      if (d_type != BNDL_REAL) {
	throw Exception ("bundle-error", "invalid real type to add");
      }
      // check if we have to resize the bundle
      if (d_slen + 1L >= d_size) resize (2 * d_size);
      p_rstk[d_slen++] = rval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a literal in this bundle
  
  void Bundle::add (Literal* lval) {
    wrlock ();
    try {
      // check for boolean
      Boolean* bobj = dynamic_cast <Boolean*> (lval);
      if ((bobj != nullptr) && ((d_type == BNDL_BOOL) || (d_slen == 0L))) {
	add (bobj->tobool ());
	unlock ();
	return;
      }	  
      // check for integer
      Integer* iobj = dynamic_cast <Integer*> (lval);
      if ((iobj != nullptr) && ((d_type == BNDL_INTG) || (d_slen == 0L))) {
	add ((long) iobj->tolong ());
	unlock ();
	return;
      }
      // check for real
      Real* zobj = dynamic_cast <Real*> (lval);
      if ((zobj != nullptr) && ((d_type == BNDL_REAL) || (d_slen == 0L))) {
	add (zobj->toreal ());
	unlock ();
	return;
      }
      // check for other
      if ((d_type == BNDL_OTHR) && 
	  ((bobj == nullptr) && (iobj == nullptr) && (zobj == nullptr))) {
	// check if we have to resize the bundle
	if (d_slen + 1L >= d_size) resize (2 * d_size);
	// add the literal
	Literal* lobj = (lval == nullptr) ? nullptr :
	  dynamic_cast <Literal*> (lval->clone ());
	Object::iref (p_lstk[d_slen++] = lobj);
      } else {
	// check inconsistent add
	throw Exception ("bundle-error", "invalid object to add",
			 Object::repr (lval));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a literal at a certain position

  void Bundle::set (const long index, Literal* lval) {
    wrlock ();
    try {
      // check that the index is valid
      if (index >= d_slen) { 
	throw Exception ("index-error","in literal bundle set");
      }
      // select valid literal
      Boolean* bobj = dynamic_cast <Boolean*> (lval);
      if ((bobj != nullptr) && (d_type == BNDL_BOOL)) {
	p_bstk[index] = bobj->tobool ();
	unlock ();
	return;
      }
      Integer* iobj = dynamic_cast <Integer*> (lval);
      if ((iobj != nullptr) && (d_type == BNDL_INTG)) {
	p_istk[index] = iobj->tolong ();
	unlock ();
	return;
      }
      Real* zobj = dynamic_cast <Real*> (lval);
      if ((zobj != nullptr) && (d_type == BNDL_REAL)) {
	p_rstk[index] = zobj->toreal ();
	unlock ();
	return;
      }
      if ((d_type == BNDL_OTHR) && 
	  (bobj == nullptr) && (iobj == nullptr) && (zobj == nullptr)) {
	Literal* lobj = (lval == nullptr) ? nullptr :
	  dynamic_cast<Literal*> (lval->clone ());
	Object::iref (lobj);
	Object::dref (p_lstk[index]);
	p_lstk[index] = lobj;
      } else {
	throw Exception ("bundle-error", "invalid object type to set",
			 Object::repr (lval));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a literal at a certain position

  Literal* Bundle::get (const long index) const {
    rdlock ();
    try {
      // check that we are bounded
      if ((index < 0) || (index >= d_slen)) { 
	throw Exception ("index-error","in literal bundle get");
      }
      Literal* result = nullptr;
      switch (d_type) {
      case BNDL_BOOL:
	result = (p_bstk == nullptr) ? nullptr : new Boolean (p_bstk[index]);
	break;
      case BNDL_INTG:
	result = (p_istk == nullptr) ? nullptr : new Integer (p_istk[index]);
	break;
      case BNDL_REAL:
	result = (p_rstk == nullptr) ? nullptr : new Real (p_rstk[index]);
	break;
      case BNDL_OTHR:
	result = (p_lstk[index] == nullptr) ? nullptr :
	dynamic_cast <Literal*> (p_lstk[index]->clone ());
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the first literal in this bundle

  Literal* Bundle::first (void) const {
    rdlock ();
    try {
      Literal* result = get (0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the last literal in this bundle

  Literal* Bundle::last (void) const {
    rdlock ();
    try {
      Literal* result = get (d_slen-1L);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a new bundle iterator

  Iterator* Bundle::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Bundleit (this);
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
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_LAST    = zone.intern ("last");
  static const long QUARK_FIRST   = zone.intern ("first");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_GETLIDX = zone.intern ("get-literal-index");
  static const long QUARK_SETLIDX = zone.intern ("set-literal-index");

  // create a new object in a generic way

  Object* Bundle::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new Bundle;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj !=nullptr) {
        long size = iobj->tolong ();
        return new Bundle (size);
      }
      throw Exception ("type-error", "invalid object with bundle",
                       Object::repr (obj));
    }
    throw Exception ("argument-error", "too many argument for bundle");
    
  }

  // return true if the given quark is defined

  bool Bundle::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Literal::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Bundle::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LAST)    return last ();
      if (quark == QUARK_FIRST)   return first ();
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_EMPTYP)  return new Boolean (empty   ());
      if (quark == QUARK_GETLIDX) return new Integer (getlidx ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	long index = argv->getlong (0);
	return get (index);
      }
      if (quark == QUARK_ADD) {
	Object*   obj = argv->get (0);
	Literal* lval = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lval == nullptr)) {
	  throw Exception ("type-error", "invalid object to add",
			   Object::repr (obj));
	}
	add (lval);
	return nullptr;
      }
      if (quark == QUARK_SETLIDX) {
	long lidx = argv->getlong (0);
	setlidx (lidx);
	return nullptr;
      }
    }

    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	long index = argv->getlong (0);
	Object* obj = argv->get (1);
	Literal* lval = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lval == nullptr)) {
	  throw Exception ("type-error", "invalid object wih set",
			   Object::repr (obj));
	}
	set (index, lval);
	return nullptr;
      }
    }
    // check the serial method
    if (Serial::isquark (quark, true) == true) {
      return Serial::apply (zobj, nset, quark, argv);
    }
    // default to literal method
    return Literal::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new literal bundle iterator

  Bundleit::Bundleit (Bundle* sobj) {
    Object::iref (p_sobj = sobj);
    begin ();
  }

  // destroy this literal bundle iterator

  Bundleit::~Bundleit (void) {
    Object::dref (p_sobj);
  }

  // return the class name

  String Bundleit::repr (void) const {
    return "Bundleit";
  }

  // reset the iterator to the begining

  void Bundleit::begin (void) {
    wrlock ();
    try {
      d_sidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Bundleit::end (void) {
    wrlock ();
    if (p_sobj != nullptr) p_sobj->rdlock ();
    try {
      if (p_sobj != nullptr) {
	d_sidx = (p_sobj->d_slen == 0) ? 0L : p_sobj->d_slen - 1L;
      } else {
	d_sidx = 0;
      }
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
    } catch (...) {
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the next object

  void Bundleit::next (void) {
    wrlock ();
    if (p_sobj != nullptr) p_sobj->rdlock ();
    try {
      if (p_sobj != nullptr) {
	if (++d_sidx >= p_sobj->d_slen) d_sidx = p_sobj->d_slen;
      } else {
	d_sidx = 0;
      }
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
    } catch (...) {
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the previous object

  void Bundleit::prev (void) {
    wrlock ();
    try {
      if (--d_sidx < 0) d_sidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Bundleit::getobj (void) const {
    rdlock ();
    if (p_sobj != nullptr) p_sobj->rdlock ();
    try {
      Object* result = nullptr;
      if ((p_sobj != nullptr) && (d_sidx < p_sobj->d_slen)) {
	result = p_sobj->get (d_sidx);
      }
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool Bundleit::isend (void) const {
    rdlock ();
    if (p_sobj != nullptr) p_sobj->rdlock ();
    try {
      bool result = false;
      if (p_sobj != nullptr) {
	result = (d_sidx >= p_sobj->d_slen);
      }
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_sobj != nullptr) p_sobj->unlock ();
      unlock ();
      throw;
    }
  }
}
