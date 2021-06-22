// ---------------------------------------------------------------------------
// - Uuid.cpp                                                                -
// - afnix:csm service - universal unique id class implementation            -
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

#include "Uuid.hpp"
#include "Nwgsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Utility.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the uuid size
  static const long UUID_SIZE_DEF = 16L;
  // the null uuid
  static const t_byte UUID_NULL_DEF[UUID_SIZE_DEF] = {
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
  };
    
  // this procedure duplicate a uuid
  static inline t_byte* uuid_byte_dup (const t_byte* uuid) {
    // check for nil
    if (uuid == nullptr) return nullptr;
    // copy the byte array
    t_byte* result = new t_byte[UUID_SIZE_DEF];
    for (long k = 0L; k < UUID_SIZE_DEF; k++) {
      result[k] = uuid[k];
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil uuid
  
  Uuid::Uuid (void) {
    p_uuid = nullptr;
    reset ();
  }

  // create a uuid by string value
  
  Uuid::Uuid (const String& uuid) {
    p_uuid = new t_byte[UUID_SIZE_DEF];
    parse (uuid);
  }

  // copy construct this uuid

  Uuid::Uuid (const Uuid& that) {
    that.rdlock ();
    try {
      p_uuid = uuid_byte_dup (that.p_uuid);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // move construct this uuid

  Uuid::Uuid (Uuid&& that) {
    that.wrlock ();
    try {
      p_uuid = that.p_uuid; that.p_uuid = nullptr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this uuid

  Uuid::~Uuid (void) {
    delete [] p_uuid;
  }
  
  // assign an uuid to this one

  Uuid& Uuid::operator = (const Uuid& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      p_uuid = uuid_byte_dup (that.p_uuid);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move an uuid to this one

  Uuid& Uuid::operator = (Uuid&& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      p_uuid = that.p_uuid; that.p_uuid = nullptr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // compare two uuid

  bool Uuid::operator == (const Uuid& uuid) const {
    rdlock ();
    uuid.rdlock ();
    try {
      // find the right uuid
      auto xuid = (p_uuid == nullptr) ? UUID_NULL_DEF : p_uuid;
      auto yuid = (uuid.p_uuid == nullptr) ? UUID_NULL_DEF : uuid.p_uuid;
      // compute result
      bool result = true;
      for (long k = 0L; k < UUID_SIZE_DEF; k++) {
	if (xuid[k] != yuid[k]) {
	  result = false;
	  break;
	}
      }
      unlock ();
      uuid.unlock ();
      return result;
    } catch (...) {
      unlock ();
      uuid.unlock ();
      throw;
    }
  }

  // compare two uuid

  bool Uuid::operator == (const String& suid) const {
    rdlock ();
    try {
      // parse the string uuid
      Uuid uuid = suid;
      // comprae by uuid
      bool result = (*this == uuid);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare two uuid

  bool Uuid::operator != (const Uuid& uuid) const {
    rdlock ();
    uuid.rdlock ();
    try {
      bool result = !(*this == uuid);
      unlock ();
      uuid.unlock ();
      return result;
    } catch (...) {
      unlock ();
      uuid.unlock ();
      throw;
    }
  }

  // compare two uuid

  bool Uuid::operator != (const String& suid) const {
    rdlock ();
    try {
      bool result = !(*this == suid);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the uuid class name

  String Uuid::repr (void) const {
    return "Uuid";
  }

  // return a clone of this object

  Object* Uuid::clone (void) const {
    return new Uuid (*this);
  }
  
  // return the serial did

  t_word Uuid::getdid (void) const {
    return SRL_DEOD_NWG;
  }

  // return the serial sid

  t_word Uuid::getsid (void) const {
    return SRL_UUID_SID;
  }
  
  // serialize this uuid

  void Uuid::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      if (p_uuid == nullptr) {
	os.write ((const char*) UUID_NULL_DEF, UUID_SIZE_DEF);
      } else {
	os.write ((const char*) p_uuid, UUID_SIZE_DEF);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this uuid

  void Uuid::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reload uuid
      if (p_uuid == nullptr) p_uuid = new t_byte[UUID_SIZE_DEF];
      for (long k = 0L; k < UUID_SIZE_DEF; k++) p_uuid[k] = is.read ();
      // check for null
      if (isnil () == true) reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this uuid

  void Uuid::reset (void) {
    wrlock ();
    try {
      delete [] p_uuid; p_uuid = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the uuid is null

  bool Uuid::isnil (void) const {
    rdlock ();
    try {
      // check for base null
      if (p_uuid == nullptr) {
	unlock ();
	return true;
      }
      for (long k = 0L; k < UUID_SIZE_DEF; k++) {
	if (p_uuid[k] != nilc) {
	  unlock ();
	  return false;
	}
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // generate a random uuid

  void Uuid::random (void) {
    wrlock ();
    try {
      // allocate if needed
      if (p_uuid == nullptr) p_uuid = new t_byte[UUID_SIZE_DEF];
      // generate random byte
      for (long k = 0L; k < UUID_SIZE_DEF; k++){
	p_uuid[k] = Utility::byternd ();
      }
      // fix version according to rfc 4122
      p_uuid[6] = 0x40U | (p_uuid[6] & 0x0FU);
      p_uuid[8] = 0x80U | (p_uuid[8] & 0x3FU);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a string uuid

  void Uuid::parse (const String& uuid) {
    wrlock ();
    try {
      // check for nil
      if (p_uuid == nullptr) p_uuid = new t_byte[UUID_SIZE_DEF];
      // check valid hyphen
      if ((uuid[8]  != '-') || (uuid[13] != '-') ||
	  (uuid[18] != '-') || (uuid[23] != '-')) {
	throw Exception ("uuid-error", "invalid uuid literal", uuid);
      }
      // 00112233-4455-6677-8899-AABBCCDDEEFF
      p_uuid[0]   = Utility::tobyte(uuid[0]) << 4;
      p_uuid[0]  |= Utility::tobyte(uuid[1]);
      p_uuid[1]   = Utility::tobyte(uuid[2]) << 4;
      p_uuid[1]  |= Utility::tobyte(uuid[3]);
      p_uuid[2]   = Utility::tobyte(uuid[4]) << 4;
      p_uuid[2]  |= Utility::tobyte(uuid[5]);
      p_uuid[3]   = Utility::tobyte(uuid[6]) << 4;
      p_uuid[3]  |= Utility::tobyte(uuid[7]);
      p_uuid[4]   = Utility::tobyte(uuid[9]) << 4;
      p_uuid[4]  |= Utility::tobyte(uuid[10]);
      p_uuid[5]   = Utility::tobyte(uuid[11]) << 4;
      p_uuid[5]  |= Utility::tobyte(uuid[12]);
      p_uuid[6]   = Utility::tobyte(uuid[14]) << 4;
      p_uuid[6]  |= Utility::tobyte(uuid[15]);
      p_uuid[7]   = Utility::tobyte(uuid[16]) << 4;
      p_uuid[7]  |= Utility::tobyte(uuid[17]);
      p_uuid[8]   = Utility::tobyte(uuid[19]) << 4;
      p_uuid[8]  |= Utility::tobyte(uuid[20]);
      p_uuid[9]   = Utility::tobyte(uuid[21]) << 4;
      p_uuid[9]  |= Utility::tobyte(uuid[22]);
      p_uuid[10]  = Utility::tobyte(uuid[24]) << 4;
      p_uuid[10] |= Utility::tobyte(uuid[25]);
      p_uuid[11]  = Utility::tobyte(uuid[26]) << 4;
      p_uuid[11] |= Utility::tobyte(uuid[27]);
      p_uuid[12]  = Utility::tobyte(uuid[28]) << 4;
      p_uuid[12] |= Utility::tobyte(uuid[29]);
      p_uuid[13]  = Utility::tobyte(uuid[30]) << 4;
      p_uuid[13] |= Utility::tobyte(uuid[31]);
      p_uuid[14]  = Utility::tobyte(uuid[32]) << 4;
      p_uuid[14] |= Utility::tobyte(uuid[33]);
      p_uuid[15]  = Utility::tobyte(uuid[34]) << 4;
      p_uuid[15] |= Utility::tobyte(uuid[35]);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // convert a uuid to a string

  String Uuid::tostring (void) const {
    rdlock ();
    try {
      // find the right uuid
      auto uuid = (p_uuid == nullptr) ? UUID_NULL_DEF : p_uuid;
      // convert to a string
      String result;
      for (long k = 0L; k < UUID_SIZE_DEF; k++) {
	if ((k == 4L) || (k == 6L) || (k == 8L) || (k == 10L)) result += '-';
	result += Utility::tohexa (uuid[k], true, false);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the hash quad of the uuid

  t_quad Uuid::hashq (void) const {
    rdlock ();
    try {
      t_quad result = Utility::hashq (p_uuid, UUID_SIZE_DEF);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute the hash octa of the uuid

  t_octa Uuid::hasho (void) const {
    rdlock ();
    try {
      t_octa result = Utility::hasho (p_uuid, UUID_SIZE_DEF);
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
  static const long QUARK_NILP     = zone.intern ("nil-p");
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_PARSE    = zone.intern ("parse");
  static const long QUARK_RANDOM   = zone.intern ("random");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way

  Object* Uuid::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Uuid;
    // check for 1 argument
    if (argc == 1) {
      String uuid = argv->getstring (0);
      return new Uuid (uuid);
    }
    throw Exception ("argument-error",
                     "too many argument with uuid constructor");
  }

  // return true if the given quark is defined

  bool Uuid::isquark (const long quark, const bool hflg) const {
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
  
  Object* Uuid::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_NILP) return new Boolean (isnil ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_RANDOM) {
	random ();
	return nullptr;
      }
      if (quark == QUARK_TOSTRING) {
	return new String (tostring ());
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	String uuid = argv->getstring (0);
	parse (uuid);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
