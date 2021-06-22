// ---------------------------------------------------------------------------
// - Codec.cpp                                                               -
// - afnix:sec module - abstract codec class implementation                  -
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

#include "Codec.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "InputOutput.hpp"
#include "OutputBuffer.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // remove the line border from a serialized buffer
  static Buffer cdc_rm_lbdr (Buffer& ibuf) {
    // remove first any ws/cr/lf
    while (ibuf.empty() == false) {
      char c = ibuf.get ();
      if ((c == ' ') || (c == '\t') || (c == '\r') || (c == '\n')) {
	ibuf.read ();
	continue;
      }
      break;
    }
    // now remove the line header if any
    if (ibuf.get () == '-') {
      while (ibuf.empty () == false) {
	if (ibuf.read () == eolc) break;
      }
    }
    // now copy the result buffer
    Buffer result;
    while (ibuf.empty () == false) {
      char c = ibuf.read ();
      if (c == '-') break;
      result.add (c);
    }
    // now remove the line footer if any
    while (ibuf.empty () == false) {
      if (ibuf.read () == eolc) break;
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default codec

  Codec::Codec (void) {
    reset ();
  }

  // create a codec by name

  Codec::Codec (const String& name) {
    d_name = name;
    reset ();
  }

  // return the codec name
    
  String Codec::getname (void) const {
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

  // reset this codec

  void Codec::reset (void) {
    wrlock ();
    try {
      d_rflg = false;
      d_encs[0] = 0L; d_encs[1] = 0L;
      d_decs[0] = 0L; d_decs[1] = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear this codec

  void Codec::clear (void) {
    wrlock ();
    try {
      d_encs[1] = 0L;
      d_decs[1] = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the codec reverse flag

  void Codec::setrflg (const bool rflg) {
    wrlock ();
    try {
      d_rflg = rflg;
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the codec reverse flag

  bool Codec::getrflg (void) const {
    rdlock ();
    try {
      bool result = d_rflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // stream an input buffer into an output buffer

  long Codec::stream (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // preset the stream
      long result = preset ();
      // stream in mode
      while (ib.empty() == false) {
	result += d_rflg ? decode (ob, ib) : encode (ob, ib);
      }
      // finish the stream procesing
      result += finish ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // stream an input stream into an output buffer

  long Codec::stream (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // preset the stream
      long result = preset ();
      // stream in mode
      while (is.valid () == true) {
	result += d_rflg ? decode (ob, is) : encode (ob, is);
      }
      // finish the stream procesing
      result += finish ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // stream an input stream into an output stream

  long Codec::stream (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // preset the stream
      long result = preset();
      // stream in mode
      while (is.valid () == true) {
	result += d_rflg ? decode (os, is) : encode (os, is);
      }
      // finish the stream procesing
      result += finish ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // serialize an object into a buffer with line border

  Buffer Codec::serialize (Object* sobj) {
    wrlock ();
    try {
      Buffer result = serialize (sobj, false);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // serialize an object into a buffer with line border

  Buffer Codec::serialize (Object* sobj, const bool lbdr) {
    wrlock ();
    try {
      // check for encoding mode
      if (d_rflg == true) {
	throw Exception ("codec-error", "invalid reverse mode with serialize");
      }
      // check for a serializable object
      auto so = dynamic_cast<Serial*>(sobj);
      if (so == nullptr) {
	throw Exception ("codec-error", "nil object with serialize");
      }
      // serialize the object into a buffer
      OutputBuffer os; so->serialize (os);
      Buffer ib = os.tobuffer ();
      // buffer result
      Buffer result;
      // add a line header if requested
      if (lbdr == true) {
	String line = "-----BEGIN AFNIX SERIALIZE ";
	line += sobj->repr().toupper();
	line += "-----\n";
	result.add (line);
      }
      // stream the buffer to an output buffer
      stream (result, ib);
      // add a line header if requested
      if (lbdr == true) {
	String line = "\n-----END AFNIX SERIALIZE ";
	line += sobj->repr().toupper();
	line += "-----";
	result.add (line);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // unserialize a buffer into an object

  Object* Codec::unserialize (Buffer& ibuf) {
    wrlock ();
    try {
      Object* result = unserialize (ibuf, false);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // unserialize a buffer into an object
  
  Object* Codec::unserialize (Buffer& ibuf, const bool lbdr) {
    wrlock ();
    try {
      // check for encoding mode
      if (d_rflg == false) {
	throw Exception ("codec-error", "invalid mode with unserialize");
      }
      // stream the input buffer
      Buffer ob;
      if (lbdr == true) {
	Buffer ib = cdc_rm_lbdr (ibuf);
	stream (ob, ib);
      } else {
	stream (ob, ibuf);
      }
      // deserialize the object
      InputOutput io (ob);
      Object* result = Serial::deserialize (io);
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
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_SERLZ   = zone.intern ("serialize");
  static const long QUARK_UNRLZ   = zone.intern ("unserialize");
  static const long QUARK_STREAM  = zone.intern ("stream");
  static const long QUARK_SETRFLG = zone.intern ("set-reverse");
  static const long QUARK_GETRFLG = zone.intern ("get-reverse");

  // return true if the given quark is defined

  bool Codec::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Codec::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETRFLG) return new Boolean (getrflg ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETRFLG) {
	bool rflg = argv->getbool (0);
	setrflg (rflg);
	return nullptr;
      }
      if (quark == QUARK_SERLZ) {
	Object* sobj = argv->get (0);
	return new Buffer (serialize (sobj));
      }
      if (quark == QUARK_UNRLZ) {
	Object* obj = argv->get (0);
	auto ibuf = dynamic_cast<Buffer*>(obj);
	if (ibuf == nullptr) {
	  throw Exception ("type-error", "invalid object with unserialize",
			   Object::repr (obj));
	}
	return unserialize (*ibuf);
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_STREAM) {
	// get the first object
	Object* obj = argv->get (0);
	// check for a buffer
	Buffer* ob = dynamic_cast <Buffer*> (obj);
	if (ob != nullptr) {
	  // get the second object
	  obj = argv->get (1);
	  // check for a buffer
	  Buffer* ib = dynamic_cast <Buffer*> (obj);
	  if (ib != nullptr) return new Integer (stream (*ob, *ib));
	  // check for an input stream
	  InputStream* is = dynamic_cast <InputStream*> (obj);
	  if (is != nullptr) return new Integer (stream (*ob, *is));
	  // type error
	  throw Exception ("type-error", "invalid object for codec stream",
			   Object::repr (obj));
	}
	// check for a stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  // get the second object
	  obj = argv->get (1);
	  // check for an input stream
	  InputStream* is = dynamic_cast <InputStream*> (obj);
	  if (is != nullptr) return new Integer (stream (*os, *is));
	  // type error
	  throw Exception ("type-error", "invalid object for codec stream",
			   Object::repr (obj));
	}
	// invalid object
	throw Exception ("type-error", "invalid object for codec stream",
			 Object::repr (obj));
      }
      if (quark == QUARK_SERLZ) {
	Object* sobj = argv->get (0);
	bool lbdr = argv->getbool (1);
	return new Buffer (serialize (sobj, lbdr));
      }
      if (quark == QUARK_UNRLZ) {
	Object* obj = argv->get (0);
	auto ibuf = dynamic_cast<Buffer*>(obj);
	if (ibuf == nullptr) {
	  throw Exception ("type-error", "invalid object with unserialize",
			   Object::repr (obj));
	}
	bool lbdr = argv->getbool (1);
	return unserialize (*ibuf, lbdr);
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
