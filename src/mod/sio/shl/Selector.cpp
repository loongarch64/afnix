// ---------------------------------------------------------------------------
// - Selector.cpp                                                            -
// - afnix:sio module - i/o select class implementation                      -
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

#include "Error.hpp"
#include "Interp.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Selector.hpp"
#include "csio.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty selector

  Selector::Selector (void) {
    p_handle = c_shnew (false);
  }

  // create a selector by mode

  Selector::Selector (const bool mflg) {
    p_handle = c_shnew (mflg);
  }

  // destroy this selector

  Selector::~Selector (void) {
    c_shfree (p_handle);
  }

  // return the class name

  String Selector::repr (void) const{
    return "Selector";
  }

  // add a new input stream

  void Selector::add (InputStream* is) {
    if (is == nullptr) return;
    wrlock ();
    try {
      if (d_isv.exists (is) == false) {
	d_isv.add (is);
	c_shiadd  (p_handle, is->getsid ());
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new output stream

  void Selector::add (OutputStream* os) {
    if (os == nullptr) return;
    wrlock ();
    try {
      if (d_osv.exists (os) == false) {
	d_osv.add (os);
	c_shoadd  (p_handle, os->getsid ());
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of input streams

  long Selector::ilength (void) const {
    rdlock ();
    try {
      long result = d_isv.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of output streams

  long Selector::olength (void) const {
    rdlock ();
    try {
      long result = d_osv.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an input stream by index

  InputStream* Selector::iget (const long index) const {
    rdlock ();
    try {
      InputStream* result = dynamic_cast <InputStream*> (d_isv.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an output stream by index

  OutputStream* Selector::oget (const long index) const {
    rdlock ();
    try {
      OutputStream* result = dynamic_cast <OutputStream*> (d_osv.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // wait for one stream to be ready

  Stream* Selector::wait (const long tout) const {
    wrlock ();
    try {
      // look into the input pushback buffer
      long len = d_isv.length ();
      for (long i = 0; i < len; i++) {
	InputStream* is = dynamic_cast <InputStream*> (d_isv.get (i));
	if (is == nullptr) continue;
	if (is->buflen () != 0) {
	  unlock ();
	  return is;
	}
      }
      // now wait for a descriptor to be ready
      long nsid = c_shwait (p_handle, tout);
      // check for error first
      if (nsid < 0) throw Error ("selector-error", c_errmsg (nsid), nsid);
      // check the input descriptors first
      for (long i = 0; i < len; i++) {
	InputStream* is = dynamic_cast <InputStream*> (d_isv.get (i));
	if (is == nullptr) continue;
	if (c_shitst (p_handle, is->getsid ()) == true) {
	  unlock ();
	  return is;
	}
      }
      // check the output descriptors
      len = d_osv.length ();
      for (long i = 0; i < len; i++) {
	OutputStream* os = dynamic_cast <OutputStream*> (d_osv.get (i));
	if (os == nullptr) continue;
	if (c_shotst (p_handle, os->getsid ()) == true) {
	  unlock ();
	  return os;
	}
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get all ready streams

  Vector* Selector::waitall (const long tout) const {
    // create a result vector
    Vector* result = new Vector;
    // lock and test
    wrlock ();
    try {
      // look into the input pushback buffer
      long len = d_isv.length ();
      for (long i = 0; i < len; i++) {
	InputStream* is = dynamic_cast <InputStream*> (d_isv.get (i));
	if (is == nullptr) continue;
	if (is->buflen () != 0) result->add (is);
      }
      // if we have something we unlock and return
      if (result->length () != 0) {
	unlock ();
	return result;
      }
      // now wait for a descriptor to be ready
      long nsid = c_shwait (p_handle, tout);
      // check for error first
      if (nsid < 0) throw Error ("selector-error", c_errmsg (nsid), nsid);
      // check the input descriptors first
      for (long i = 0; i < len; i++) {
	InputStream* is = dynamic_cast <InputStream*> (d_isv.get (i));
	if (is == nullptr) continue;
	if (c_shitst (p_handle, is->getsid ()) == true) result->add (is);
      }
      // check the output descriptors
      len = d_osv.length ();
      for (long i = 0; i < len; i++) {
	OutputStream* os = dynamic_cast <OutputStream*> (d_osv.get (i));
	if (os == nullptr) continue;
	if (c_shotst (p_handle, os->getsid ()) == true) result->add (os);
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // mark a selector

  void Selector::mark (void) {
    d_mmon.enter ();
    try {
      c_shmark (p_handle);
      d_mmon.leave ();
    } catch (...) {
      d_mmon.leave ();
      throw;
    }
  }

  // check if a selector is marked

  bool Selector::ismarked (void) const {
    d_mmon.enter ();
    try {
      bool result = c_shmtst (p_handle);
      d_mmon.leave ();
      return result;
    } catch (...) {
      d_mmon.leave ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_WAIT    = zone.intern ("wait");
  static const long QUARK_MARK    = zone.intern ("mark");
  static const long QUARK_IADD    = zone.intern ("input-add");
  static const long QUARK_OADD    = zone.intern ("output-add");
  static const long QUARK_IGET    = zone.intern ("input-get");
  static const long QUARK_OGET    = zone.intern ("output-get");
  static const long QUARK_MARKEDP = zone.intern ("marked-p");
  static const long QUARK_ILENGTH = zone.intern ("input-length");
  static const long QUARK_OLENGTH = zone.intern ("output-length");
  static const long QUARK_WAITALL = zone.intern ("wait-all");

  // create a new object in a generic way

  Object* Selector::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Selector;
    // initialize selector mode and indexes
    long    i = 0;
    bool mflg = false;
    // check for a boolean
    Object*   obj = argv->get (0);
    Boolean* bobj = dynamic_cast <Boolean*> (obj);
    if (bobj != nullptr) {
      mflg = bobj->tobool ();
      i = 1;
    }
    // create a selector by mode
    Selector* result = new Selector (mflg);
    // try to add streams
    for (; i < argc; i++) {
      // get the next object
      obj = argv->get (i);
      // check for an input stream
      InputStream*  is  = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) {
	result->add (is);
	continue;
      }
      // check for an output stream
      OutputStream* os = dynamic_cast <OutputStream*> (obj);
      if (os != nullptr) {
	result->add (os);
	continue;
      }
      delete result;
      throw Exception ("type-error", "input or output stream expected");
    }
    return result;
  }

  // return true if the given quark is defined

  bool Selector::isquark (const long quark, const bool hflg) const {
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

  Object* Selector::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
 
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_ILENGTH) return new Integer (ilength  ());
      if (quark == QUARK_OLENGTH) return new Integer (olength  ());
      if (quark == QUARK_MARKEDP) return new Boolean (ismarked ());
      if (quark == QUARK_MARK) {
	mark ();
	return nullptr;
      }
      if (quark == QUARK_WAIT) {
	wrlock ();
	try{
	  Object* result = wait (-1);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_WAITALL) {
	wrlock ();
	try{
	  Object* result = waitall (-1);
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
      if (quark == QUARK_WAIT) {
	long tout = argv->getlong (0);
	wrlock ();
	try {
	  Object* result = wait (tout);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_WAITALL) {
	long tout = argv->getlong (0);
	wrlock ();
	try {
	  Object* result = waitall (tout);
	  zobj->post (result);
	  unlock();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	InputStream* is  = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  add (is);
	  return nullptr;
	}
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  add (os);
	  return nullptr;
	}
	throw Exception ("type-error", "input or output stream expected");
      }
      if (quark == QUARK_IADD) {
	Object* obj = argv->get (0);
	InputStream* is  = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "input or output stream expected");
	}
	add (is);
	return nullptr;
      }
      if (quark == QUARK_OADD) {
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "input or output stream expected");
	}
	add (os);
	return nullptr;
      }
      if (quark == QUARK_IGET) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = iget (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_OGET) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = oget (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the object method 
    return Object::apply (zobj, nset, quark, argv);
  }
}
