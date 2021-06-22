// ---------------------------------------------------------------------------
// - Pem.cpp                                                                 -
// - afnix:uri service - pem codec class implementation                      -
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

#include "Pem.hpp"
#include "Item.hpp"
#include "Vector.hpp"
#include "Regex.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the pem string content (RFC 7468)
  static const String PEM_BSTR_MARK = "-----";
  static const String PEM_BSTR_BMRK = "-----BEGIN ";
  static const String PEM_BSTR_EMRK = "-----END ";
  static const String PEM_XSTR_NONE = "";
  static const String PEM_BSTR_RSAP = "RSA PRIVATE KEY";
  static const String PEM_BSTR_RSAC = "RSA PUBLIC KEY";
  static const String PEM_BSTR_CERT = "CERTIFICATE";

  // this procedure maps the pem type begin string
  static String pem_pemc_bstr (const Pem::t_pemc pemc) {
    String result = PEM_BSTR_BMRK;
    switch (pemc) {
    case Pem::PEMC_NONE:
      result = PEM_XSTR_NONE;
      break;
    case Pem::PEMC_RSAP:
      result += PEM_BSTR_RSAP + PEM_BSTR_MARK;
      break;
    case Pem::PEMC_RSAC:
      result += PEM_BSTR_RSAC + PEM_BSTR_MARK;
      break;
    case Pem::PEMC_CERT:
      result += PEM_BSTR_CERT + PEM_BSTR_MARK;
      break;
    }
    return result;
  }
  
  // this procedure maps the pem type end string
  static String pem_pemc_estr (const Pem::t_pemc pemc) {
    String result = PEM_BSTR_BMRK;
    switch (pemc) {
    case Pem::PEMC_NONE:
      result = PEM_XSTR_NONE;
      break;
    case Pem::PEMC_RSAP:
      result += PEM_BSTR_RSAP + PEM_BSTR_MARK;
      break;
    case Pem::PEMC_RSAC:
      result += PEM_BSTR_RSAC + PEM_BSTR_MARK;
      break;
    case Pem::PEMC_CERT:
      result += PEM_BSTR_CERT + PEM_BSTR_MARK;
      break;
    }
    return result;
  }
  
  // this procedure computes the pem type with the begin string
  static Pem::t_pemc pem_bstr_pemc (const String& bstr) {
    // check for null string
    if (bstr == PEM_XSTR_NONE) return Pem::PEMC_NONE;
    // the begin string regex
    Regex re("(-----BEGIN )(<$a >+)(-----)");
    if (re == bstr) {
      String tstr = re.getstring(1);
      if (tstr == PEM_BSTR_RSAP) return Pem::PEMC_RSAP;
      if (tstr == PEM_BSTR_RSAC) return Pem::PEMC_RSAC;
      if (tstr == PEM_BSTR_CERT) return Pem::PEMC_CERT;
    }
    // invalid format
    throw Exception ("pem-error", "unsupported pem format", bstr);
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                         -
  // -------------------------------------------------------------------------

  // create default pem codec

  Pem::Pem (void) : Basexx (BASE_SC64) {
    d_pemc = PEMC_NONE;
  }
  
  // create a base codec by type
  
  Pem::Pem (const t_pemc pemc) : Basexx (BASE_SC64) {
    d_pemc = pemc;
  }
  
  // return the class name

  String Pem::repr (void) const {
    return "Pem";
  }

  // reset this base codec

  void Pem::reset (void) {
    wrlock ();
    try {
      // reset the base codec
      Basexx::reset ();
      // reset the content
      d_pemc = PEMC_NONE;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the pem content type

  Pem::t_pemc Pem::getpemc (void) const {
    rdlock ();
    try {
      t_pemc result = d_pemc;
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

  // encode an input buffer into an output buffer

  long Pem::encode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
        throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // map the begin write
      auto bstr = pem_pemc_bstr (d_pemc); ob.add (bstr); ob.add (eolc);
      // compute the encoded buffer
      Buffer eb; Basexx::encode(eb, ib); ob.add (eb); ob.add (eolc);
      // map the end string
      auto estr = pem_pemc_estr (d_pemc); ob.add (estr); ob.add (eolc);
      // extract result
      long result = ob.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // encode an input stream into an output buffer

  long Pem::encode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
        throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // map the begin write
      auto bstr = pem_pemc_bstr (d_pemc); ob.add (bstr); ob.add (eolc);
      // compute the encoded buffer
      Buffer eb; Basexx::encode(eb, is); ob.add (eb); ob.add (eolc);
      // map the end string
      auto estr = pem_pemc_estr (d_pemc); ob.add (estr); ob.add (eolc);
      // extract result
      long result = ob.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output buffer

  long Pem::encode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
        throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      long result = 0L;
      // map the begin write
      auto bstr = pem_pemc_bstr (d_pemc);
      result = os.write (bstr); result += os.write(eolc);
      // compute the encoded stream
      result += Basexx::encode(os, is); result += os.write (eolc);
      // map the end string
      auto estr = pem_pemc_estr (d_pemc);
      result += os.write (estr); result += os.write (eolc);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input buffer into an output buffer

  long Pem::decode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
        throw Exception ("cipher-error", "calling decode in non reverse mode");
      }
      // read the buffer line - get the pem type
      String line = ib.readln ();
      Pem::t_pemc pemc = pem_bstr_pemc (line);
      // get the end string
      String estr = pem_pemc_estr (pemc);
      // loop in the input buffer
      Buffer db; bool lend = false;
      while (ib.empty() == false) {
	// get the next available line
	line = ib.readln ();
	// check for end string
	if (line == estr) {
	  lend = true;
	  break;
	}
	// save the line
	db.add (line);
      }
      if (lend == false) {
	throw Exception ("pem-error", "missing pem end marker line");
      }
      // decode the buffer
      long result = Basexx::decode (ob, db);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode an input stream into an output buffer

  long Pem::decode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
        throw Exception ("cipher-error", "calling decode in non reverse mode");
      }
      // read the buffer line - get the pem type
      String line = is.readln ();
      Pem::t_pemc pemc = pem_bstr_pemc (line);
      // get the end string
      String estr = pem_pemc_estr (pemc);
      // loop in the input stream
      Buffer db; bool lend = false;
      while (is.valid() == true) {
	// get the next available line
	line = is.readln ();
	// check for end string
	if (line == estr) {
	  lend = true;
	  break;
	}
	// save the line
	db.add (line);
      }
      if (lend == false) {
	throw Exception ("pem-error", "missing pem end marker line");
      }
      // decode the buffer
      long result = Basexx::decode (ob, db);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output stream

  long Pem::decode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
        throw Exception ("cipher-error", "calling decode in non reverse mode");
      }
      // read the buffer line - get the pem type
      String line = is.readln ();
      Pem::t_pemc pemc = pem_bstr_pemc (line);
      // get the end string
      String estr = pem_pemc_estr (pemc);
      // loop in the input stream
      Buffer db; bool lend = false;
      while (is.valid() == true) {
	// get the next available line
	line = is.readln ();
	// check for end string
	if (line == estr) {
	  lend = true;
	  break;
	}
	// save the line
	db.add (line);
      }
      if (lend == false) {
	throw Exception ("pem-error", "missing pem end marker line");
      } 
      // decode the buffer
      Buffer ob;
      long result = Basexx::decode (ob, db);
      if (os.write (ob) != result) {
	throw Exception ("pem-error", "inconsistent decode write");
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

  // the object eval quarks
  static const long QUARK_PEM      = String::intern ("Pem");
  static const long QUARK_PEMCNONE = String::intern ("NONE");
  static const long QUARK_PEMCRSAP = String::intern ("RSA-PRIVATE-KEY");
  static const long QUARK_PEMCRSAC = String::intern ("RSA-PUBLIC-KEY");
  static const long QUARK_PEMCCERT = String::intern ("CERTIFICATE");

  // map an item to a base type
  static inline Pem::t_pemc item_to_pemc (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_PEM)
      throw Exception ("item-error", "item is not a pem item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_PEMCNONE) return Pem::PEMC_NONE;
    if (quark == QUARK_PEMCRSAP) return Pem::PEMC_RSAP;
    if (quark == QUARK_PEMCRSAC) return Pem::PEMC_RSAC;
    if (quark == QUARK_PEMCCERT) return Pem::PEMC_CERT;
    throw Exception ("item-error", "cannot map item to pem type");
  }

  // evaluate an object data member

  Object* Pem::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    // key type part
    if (quark == QUARK_PEMCNONE) return new Item (QUARK_PEM, QUARK_PEMCNONE);
    if (quark == QUARK_PEMCRSAP) return new Item (QUARK_PEM, QUARK_PEMCRSAP);
    if (quark == QUARK_PEMCRSAC) return new Item (QUARK_PEM, QUARK_PEMCRSAC);
    if (quark == QUARK_PEMCCERT) return new Item (QUARK_PEM, QUARK_PEMCCERT);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
  
  // create a new object in a generic way
  
  Object* Pem::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Basexx;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto item = dynamic_cast<Item*> (obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object with pem constructor",
			 Object::repr (obj));
      }
      t_pemc pemc = item_to_pemc (*item);
      return new Pem (pemc);
    }
    throw Exception ("argument-error", "too many arguments with basexx");
  }
}
