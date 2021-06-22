// ---------------------------------------------------------------------------
// - Session.cpp                                                             -
// - afnix:csm service - session class implementation                        -
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

#include "Time.hpp"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "System.hpp"
#include "Crypto.hpp"
#include "Session.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the random session key size
  static const long SESS_KEY_SIZE = 1024;

  // generate a random session key
  static String sess_new_skey (void) {
    Relatif rn = Relatif::random (SESS_KEY_SIZE);
    return rn.tohstr ();
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty session

  Session::Session (void) {
    d_skey = sess_new_skey ();
    d_path = System::tempnam ();
    d_ctim = Time::gettclk ();
    d_mtim = d_ctim;
    d_mage = 0LL;
    p_visa = nullptr;
  }

  // create a session by name

  Session::Session (const String& name) : Taggable (name) {
    d_skey = sess_new_skey ();
    d_path = System::tempnam ();
    d_ctim = Time::gettclk ();
    d_mtim = d_ctim;
    d_mage = 0LL;
    p_visa = nullptr;
  }

  // create a session by name and info

  Session::Session (const String& name, const String& info) :
    Taggable (name, info) {
    d_skey = sess_new_skey ();
    d_path = System::tempnam ();
    d_ctim = Time::gettclk ();
    d_mtim = d_ctim;
    d_mage = 0LL;
    p_visa = nullptr;
  }

  // create a session by name, info and maximum age

  Session::Session (const String& name, const String& info, const t_long mage) :
    Taggable (name, info) {
    d_skey = sess_new_skey ();
    d_ctim = Time::gettclk ();
    d_mtim = d_ctim;
    d_mage = (mage < 0LL) ? 0LL : mage;
    p_visa = nullptr;
  }

  // copy construct this session object

  Session::Session (const Session& that) {
    that.rdlock ();
    try {
      Taggable::operator = (that);
      d_skey = that.d_skey;
      d_shnm = that.d_shnm;
      d_shid = that.d_shid;
      d_path = that.d_path;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      d_mage = that.d_mage;
      Object::iref (p_visa = that.p_visa);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a session object to this one

  Session& Session::operator = (const Session& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Taggable::operator = (that);
      d_skey = that.d_skey;
      d_shnm = that.d_shnm;
      d_shid = that.d_shid;
      d_path = that.d_path;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      d_mage = that.d_mage;
      Object::dref (p_visa); Object::iref (p_visa = that.p_visa);
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

  String Session::repr (void) const {
    return "Session";
  }

  // return a clone of this object

  Object* Session::clone (void) const {
    return new Session (*this);
  }
  
  // return the serial did

  t_word Session::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Session::getsid (void) const {
    return SRL_SESS_SID;
  }
  
  // serialize this session

  void Session::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Taggable::wrstream (os);
      d_skey.wrstream (os);
      d_shnm.wrstream (os);
      d_shid.wrstream (os);
      d_path.wrstream (os);
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      Serial::wrlong (d_mage, os);
      if (p_visa != nullptr) p_visa->serialize (os); else Serial::wrnilid (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this session

  void Session::rdstream (InputStream& is) {
    wrlock ();
    try {
      Taggable::rdstream (is);
      d_skey.rdstream     (is);
      d_shnm.rdstream     (is);
      d_shid.rdstream     (is);
      d_path.rdstream     (is);
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      d_mage = Serial::rdlong (is);
      Object::iref(p_visa = dynamic_cast<Visa*>(Serial::deserialize (is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session name

  String Session::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the session hash id
  
  void Session::setshid (const String& shid) {
    wrlock ();
    try {
      d_shid = shid;
      d_mtim = Time::gettclk ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session hash id

  String Session::getshid (void) const {
    rdlock ();
    try {
      // prepare result
      String result = d_shid;
      // check for nil and rehash
      if (result.isnil () == true) {
	// prepare the hash value
	String hs = d_name + d_skey;
	// hash the string
	result = Crypto().hash (hs);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the session path
  
  void Session::setpath (const String& path) {
    wrlock ();
    try {
      d_path = path;
      d_mtim = Time::gettclk ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session path

  String Session::getpath (void) const {
    rdlock ();
    try {
      String result = d_path;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the session creation time

  t_long Session::getctim (void) const {
    rdlock ();
    try {
      t_long result = d_ctim;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session modification time 

  t_long Session::getmtim (void) const {
    rdlock ();
    try {
      t_long result = d_mtim;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the session has expired

  bool Session::isetim (void) const {
    rdlock ();
    try {
      // get the current time
      t_long time = Time::gettclk ();
      // check for expiration
      bool result = (d_mage == 0LL) ? false : (d_ctim + d_mage <= time);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // set the session expiration time
  void Session::setetim (const t_long etim) {
    wrlock ();
    try {
      d_mtim = Time::gettclk ();
      d_mage = etim - d_ctim;
      if (d_mage < 0LL) d_mage = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session expiration time

  t_long Session::getetim (void) const {
    rdlock ();
    try {
      t_long result = d_ctim + d_mage;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session valid remaining time

  t_long Session::getrtim (void) const {
    rdlock ();
    try {
      // get the current time
      t_long time = Time::gettclk ();
      // compute remaining time
      t_long vldt = (d_mage == 0) ? 0 : (d_ctim + d_mage - time);
      if (vldt < 0) vldt = 0;
      // unlock and return
      unlock ();
      return vldt;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the session maximum age
  
  void Session::setmage (const t_long mage) {
    wrlock ();
    try {
      d_mtim = Time::gettclk ();
      d_mage = mage;
      if (d_mage < 0LL) d_mage = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session maximum age

  t_long Session::getmage (void) const {
    rdlock ();
    try {
      t_long result = d_mage;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the session visa

  void Session::setvisa (Visa* visa) {
    wrlock ();
    try {
      Object::iref (visa); Object::dref (p_visa);
      p_visa = visa;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the session visa

  Visa* Session::getvisa (void) const {
    rdlock ();
    try {
      Visa* result = p_visa;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // bake a new cookie from the session information

  Cookie* Session::getcookie (const String& name) {
    rdlock ();
    try {
      // save the session hash name
      d_shnm = name;
      d_mtim = Time::gettclk ();
      // create a new cookie
      Cookie* result = new Cookie (d_shnm, getshid (), d_mage);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // close a session by reseting the maximum age

  Cookie* Session::close (void) {
    wrlock ();
    try {
      // close the session
      setmage (0LL);
      // get a closing cookie
      Cookie* result = new Cookie (d_shnm, getshid (), d_mage);
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
  static const long QUARK_ZONE_LENGTH = 16;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CLOSE   = zone.intern ("close");
  static const long QUARK_ETIMP   = zone.intern ("expire-p");
  static const long QUARK_SETSHID = zone.intern ("set-hash-id");
  static const long QUARK_GETSHID = zone.intern ("get-hash-id");
  static const long QUARK_GETPATH = zone.intern ("get-path");
  static const long QUARK_SETPATH = zone.intern ("set-path");
  static const long QUARK_GETCOOK = zone.intern ("get-cookie");
  static const long QUARK_SETMAGE = zone.intern ("set-max-age");
  static const long QUARK_GETMAGE = zone.intern ("get-max-age");
  static const long QUARK_SETETIM = zone.intern ("set-expire-time");
  static const long QUARK_GETETIM = zone.intern ("get-expire-time");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETRTIM = zone.intern ("get-remaining-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");
  static const long QUARK_SETVISA = zone.intern ("set-visa");
  static const long QUARK_GETVISA = zone.intern ("get-visa");

  // create a new object in a generic way

  Object* Session::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String name  = argv->getstring (0);
      return new Session (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Session (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      t_long mage = argv->getlong   (2);
      return new Session (name, info, mage);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with session");
  }

  // return true if the given quark is defined

  bool Session::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Session::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ETIMP)   return new Boolean (isetim  ());
      if (quark == QUARK_GETSHID) return new String  (getshid ());
      if (quark == QUARK_GETPATH) return new String  (getpath ());
      if (quark == QUARK_GETETIM) return new Integer (getetim ());
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
      if (quark == QUARK_GETMAGE) return new Integer (getmage ());
      if (quark == QUARK_GETRTIM) return new Integer (getrtim ());
      if (quark == QUARK_CLOSE)   return close ();
      if (quark == QUARK_GETVISA) {
	rdlock ();
	try {
	  Object* result = getvisa ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETSHID) {
	String shid = argv->getstring (0);
	setshid (shid);
	return nullptr;
      }
      if (quark == QUARK_SETPATH) {
	String path = argv->getstring (0);
	setpath (path);
	return nullptr;
      }
      if (quark == QUARK_SETETIM) {
	t_long time = argv->getlong (0);
	setetim (time);
	return nullptr;
      }
      if (quark == QUARK_SETMAGE) {
	t_long mage = argv->getlong (0);
	setmage (mage);
	return nullptr;
      }
      if (quark == QUARK_GETCOOK) {
	String name = argv->getstring (0);
	return getcookie (name);
      }
      if (quark == QUARK_SETVISA) {
	Object* obj = argv->get (0);
	auto visa = dynamic_cast<Visa*> (obj);
	if ((visa == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object for set-visa",
			   Object::repr (obj));
	}
	setvisa (visa);
	return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
