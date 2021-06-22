// ---------------------------------------------------------------------------
// - Sockopt.cpp                                                          -
// - afnix:net module - socket options class implementation                  -
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
#include "Sockopt.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the socket eval quarks
  static const long QUARK_SORADR = String::intern ("SO-REUSE-ADDRESS");
  static const long QUARK_SORPRT = String::intern ("SO-REUSE-PORT");
  static const long QUARK_SOBCST = String::intern ("SO-BROADCAST");
  static const long QUARK_SODNTR = String::intern ("SO-DONT-ROUTE");
  static const long QUARK_SOKLIV = String::intern ("SO-KEEP-ALIVE");
  static const long QUARK_SOLIGT = String::intern ("SO-LINGER");
  static const long QUARK_SORSIZ = String::intern ("SO-RECV-SIZE");
  static const long QUARK_SOSSIZ = String::intern ("SO-SEND-SIZE");
  static const long QUARK_SOSHOP = String::intern ("SO-HOP-LIMIT");
  static const long QUARK_SOMLBK = String::intern ("SO-MCAST-LOOPBACK");
  static const long QUARK_SOMHOP = String::intern ("SO-MCAST-HOP-LIMIT");
  static const long QUARK_SOMSIZ = String::intern ("SO-MAX-SEGMENT-SIZE");
  static const long QUARK_SONDLY = String::intern ("SO-NO-DELAY");
  static const long QUARK_SOKOPT = String::intern ("Sockopt");

  // map an enumeration item to a socket option
  static inline Sockopt::t_so item_to_so (const Item& item) {
    // check for a socket item
    if (item.gettid () != QUARK_SOKOPT)
      throw Exception ("item-error", "item is not an socket item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_SORADR) return Sockopt::SO_RADR;
    if (quark == QUARK_SORPRT) return Sockopt::SO_RPRT;
    if (quark == QUARK_SOBCST) return Sockopt::SO_BCST;
    if (quark == QUARK_SODNTR) return Sockopt::SO_DNTR;
    if (quark == QUARK_SOKLIV) return Sockopt::SO_KLIV;
    if (quark == QUARK_SOLIGT) return Sockopt::SO_LIGT;
    if (quark == QUARK_SORSIZ) return Sockopt::SO_RSIZ;
    if (quark == QUARK_SOSSIZ) return Sockopt::SO_SSIZ;
    if (quark == QUARK_SOSHOP) return Sockopt::SO_SHOP;
    if (quark == QUARK_SOMLBK) return Sockopt::SO_MLBK;
    if (quark == QUARK_SOMHOP) return Sockopt::SO_MHOP;
    if (quark == QUARK_SOMSIZ) return Sockopt::SO_MSIZ;
    if (quark == QUARK_SONDLY) return Sockopt::SO_NDLY;
    throw Exception ("item-error", "cannot map item to socket option");
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default socket option

  Sockopt::Sockopt (void) {
  }
  
  // copy move this socktop

  Sockopt::Sockopt (Sockopt&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this socket option into this one

  Sockopt& Sockopt::operator = (Sockopt&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_SETOPTION = zone.intern ("set-option");
  static const long QUARK_GETOPTION = zone.intern ("get-option");
  
  // evaluate a quark statically

  Object* Sockopt::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_SORADR) 
      return new Item (QUARK_SOKOPT, QUARK_SORADR);
    if (quark == QUARK_SORPRT) 
      return new Item (QUARK_SOKOPT, QUARK_SORPRT);
    if (quark == QUARK_SOBCST) 
      return new Item (QUARK_SOKOPT, QUARK_SOBCST);
    if (quark == QUARK_SODNTR) 
      return new Item (QUARK_SOKOPT, QUARK_SODNTR);
    if (quark == QUARK_SOKLIV) 
      return new Item (QUARK_SOKOPT, QUARK_SOKLIV);
    if (quark == QUARK_SOLIGT)    
      return new Item (QUARK_SOKOPT, QUARK_SOLIGT);
    if (quark == QUARK_SORSIZ)   
      return new Item (QUARK_SOKOPT, QUARK_SORSIZ);
    if (quark == QUARK_SOSSIZ)   
      return new Item (QUARK_SOKOPT, QUARK_SOSSIZ);
    if (quark == QUARK_SOSHOP)  
      return new Item (QUARK_SOKOPT, QUARK_SOSHOP);
    if (quark == QUARK_SOMLBK) 
      return new Item (QUARK_SOKOPT, QUARK_SOMLBK);
    if (quark == QUARK_SOMHOP)  
      return new Item (QUARK_SOKOPT, QUARK_SOMHOP);
    if (quark == QUARK_SOMSIZ) 
      return new Item (QUARK_SOKOPT, QUARK_SOMSIZ);
    if (quark == QUARK_SONDLY) 
      return new Item (QUARK_SOKOPT, QUARK_SONDLY);
    throw Exception ("eval-error", "cannot evaluate member",
		     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Sockopt::isquark (const long quark, const bool hflg) const {
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

  Object* Sockopt::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETOPTION) {
	Item* item = dynamic_cast <Item*> (argv->get (0));
	if (item == nullptr) throw Exception ("argument-error",
					   "invalid object as socket option");
	t_so opt = item_to_so (*item);
	Object* result = nullptr;
	switch (opt) {
	case SO_RADR:
	case SO_RPRT:
	case SO_BCST:
	case SO_DNTR:
	case SO_KLIV:
	case SO_MLBK:
	case SO_NDLY:
	  result = new Boolean (getbopt (opt));
	  break;
	case SO_LIGT:
	case SO_RSIZ:
	case SO_SSIZ:
	case SO_SHOP:
	case SO_MHOP:
	case SO_MSIZ:
	  result = new Integer (getlopt (opt));
	}
	return result;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETOPTION) {
	Item* item = dynamic_cast <Item*> (argv->get (0));
	if (item == nullptr) throw Exception ("argument-error",
					   "invalid object as socket option");
	t_so opt = item_to_so (*item);
	Object* obj = argv->get (1);
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) {
	  bool val = bobj->tobool ();
	  return new Boolean (setopt (opt, val));
	}
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long val = iobj->tolong (); 
	  return new Boolean (setopt (opt, val));
	}
	throw Exception ("argument-error", "invalid argument with set-option");
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
