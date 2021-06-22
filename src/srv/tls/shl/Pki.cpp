// ---------------------------------------------------------------------------
// - Pki.cpp                                                                 -
// - afnix:tls service - public key infrastructure implementation            -
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

#include "Pki.hpp"
#include "Item.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputFile.hpp"
#include "OutputFile.hpp"
#include "InputMapped.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the pki encoding format extension
  static const String PKI_XPEM = "PEM";
  static const String PKI_XDER = "DER";

  // map a string to an encoding type

  Pki::t_efmt Pki::toefmt (const String& sfmt) {
    String ufmt = sfmt.toupper ();
    if (ufmt == PKI_XPEM) return Pki::EFMT_XPEM;
    if (ufmt == PKI_XDER) return Pki::EFMT_XDER;
    throw Exception ("pki-error", "invalid format extension", sfmt);
  }
  
  // map an encoding type to a string
  
  String Pki::tostring (const t_efmt efmt) {
    switch (efmt) {
    case EFMT_XPEM:
      return PKI_XPEM;
      break;
    case EFMT_XDER:
      return PKI_XDER;
      break;
    }
    throw Exception ("pki-error", "cannot map pki encoding type to string");
  }

  // map a file to a buffer by encoding

  Buffer* Pki::tobuffer (const String& path, const Pem::t_pemc pemc) {
    // get the extension
    Pki::t_efmt efmt = Pki::toefmt (System::xext (path));
    // check for der format
    if (efmt == Pki::EFMT_XDER) {
      InputMapped im (path);
      return im.tobuffer ();
    }
    if (efmt == Pki::EFMT_XPEM) {
      // create an input file
      InputFile is (path);
      // the output buffer
      Buffer* ob = new Buffer;
      try {
	// create a pem reader
	Pem pem; pem.setrflg (true); pem.stream (*ob, is);
	is.close ();
	// validate the pem file
	if (pem.getpemc () != pemc) {
	  throw Exception ("pki-error", "inconsistent pem type in tobuffer");
	}
      } catch (...) {
	delete ob;
	throw;
      }
      return ob;
    }
    // internal error
    throw Exception ("pki-error", "internal error with tobuffer");
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default pki

  Pki::Pki (void) {
    d_pemc = Pem::PEMC_NONE;
  }
  
  // create a pki by type

  Pki::Pki (const Pem::t_pemc pemc) {
    d_pemc = pemc;
  }

  // copy construct this pki

  Pki::Pki (const Pki& that) {
    that.rdlock ();
    try {
      d_pemc = that.d_pemc;
      d_xbuf = that.d_xbuf;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a pki to this one

  Pki& Pki::operator = (const Pki& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_pemc = that.d_pemc;
      d_xbuf = that.d_xbuf;
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
  
  // reset this pki

  void Pki::reset (void) {
    wrlock ();
    try {
      d_xbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // read an object by path

  void Pki::read (const String& path) {
    wrlock ();
    try {
      // create the input file
      InputFile is (path);
      // read the file
      read (&is, Pki::toefmt(System::xext(path)));
      unlock ();
    } catch (...){
      unlock ();
      throw;
    }    
  }

  // read an object by stream and format

  void Pki::read (InputStream* is, const t_efmt efmt) {
    wrlock ();
    try {
      // check for a valid stream
      if (is == nullptr) {
	throw Exception ("pki-error", "nil input stream in read");
      }
      // reset the pki object
      reset ();
      // just fill the buffer in der mode
      if (efmt == EFMT_XDER) {
	while (is->valid () == true) d_xbuf.add (is->read ());
      }
      // parse the stream in pem mode
      if (efmt == EFMT_XPEM) {
	// read with a pem codec
	Pem pem; pem.setrflg (true); pem.stream (d_xbuf, *is);
	// check the content type
	if (d_pemc != pem.getpemc ()) {
	  throw Exception ("pki-error", "inconsistent content type");
	}
      }
      // decode the pki buffer
      if (decode () == false) {
	throw Exception ("pki-error", "cannnot decode pki buffer");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an object by path

  void Pki::write (const String& path) {
    wrlock ();
    try {
      // create the output file
      OutputFile os (path);
      // write the file
      write (&os, Pki::toefmt(System::xext(path)));
      unlock ();
    } catch (...){
      unlock ();
      throw;
    }    
  }

  // write an object by path and format

  void Pki::write (OutputStream* os, const t_efmt efmt) {
    wrlock ();
    try {
      // encode in the pki buffer
      if (encode () == false) {
	throw Exception ("pki-error", "cannot encode pki buffer");
      }
      // check for a valid stream
      if (os == nullptr) {
	throw Exception ("pki-error", "nil output stream in write");
      }
      // write thr buffer in der mode
      if (efmt == EFMT_XDER) {
	os->write (d_xbuf);
      }
      // encode in pem mode
      if (efmt == EFMT_XPEM) {
	// create a pem encoder and stream
	Pem pem (d_pemc); Buffer ob; pem.stream (ob, d_xbuf);
	os->write (ob);
      }
      unlock ();
    } catch (...){
      unlock ();
      throw;
    }    
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the item eval quarks
  static const long QUARK_PKI = String::intern ("Pki");
  static const long QUARK_PEM = String::intern ("PEM");
  static const long QUARK_DER = String::intern ("DER");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_READ    = zone.intern ("read");
  static const long QUARK_WRITE   = zone.intern ("write");

  // map an item to an encoding format
  
  Pki::t_efmt pki_toefmt (const Item& item) {
    // check for a pki item
    if (item.gettid () != QUARK_PKI)
      throw Exception ("item-error", "item is not a pki item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_PEM) return Pki::EFMT_XPEM;
    if (quark == QUARK_DER) return Pki::EFMT_XDER;
    throw Exception ("item-error", "cannot map item to tls format encoding");
  }

  // evaluate an object data member

  Object* Pki::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_PEM)
      return new Item (QUARK_PKI, QUARK_PEM);
    if (quark == QUARK_DER)
      return new Item (QUARK_PKI, QUARK_DER);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Pki::isquark (const long quark, const bool hflg) const {
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

  Object* Pki::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_READ) {
	String path = argv->getstring (0);
	read (path);
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	String path = argv->getstring (0);
	write (path);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_READ) {
	// check for an input stream
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("argument-error", 
			   "invalid arguments with pki read",
			   Object::repr (obj));
	}
	// check for an item
	obj = argv->get (1);
	Item* item = dynamic_cast <Item*> (obj);
	if (item == nullptr) {
	  throw Exception ("argument-error", 
			   "invalid arguments with pki read",
			   Object::repr (obj));
	}
	t_efmt efmt = pki_toefmt (*item);
	read (is, efmt);
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	// check for an output stream
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("argument-error", 
			   "invalid arguments with pki write",
			   Object::repr (obj));
	}
	// check for an item
	obj = argv->get (1);
	Item* item = dynamic_cast <Item*> (obj);
	if (item == nullptr) {
	  throw Exception ("argument-error", 
			   "invalid arguments with pki write",
			   Object::repr (obj));
	}
	t_efmt efmt = pki_toefmt (*item);
	write (os, efmt);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
