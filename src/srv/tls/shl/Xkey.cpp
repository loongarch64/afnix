// ---------------------------------------------------------------------------
// - Xkey.cpp                                                                -
// - afnix:tls service - X509 public key class implementation                -
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

#include "Xkey.hpp"
#include "Xoid.hpp"
#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBuffer.hpp"
#include "AsnInteger.hpp"
#include "AsnSequence.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure maps the bitset into a rsa key
  static Key* map_key_rsae (const Bitset& bset) {
    // create a asn buffer by bitset
    AsnBuffer abuf (bset);
    // map the buffer to a node
    AsnNode* node = abuf.mapnode ();
    AsnSequence* rsas = dynamic_cast <AsnSequence*> (node);
    try {
      // check for a sequence
      if (rsas == nullptr) {
	delete node;
	throw Exception ("xkey-error",
			 "cannot map rsa encryption sequence node");
      }
      // check the sequence length
      if (rsas->getnlen () != 2) {
	delete node;
	throw Exception ("xkey-error",
			 "invalid rsa encryption sequence length");
      }
      // get the modulus node
      node = rsas->getnode (0);
      AsnInteger* mnod = dynamic_cast <AsnInteger*> (node);
      if (mnod == nullptr) {
	delete rsas;
	throw Exception ("xkey-error", "cannot map rsa modulus node");
      }
      // get the exponent node
      node = rsas->getnode (1);
      AsnInteger* enod = dynamic_cast <AsnInteger*> (node);
      if (enod == nullptr) {
	delete rsas;
	throw Exception ("xkey-error", "cannot map rsa exponent node");
      }
      // create a vector key element
      Vector ovec;
      ovec.add (new Relatif (mnod->torelatif ()));
      ovec.add (new Relatif (enod->torelatif ()));
      // create the result key
      Key* result = new Key (Key::CKEY_KRSA, ovec);
      delete rsas;
      return result;
    } catch (...) {
      delete rsas;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default certificate key

  Xkey::Xkey (void) {
    reset ();
  }

  // create a certificate key by algorithm and bitset

  Xkey::Xkey (const Xalgo& algo, const Bitset& bset) {
    d_algo = algo;
    d_bset = bset;
  }

  // copy construct this object

  Xkey::Xkey (const Xkey& that) {
    that.rdlock ();
    try {
      d_algo = that.d_algo;
      d_bset = that.d_bset;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an object to this one

  Xkey& Xkey::operator = (const Xkey& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_algo = that.d_algo;
      d_bset = that.d_bset;
      // unlock and return
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

  String Xkey::repr (void) const {
    return "Xkey";
  }

  // return a clone of this object
  
  Object* Xkey::clone (void) const {
    return new Xkey (*this);
  }

  // reset this key

  void Xkey::reset (void) {
    wrlock ();
    try {
      d_algo.reset ();
      d_bset.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // build a key by algorithm and bitset

  Key* Xkey::tokey (void) const{
    wrlock ();
    try {
      Key* result = nullptr;
      // get the algorithm oid
      String aid = d_algo.getaid ();
      // map the algorithm oid
      Xoid::t_toid toid = Xoid::totoid (d_algo.getaid ());
      // select the key mapper
      switch (toid) {
      case Xoid::TLS_ALGO_RSAE:
	result = map_key_rsae (d_bset);
	break;
      default:
	result = nullptr;
	break;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET = zone.intern ("reset");
  static const long QUARK_TOKEY = zone.intern ("to-key");

  // create a new object in a generic way
 
  Object* Xkey::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // create a default object
    if (argc == 0) return new Xkey;
    // too many arguments
    throw Exception ("argument-error",
                     "too many arguments with certificate key constructor");
  }

  // return true if the given quark is defined

  bool Xkey::isquark (const long quark, const bool hflg) const {
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

  Object* Xkey::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOKEY) return tokey ();
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
