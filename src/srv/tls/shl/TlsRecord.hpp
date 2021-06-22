// ---------------------------------------------------------------------------
// - TlsRecord.hpp                                                           -
// - afnix:tls service - tls record class definition                         -
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

#ifndef  AFNIX_TLSRECORD_HPP
#define  AFNIX_TLSRECORD_HPP

#ifndef  AFNIX_HMAC_HPP
#include "Hmac.hpp"
#endif

#ifndef  AFNIX_TLSCHUNK_HPP
#include "TlsChunk.hpp"
#endif

#ifndef  AFNIX_TLSCODEC_HPP
#include "TlsCodec.hpp"
#endif

#ifndef  AFNIX_TLSHEADER_HPP
#include "TlsHeader.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The TlsRecord class is the raw record class for the tls protocol.
  /// A record object is created from an input stream with a byte data
  /// array those length is the record length. The tls protocol restricts
  /// the record length to 16 KiB. The record length is the total record
  /// size after reception and cipher decoding. The extra length is an
  /// indication of extract data. Normally during the decoding process,
  /// extra length is the hmac length for normal cipher. During encoding
  /// the extra length is to be interpreted with respect to the cipher and
  /// the total data is the record length + extra length.
  /// @author amaury darsch

  class TlsRecord : public TlsHeader {
  protected:
    /// the record type
    t_byte  d_type;
    /// the major version
    t_byte  d_vmaj;
    /// the minor version
    t_byte  d_vmin;
    /// the record length
    long    d_rlen;
    /// the extra length
    long    d_xlen;
    /// the record data
    t_byte* p_data;

  public:
    /// create an empty record
    TlsRecord (void);

    /// create a record by input stream
    /// @param is the input stream to read
    TlsRecord (InputStream* is);

    /// create a record by input stream and codec
    /// @param is the input stream to read
    /// @param dc the decoding codec
    TlsRecord (InputStream* is, TlsCodec* dc);
    
    /// create a record by type and version
    /// @param type the record type
    /// @param vmaj the record major version
    /// @param vmin the record minor version
    TlsRecord (const t_byte type, const t_byte vmaj, const t_byte vmin);

    /// copy construct this record
    /// @param that the object to copy
    TlsRecord (const TlsRecord& that);

    /// move construct this record
    /// @param that the object to move
    TlsRecord (TlsRecord&& that);

    /// destroy this record
    ~TlsRecord (void);

    /// assign a record to this one
    /// @param that the record to assign
    TlsRecord& operator = (const TlsRecord& that);

    /// move a record to this one
    /// @param that the record to move
    TlsRecord& operator = (TlsRecord&& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this record
    void reset (void);

    /// @return the header type
    t_byte gettype (void) const;

    /// @return the header major version
    t_byte getmajor (void) const;

    /// @return the header minor version
    t_byte getminor (void) const;

    /// @return the record length
    long length (void) const;

    /// get a record byte by position
    /// @param pos the record position
    t_byte getbyte (const long pos) const;

    /// add a chunk block to the record
    /// @param chnk the chunk block to add
    virtual void add (const TlsChunk& chnk);

    /// push the record data into a buffer
    /// @param buf the buffer to fill
    virtual Buffer& pushb (Buffer& buf) const;

    /// fill a record from an input stream
    /// @param is the input stream to read
    virtual long read (InputStream* is);

    /// fill a record from an input stream
    /// @param is the input stream to read
    /// @param dc the decoding codec
    virtual long read (InputStream* is, TlsCodec* dc);

    /// write a record to an output stream
    /// @param os the output stream
    virtual long write (OutputStream* os);

    /// write a record to an output stream
    /// @param os the output stream
    /// @param ec the encoding codec
    virtual long write (OutputStream* os, TlsCodec* ec);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
