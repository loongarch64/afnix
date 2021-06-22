// ---------------------------------------------------------------------------
// - TlsRecord.cpp                                                           -
// - afnix:tls service - tls record class implementation                     -
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
#include "TlsTypes.hxx"
#include "Character.hpp"
#include "TlsRecord.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AeadCipher.hpp"
#include "ModeCipher.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty record

  TlsRecord::TlsRecord (void) {
    p_data = new t_byte[TLS_RLEN_MAX];
    reset ();
  }

  // create a record by input stream

  TlsRecord::TlsRecord (InputStream* is) {
    p_data = new t_byte[TLS_RLEN_MAX];
    read (is);
  }

  // create a record by input stream and cipher

  TlsRecord::TlsRecord (InputStream* is, TlsCodec* dc) {
    p_data = new t_byte[TLS_RLEN_MAX];
    read (is, dc);
  }
  
  // create a record by type and version

  TlsRecord::TlsRecord (const t_byte type, 
			const t_byte vmaj, const t_byte vmin) {
    // validdate the version
    if (tls_vers_valid (vmaj, vmin) == false) {
      throw Exception ("tls-error", "unsupported record version",
		       tls_vers_tostring (vmaj, vmin));
    }
    // validate the record type
    if (tls_type_valid (type) == false) {
      throw Exception ("tls-error", "invalid record type");
    }
    // allocate and reset
    p_data = new t_byte[TLS_RLEN_MAX];
    reset ();
    // set type and version
    d_type = type;
    d_vmaj = vmaj;
    d_vmin = vmin;
  }

  // copy construct this record

  TlsRecord::TlsRecord (const TlsRecord& that) {
    that.rdlock ();
    try {
      // record header
      d_type = that.d_type;
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      d_rlen = that.d_rlen;
      d_xlen = that.d_xlen;
      // allocate and copy locally
      p_data = new t_byte[TLS_RLEN_MAX]; long tlen = d_rlen + d_xlen;
      for (long k = 0; k < tlen; k++) p_data[k] = that.p_data[k];
      for (long k = tlen; k < TLS_RLEN_MAX; k++) p_data[k] = nilc;
    } catch (...) {
      that.unlock ();
      throw;
    }	
  }

  // move construct this record

  TlsRecord::TlsRecord (TlsRecord&& that) {
    d_type = that.d_type;
    d_vmaj = that.d_vmaj;
    d_vmin = that.d_vmin;
    d_rlen = that.d_rlen;
    d_xlen = that.d_xlen;
    p_data = that.p_data;
    // reallocate original object
    that.p_data = new t_byte[TLS_RLEN_MAX];
    that.reset ();
  }

  // destroy this record

  TlsRecord::~TlsRecord (void) {
    // nullify the memory
    reset ();
    // delete data record
    delete [] p_data;
  }

  // assign a record to this one

  TlsRecord& TlsRecord::operator = (const TlsRecord& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // record header
      d_type = that.d_type;
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      d_rlen = that.d_rlen;
      d_xlen = that.d_xlen;
      // copy locally
      long tlen = d_rlen + d_xlen;
      for (long k = 0; k < tlen; k++) p_data[k] = that.p_data[k];
      for (long k = tlen; k < TLS_RLEN_MAX; k++) p_data[k] = nilc;
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a record to this one

  TlsRecord& TlsRecord::operator = (TlsRecord&& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // move locally
    d_type = that.d_type;
    d_vmaj = that.d_vmaj;
    d_vmin = that.d_vmin;
    d_rlen = that.d_rlen;
    d_xlen = that.d_xlen;
    p_data = that.p_data;
    // reallocate original object
    that.p_data = new t_byte[TLS_RLEN_MAX];
    that.reset ();
    // here it is
    return *this;
  }

  // return the class name
  
  String TlsRecord::repr (void) const {
    return "TlsRecord";
  }

  // clone this object

  Object* TlsRecord::clone (void) const {
    return new TlsRecord (*this);
  }

  // reset the record

  void TlsRecord::reset (void) {
    wrlock ();
    try {
      // reset record header
      d_type = nilc;
      d_vmaj = nilc;
      d_vmin = nilc;
      d_rlen = 0L;
      d_xlen = 0L;
      // nullify the memory
      for (long k = 0; k < TLS_RLEN_MAX; k++) p_data[k] = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the header type

  t_byte TlsRecord::gettype (void) const {
    rdlock ();
    try {
      t_byte result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the header major version

  t_byte TlsRecord::getmajor (void) const {
    rdlock ();
    try {
      t_byte result = d_vmaj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the header minor version

  t_byte TlsRecord::getminor (void) const {
    rdlock ();
    try {
      t_byte result = d_vmin;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the record length

  long TlsRecord::length (void) const {
    rdlock ();
    try {
      long result = d_rlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a record byte by position

  t_byte TlsRecord::getbyte (const long pos) const {
    rdlock ();
    try {
      // check for valid position
      if ((pos < 0) || (pos > d_rlen)) {
	throw Exception ("tls-record", "invalid byte position in get");
      }
      // get byte and unlock
      t_byte result = p_data[pos];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a chunk block to the record
  
  void TlsRecord::add (const TlsChunk& chnk) {
    wrlock ();
    try {
      // lock and check
      chnk.rdlock ();
      long rlen = d_rlen + chnk.d_clen;
      if (rlen >= TLS_RLEN_MAX) {
	throw Exception ("tls-error", "record overflow during chunk add");
      }
      for (long k = 0L; k < chnk.d_clen; k++) {
	p_data[d_rlen++] = chnk.p_data[k];
      }
      chnk.unlock ();
      unlock ();
    } catch (...) {
      chnk.unlock ();
      unlock ();
      throw;
    }
  }

  // push the record data to a buffer

  Buffer& TlsRecord::pushb (Buffer& buf) const {
    rdlock ();
    try {
      if (d_rlen > 0L) buf.add ((char*) p_data, d_rlen);
      unlock ();
      return buf;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // read a record from an input stream

  long TlsRecord::read (InputStream* is) {
    wrlock ();
    try {
      long result = read (is, nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // read a record from an input stream

  long TlsRecord::read (InputStream* is, TlsCodec* dc) {
    // check for nil
    if (is == nullptr) return 0L;
    // lock and read
    wrlock ();
    try {
      // reset the record
      reset ();
      // read the type byte
      d_type = is->read ();
      // read the major version
      d_vmaj = is->read ();
      d_vmin = is->read ();
      // read the length
      t_byte hbyt = is->read ();
      t_byte lbyt = is->read ();
      t_word rlen = (((t_word) hbyt) << 8) + ((t_word) lbyt);
      d_rlen = rlen;
      if (d_rlen > TLS_RLEN_MAX) {
	throw Exception ("tls-error", "record size exceed maximum size");
      }
      // read the record
      long result = is->copy ((char*) p_data, d_rlen);
      if (result != d_rlen) {
	throw Exception ("tls-error", "cannot read record from stream");
      }
      // decode the block with a cipher
      if (dc != nullptr) {
	TlsBuffer ib (d_rlen); ib.add ((const char*) p_data, d_rlen);
	if (ib.bind (d_type, d_vmaj, d_vmin) == false) {
	  throw Exception ("tls-error", "cannot bind record tls buffer");
	}
	// decode the stream into a buffer
	TlsBuffer ob; d_rlen = dc->decode (ob, ib);
	if (d_rlen < 0L) {
	  throw Exception ("tls-error", "cannot decode record buffer");
	}
	// update the record with data and xtra block
	long result = ob.copy ((char*) p_data, ob.length ());
	// update extra length
	d_xlen = result - d_rlen;
      }
      unlock ();
      return result;
    } catch (...) {
      // reset everything
      reset ();
      // unlock and throw
      unlock ();
      throw;
    }
  }
  
  // write a record to an output stream

  long TlsRecord::write (OutputStream* os) {
    wrlock ();
    try {
      long result = write (os, nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write a record to an output stream

  long TlsRecord::write (OutputStream* os, TlsCodec* ec) {
    // check for nil
    if (os == nullptr) return 0L;
    wrlock ();
    try {
      if (ec != nullptr) {
	TlsBuffer ib (d_rlen); ib.add ((const char*) p_data, d_rlen);
	if (ib.bind (d_type, d_vmaj, d_vmin) == false) {
	  throw Exception ("tls-error", "cannot bind record tls buffer");
	}
	// encode the stream into a buffer
	TlsBuffer ob; d_rlen = ec->encode (ob, ib);
	if (d_rlen < 0L) {
	  throw Exception ("tls-error", "cannot encode record buffer");
	}
	// update extra length if it exists
	d_xlen = ob.length () - d_rlen;
	// update the record with data and extra block
	long rlen = ob.copy ((char*) p_data, (d_rlen + d_xlen));
	if (rlen != (d_rlen + d_xlen)) {
	  throw Exception ("tls-error", "inconsistent record buffer size");
	}
      }
      // initialize result
      long result = 0L; long tlen = d_rlen + d_xlen;
      // write the record header
      char rhrd[5];
      rhrd[0] = (char) d_type;
      rhrd[1] = (char) d_vmaj;
      rhrd[2] = (char) d_vmin;
      rhrd[3] = (char) ((tlen & 0xFF00) >> 8);
      rhrd[4] = (char) (tlen & 0x00FF);
      result += os->write (rhrd, sizeof(rhrd));
      // write the record data
      result += os->write ((const char*) p_data, tlen);
      // here it is
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_READ = zone.intern ("read");

  // create a new object in a generic way

  Object* TlsRecord::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsRecord;
    // check for 1 argument
    if (argc == 1) {
      Object*     obj = argv->get (0);
      InputStream* is = dynamic_cast<InputStream*> (obj);
      if (is == nullptr) {
	throw Exception ("type-error", "invalid object as tls input stream",
			 Object::repr (obj));
      }
      return new TlsRecord (is);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls record constructor");
  }

  // return true if the given quark is defined

  bool TlsRecord::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsHeader::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsRecord::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_READ) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast<InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid object as tls input stream",
			   Object::repr (obj));
	}
	return new Integer (read (is));
      }
    }
    // call the tle header method
    return TlsHeader::apply (zobj, nset, quark, argv);
  }
}
