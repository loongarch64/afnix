// ---------------------------------------------------------------------------
// - TlsBuffer.cpp                                                           -
// - afnix:tls service - tls buffer class implementation                     -
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

#include "TlsBuffer.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tls buffer

  TlsBuffer::TlsBuffer (void) : Buffer (Encoding::EMOD_BYTE) {
    d_type = nilc;
    d_vmaj = nilc;
    d_vmin = nilc;
  }

  // create a tls buffer by size

  TlsBuffer::TlsBuffer (const long size) : Buffer (size, Encoding::EMOD_BYTE) {
    d_type = nilc;
    d_vmaj = nilc;
    d_vmin = nilc;
  }

  // create a tls buffer by buffer

  TlsBuffer::TlsBuffer (const Buffer& xbuf) : Buffer (xbuf) {
    d_type = nilc;
    d_vmaj = nilc;
    d_vmin = nilc;
  }
  
  // create a tls buffer by header

  TlsBuffer::TlsBuffer (const t_byte type, const t_byte vmaj,
			const t_byte vmin) : Buffer (Encoding::EMOD_BYTE) {
    reset ();
    if (bind (type, vmaj, vmin) == false) {
      throw Exception ("tls-error", "cannot bind tls buffer");
    }
  }

  // copy construct this tls buffer

  TlsBuffer::TlsBuffer (const TlsBuffer& that) {
    that.rdlock ();
    try {
      // copy base buffer
      Buffer::operator = (that);
      // copy locally
      d_type = that.d_type;
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a tls parameters to this one

  TlsBuffer& TlsBuffer::operator = (const TlsBuffer& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base buffer
      Buffer::operator = (that);
      // assign locally
      d_type = that.d_type;
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String TlsBuffer::repr (void) const {
    return "TlsBuffer";
  }

  // return a clone of this object

  Object* TlsBuffer::clone (void) const {
    return new TlsBuffer (*this);
  }

  // reset the tls buffer object

  void TlsBuffer::reset (void) {
    wrlock ();
    try {
      // reset base buffer
      Buffer::reset ();
      // reset locally
      d_type = nilc;
      d_vmaj = nilc;
      d_vmin = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the buffer type

  t_byte TlsBuffer::gettype (void) const {
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
  
  // get the buffer major version

  t_byte TlsBuffer::getvmaj (void) const {
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
  
  // get the buffer minor version

  t_byte TlsBuffer::getvmin (void) const {
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
  // bind the tls buffer

  bool TlsBuffer::bind (const t_byte type, const t_byte vmaj,
			const t_byte vmin) {
    wrlock ();
    try {
      d_type = type;
      d_vmaj = vmaj;
      d_vmin = vmin;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // rebind a buffer from this buffer

  TlsBuffer TlsBuffer::rebind (const long boff, const long size) const {
    rdlock ();
    try {
      // create the result buffer
      TlsBuffer result (d_type, d_vmaj, d_vmin);
      // extract if requester
      if (size > 0L) result.Buffer::operator = (extract (boff, size));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
