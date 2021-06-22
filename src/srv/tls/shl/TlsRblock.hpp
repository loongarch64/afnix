// ---------------------------------------------------------------------------
// - TlsRblock.hpp                                                           -
// - afnix:tls service - tls record block class definition                   -
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

#ifndef  AFNIX_TLSRBLOCK_HPP
#define  AFNIX_TLSRBLOCK_HPP

#ifndef  AFNIX_TLSRECORD_HPP
#include "TlsRecord.hpp"
#endif

namespace afnix {

  /// The TlsRblock class is a tls record block class. The class is designed
  /// to create a block of data from a record data. Such data is designed
  /// by an offset and a length into the record data.
  /// @author amaury darsch

  class TlsRblock : public TlsBlock {
  protected:
    /// the block record
    TlsRecord* p_brcd;
    /// the block position 
    long d_bpos;
    /// the block length
    long d_blen;
    
  public:
    /// create an empty block
    TlsRblock (void);

    /// create a block by record, position, and length
    /// @param brcd the block record
    /// @param bpos the block position
    /// @param blen the block length
    TlsRblock (TlsRecord*   brcd, const long bpos, const long blen);

    /// copy construct this block
    /// @param that the object to copy
    TlsRblock (const TlsRblock& that);

    /// destroy this block
    ~TlsRblock (void);

    /// assign a block to this one
    /// @param that the header to assign
    TlsRblock& operator = (const TlsRblock& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this block
    void reset (void);

    /// @return the block type
    t_byte gettype (void) const;

    /// @return the block length
    long length (void) const;

    /// get a block byte by position
    /// @param pos the block position
    t_byte getbyte (const long pos) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
