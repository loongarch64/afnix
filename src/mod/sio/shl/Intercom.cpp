// ---------------------------------------------------------------------------
// - Intercom.cpp                                                            -
// - afnix:sio module - interpreter communication class implementation       -
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

#include "Vector.hpp"
#include "Evaluable.hpp"
#include "Intercom.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default intercom

  Intercom::Intercom (void) {
    p_is = nullptr;
    p_os = nullptr;
  }

  // create an intercom by input stream

  Intercom::Intercom (InputStream* is) {
    p_is = nullptr;
    p_os = nullptr;
    setis (is);
  }

  // create an intercom by output stream

  Intercom::Intercom (OutputStream* os) {
    p_is = nullptr;
    p_os = nullptr;
    setos (os);
  }

  // create an intercom by input and output stream

  Intercom::Intercom (InputStream* is, OutputStream* os) {
    p_is = nullptr;
    p_os = nullptr;
    setis (is);
    setos (os);
  }

  // destroy this object

  Intercom::~Intercom (void) {
    Object::dref (p_is);
    Object::dref (p_os);
  }

  // return the class name

  String Intercom::repr (void) const {
    return "Intercom";
  }

  // set the intercom input stream

  void Intercom::setis (InputStream* is) {
    wrlock ();
    try {
      // protect the object
      Object::iref (is);
      // remove the stream and assign
      Object::dref (p_is);
      p_is = is;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the intercom input stream

  InputStream* Intercom::getis (void) const {
    rdlock ();
    try {
      InputStream* result = p_is;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the intercom output stream

  void Intercom::setos (OutputStream* os) {
    wrlock ();
    try {
      // protect the object
      Object::iref (os);
      // remove the stream
      Object::dref (p_os);
      p_os = os;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the intercom output stream

  OutputStream* Intercom::getos (void) const {
    rdlock ();
    try {
      OutputStream* result = p_os;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // send an object by serialization

  void Intercom::send (Object* obj) {
    // lock and send
    wrlock ();
    try {
      // check for nil
      if ((obj == nullptr) && (p_os != nullptr)) {
	Serial::wrnilid (*p_os);
	unlock ();
	return;
      }
      // check for serial object
      Serial* sobj = dynamic_cast <Serial*> (obj);
      if (sobj == nullptr) {
	throw Exception ("type-error", "invalid object for intercom send",
			 Object::repr (obj));
      }
      // flush and serialize in the output buffer
      d_ob.flush ();
      sobj->serialize (d_ob);
      // write the buffer and unlock
      d_ob.send (p_os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // receive an object by deserialization

  Object* Intercom::recv (void) {
    // lock and receive
    wrlock ();
    try {
      Object* result =
	(p_is == nullptr) ? nullptr : Serial::deserialize (*p_is);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // perform a send-receive request at once

  Object* Intercom::rqst (Object* obj) {
    wrlock ();
    try {
      // send the object
      send (obj);
      // receive the reply
      Object* result = recv ();
      // done 
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
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SEND  = zone.intern ("send");
  static const long QUARK_RECV  = zone.intern ("recv");
  static const long QUARK_RQST  = zone.intern ("request");
  static const long QUARK_SETIS = zone.intern ("set-input-stream");
  static const long QUARK_GETIS = zone.intern ("get-input-stream");
  static const long QUARK_SETOS = zone.intern ("set-output-stream");
  static const long QUARK_GETOS = zone.intern ("get-output-stream");

  // create a new object in a generic way

  Object* Intercom::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 arguments
    if (argc == 0) return new Intercom;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new Intercom (is);
      // check for an output stream
      OutputStream* os = dynamic_cast <OutputStream*> (obj);
      if (os != nullptr) return new Intercom (os);
      // invalid object
      throw Exception ("type-error", "invalid object with intercom",
                       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is == nullptr) {
        throw Exception ("type-error", "invalid object with intercom",
                         Object::repr (obj));   
      }
      obj = argv->get (1);
      // check for an output stream
      OutputStream* os = dynamic_cast <OutputStream*> (obj);
      if (os == nullptr) {
        throw Exception ("type-error", "invalid object with intercom",
                         Object::repr (obj));
      }
      return new Intercom (is, os);

    }
    throw Exception ("argument-error", "too many argument with intercom");
  }

  // return true if the given quark is defined

  bool Intercom::isquark (const long quark, const bool hflg) const {
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
  
  Object* Intercom::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_RECV) return recv ();
      if (quark == QUARK_GETIS) {
	rdlock ();
	try {
	  Object* result = getis ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETOS) {
	rdlock ();
	try {
	  Object* result = getos ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SEND) {
	Object* obj = argv->get (0);
	send (obj);
	return nullptr;
      }
      if (quark == QUARK_RQST) {
	Object* obj = argv->get (0);
	return rqst (obj);
      }
      if (quark == QUARK_SETIS) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid input stream for intercom",
			   Object::repr (obj));
	}
	setis (is);
	return nullptr;
      }
      if (quark == QUARK_SETOS) {
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid output stream for intercom",
			   Object::repr (obj));
	}
	setos (os);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
