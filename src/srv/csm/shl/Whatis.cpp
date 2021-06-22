// ---------------------------------------------------------------------------
// - Whatis.cpp                                                              -
// - afnix:csm service - blob whatis class implementation                    -
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
#include "Whom.hxx"
#include "Whatis.hpp"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil blob whatis

  Whatis::Whatis (void) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
  }

  // copy construct this blob whatis

  Whatis::Whatis (const Whatis& that) {
    that.rdlock ();
    try {
      // copy base part
      Taggable::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_kid  = that.d_kid;
      d_type = that.d_type;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy construct this blob whis

  Whatis& Whatis::operator = (const Whatis& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base object
      Taggable::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_kid  = that.d_kid;
      d_type = that.d_type;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the blob whatis class name

  String Whatis::repr (void) const {
    return "Whatis";
  }

  // return the serial did

  t_word Whatis::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Whatis::getsid (void) const {
    return SRL_WAIS_SID;
  }
  
  // serialize this blob whatis

  void Whatis::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the taggable
      Taggable::wrstream (os);
      // serialize locally
      d_rid.wrstream (os);
      d_kid.wrstream (os);
      d_type.wrstream (os);
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this blob whatis

  void Whatis::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the taggable
      Taggable::rdstream (is);
      // deserialize locally
      d_rid.rdstream (is);
      d_kid.rdstream (is);
      d_type.rdstream (is);
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the whatis blob is valid

  bool Whatis::valid (void) const {
    rdlock ();
    try {
      bool result = !(d_rid.isnil () || d_kid.isnil ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the blob rid

  String Whatis::getrid (void) const {
    rdlock ();
    try {
      String result = d_rid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the blob kid

  Uuid Whatis::getkid (void) const {
    rdlock ();
    try {
      Uuid result = d_kid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the blob type

  String Whatis::gettype (void) const {
    rdlock ();
    try {
      String result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // get the creation time

  t_long Whatis::getctim (void) const {
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
  
  // get the modification time

  t_long Whatis::getmtim (void) const {
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

  // get the whatis list

  Plist Whatis::getplst (void) const {
    rdlock ();
    try {
      // get the local plist
      Plist result;
      result.add (PN_PRT_NAME, PI_PRT_NAME, d_name);
      result.add (PN_PRT_INFO, PI_PRT_INFO, d_info);
      result.add (PN_BLB_XRID, PI_BLB_XRID, d_rid);
      result.add (PN_PRT_UUID, PI_PRT_UUID, d_kid.tostring ());
      result.add (PN_BLB_TYPE, PI_BLB_TYPE, d_type);
      if (d_ctim > 0) {
	result.add (PN_BLB_CTIM, PI_BLB_CTIM, Date(d_ctim).toiso(true));
      }
      if (d_mtim > 0) {
	result.add (PN_BLB_MTIM, PI_BLB_MTIM, Date(d_mtim).toiso(true));
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  static const long QUARK_GETRID  = zone.intern ("get-rid");
  static const long QUARK_GETKID  = zone.intern ("get-kid");
  static const long QUARK_GETTYPE = zone.intern ("get-type");
  static const long QUARK_GETPLST = zone.intern ("get-plist");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");

  // create a new object in a generic way

  Object* Whatis::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Whatis;
    // invalid arguments
    throw Exception ("argument-error",
                     "too many argument with domain constructor");
  }

  // return true if the given quark is defined
  
  bool Whatis::isquark (const long quark, const bool hflg) const {
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
  
  Object* Whatis::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETRID)  return new String  (getrid  ());
      if (quark == QUARK_GETKID)  return new Uuid    (getkid  ());
      if (quark == QUARK_VALIDP)  return new Boolean (valid   ());
      if (quark == QUARK_GETTYPE) return new String  (gettype ());
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
      if (quark == QUARK_GETPLST) return new Plist   (getplst ());
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
