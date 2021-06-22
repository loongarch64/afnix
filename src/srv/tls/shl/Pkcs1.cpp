// ---------------------------------------------------------------------------
// - Pkcs1.cpp                                                               -
// - afnix:tls service - public key cryptographic standard implementation    -
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

#include "Pkcs1.hpp"
#include "Vector.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBuffer.hpp"
#include "AsnInteger.hpp"
#include "AsnSequence.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // get the pkcs1 node from the asn buffer
  static AsnSequence* pkcs1_topseq (const AsnBuffer& abuf) {
    // map the buffer to a node
    AsnNode* node = abuf.mapnode ();
    // check for a sequence
    AsnSequence* pseq = dynamic_cast <AsnSequence*> (node);
    if (pseq == nullptr) {
      delete node;
      throw Exception ("pkcs-error", "cannot map pkcs sequence node");
    }
    // check the sequence length
    if (pseq->getnlen () != 9) {
      delete node;
      throw Exception ("pkcs-error", "invalid pkcs sequence length");
    }
    return pseq;
  }

  // get the pkcs1 relatif number by index
  static Relatif pkcs1_torval (const AsnSequence* pseq, const long index) {
    // check for nil
    if (pseq == nullptr) {
      throw Exception ("pkcs-error", "invalid nil pkcs sequence node");
    }
    // get the node by index
    auto inod = dynamic_cast <AsnInteger*> (pseq->getnode (index));
    if (inod == nullptr) {
      throw Exception ("pkcs-error", "cannot map integer node");
    }
    return inod->torelatif ();
  }

  // -------------------------------------------------------------------------
  // - pkcs1 section                                                         -
  // -------------------------------------------------------------------------

  // create a default pkcs key
  
  Pkcs1::Pkcs1 (void) : Pki (Pem::PEMC_RSAP) {
    p_pkey = nullptr;
    reset ();
  }

  // create a pkcs key by key

  Pkcs1::Pkcs1 (Key* pkey) : Pki (Pem::PEMC_RSAP) {
    // check for valid key
    if ((pkey != nullptr) && (pkey->gettype () != Key::CKEY_KRSA)) {
      throw Exception ("pkcs-error" "invalid key type for pkcs");
    }
    Object::iref (p_pkey = pkey);
  }

  // create a pkcs key by path

  Pkcs1::Pkcs1 (const String& path) : Pki (Pem::PEMC_RSAP) {
    // preset objects
    p_pkey = nullptr;
    // read by path
    Pki::read (path);
  }
  
  // copy construct this pkcs key

  Pkcs1::Pkcs1 (const Pkcs1& that) {
    that.rdlock ();
    try {
      // assign base object
      Pki::operator = (that);
      // copy locally
      p_pkey = (that.p_pkey == nullptr) ? nullptr : new Key (*that.p_pkey);
      Object::iref (p_pkey);
      // here it is
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
      
  // destroy this pkcs key

  Pkcs1::~Pkcs1 (void) {
    Object::dref (p_pkey);
  }

  // assign a pkcs key to this one

  Pkcs1& Pkcs1::operator = (const Pkcs1& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base object
      Pki::operator = (that);
      // copy locally
      Object::dref (p_pkey);
      p_pkey = (that.p_pkey == nullptr) ? nullptr : new Key (*that.p_pkey);
      Object::iref (p_pkey);
      // here it is
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the object class name

  String Pkcs1::repr (void) const {
    return "Pkcs1";
  }

  // return a clone of this object

  Object* Pkcs1::clone (void) const {
    return new Pkcs1 (*this);
  }
  
  // reset the pkcs key

  void Pkcs1::reset (void) {
    wrlock ();
    try {
      // reset base object
      Pki::reset ();
      // reset locally
      Object::dref (p_pkey); p_pkey = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the pkcs key

  Key* Pkcs1::getkey (void) const {
    rdlock ();
    try {
      Key* result = p_pkey;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // encode the pkcs buffer

  bool Pkcs1::encode (void) {
    wrlock ();
    AsnSequence* pseq = nullptr;
    try {
      //extract the key data
      Relatif pmod = p_pkey->getrkey (Key::KRSA_PMOD);
      Relatif pexp = p_pkey->getrkey (Key::KRSA_PEXP);
      Relatif sexp = p_pkey->getrkey (Key::KRSA_SEXP);
      Relatif spvp = 0;
      Relatif spvq = 0;
      Relatif crtp = 0;
      Relatif crtq = 0;
      Relatif crti = 0;
      // create a sequence
      pseq = new AsnSequence;
      // add the version
      pseq->add (new AsnInteger(0));
      // add the key data
      pseq->add (new AsnInteger(pmod));
      pseq->add (new AsnInteger(pexp));
      pseq->add (new AsnInteger(sexp));
      pseq->add (new AsnInteger(spvp));
      pseq->add (new AsnInteger(spvq));
      pseq->add (new AsnInteger(crtp));
      pseq->add (new AsnInteger(crtq));
      pseq->add (new AsnInteger(crti));
      // write the sequence
      pseq->write (d_xbuf);
      // clean and return
      delete pseq;
      unlock ();
      return true;
    } catch (...) {
      delete pseq;
      unlock ();
      throw;
    }
  }
  
  // decode the pkcs buffer
  
  bool Pkcs1::decode (void) {
    wrlock ();
    AsnSequence* pseq = nullptr;
    try {
      // create an asn buffer
      AsnBuffer abuf (d_xbuf);
      // map the buffer to a sequence
      pseq = pkcs1_topseq (abuf);
      // get the object version
      long vers = pkcs1_torval(pseq,0).tolong ();
      if (vers != 0L) {
	throw Exception ("pkcs-error", "invalid pkcs key version");
      }
      // get the key element
      Relatif pmod = pkcs1_torval (pseq, 1);
      Relatif pexp = pkcs1_torval (pseq, 2);
      Relatif sexp = pkcs1_torval (pseq, 3);
      Relatif spvp = pkcs1_torval (pseq, 4);
      Relatif spvq = pkcs1_torval (pseq, 5);
      Relatif crtp = pkcs1_torval (pseq, 6);
      Relatif crtq = pkcs1_torval (pseq, 7);
      Relatif crti = pkcs1_torval (pseq, 8);
      // create a key vector argument
      Vector kvec;
      kvec.add (new Relatif (pmod));
      kvec.add (new Relatif (pexp));
      kvec.add (new Relatif (sexp));
      kvec.add (new Relatif (spvp));
      kvec.add (new Relatif (spvq));
      kvec.add (new Relatif (crtp));
      kvec.add (new Relatif (crtq));
      kvec.add (new Relatif (crti));
      // create the key
      p_pkey = new Key (Key::CKEY_KRSA, kvec);
      Object::iref (p_pkey);
      // clean and return
      delete pseq;
      unlock ();
      return true;
    } catch (...) {
      delete pseq;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - pkcs1 object section                                                  -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETKEY = zone.intern ("get-key");

  // create a new object in a generic way
 
  Object* Pkcs1::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // create a default object
    if (argc == 0) return new Pkcs1;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* path = dynamic_cast <String*> (obj);
      if (path != nullptr) return new Pkcs1 (*path);
      // check for a key
      Key* key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Pkcs1 (key);
      // invalid object
      throw Exception ("type-error", "invalif object with pkcs",
		       Object::repr (obj));
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with pkcs constructor");
  }

  // return true if the given quark is defined

  bool Pkcs1::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Pki::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Pkcs1::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETKEY) {
	rdlock ();
	try {
	  Object* result = getkey ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the pkcs method
    return Pki::apply (zobj, nset, quark, argv);
  }
}
