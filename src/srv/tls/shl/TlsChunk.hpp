// ---------------------------------------------------------------------------
// - TlsChunk.hpp                                                            -
// - afnix:tls service - tls data chunk class definition                     -
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

#ifndef  AFNIX_TLSCHUNK_HPP
#define  AFNIX_TLSCHUNK_HPP

#ifndef  AFNIX_TLSINFOS_HPP
#include "TlsInfos.hpp"
#endif

namespace afnix {

  /// The TlsChunk class is a data chunk container for the tls protocol. A
  /// chunk of of byte cannot exceed the maximum record size and is mostly
  /// used for block encoding.
  /// @author amaury darsch

  class TlsChunk : public TlsInfos {
  private:
    /// the chunk length
    long d_clen;
    /// the chunk data
    t_byte* p_data;

  public:
    /// create an empty chunk
    TlsChunk (void);

    /// create a chunk by buffer
    /// @param buf the byte buffer
    TlsChunk (const Buffer& buf);
    
    /// copy construct this data chunk
    /// @param that the chunk to copy
    TlsChunk (const TlsChunk& that);

    /// destroy this chunk
    ~TlsChunk (void);

    /// assign a chunk to thiks one
    /// @param that the chunk to assign
    TlsChunk& operator = (const TlsChunk& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this alert
    void reset (void);

    /// @return the block info as a plist
    Plist getinfo (void) const;

    /// @return the block length
    virtual long length (void) const;

    /// add a byte to the chunk
    /// @param val the byte value
    virtual void add (const t_byte val);

    /// add a buffer to the chunk
    /// @param buf the buffer to add
    virtual void add (const Buffer& buf);

    /// add a chunk to the chunk
    /// @param chnk the chunk to add
    virtual void add (const TlsChunk& chnk);

    /// set a byte by position
    /// @param pos the block position
    /// @param val the byte value
    virtual void set (const long pos, const t_byte val);

    /// get a byte by position
    /// @param pos the block position
    virtual t_byte get (const long pos) const;

  private:
    // make the record class a frien
    friend class TlsRecord;

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
