// ---------------------------------------------------------------------------
// - Blob.cpp                                                                -
// - afnix:csm service - registered blob class implementation                -
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

#include "Blob.hpp"
#include "Date.hpp"
#include "Whom.hxx"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the performance plist
  static const String PN_PRF_RSTM = "PN-PRF-RSTM";
  static const String PI_PRF_RSTM = "BLOB RDSTREAM COUNTER";
  static const String PN_PRF_WSTM = "PN-PRF-WSTM";
  static const String PI_PRF_WSTM = "BLOB WRSTREAM COUNTER";
  static const String PN_PRF_HEAD = "PN-PRF-HEAD";
  static const String PI_PRF_HEAD = "BLOB GETHEAD COUNTER";
  static const String PN_PRF_PLST = "PN-PRF-PLST";
  static const String PI_PRF_PLST = "BLOB GETPLST COUNTER";
  
  // the performance counters
  struct s_perf {
    // the rdstream counter
    long d_rstm;
    // the wrstream counter
    long d_wstm;
    // the gethead counter
    long d_head;
    // the getplst counter
    long d_plst;
    s_perf (void) {
      d_rstm = 0L;
      d_wstm = 0L;
      d_head = 0L;
      d_plst = 0L;
    }
    // get a performance plist
    Plist getplst (void) const {
      Plist result;
      result.add (PN_PRF_RSTM, PI_PRF_RSTM, (t_long) d_rstm);
      result.add (PN_PRF_WSTM, PI_PRF_WSTM, (t_long) d_wstm);
      result.add (PN_PRF_HEAD, PI_PRF_HEAD, (t_long) d_head);
      result.add (PN_PRF_PLST, PI_PRF_PLST, (t_long) d_plst);
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default blob

  Blob::Blob (void) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
    d_tflg = false;
    p_perf = new s_perf;
  }

  // create an blob by name

  Blob::Blob (const String& name) : Part (name) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
    d_tflg = false;
    p_perf = new s_perf;
  }

  // create an blob by name and info

  Blob::Blob (const String& name, const String& info) : Part (name, info) {
    d_rid.clear ();
    d_ctim = 0L;
    d_mtim = 0L;
    d_tflg = false;
    p_perf = new s_perf;
  }

  // create an blob by rid, name and info

  Blob::Blob (const String& rid,
	      const String& name, const String& info) : Part (name, info) {
    d_ctim = 0L;
    d_mtim = 0L;
    d_tflg = false;
    p_perf = new s_perf;
    setrid (rid);
  }

  // copy construct this blob

  Blob::Blob (const Blob& that) {
    that.rdlock ();
    try {
      // copy base part
      Part::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      d_tflg = that.d_tflg;
      // allocate locally
      p_perf = new s_perf;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this blob

  Blob::~Blob (void) {
    delete p_perf;
  }
  
  // assign an blob to this one

  Blob& Blob::operator = (const Blob& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base part
      Part::operator = (that);
      // copy locally
      d_rid  = that.d_rid;
      d_ctim = that.d_ctim;
      d_mtim = that.d_mtim;
      d_tflg = that.d_tflg;
      // allocate locally
      p_perf = new s_perf;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the blob class name

  String Blob::repr (void) const {
    return "Blob";
  }

  // return a clone of this object

  Object* Blob::clone (void) const {
    return new Blob (*this);
  }

  // return the serial did

  t_word Blob::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Blob::getsid (void) const {
    return SRL_BLOB_SID;
  }
  
  // serialize this blob

  void Blob::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the part
      Part::wrstream (os);
      // serialize locally
      d_rid.wrstream (os);
      Serial::wrlong (d_ctim, os);
      Serial::wrlong (d_mtim, os);
      Serial::wrbool (d_tflg, os);
      // update counters
      p_perf->d_wstm++;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this blob

  void Blob::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the part
      Part::rdstream (is);
      // deserialize locally
      d_rid.rdstream (is);
      d_ctim = Serial::rdlong (is);
      d_mtim = Serial::rdlong (is);
      d_tflg = Serial::rdbool (is);
      // update counters
      p_perf->d_rstm++;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // request an information list

  Plist Blob::getplst (void) const {
    rdlock ();
    try {
      Plist result = Part::getplst ();
      result.add (PN_BLB_TYPE, PI_BLB_TYPE, repr().toupper());
      result.add (PN_BLB_XRID, PI_BLB_XRID, d_rid);
      if (d_ctim > 0) {
	result.add (PN_BLB_CTIM, PI_BLB_CTIM, Date(d_ctim).toiso(true));
      }
      if (d_mtim > 0) {
	result.add (PN_BLB_MTIM, PI_BLB_MTIM, Date(d_mtim).toiso(true));
      }
      result.add (PN_BLB_TFLG, PI_BLB_TFLG, d_tflg);
      // update counters
      p_perf->d_plst++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check is this blob is swappable

  bool Blob::isswap (Part* part) const {
    rdlock ();
    try {
      // check base part
      bool status = Part::isswap (part);
      // check for a blob
      if (status == true) {
	// check for a blob
	auto blob = dynamic_cast<Blob*>(part);
	if (blob == nullptr) status = false;
	// check for valid rid
	if (status == true) status = (blob->getrid () == d_rid);
      }
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the blob creation time

  t_long Blob::getctim (void) const {
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
  
  // get the blob modification time

  t_long Blob::getmtim (void) const {
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
  
  // set the blob transient flag

  bool Blob::settflg (const bool tflg) {
    wrlock ();
    try {
      bool result = d_tflg; d_tflg = tflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the blob transient flag

  bool Blob::gettflg (void) const {
    rdlock ();
    try {
      bool result = d_tflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a blob registration id

  bool Blob::isrid (void) const {
    rdlock ();
    try {
      bool result = d_rid.isnil () ? false : true;
      unlock ();
      return result;
    } catch  (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a blob registration id

  bool Blob::isrid (const String& rid) const {
    rdlock ();
    try {
      bool result = (d_rid == rid);
      unlock ();
      return result;
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // set the blob registration id

  void Blob::setrid (const String& rid) {
    wrlock ();
    try {
      // set the rid
      d_rid = rid;
      // update the times
      if (d_ctim == 0L) {
        d_ctim = Time::gettclk ();
        d_mtim = d_ctim;
      } else {
        d_mtim = Time::gettclk ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the blob registration id

  String Blob::getrid (void) const {
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

  // get the blob whatis

  Whatis Blob::whatis (void) const {
    rdlock ();
    try {
      Whatis wais;
      wais.setname (getname (), getinfo ());
      wais.d_rid  = getrid  ();
      wais.d_kid  = getkid  ();
      wais.d_type = repr().toupper();
      wais.d_ctim = getctim ();
      wais.d_mtim = getmtim ();
      unlock ();
      return wais;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_WHATIS  = zone.intern ("whatis");
  static const long QUARK_ISRIDP  = zone.intern ("rid-p");
  static const long QUARK_SETRID  = zone.intern ("set-rid");
  static const long QUARK_GETRID  = zone.intern ("get-rid");
  static const long QUARK_GETCTIM = zone.intern ("get-creation-time");
  static const long QUARK_GETMTIM = zone.intern ("get-modification-time");
  static const long QUARK_GETTFLG = zone.intern ("transient-p");
  static const long QUARK_SETTFLG = zone.intern ("set-transient-mode");
  
  // create a new object in a generic way

  Object* Blob::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default blob
    if (argc == 0) return new Blob;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Blob (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Blob (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String  rid = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new Blob (rid, name, info);
    }
    throw Exception ("argument-error",
                     "too many argument with blob constructor");
  }

  // return true if the given quark is defined

  bool Blob::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Part::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Blob::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_WHATIS)  return new Whatis  (whatis  ());
      if (quark == QUARK_GETRID)  return new String  (getrid  ());
      if (quark == QUARK_ISRIDP)  return new Boolean (isrid   ());
      if (quark == QUARK_GETCTIM) return new Integer (getctim ());
      if (quark == QUARK_GETMTIM) return new Integer (getmtim ());
      if (quark == QUARK_GETTFLG) return new Boolean (gettflg ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ISRIDP) {
	String rid = argv->getstring (0);
	return new Boolean (isrid (rid));
      }
      if (quark == QUARK_SETRID) {
	String rid = argv->getstring (0);
	setrid (rid);
	return nullptr;
      }
      if (quark == QUARK_SETTFLG) {
	bool tflg = argv->getbool (0);
	return new Boolean (settflg (tflg));
      }
    }
    // call the part method
    return Part::apply (zobj, nset, quark, argv);
  }
}
