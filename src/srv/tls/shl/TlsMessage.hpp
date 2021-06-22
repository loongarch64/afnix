// ---------------------------------------------------------------------------
// - TlsMessage.hpp                                                          -
// - afnix:tls service - tls message record class definition                 -
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

#ifndef  AFNIX_TLSMESSAGE_HPP
#define  AFNIX_TLSMESSAGE_HPP

#ifndef  AFNIX_TLSRECORD_HPP
#include "TlsRecord.hpp"
#endif

namespace afnix {

  /// The TlsMessage class is the base class for the tls messages which
  /// encapsulates the behavior of the record object. By storing a record
  /// the message data are kept localised to the original record.
  /// @author amaury darsch

  class TlsMessage : public TlsHeader {
  protected:
    /// the block record
    TlsRecord* p_brcd;
    
  public:
    /// create an empty message
    TlsMessage (void);

    /// create a message by record
    /// @para, rcd the message record
    TlsMessage (TlsRecord* rcd);

    /// create a message by record type and version
    /// @param type the record type
    /// @param vmaj the record major version
    /// @param vmin the record minor version
    TlsMessage (const t_byte type, const t_byte vmaj, const t_byte vmin);

    /// copy construct this message
    /// @param that the object to copy
    TlsMessage (const TlsMessage& that);

    /// move construct this message
    /// @param that the object to move
    TlsMessage (TlsMessage&& that) noexcept;

    /// destroy this message
    ~TlsMessage (void);

    /// assign a message to this one
    /// @param that the message to assign
    TlsMessage& operator = (const TlsMessage& that);

    /// move a message to this one
    /// @param that the message to move
    TlsMessage& operator = (TlsMessage&& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this message
    void reset (void);

    /// @return the message type
    t_byte gettype (void) const;

    /// @return the message length
    long length (void) const;

    /// get a message byte by position
    /// @param pos the record position
    t_byte getbyte (const long pos) const;

    /// @return the message major version
    t_byte getmajor (void) const;

    /// @return the message minor version
    t_byte getminor (void) const;

    /// add a chunk block to the message
    /// @param chnk the chunk block to add
    virtual void add (const TlsChunk& chnk);

    /// push the record data into a buffer
    /// @param buf the buffer to fill
    virtual Buffer& pushb (Buffer& buf) const;
    
    /// write a message to an output stream
    /// @param os the output stream
    virtual long write (OutputStream* os);

    /// write a message to an output stream
    /// @param os the output stream
    /// @param ec the encoding codec
    virtual long write (OutputStream* os, TlsCodec* ec);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
