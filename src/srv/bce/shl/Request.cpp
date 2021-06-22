// ---------------------------------------------------------------------------
// - Request.cpp                                                             -
// - afnix:bce service - request part class implementation                   -
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

#include "Date.hpp"
#include "Vector.hpp"
#include "Bcesid.hxx"
#include "Integer.hpp"
#include "Request.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the request information plist
  static const String PN_RQS_CODE = "PN-RQS-CODE";
  static const String PI_RQS_CODE = "REQUEST CODE";
  static const String PN_RQS_STMP = "PN-RQS-STMP";
  static const String PI_RQS_STMP = "REQUEST TIMESTAMP";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default request

  Request::Request (void) {
    d_code = 0L;
    d_stmp = 0LL;
    p_tran = nullptr;
  }
  
  // create a request by transaction

  Request::Request (Transaction* tran) {
    d_code = 0L;
    d_stmp = Time::gettclk ();
    Object::iref (p_tran = tran);
  }

  // create a request by code and transaction

  Request::Request (const long code, Transaction* tran) {
    d_code = code;
    d_stmp = Time::gettclk ();
    Object::iref (p_tran = tran);
  }

  // copy construct this request

  Request::Request (const Request& that) {
    that.rdlock ();
    try {
      d_code = that.d_code;
      d_stmp = that.d_stmp;
      Object::iref (p_tran = that.p_tran);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this request

  Request::Request (Request&& that) noexcept :
    Serial (static_cast<Serial&&>(that)){
    that.wrlock ();
    try {
      d_code = that.d_code; that.d_code = 0L;
      d_stmp = that.d_stmp; that.d_stmp = 0LL;
      p_tran = that.p_tran; that.p_tran = nullptr;
    } catch (...) {
      d_code = 0L;
      d_stmp = 0LL;
      p_tran = nullptr;
    }
    that.unlock ();
  }

  // destroy this request

  Request::~Request (void) {
    Object::dref (p_tran);
  }
    
  // assign a request to this one

  Request& Request::operator = (const Request& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_code = that.d_code;
      d_stmp = that.d_stmp;
      Object::iref (that.p_tran); Object::dref (p_tran); p_tran = that.p_tran;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a request to this one

  Request& Request::operator = (Request&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_code = that.d_code; that.d_code = 0L;
      d_stmp = that.d_stmp; that.d_stmp = 0LL;
      Object::dref (p_tran); p_tran = that.p_tran; that.p_tran = nullptr;
    } catch (...) {
      d_code = 0L;
      d_stmp = 0LL;
      p_tran = nullptr;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the request class name
  
  String Request::repr (void) const {
    return "Request";
  }
  
  // return a clone of this object

  Object* Request::clone (void) const {
    return new Request (*this);
  }
  
  // return the serial did

  t_word Request::getdid (void) const {
    return SRL_DEOD_BCE;
  }

  // return the serial sid
  
  t_word Request::getsid (void) const {
    return SRL_RQST_SID;
  }

  // serialize this request

  void Request::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_code, os);
      Serial::wrlong (d_stmp, os);
      if (p_tran == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_tran->serialize(os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this request

  void Request::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_code = Serial::rdlong (is);
      d_stmp = Serial::rdlong (is);
      Object::iref (p_tran =
		    dynamic_cast<Transaction*>(Serial::deserialize(is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the request information plist

  Plist Request::getplst (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist result;
      // set the request code
      result.add (PN_RQS_CODE, PI_RQS_CODE, (t_long) d_code);
      result.add (PN_RQS_STMP, PI_RQS_STMP, Date(d_stmp).toiso(true));
      if (p_tran != nullptr) result += p_tran->getplst ();
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the request code

  void Request::setcode (const long code) {
    wrlock ();
    try {
      d_code = code;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // get the request code

  long Request::getcode (void) const {
    rdlock ();
    try {
      long result = d_code;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the request timestamp

  t_long Request::getstmp (void) const {
    rdlock ();
    try {
      t_long result = d_stmp;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the request transaction

  void Request::settran (Transaction* tran) {
    wrlock ();
    try {
      Object::iref (tran); Object::dref (p_tran); p_tran = tran;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // get the request transaction

  Transaction* Request::gettran (void) const {
    rdlock ();
    try {
      Transaction* result = p_tran;
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETCODE = zone.intern ("set-code");
  static const long QUARK_GETCODE = zone.intern ("get-code");
  static const long QUARK_SETTRAN = zone.intern ("set-transaction");
  static const long QUARK_GETTRAN = zone.intern ("get-transaction");
  static const long QUARK_GETSTMP = zone.intern ("get-timestamp");

  // create a new object in a generic way

  Object* Request::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default request
    if (argc == 0) return new Request;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      auto tran = dynamic_cast<Transaction*>(obj);
      if (tran == nullptr) {
	throw Exception ("type-error", "invalid object as transaction",
			 Object::repr (obj));
      }
      return new Request (tran);
    }
    // check for 2 arguments
    if (argc == 2) {
      long code = argv->getlong (0);
      Object* obj = argv->get (1);
      auto tran = dynamic_cast<Transaction*>(obj);
      if (tran == nullptr) {
	throw Exception ("type-error", "invalid object as transaction",
			 Object::repr (obj));
      }
      return new Request (code, tran);
    }
    throw Exception ("argument-error",
                     "too many argument with request constructor");
  }

  // return true if the given quark is defined

  bool Request::isquark (const long quark, const bool hflg) const {
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
  
  Object* Request::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCODE) return new Integer (getcode ());
      if (quark == QUARK_GETSTMP) return new Integer (getstmp ());
      if (quark == QUARK_GETTRAN) {
	rdlock ();
	try {
	  Object* result = gettran ();
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
      if (quark == QUARK_SETCODE) {
	long code = argv->getlong (0);
	setcode (code);
	return nullptr;
      }
      if (quark == QUARK_SETTRAN) {
	Object* obj = argv->get (0);
	auto tran = dynamic_cast<Transaction*>(obj);
	if (tran == nullptr) {
	  throw Exception ("type-error", "invalid object as transaction",
			   Object::repr (obj));
	}
	settran (tran);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
