// ---------------------------------------------------------------------------
// - TlsExtension.cpp                                                        -
// - afnix:tls service - tls extension class implementation                  -
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
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "TlsExtension.hpp"
#include "TlsExtension.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the extension name and info
  static const String PN_EXT_CODE  = "TLS-EXT-CODE";
  static const String PI_EXT_CODE  = "TLS EXTENSION CODE";
  static const String PN_EXT_SIZE  = "TLS-EXT-SIZE";
  static const String PI_EXT_SIZE  = "TLS EXTENSION SIZE";

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // create a vector of extension by date

  Vector* TlsExtension::toexts (const long size, const t_byte* data) {
    // check for null
    if ((size < 0L) || (data == nullptr)) return nullptr;
    // check valid size
    if (size < 2L) {
      throw Exception ("tls-error", "invalid extension byte data");
    }
    // get the extension block size
    long boff = 0L;
    long bsiz = ((long) data[boff++] << 8); bsiz += ((long) data[boff++]);
    if (bsiz + 2L != size) {
      throw Exception ("tls-error", "inconsistent extension data size");
    }
    if (bsiz == 0L) return nullptr;
    // buid the extension array
    Vector* result = new Vector;
    while (boff < size) {
      // get the extension code
      t_word code = ((t_word) data[boff++] << 8); code += (t_word) data[boff++];
      // the the extension size
      long esiz = ((long) data[boff++] << 8); esiz += ((long) data[boff++]);
      // add the extension
      result->add (new TlsExtension (code, esiz, &data[boff]));
      boff += esiz;
    }
    return result;
  }

  // decode an extension by vector and code

  s_extn* TlsExtension::decode (Vector* exts, const t_word code) {
    // check for nil
    if (exts == nullptr) return nullptr;
    // loop in the vector
    long elen = exts->length ();
    for (long k = 0L; k < elen; k++) {
      // get the extension
      auto extn = dynamic_cast<TlsExtension*>(exts->get(k));
      if (extn == nullptr) continue;
      // check by code and decode
      if (extn->getcode () == code) return extn->decode ();
    }
    // not found
    return nullptr;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default extension

  TlsExtension::TlsExtension (void) {
    d_code = 0x0000U;
    d_size = 0L;
    p_data = nullptr;
  }

  // create a extension by code and data

  TlsExtension::TlsExtension (const t_word code,
			      const long size, const t_byte* data) {
    d_code = code;
    d_size = size;
    p_data = nullptr;
    if ((size > 0L) && (data != nullptr)) {
      p_data = new t_byte[size];
      for (long k = 0L; k < size; k++) p_data[k] = data[k];
    }
  }
  
  // destroy this extension

  TlsExtension::~TlsExtension (void) {
    delete [] p_data;
  }

  // return the class name
  
  String TlsExtension::repr (void) const {
    return "TlsExtension";
  }

  // reset the 

  void TlsExtension::reset (void) {
    wrlock ();
    try {
      d_code = 0x0000U;
      d_size = 0L;
      delete [] p_data; p_data = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the extension info as a plist
  
  Plist TlsExtension::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      plst.add (PN_EXT_CODE, PI_EXT_CODE, (t_long) d_code);
      plst.add (PN_EXT_SIZE, PI_EXT_SIZE, (t_long) d_size);
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the extension code

  t_word TlsExtension::getcode (void) const {
    rdlock ();
    try {
      t_word code = d_code;
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the extension buffer

  Buffer* TlsExtension::getebuf (void) const {
    rdlock ();
    try {
      Buffer* result = new Buffer (d_size);
      result->add (reinterpret_cast<const char*>(p_data), d_size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode an extension

  s_extn* TlsExtension::decode (void) const {
    rdlock ();
    try {
      s_extn* result = tls_extn_xx (d_code, d_size, p_data);
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
  static const long QUARK_GETCODE = zone.intern ("get-code");
  static const long QUARK_GETEBUF = zone.intern ("get-buffer");

  // create a new object in a generic way

  Object* TlsExtension::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new TlsExtension;
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls cipher extension");
  }

  // return true if the given quark is defined

  bool TlsExtension::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsInfos::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* TlsExtension::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCODE) return new Integer (getcode ());
      if (quark == QUARK_GETEBUF) return getebuf ();
    }
    // call the tls infos method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
