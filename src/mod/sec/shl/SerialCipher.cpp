// ---------------------------------------------------------------------------
// - SerialCipher.cpp                                                        -
// - afnix:sec module - serial cipher class implementation                   -
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
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "SerialCipher.hpp"

namespace afnix {
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  static const long DEF_BLOK_SIZE = 4048;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a serial cipher by key

  SerialCipher::SerialCipher (const String& name) : Cipher (name) {
    d_sbsz = DEF_BLOK_SIZE;
  }

  // reset this serial cipher

  void SerialCipher::reset (void) {
    wrlock ();
    try {
      // reset base cipher
      Cipher::reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear this serial cipher

  void SerialCipher::clear (void) {
    wrlock ();
    try {
      // clear base cipher
      Cipher::clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the serial cipher key

  void SerialCipher::setkey (const Key& key) {
    wrlock ();
    try {
      // check the key for validity
      if (key.isck () == false) {
	throw Exception ("cipher-error", "invalid key for serial cipher");
      } 
      // set the key and return
      Cipher::setkey (key);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

    // set the serial block size

  void SerialCipher::setsbsz (const long sbsz) {
    wrlock ();
    try {
      d_sbsz = (sbsz <= 0) ? 0 : sbsz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // return the serial block size

  long SerialCipher::getsbsz (void) const {
    rdlock ();
    try {
      long result = d_sbsz;
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

  long SerialCipher::encode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }
      // initialize the result counter
      long cc = 0;
      if (d_sbsz == 0) {
	while (ib.empty () == false) {
	  // encode a byte
	  t_byte bo = encode (ib.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      } else {
	for (long i = 0; i < d_sbsz; i++) {
	  // check for a null buffer
	  if (ib.empty () == true) break;
	  // encode a byte
	  t_byte bo = encode (ib.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      }
      d_encs[0] += cc; d_encs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output buffer

  long SerialCipher::encode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }       // initialize the result counter
      long cc = 0;
      if (d_sbsz == 0) {
	while (is.valid () == true) {
	  // encode a byte
	  t_byte bo = encode (is.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      } else {
	for (long i = 0; i < d_sbsz; i++) {
	  // encode a byte
	  t_byte bo = encode (is.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      }
      d_encs[0] += cc; d_encs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode an input stream into an output stream

  long SerialCipher::encode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == true) {
	throw Exception ("cipher-error", "calling encode in reverse mode");
      }       // initialize the result counter
      long cc = 0;
      if (d_sbsz == 0) {
	while (is.valid () == true) {
	  // encode a byte
	  t_byte bo = encode (is.read ());
	  // write the byte
	  os.write ((char) bo);
	  cc++;
	}
      } else {
	for (long i = 0; i < d_sbsz; i++) {
	  // encode a byte
	  t_byte bo = encode (is.read ());
	  // write the byte
	  os.write ((char) bo);
	  cc++;
	}
      }
      d_encs[0] += cc; d_encs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input buffer into an output buffer

  long SerialCipher::decode (Buffer& ob, Buffer& ib) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }
      // initialize the result counter
      long cc = 0;
      if (d_sbsz == 0) {
	while (ib.empty () == false) {
	  // encode a byte
	  t_byte bo = decode (ib.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      } else {
	for (long i = 0; i < d_sbsz; i++) {
	  // check for a null buffer
	  if (ib.empty () == true) break;
	  // encode a byte
	  t_byte bo = decode (ib.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      }
      d_decs[0] += cc; d_decs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output buffer

  long SerialCipher::decode (Buffer& ob, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }
      // initialize the result counter
      long cc = 0;
      if (d_sbsz == 0) {
	while (is.valid () == true) {
	  // encode a byte
	  t_byte bo = decode (is.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      } else {
	for (long i = 0; i < d_sbsz; i++) {
	  // encode a byte
	  t_byte bo = decode (is.read ());
	  // write the byte
	  ob.add ((char) bo);
	  cc++;
	}
      }
      d_decs[0] += cc; d_decs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an input stream into an output stream

  long SerialCipher::decode (OutputStream& os, InputStream& is) {
    wrlock ();
    try {
      // check valid mode
      if (d_rflg == false) {
	throw Exception ("cipher-error", "calling decode in normal mode");
      }
      // initialize the result counter
      long cc = 0;
      if (d_sbsz == 0) {
	while (is.valid () == true) {
	  // encode a byte
	  t_byte bo = decode (is.read ());
	  // write the byte
	  os.write ((char) bo);
	  cc++;
	}
      } else {
	for (long i = 0; i < d_sbsz; i++) {
	  // encode a byte
	  t_byte bo = decode (is.read ());
	  // write the byte
	  os.write ((char) bo);
	  cc++;
	}
      }
      d_decs[0] += cc; d_decs[1] += cc;
      unlock ();
      return cc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish the stream processing

  long SerialCipher::preset (void) {
    wrlock ();
    try {
      long result = 0L;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // finish the stream processing

  long SerialCipher::finish (void) {
    wrlock ();
    try {
      long result = 0L;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETSBSZ = zone.intern ("get-serial-size");
  static const long QUARK_GETSBSZ = zone.intern ("get-serial-size");

  // return true if the given quark is defined

  bool SerialCipher::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Cipher::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* SerialCipher::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETSBSZ) return new Integer  (getsbsz ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETSBSZ) {
	long sbsz = argv->getlong (0);
	setsbsz (sbsz);
	return nullptr;
      }
    }
    // call the cipher method
    return Cipher::apply (zobj, nset, quark, argv);
  }
}
