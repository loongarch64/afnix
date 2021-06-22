// ---------------------------------------------------------------------------
// - Stream.cpp                                                              -
// - standard object library - base stream class implementation              -
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
#include "Stream.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default stream

  Stream::Stream (void) {
    d_emod = Encoding::EMOD_BYTE;
  }

  // create a stream by coding mode

  Stream::Stream (const Encoding::t_emod emod) {
    d_emod = emod;
  }

  // close this stream

  bool Stream::close (void) {
    wrlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // return the stream descriptor

  int Stream::getsid (void) const {
    return -1;
  }

  // return the stream encoding mode

  Encoding::t_emod Stream::getemod (void) const {
    rdlock ();
    try {
      Encoding::t_emod result = d_emod;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream encoding mode

  void Stream::setemod (const Encoding::t_emod emod) {
    wrlock ();
    try {
      // save mode
      d_emod = emod;
      // reset transcoder mode
      switch (emod) {
      case Encoding::EMOD_UTF8:
	settmod (Encoding::TMOD_NONE);
	break;
      default:
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream encoding mode by name

  void Stream::setemod (const String& mode) {
    // check for mode first
    if (mode.isnil () == true) return;
    // compute encoding and transcoder mode
    Encoding::t_emod emod = Encoding::toemod (mode);
    Encoding::t_tmod tmod = Encoding::totmod (mode);
    // set the modes
    wrlock ();
    try {
      settmod (tmod);
      setemod (emod);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the object eval quarks
  static const long QUARK_BYTE    = String::intern ("BYTE");
  static const long QUARK_UTF8    = String::intern ("UTF-8");
  static const long QUARK_STREAM  = String::intern ("Stream");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CLOSE   = zone.intern ("close");
  static const long QUARK_GETEMOD = zone.intern ("get-encoding-mode");
  static const long QUARK_SETEMOD = zone.intern ("set-encoding-mode");

  // map an enumeration item to a stream coding mode
  static inline Encoding::t_emod item_to_emod (const Item& item) {
    // check for a stream item 
    if (item.gettid () != QUARK_STREAM)
      throw Exception ("item-error", "item is not a stream item");
    // map the item to the enumeration
    long quark = item.getquark (); 
    if (quark == QUARK_BYTE) return Encoding::EMOD_BYTE;
    if (quark == QUARK_UTF8) return Encoding::EMOD_UTF8;
    throw Exception ("item-error", "cannot map item to stream encoding mode");
  }

  // map a coding mode to an item
  static inline Item* emod_to_item (const Encoding::t_emod emod) {
    if (emod == Encoding::EMOD_BYTE) return new Item (QUARK_STREAM, QUARK_BYTE);
    if (emod == Encoding::EMOD_UTF8) return new Item (QUARK_STREAM, QUARK_UTF8);
    throw Exception ("mode-error", "cannot map stream encoding mode to item");
  }

  // evaluate a quark statically

  Object* Stream::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_BYTE) 
      return new Item (QUARK_STREAM, QUARK_BYTE);
    if (quark == QUARK_UTF8) 
      return new Item (QUARK_STREAM, QUARK_UTF8);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Stream::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Transcoder::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Stream::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_CLOSE) return new Boolean (close ());
      if (quark == QUARK_GETEMOD) return emod_to_item (getemod ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETEMOD) {
	// check for an item
	Object*  obj = argv->get (0);
	Item*   item = dynamic_cast <Item*> (obj);
	if (item != nullptr) {	  
	  Encoding::t_emod emod = item_to_emod (*item);
	  setemod (emod);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {	  
	  setemod (*sobj);
	  return nullptr;
	}
	throw Exception ("argument-error", 
			 "invalid arguments with set-coding-mode");
      }
    }
    // apply these arguments with the transcoder
    return Transcoder::apply (zobj, nset, quark, argv);
  }
}
