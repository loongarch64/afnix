// ---------------------------------------------------------------------------
// - Serial.cpp                                                              -
// - standard object library - serializable object implementation            -
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

#include "Set.hpp"
#include "Byte.hpp"
#include "Cons.hpp"
#include "List.hpp"
#include "Real.hpp"
#include "Regex.hpp"
#include "Plist.hpp"
#include "Stdsrl.hxx"
#include "System.hpp"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Utility.hpp"
#include "Strfifo.hpp"
#include "Relatif.hpp"
#include "Boolean.hpp"
#include "Message.hpp"
#include "Character.hpp"
#include "NameTable.hpp"
#include "QuarkZone.hpp"
#include "PrintTable.hpp"
#include "InputStream.hpp"
#include "cmem.hpp"
#include "ccnv.hpp"
#include "OutputTerm.hpp"
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // The serialize code is a 32 bits code, organized into a 16 bits dispatch
  // code and a 16 bits serial code. A dispatch code is normally associated
  // with a module or a service. The maximum number of dispatch codes is 2^14,
  // with the last two bits as the version bits (default to version 00). The
  // 2 bits type (org[00], com[01], biz[10], usr[11]), 5 bits project code,
  // 2 bits library code (lib[00], mod[01], srv[10], usr[11]) and 5 bits unit
  // code compose the 14 bits did.

  // +-----------------------------------------+
  // | vvdd dddd dddd dddd ssss ssss ssss ssss | v:version d:did s:sid
  // +-----------------------------------------+
  // +-----------------------------------------+
  // | 00tt pppp pllu uuuu ssss ssss ssss ssss | t:type p:projet l:library
  // +-----------------------------------------+
  //   0000 0000 0ddd dddd ssss ssss ssss ssss   afnix
  //   0001 dddd dddd dddd ssss ssss ssss ssss   com
  //   0010 dddd dddd dddd ssss ssss ssss ssss   biz
  //   0011 dddd dddd dddd ssss ssss ssss ssss   usr
  
  // the default array size
  static const long      SRL_ARRY_SIZ = 1L << 14;
  // the maximum number of serial dispatcher
  static const long      SRL_DEOD_MAX = 1L << 14;
  // the dispatch id mask
  static const t_word    SRL_DEOD_MSK = 0xC000U;
  // the serial dispatcher table
  static Serial::t_deod* SRL_DEOD_TBL = nullptr;

  // the serial dispatcher deallocator
  static void del_deod_tbl (void) {
    delete [] SRL_DEOD_TBL;
    SRL_DEOD_TBL = nullptr;
  }

  // create the serial dispatcher table
  static void new_deod_tbl (void) {
    if (SRL_DEOD_TBL == nullptr) {
      SRL_DEOD_TBL = new Serial::t_deod[SRL_DEOD_MAX];
      for (long k = 0L; k < SRL_DEOD_MAX; k++) SRL_DEOD_TBL[k] = nullptr;
      c_gcleanup (del_deod_tbl);
    }
  }

  // check if a did is valid

  static bool chk_deod_tbl (const t_word did) {
    // make sure the table is here
    new_deod_tbl ();
    // check for a table
    if (SRL_DEOD_TBL == nullptr) return false;
    // check for valid dispatch id
    if ((did & SRL_DEOD_MSK) != 0x0000U) return false;
    // check for valid entry
    if (SRL_DEOD_TBL[did] == nullptr) return false;
    return true;
  }
  
  // add a serial dispatcher

  static void add_deod_tbl (const t_word did, Serial::t_deod deod) {
    // create the serial table
    new_deod_tbl ();
    // check for valid dispatch id
    if ((did & SRL_DEOD_MSK) != 0x0000U) {
      throw Exception ("serial-error", "out of range dispatch id");
    }
    // check for dispatcher value
    if (SRL_DEOD_TBL[did] != nullptr) {
      throw Exception ("serial-error", "dispatch id already set",
		       Utility::tohexa (did, true, true));
    }
    // set the serial dispatcher
    SRL_DEOD_TBL[did] = deod;
  }

  // get a serial object by did/sid
  static Serial* new_deod_obj (const t_word did, const t_word sid) {
    // check for valid dispatch id
    if ((did & SRL_DEOD_MSK) != 0x0000U) {
      throw Exception ("serial-error", "out of range dispatch id",
		       Utility::tohexa(did, true, true));
    }
    // get the dispatch by id
    Serial::t_deod deod = SRL_DEOD_TBL[did];
    // call by serial id
    return (deod == nullptr) ? nullptr : deod (sid);
  }

  // get a array data size by type
  static long srl_arry_dsz (const Serial::Array::t_btyp btyp) {
    long result = 0;
    switch (btyp) {
    case Serial::Array::ATYP_BYTE:
    case Serial::Array::ATYP_BOOL:
      result = sizeof (t_byte);
      break;
    case Serial::Array::ATYP_LONG:
      result = sizeof (t_long);
      break;
    case Serial::Array::ATYP_REAL:
      result = sizeof (t_real);
      break;
    case Serial::Array::ATYP_RPT2:
      result = 2 * sizeof (t_long) + sizeof (t_real);
      break;
    }
    return result;
  }
   
  // -------------------------------------------------------------------------
  // - array section                                                         -
  // -------------------------------------------------------------------------

  // create a default empty array

  Serial::Array::Array (void) {
    d_btyp = ATYP_BYTE;
    d_size = 0;
    d_dsiz = 0;
    d_bsiz = 0;
    d_blen = 0;
    p_byte = nullptr;
  }

  // create an empty array by sid

  Serial::Array::Array (const t_word sid) {
    switch (sid) {
    case SRL_OBLK_SID:
      d_btyp = ATYP_BYTE;
      break;
    case SRL_BBLK_SID:
      d_btyp = ATYP_BOOL;
      break;
    case SRL_LBLK_SID:
      d_btyp = ATYP_LONG;
      break;
    case SRL_RBLK_SID:
      d_btyp = ATYP_REAL;
      break;
    case SRL_RPT2_SID:
      d_btyp = ATYP_RPT2;
      break;
    default:
      throw Exception ("serial-error", "invalid serial id for array",
		       Utility::tohexa(sid, true, true));
      break;
    }
    d_size = 0;
    d_dsiz = srl_arry_dsz (d_btyp);
    d_bsiz = 0;
    d_blen = 0;
    p_byte = nullptr;
  }

  // create a serial array by size and type

  Serial::Array::Array (const long size, const t_btyp btyp) {
    // create the array
    d_btyp = btyp;
    d_size = (size <= 0) ? 0 : size;
    d_dsiz = srl_arry_dsz (d_btyp);
    d_blen = 0;
    d_bsiz = size * d_dsiz;
    p_byte = (d_bsiz == 0) ? nullptr : new t_byte[d_bsiz];
    // clear the array
    clear ();
  }
  
  // copy construct this serial array

  Serial::Array::Array (const Array& that) {
    d_btyp = that.d_btyp;
    d_size = that.d_size;
    d_dsiz = that.d_dsiz;
    d_bsiz = that.d_bsiz;
    d_blen = that.d_blen;
    p_byte = (d_bsiz == 0) ? nullptr : new t_byte[d_bsiz];
    for (long k = 0; k < d_bsiz; k++) p_byte[k] = that.p_byte[k];
  }
  
  // move construct this serial array

  Serial::Array::Array (Array&& that) noexcept {
    d_btyp = that.d_btyp; that.d_btyp = ATYP_BYTE;
    d_size = that.d_size; that.d_size = 0;
    d_dsiz = that.d_dsiz; that.d_dsiz = 0;
    d_bsiz = that.d_bsiz; that.d_bsiz = 0;
    d_blen = that.d_blen; that.d_blen = 0;
    p_byte = that.p_byte; that.p_byte = nullptr;
  }
  // destroy this serial array

  Serial::Array::~Array (void) {
    if (d_bsiz > 0) delete [] p_byte;
  }

  // assign a serial array to this one

  Serial::Array& Serial::Array::operator = (const Array& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // clean locally
    if (d_bsiz > 0) delete[] p_byte;
    // assign locally
    d_btyp = that.d_btyp;
    d_size = that.d_size;
    d_dsiz = that.d_dsiz;
    d_bsiz = that.d_bsiz;
    d_blen = that.d_blen;
    p_byte = (d_bsiz == 0) ? nullptr : new t_byte[d_bsiz];
    for (long k = 0; k < d_bsiz; k++) p_byte[k] = that.p_byte[k];
    // done
    return *this;
  }
  
  // move a serial array to this one

  Serial::Array& Serial::Array::operator = (Array&& that) noexcept {
    // clean locally
    if (d_bsiz > 0) delete[] p_byte;
    // assign locally
    d_btyp = that.d_btyp; that.d_btyp = ATYP_BYTE;
    d_size = that.d_size; that.d_size = 0;
    d_dsiz = that.d_dsiz; that.d_dsiz = 0;
    d_bsiz = that.d_bsiz; that.d_bsiz = 0;
    d_blen = that.d_blen; that.d_blen = 0;
    p_byte = that.p_byte; that.p_byte = nullptr;
    // done
    return *this;
  }
  
  // clear the serial array

  void Serial::Array::clear (void) {
    d_blen = 0;
    for (long k = 0; k < d_bsiz; k++) p_byte[k] = nilc;
  }

  // return true if the array is empty

  bool Serial::Array::empty (void) const {
    return (d_blen == 0);
  }

  // return true if the array is full

  bool Serial::Array::full (void) const {
    return (d_blen >= d_size);
  }

  // return the array length

  long Serial::Array::length (void) const {
    return d_blen;
  }

  // add a byte to the array

  void Serial::Array::add (const t_byte bval) {
    if (d_btyp != ATYP_BYTE) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if (d_blen >= d_size) {
      throw Exception ("serial-error", "cannot add in full serial array");
    }
    p_byte[d_blen++] = bval;
  }

  // get a byte value by index

  t_byte Serial::Array::getbyte (const long index) const {
    if (d_btyp != ATYP_BYTE) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if ((index < 0) || (index >= d_blen)) {
      throw Exception ("serial-error", "invalid index in serial array");
    }
    return p_byte[index];
  }

  // add a boolean to the array

  void Serial::Array::add (const bool bval) {
    if (d_btyp != ATYP_BOOL) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if (d_blen >= d_size) {
      throw Exception ("serial-error", "cannot add in full serial array");
    }
    p_byte[d_blen++] = bval ? 0x01 : nilc;
  }

  // get a boolean value by index

  bool Serial::Array::getbool (const long index) const {
    if (d_btyp != ATYP_BOOL) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if ((index < 0) || (index >= d_blen)) {
      throw Exception ("serial-error", "invalid index in serial array");
    }
    return (p_byte[index] == nilc) ? false : true;
  }

  // add an integer to the array

  void Serial::Array::add (const t_long lval) {
    if (d_btyp != ATYP_LONG) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if (d_blen >= d_size) {
      throw Exception ("serial-error", "cannot add in full serial array");
    }
    c_ohton (lval, &(p_byte[d_blen++ * d_dsiz]));
  }

  // get an integer value by index

  t_long Serial::Array::getlong (const long index) const {
    if (d_btyp != ATYP_LONG) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if ((index < 0) || (index >= d_blen)) {
      throw Exception ("serial-error", "invalid index in serial array");
    }
    return c_ontoh (&(p_byte[index * d_dsiz]));
  }

  // add a real to the array

  void Serial::Array::add (const t_real rval) {
    if (d_btyp != ATYP_REAL) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if (d_blen >= d_size) {
      throw Exception ("serial-error", "cannot add in full serial array");
    }
    c_rhton (rval, &(p_byte[d_blen++ * d_dsiz]));
  }

  // add a real point to the array

  void Serial::Array::add (const t_long xval, const t_long yval, 
			   const t_real rval) {
    if (d_btyp != ATYP_RPT2) {
      throw Exception ("serial-error", "inconsistent serial array type");
    }
    if (d_blen >= d_size) {
      throw Exception ("serial-error", "cannot add in full serial array");
    }
    // add the x coordinate
    c_ohton (xval, &(p_byte[d_blen * d_dsiz]));
    // add the y coordinate
    c_ohton (yval, &(p_byte[(d_blen * d_dsiz) + sizeof (t_long)]));
    // add the real value
    c_rhton (rval, &(p_byte[(d_blen++ * d_dsiz) + (2 * sizeof (t_long))]));
  }

  // get a real value by index

  t_real Serial::Array::getreal (const long index) const {
    // check index
    if ((index < 0) || (index >= d_blen)) {
      throw Exception ("serial-error", "invalid index in serial array");
    }
    // check for real
    if (d_btyp == ATYP_REAL) 
      return c_ontor (&(p_byte[index * d_dsiz]));
    // must be 
    if (d_btyp == ATYP_RPT2) 
      return c_ontor (&(p_byte[(index * d_dsiz) + (2 * sizeof (t_long))]));
    // invalid type
    throw Exception ("serial-error", "inconsistent serial array type");
  }

  // get the array dispatch code

  t_word Serial::Array::getdid (void) const {
    return SRL_DEOD_STD;
  }
  
  // get the array serial code

  t_word Serial::Array::getsid (void) const {
    t_word sid = nilw;
    switch (d_btyp) {
    case ATYP_BYTE:
      sid = SRL_OBLK_SID;
      break;
    case ATYP_BOOL:
      sid = SRL_BBLK_SID;
      break;
    case ATYP_LONG:
      sid = SRL_LBLK_SID;
      break;
    case ATYP_REAL:
      sid = SRL_RBLK_SID;
      break;
    case ATYP_RPT2:
      sid = SRL_RPT2_SID;
      break;
    }
    return sid;
  }

  // serialize this array

  void Serial::Array::wrstream (OutputStream& os) const {
    // write size and length
    Serial::wrlong (d_size, os);
    Serial::wrlong (d_blen, os);
    // write the byte array
    if (d_size > 0) {
      long bsiz = d_blen * d_dsiz;
      if (bsiz > d_bsiz) {
	throw Exception ("serial-error", "inconsistent size in serial array");
      }
      long rsiz = os.write ((char*) p_byte, bsiz);
      if (rsiz != bsiz) {
	throw Exception ("serial-error", "inconsistent size in serial array");
      }
    }      
  }

  // deserialize this array

  void Serial::Array::rdstream (InputStream& is) {
    // read size and length
    d_dsiz = 0;
    d_size = Serial::rdlong (is);
    d_dsiz = srl_arry_dsz (d_btyp);
    d_blen = Serial::rdlong (is);
    d_bsiz = d_size * d_dsiz;
    // allocate the byte array
    if (d_bsiz > 0) {
      // allocate the array
      p_byte = new t_byte[d_bsiz];
      // get the array from the stream
      long bsiz = d_blen * d_dsiz;
      if (bsiz > d_bsiz) {
	throw Exception ("serial-error", "inconsistent size in serial array");
      }
      long rsiz = is.copy ((char*) p_byte, bsiz);
      if (rsiz != bsiz) {
	throw Exception ("serial-error", "inconsistent size in serial array");
      }
    } else {
      p_byte = nullptr;
    }
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // check if a serial dispatcher is valid

  bool Serial::isvdid (const t_word did) {
    return chk_deod_tbl (did);
  }
  
  // add a serial dispatcher

  t_word Serial::addsd (const t_word did, t_deod deod) {
    add_deod_tbl (did, deod);
    return did;
  }

  // create a new serial object by dispatch/serial id

  Serial* Serial::newso (const t_word did, const t_word sid) {
    return new_deod_obj (did, sid);
  }
  

  // check if a nil serial id is present

  bool Serial::isnilid (InputStream& is) {
    is.wrlock ();
    try {
      t_word did = is.readw (false);
      t_word sid = is.readw (false);
      is.pushback ((char*) &did, sizeof(did));
      is.pushback ((char*) &sid, sizeof(sid));
      bool result = ((did == SRL_DEOD_DID) && (sid == SRL_NILP_SID));
      is.unlock ();
      return result;
    } catch (...) {
      is.unlock ();
      throw;
    }
  }

  // write a nil id to an output stream

  void Serial::wrnilid (OutputStream& os) {
    t_word did = System::wswap (SRL_DEOD_STD);
    t_word sid = System::wswap (SRL_NILP_SID);
    os.write ((char*) &did, sizeof(t_word));
    os.write ((char*) &sid, sizeof(t_word));
  }

  // serialize a boolean to an output stream

  void Serial::wrbool (const bool value, class OutputStream& os) {
    Boolean bobj (value);
    bobj.wrstream (os);
  }

  // serialize a boolean array with a array
  
  void Serial::wrbool (const long size, const bool* data, OutputStream& os) {
    // check for nil
    if (size == 0L) return;
    // create an operating array
    Serial::Array arry (SRL_ARRY_SIZ, Serial::Array::ATYP_BOOL);
    for (long k = 0L; k < size; k++) {
      arry.add (data[k]);
      if (arry.full () == false) continue;
      Serial::wrarry (arry, os);
      arry.clear ();
    }
    if (arry.empty () == false) Serial::wrarry (arry, os);	
  }

  // deserialize a boolean

  bool Serial::rdbool (InputStream& is) {
    Boolean bobj;
    bobj.rdstream (is);
    return bobj.tobool ();
  }

  // deserialize a boolean array

  bool* Serial::rdbool (InputStream& is, const long size) {
    // check for nill
    if ((size == 0L) || (is.iseos () == true)) return nullptr;
    // create a data array
    bool* result = new bool[size];
    try {
      // read the data array
      for (long i = 0; i < size; i++) {
	Array arry = Serial::rdarry (is);
	long  blen = arry.length ();
	for (long k = 0; k < blen; k++) result[i+k] = arry.getbool (k);
	i += (blen - 1L);
      }
      return result;
    } catch (...) {
      delete [] result;
      throw;
    }
  }

  // serialize a byte to an output stream

  void Serial::wrbyte (const t_byte value, class OutputStream& os) {
    os.write ((char) value);
  }

  // deserialize a boolean

  t_byte Serial::rdbyte (InputStream& is) {
    return (t_byte) is.read ();
  }
 
  // serialize a character to an output stream

  void Serial::wrchar (const t_quad value, class OutputStream& os) {
    Character cobj (value);
    cobj.wrstream (os);
  }

  // deserialize a boolean

  t_quad Serial::rdchar (InputStream& is) {
    // check for eos
    if (is.iseos () == true) throw Exception ("serial-error", "eos in rdchar");
    // read the character
    Character cobj; cobj.rdstream (is);
    return cobj.toquad ();
  }
 
  // serialize an integer to an output stream

  void Serial::wrlong (const t_long value, OutputStream& os) {
    Integer iobj (value);
    iobj.wrstream (os);
  }

  // serialize an integer array with a array
  
  void Serial::wrlong (const long size, const long* data, OutputStream& os) {
    // check for nil
    if (size == 0L) return;
    // create an operating array
    Serial::Array arry (SRL_ARRY_SIZ, Serial::Array::ATYP_LONG);
    for (long k = 0L; k < size; k++) {
      arry.add ((t_long) data[k]);
      if (arry.full () == false) continue;
      Serial::wrarry (arry, os);
      arry.clear ();
    }
    if (arry.empty () == false) Serial::wrarry (arry, os);	
  }

  // deserialize an integer
  
  t_long Serial::rdlong (InputStream& is) {
    // check for eos
    if (is.iseos () == true) throw Exception ("serial-error", "eos in rdlong");
    // read the integer
    Integer iobj; iobj.rdstream (is);
    return iobj.tolong ();
  }

  // deserialize an integer array

  long* Serial::rdlong (InputStream& is, const long size) {
    // check for nill
    if ((size == 0L) || (is.iseos () == true)) return nullptr;
    // create a data array
    long* result = new long[size];
    try {
      // read the data array
      for (long i = 0; i < size; i++) {
	Array arry = Serial::rdarry (is);
	long  blen = arry.length ();
	for (long k = 0; k < blen; k++) result[i+k] = arry.getlong (k);
	i += (blen - 1);
      }
      return result;
    } catch (...) {
      delete [] result;
      throw;
    }
  }

  // serialize a real to an output stream

  void Serial::wrreal (const t_real value, OutputStream& os) {
    Real zobj (value);
    zobj.wrstream (os);
  }

  // serialize a real array with a array
  
  void Serial::wrreal (const long size, const t_real* data, OutputStream& os) {
    // check for nil
    if (size == 0L) return;
    // create an operating array
    Serial::Array arry (SRL_ARRY_SIZ, Serial::Array::ATYP_REAL);
    for (long k = 0L; k < size; k++) {
      arry.add (data[k]);
      if (arry.full () == false) continue;
      Serial::wrarry (arry, os);
      arry.clear ();
    }
    if (arry.empty () == false) Serial::wrarry (arry, os);	
  }

  // deserialize a real
  
  t_real Serial::rdreal (InputStream& is) {
    // check for eos
    if (is.iseos () == true) throw Exception ("serial-error", "eos in rdreal");
    // read the real
    Real zobj; zobj.rdstream (is);
    return zobj.toreal ();
  }

  // deserialize a real data array

  t_real* Serial::rdreal (InputStream& is, const long size) {
    // check for nil
    if ((size == 0L) || (is.iseos () == true)) return nullptr;
    // create a data array
    t_real* result = new t_real[size];
    try {
      // read the data array
      for (long i = 0; i < size; i++) {
	Array arry = Serial::rdarry (is);
	long  blen = arry.length ();
	for (long k = 0; k < blen; k++) result[i+k] = arry.getreal (k);
	i += (blen - 1);
      }
      return result;
    } catch (...) {
      delete [] result;
      throw;
    }
  }

  // serialize a array to an output stream

  void Serial::wrarry (const Array& arry, OutputStream& os) {
    // write the did/sid
    t_word did = System::wswap (arry.getdid ());
    t_word sid = System::wswap (arry.getsid ());
    os.write ((char*) &did, sizeof(did));
    os.write ((char*) &sid, sizeof(sid));
    // serialize the object
    arry.wrstream (os);
  }

  // deserialize a array

  Serial::Array Serial::rdarry (InputStream& is) {
    // check for eos
    if (is.iseos () == true) throw Exception ("serial-error", "eos in rdarry");
    // get a array by serial id
    t_word did = is.readw(false);
    t_word sid = is.readw (false);
    Serial::Array arry (sid);
    if (arry.getdid () != did) {
      throw Exception ("serial-error", "invalid array did",
		       Utility::tohexa(did, true, true));
    }
    // read in the array
    arry.rdstream (is);
    return arry;
  }

  // identify an object
  
  String Serial::identify (InputStream& is) {
    try {
      // check for eos
      if (is.iseos () == true) return String();
      // get a new object by serial id
      t_word   did = is.readw (false);
      t_word   sid = is.readw (false);
      Serial* sobj = Serial::newso (did, sid);
      if (sobj == nullptr) return String();
      return sobj->repr ();
    } catch (...) {
      return String ();
    }
  }

  // return an object by deserialization

  Object* Serial::deserialize (InputStream& is) {
    // check for eos
    if (is.iseos () == true) return nullptr;
    // get a new object by serial id
    t_word   did = is.readw (false);
    t_word   sid = is.readw (false);
    Serial* sobj = Serial::newso (did, sid);
    if (sobj == nullptr) return nullptr;
    // read in the object
    sobj->rdstream (is);
    return sobj;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // copy move this serial - no lock since abstract

  Serial::Serial (Serial&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this serial into this one - no lock since abstract

  Serial& Serial::operator = (Serial&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }
  
  // return the object did

  t_word Serial::getdid (void) const {
    throw Exception ("serial-error", "cannot get did for", repr ());
  }

  // return the object sid

  t_word Serial::getsid (void) const {
    throw Exception ("serial-error", "cannot get sid for", repr ());
  }

  // serialize an object to an output stream

  void Serial::wrstream (OutputStream& os) const {
    throw Exception ("serial-error", "cannot serialize object", repr ());
  }

  // deserialize an object from an input stream

  void Serial::rdstream (InputStream& is) {
    throw Exception ("serial-error", "cannot deserialize object", repr ());
  }

  // serialize an object with it serial id

  void Serial::serialize (OutputStream& os) const {
    rdlock ();
    try {
      // get the did/sid
      t_word did = System::wswap (getdid ());
      t_word sid = System::wswap (getsid ());
      // write the did/sid
      os.write ((char*) &did, sizeof(did));
      os.write ((char*) &sid, sizeof(sid));
      // serialize the object
      wrstream (os);
      // unlock done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize an object by input stream

  void Serial::unserialize (InputStream& is) {
    wrlock ();
    try {
      // get a new object by serial id
      t_word did = is.readw (false);
      t_word sid = is.readw (false);
      if (did != getdid ()) {
	throw Exception ("serial-error", "invalid did in unserialize");
      }	
      if (sid != getsid ()) {
	throw Exception ("serial-error", "invalid sid in unserialize");
      }
      // read in the object
      rdstream (is);
      // unlock done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RSRLZ = zone.intern ("read-serial");
  static const long QUARK_WSRLZ = zone.intern ("write-serial");
  static const long QUARK_SERLZ = zone.intern ("serialize");
  static const long QUARK_UNRLZ = zone.intern ("unserialize");

  // return true if the given quark is defined

  bool Serial::isquark (const long quark, const bool hflg) const {
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

  Object* Serial::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_RSRLZ) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid object with read-serial",
			   Object::repr (obj));
	}
	rdstream (*is);
	return nullptr;
      }
      if (quark == QUARK_UNRLZ) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid object with unserialize",
			   Object::repr (obj));
	}
	unserialize (*is);
	return nullptr;
      }
      if (quark == QUARK_WSRLZ) {
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid object with write-serial",
			   Object::repr (obj));
	}
	wrstream (*os);
	return nullptr;
      }
      if (quark == QUARK_SERLZ) {
	Object* obj = argv->get (0);
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid object with serialize",
			   Object::repr (obj));
	}
	serialize (*os);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply(zobj, nset, quark, argv);
  }
}
