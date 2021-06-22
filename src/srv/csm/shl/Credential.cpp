// ---------------------------------------------------------------------------
// - Credential.cpp                                                          -
// - afnix:csm service - credential class implementation                     -
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

#include "Item.hpp"
#include "Vector.hpp"
#include "Csmsid.hxx"
#include "Crypto.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Credential.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the credential structure
  struct s_cred {
    // the credential type
    Credential::t_cred d_type;
    /// the credential value
    String d_cval;
    // create a credential
    s_cred (void) {
      d_type = Credential::CRED_NONE;
    }
    // copy construct this credential
    s_cred (const s_cred& that) {
      d_type = that.d_type;
      d_cval = that.d_cval;
    }
    ~s_cred (void) {
    }
    // assign a credential to this one
    s_cred& operator = (const s_cred& that) {
      // check for self-assignation
      if (this == &that) return *this;
      // assign locally
      d_type = that.d_type;
      d_cval = that.d_cval;
      return *this;
    }
    // set a credential by type and string
    void set (const Credential::t_cred type, const String& cval,
	      const bool nopf) {
      switch (type) {
      case Credential::CRED_PASS:
	d_type = type;
	d_cval = nopf ? cval : Crypto().hash (cval);
	break;
      default:
	d_type = type;
	d_cval = "";
	break;
      }
    }
    // get a credential object
    String getcval (void) const {
      return d_cval;
    }
    // clone an array of credentials
    static s_cred* clone (const long size, const s_cred* cred) {
      // check for null
      if (size <= 0L) return nullptr;
      // allocate and copy
      s_cred* result = new s_cred[size];
      for (long k = 0L; k < size; k++) result[k] = cred[k];
      return result;
    }
    // extend an array of credentials
    static s_cred* extend (const long size, const s_cred* cred) {
      // allocate and copy
      s_cred* result = new s_cred[size+1L];
      for (long k = 0L; k < size; k++) result[k] = cred[k];
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default random credential

  Credential::Credential (void) {
    d_size = 0L;
    p_cred = nullptr;
  }

  // create a credential by pass string

  Credential::Credential (const String& ppse) {
    d_size = 0L;
    p_cred = nullptr;
    add (CRED_PASS, ppse);
  }
  
  // copy construct this credential

  Credential::Credential (const Credential& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      p_cred = s_cred::clone (d_size, that.p_cred);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // move construct this credential

  Credential::Credential (Credential&& that) {
    that.wrlock ();
    try {
      d_size = that.d_size; that.d_size = 0L;
      p_cred = that.p_cred; that.p_cred = nullptr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this credential

  Credential::~Credential (void) {
    delete [] p_cred;
  }

  // assign a credential to this one

  Credential& Credential::operator = (const Credential& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // delete old credential
      delete [] p_cred;
      // copy locally
      d_size = that.d_size;
      p_cred = s_cred::clone (d_size, that.p_cred);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a credential to this one

  Credential& Credential::operator = (Credential&& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // delete old credential
      delete [] p_cred;
      // move locally
      d_size = that.d_size; that.d_size = 0L;
      p_cred = that.p_cred; that.p_cred = nullptr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Credential::repr (void) const {
    return "Credential";
  }

  // return a clone of this object

  Object* Credential::clone (void) const {
    return new Credential (*this);
  }

  // return the serial did

  t_word Credential::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Credential::getsid (void) const {
    return SRL_CRED_SID;
  }
  
  // serialize this credential

  void Credential::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_size, os);
      for (long k = 0L; k < d_size; k++) {
	os.write ((char) p_cred[k].d_type);
	p_cred[k].d_cval.wrstream  (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this credential

  void Credential::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_size = Serial::rdlong (is);
      if (d_size > 0L) p_cred = new s_cred[d_size];
      for (long k = 0L; k < d_size; k++) {
	t_byte bval = is.read ();
	switch (bval) {
	case 0x00U:
	  p_cred[k].d_type = CRED_NONE;
	  break;
	case 0x01U:
	  p_cred[k].d_type = CRED_PASS;
	  break;
	default:
	  throw Exception ("credential-error", "invalid type serial code");
	  break;
	}
	p_cred[k].d_cval.rdstream (is);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this credential

  void Credential::reset (void) {
    wrlock ();
    try {
      d_size = 0L;
      delete p_cred; p_cred = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the number of credential

  long Credential::length (void) const {
    rdlock ();
    try {
      long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a credential by type and string

  void Credential::add (const t_cred type, const String& cval) {
    wrlock ();
    try {
      // add in process mode
      add (type, cval, false);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }  
  // add a credential by type and string

  void Credential::add (const t_cred type,const String& cval,const bool nopf) {
    wrlock ();
    try {
      // extend old credential
      s_cred* cred = s_cred::extend (d_size, p_cred);
      // add the credential
      cred[d_size].set (type, cval, nopf);
      // update the array
      delete [] p_cred;
      p_cred = cred;
      d_size++;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a credential type by index

  Credential::t_cred Credential::gettype (const long cidx) const {
    rdlock ();
    try {
      // check for valid index
      if ((cidx < 0L) || (cidx >= d_size)) {
	throw Exception ("credential-error", "invalid credential index");
      }
      // get the type
      t_cred result = p_cred[cidx].d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a credential value by index

  String Credential::getcval (const long cidx) const {
    rdlock ();
    try {
      // check for valid index
      if ((cidx < 0L) || (cidx >= d_size)) {
	throw Exception ("credential-error", "invalid credential index");
      }
      // get the type
      String result = p_cred[cidx].getcval ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // validate a credential by type and value

  bool Credential::validate (const t_cred type, const String& cval) const {
    rdlock ();
    try {
      // do not validate by default
      bool result = false;
      for (long k = 0L; k < d_size; k++) {
	if ((p_cred[k].d_type == type) && (p_cred[k].d_cval == cval)) {
	  result = true;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a credential for acceptance

  bool Credential::validate (const Credential& cred) const {
    rdlock ();
    try {
      // check for size
      if (cred.d_size < d_size) {
	unlock ();
	return false;
      }
      // validate each credential
      for (long k = 0L; k < d_size; k++) {
	if (cred.validate (p_cred[k].d_type, p_cred[k].d_cval) == false) {
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
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_CRED     = String::intern ("Credential");
  static const long QUARK_CREDNONE = String::intern ("NONE");
  static const long QUARK_CREDPASS = String::intern ("PASS");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD      = zone.intern ("add");
  static const long QUARK_LENGTH   = zone.intern ("length");
  static const long QUARK_VALIDP   = zone.intern ("valid-p");
  static const long QUARK_GETTYPE  = zone.intern ("get-type");
  static const long QUARK_GETCVAL  = zone.intern ("get-value");
  
  // map an item to a credential type
  static inline Credential::t_cred item_to_cred (const Item& item) {
    // check for a credential item
    if (item.gettid () != QUARK_CRED)
      throw Exception ("item-error", "item is not a credential item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_CREDNONE) return Credential::CRED_NONE;
    if (quark == QUARK_CREDPASS) return Credential::CRED_PASS;
    throw Exception ("item-error", "cannot map item to credential type");
  }
  
  // map a credential type to an item
  static inline Item* cred_to_item (const Credential::t_cred type) {
    switch (type) {
    case Credential::CRED_NONE:
      return new Item (QUARK_CRED, QUARK_CREDNONE);
      break;
    case Credential::CRED_PASS:
      return new Item (QUARK_CRED, QUARK_CREDPASS);
      break;
    }
    return nullptr;
  }

  // evaluate an object data member

  Object* Credential::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    // credential type part
    if (quark == QUARK_CREDNONE)
      return new Item (QUARK_CRED, QUARK_CREDNONE);
    if (quark == QUARK_CREDPASS)
      return new Item (QUARK_CRED, QUARK_CREDPASS);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
  
  // create a new object in a generic way
  
  Object* Credential::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Credential;
    // check for 1 argument
    if (argc == 1) {
      String ppse = argv->getstring (0);
      return new Credential (ppse);
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with credential");
  }
  
  // return true if the given quark is defined

  bool Credential::isquark (const long quark, const bool hflg) const {
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
  
  Object* Credential::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)   return new Integer (length ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETTYPE) {
	long cidx = argv->getlong (0);
	return cred_to_item (gettype (cidx));
      }
      if (quark == QUARK_GETCVAL) {
	long cidx = argv->getlong (0);
	return new String (getcval (cidx));
      }
      if (quark == QUARK_VALIDP) {
	// check for a credential object
	Object* obj = argv->get (0);
	Credential* cred = dynamic_cast <Credential*> (obj);
	if (cred == nullptr) {
	  throw Exception ("type-error", "invalid object with valid-p",
			   Object::repr (obj));
	}
	return new Boolean (validate (*cred));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	// check for an item type
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with add",
			   Object::repr (obj));
	}
	t_cred type = item_to_cred (*iobj);
	// get the string value
	String cval = argv->getstring (1);
	add (type, cval);
	return nullptr;
      }
      if (quark == QUARK_VALIDP) {
	// check for an item type
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with valid-p",
			   Object::repr (obj));
	}
	t_cred type = item_to_cred (*iobj);
	// get the string value
	String cval = argv->getstring (1);
	return new Boolean (validate (type, cval));
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
