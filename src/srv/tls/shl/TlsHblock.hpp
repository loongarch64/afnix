// ---------------------------------------------------------------------------
// - TlsHblock.hpp                                                           -
// - afnix:tls service - tls handshake block class definition                -
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

#ifndef  AFNIX_TLSHBLOCK_HPP
#define  AFNIX_TLSHBLOCK_HPP

#ifndef  AFNIX_TLSRECORD_HPP
#include "TlsRecord.hpp"
#endif

namespace afnix {

  /// The TlsHblock class is the tls handshake block class. The class
  /// contains on top of the record, the block data index, the block
  /// type and the block length. For the block type, it is possible to
  /// derive specialized class for each type of block.
  /// @author amaury darsch

  class TlsHblock : public TlsBlock {
  protected:
    /// the block record
    TlsRecord* p_brcd;
    /// the block type
    t_byte d_type;
    /// the block position 
    long d_bpos;
    /// the block length
    long d_blen;
    
  public:
    /// create an empty block
    TlsHblock (void);

    /// create a block by record, position, type, and length
    /// @param brcd the block record
    /// @param type the block type
    /// @param bpos the block position
    /// @param blen the block length
    TlsHblock (TlsRecord*   brcd, 
	       const t_byte type, const long bpos, const long blen);

    /// copy construct this block
    /// @param that the object to copy
    TlsHblock (const TlsHblock& that);

    /// destroy this block
    ~TlsHblock (void);

    /// assign a block to this one
    /// @param that the header to assign
    TlsHblock& operator = (const TlsHblock& that);

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
    /// @param pos the record position
    t_byte getbyte (const long pos) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
