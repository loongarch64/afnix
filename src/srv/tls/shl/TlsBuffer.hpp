// ---------------------------------------------------------------------------
// - TlsBuffer.hpp                                                           -
// - afnix:tls service - tls buffer class definition                         -
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

#ifndef  AFNIX_TLSBUFFER_HPP
#define  AFNIX_TLSBUFFER_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

namespace afnix {

  /// The TlsBuffer class is a buffer augmented object with record data.
  /// Typicaly when a record is prepared, the record header must be set
  /// before any cipher can be applied. This is particularly true for the
  /// hmac computation which comes with it.
  /// @author amaury darsch

  class TlsBuffer : public Buffer {
  protected:
    /// the buffer type
    t_byte  d_type;
    /// the major version
    t_byte  d_vmaj;
    /// the minor version
    t_byte  d_vmin;
    
  public:
    /// create a default tls buffer
    TlsBuffer (void);
    
    /// create a tls buffer by size
    /// @param size the buffer size
    TlsBuffer (const long size);

    /// create a tls buffer by buffer
    /// @param xbuf the buffer to bind
    TlsBuffer (const Buffer& xbuf);
    
    /// create a buffer by header
    /// @param type the buffer type
    /// @param vmaj the major version
    /// @param vmin the minor version
    TlsBuffer (const t_byte type, const t_byte vmaj, const t_byte vmin);

    /// copy construct this buffer
    /// @param that the buffer to copy
    TlsBuffer (const TlsBuffer& that);

    /// assign a tls buffer to this one
    /// @param that the buffer to assign
    TlsBuffer& operator = (const TlsBuffer& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// reset this buffer
    void reset (void)  override;

    /// @return the buffer type
    virtual t_byte gettype (void) const;
    
    /// @return the buffer major version
    virtual t_byte getvmaj (void) const;
    
    /// @return the buffer minor version
    virtual t_byte getvmin (void) const;
    
    /// bind the tls buffer
    /// @param type the buffer type
    /// @param vmaj the major version
    /// @param vmin the minor version
    virtual bool bind (const t_byte type, const t_byte vmaj, const t_byte vmin);

    /// rebind a buffer from this buffer
    /// @param boff the buffer offset
    /// @param size the buffer size
    virtual TlsBuffer rebind (const long boff, const long size) const;
  };
}

#endif
