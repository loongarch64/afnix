// ---------------------------------------------------------------------------
// - Signature.cpp                                                           -
// - afnix:sec module - message signature class implementation               -
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
#include "Ascii.hpp"
#include "Secsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Relatif.hpp"
#include "Unicode.hpp"
#include "Signature.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure extract a relatif from an object
  static inline Relatif sign_obj_rel (Object* obj) {
    // check for nil
    if (obj == nullptr) {
      throw Exception ("type-error", "invalid nil object as relatif");
    }
    // check for an integer
    Integer* iobj = dynamic_cast <Integer*> (obj);
    if (iobj != nullptr) return iobj->tolong ();
    // check fo a relatif
    Relatif* zobj = dynamic_cast <Relatif*> (obj);
    if (zobj != nullptr) return *zobj;
    // invalid object
    throw Exception ("type-error", "invalid object as a relatif",
		     Object::repr (obj));    
  }
  
  // the dsa signature
  struct s_sdsa {
    // the dsa r value
    Relatif d_r;
    // the dsa s value
    Relatif d_s;
    // create a default signature
    s_sdsa (void) {
      d_r = 0;
      d_s = 0;
    }
    // create a dsa signature by value
    s_sdsa (const Relatif& r, const Relatif& s) {
      d_r = r;
      d_s = s;
    }
    // copy construct this dsa signature
    s_sdsa (const s_sdsa& that) {
      d_r = that.d_r;
      d_s = that.d_s;
    }
    // serialize the signature
    void wrstream (OutputStream& os) {
      d_r.wrstream (os);
      d_s.wrstream (os);
    }
    // deserialize the stream
    void rdstream (InputStream& is) {
      d_r.rdstream (is);
      d_s.rdstream (is);
    }
    // load a signature by a number vector
    void ldnvec (const Vector& nvec) {
      // check vector length
      long vlen = nvec.length ();
      if (vlen != 2) {
	throw Exception ("signat'ure-error",
			 "invalid dsa signature vector size");
      }
      // set base elements
      d_r = sign_obj_rel (nvec.get (0));
      d_s = sign_obj_rel (nvec.get (1));
    }
    // return a formatted component by type
    String format (const Signature::t_isgn type) const {
      if (type == Signature::SDSA_SCMP) return d_s.tohexa ();
      if (type == Signature::SDSA_RCMP) return d_r.tohexa ();
      throw Exception ("signature-error", "invalid dsa component accessor");
    }
    // return a relatif component by type
    Relatif getscmp (const Signature::t_isgn type) const {
      if (type == Signature::SDSA_SCMP) return d_s;
      if (type == Signature::SDSA_RCMP) return d_r;
      throw Exception ("signature-error", "invalid dsa component accessor");
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default signature

  Signature::Signature (void) {
    d_type = SIGN_SNIL;
    p_snil = nullptr;
  }

  // create a signature by type and object vector
  
  Signature::Signature (const t_sign type, const Vector& ovec) {
    // check for nil key
    if (type == SIGN_SNIL) {
      throw Exception ("type-error",
		       "invalid signature nil type with object vector");
    }
    // check for dsa signature key
    if (type == SIGN_SDSA) {
      // save the type
      d_type = type;
      // create the rsa key
      p_sdsa = new s_sdsa();
      // load the signature with a number vector
      p_sdsa->ldnvec (ovec);
    }
  }
  
  // copy construct this signature

  Signature::Signature (const Signature& that) {
    that.rdlock ();
    try {
      d_type = that.d_type;
      switch (d_type) {
      case SIGN_SNIL:
	p_snil = nullptr;
	break;
      case SIGN_SDSA:
	p_sdsa = new s_sdsa (*that.p_sdsa);
	break;
      }
      d_pubk = that.d_pubk;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this signature

  Signature::~Signature (void) {
    switch (d_type) {
    case SIGN_SDSA:
      delete p_sdsa;
      break;
    default:
      break;
    }
  }

  // return the class name

  String Signature::repr (void) const {
    return "Signature";
  }

 // return a clone of this object

  Object* Signature::clone (void) const {
    return new Signature (*this);
  }

  // assign a signature to this one

  Signature& Signature::operator = (const Signature& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      if (this != &that) {
	// reset the signature
	reset ();
	// set the new type
	d_type = that.d_type;
	// set the new signature
	switch (d_type) {
	case SIGN_SNIL:
	  p_snil = nullptr;
	  break;
	case SIGN_SDSA:
	  p_sdsa = new s_sdsa (*that.p_sdsa);
	  break;
	}
      }
      d_pubk = that.d_pubk;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the serial did

  t_word Signature::getdid (void) const {
    return SRL_DEOD_SEC;
  }

  // return the serial sid

  t_word Signature::getsid (void) const {
    return SRL_SIGN_SID;
  }
  
  // serialize this signature

  void Signature::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      os.write ((char) d_type);
      switch (d_type) {
      case SIGN_SDSA:
	p_sdsa->wrstream (os);
	break;
      default:
	break;
      }
      d_pubk.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this key

  void Signature::rdstream (InputStream& is) {
    wrlock ();
    try {
      switch (is.read ()) {
      case SIGN_SNIL:
	d_type = SIGN_SNIL;
	break;
      case SIGN_SDSA:
	d_type = SIGN_SDSA;
	p_sdsa = new s_sdsa; p_sdsa->rdstream (is);
	break;
      }
      d_pubk.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this signature

  void Signature::reset (void) {
    wrlock ();
    try {
      // clean the old signature
      switch (d_type) {
      case SIGN_SDSA:
	delete p_sdsa;
	p_sdsa = nullptr;
	break;
      default:
	break;
      }
      // reset the signature
      d_type = SIGN_SNIL;
      p_snil = nullptr;
      // reset the public key
      d_pubk.reset ();
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the signature type

  Signature::t_sign Signature::gettype (void) const {
    rdlock ();
    try {
      t_sign result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a formatted component representation by type

  String Signature::format (const t_isgn type) const {
    rdlock ();
    try {
      // initialize result
      String result;
      // select the key
      switch (d_type) {
      case SIGN_SDSA:
	result = p_sdsa->format (type);
	break;
      default:
	throw Exception ("signature-error", "unsupported component accessor");
	break;
      }
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a relatif key by type

  Relatif Signature::getrcmp (const t_isgn type) const {
    rdlock ();
    try {
      // initialize result
      Relatif result = 0;
      // select the key
      switch (d_type) {
      case SIGN_SDSA:
	result = p_sdsa->getscmp (type);
	break;
      default:
	throw Exception ("signature-error", "unsupported component accessor");
	break;
      }
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the signature public key

  void Signature::setpubk (const Key& pubk) {
    wrlock ();
    try {
      d_pubk = pubk;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the signature public key

  Key Signature::getpubk (void) const {
    rdlock ();
    try {
      Key result = d_pubk;
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

  // the object eval quarks
  static const long QUARK_SIGN     = String::intern ("Signature");
  static const long QUARK_SIGNSNIL = String::intern ("SNIL");
  static const long QUARK_SIGNSDSA = String::intern ("SDSA");
  static const long QUARK_SDSASCMP = String::intern ("DSA-S-COMPONENT");
  static const long QUARK_SDSARCMP = String::intern ("DSA-R-COMPONENT");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the signature supported quarks
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_FORMAT   = zone.intern ("format");
  static const long QUARK_GETPUBK  = zone.intern ("get-public-key");
  static const long QUARK_GETRCMP  = zone.intern ("get-relatif-component");

  // map an item to a signature type
  static inline Signature::t_sign item_to_sign (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_SIGN)
      throw Exception ("item-error", "item is not a signature item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_SIGNSNIL) return Signature::SIGN_SNIL;
    if (quark == QUARK_SIGNSDSA) return Signature::SIGN_SDSA;
    throw Exception ("item-error", 
		     "cannot map item to signature index accesoor");
  }
  
  // map an item to a component accessor
  static inline Signature::t_isgn item_to_isgn (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_SIGN)
      throw Exception ("item-error", "item is not a signature item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_SDSASCMP) return Signature::SDSA_SCMP;
    if (quark == QUARK_SDSARCMP) return Signature::SDSA_RCMP;
    throw Exception ("item-error", 
		     "cannot map item to signature index accesoor");
  }

  // evaluate an object data member

  Object* Signature::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    // key type part
    if (quark == QUARK_SIGNSNIL)
      return new Item (QUARK_SIGN, QUARK_SIGNSNIL);
    if (quark == QUARK_SIGNSDSA)
      return new Item (QUARK_SIGN, QUARK_SIGNSDSA);
    // component accessor
    if (quark == QUARK_SDSASCMP)
      return new Item (QUARK_SIGN, QUARK_SDSASCMP);
    if (quark == QUARK_SDSARCMP)
      return new Item (QUARK_SIGN, QUARK_SDSARCMP);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
  
  // create a new object in a generic way
  
  Object* Signature::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Signature;
    // check for 2 arguments
    if (argc == 2) {
      // load first object
      Object* obj = argv->get (0);
      // check for an item type
      Item*  iobj = dynamic_cast <Item*> (obj);
      if (iobj == nullptr) {
	throw Exception ("type-error",
			 "invalid object with signature constructor",
			 Object::repr (obj));
      }
      t_sign type = item_to_sign (*iobj);
      // load second object
      obj = argv->get (1);
      // check for a vector
      Vector* vobj = dynamic_cast <Vector*> (obj);
      if (vobj != nullptr) return new Signature (type, *vobj);
      // invalid type
      throw Exception ("type-error",
		       "invalid object with signature constructor",
		       Object::repr (obj));
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with signature");
  }

  // return true if the given quark is defined

  bool Signature::isquark (const long quark, const bool hflg) const {
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
  
  Object* Signature::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPUBK) return new Key (getpubk ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_FORMAT) {
	// check for an item type
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with format",
			   Object::repr (obj));
	}
	t_isgn type = item_to_isgn (*iobj);
	// get the relatif
	return new String (format (type));
      }
      if (quark == QUARK_GETRCMP) {
	// check for an item type
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", 
			   "invalid object with get-relatif-component",
			   Object::repr (obj));
	}
	t_isgn type = item_to_isgn (*iobj);
	// get the relatif
	return new Relatif (getrcmp (type));
      }      
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
