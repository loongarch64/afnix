// ---------------------------------------------------------------------------
// - TlsCblock.hpp                                                           -
// - afnix:tls service - tls ciphered block class definition                 -
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

#ifndef  AFNIX_TLSCBLOCK_HPP
#define  AFNIX_TLSCBLOCK_HPP

#ifndef  AFNIX_HMAC_HPP
#include "Hmac.hpp"
#endif

#ifndef  AFNIX_TLSRBLOCK_HPP
#include "TlsRblock.hpp"
#endif

namespace afnix {

  /// The TlsCblock class is a tls ciphered block class. The class is designed
  /// to map a record data into two components, the data block and the mac. In
  /// normal mode the ciphered block is constructed from a record and a hmac.
  /// The hmac length is used to compute the data block size.
  /// @author amaury darsch

  class TlsCblock : public TlsBlock {
  protected:
    /// the data block
    TlsBlock* p_data;
    /// the mac block
    TlsBlock* p_bmac;
    /// the hmac object
    Hmac* p_hmac;
    
  public:
    /// create an empty block
    TlsCblock (void);

    /// create a block by record, and mac length
    /// @param brcd the block record
    /// @param hmac the mac object
    TlsCblock (TlsRecord* brcd, Hmac* hmac);

    /// create a block by data and mac
    /// @param data the data block
    /// @param bmac the mac block
    TlsCblock (TlsBlock* data, TlsBlock* bmac);

    /// create a block by data and mac
    /// @param data the data block
    /// @param bmac the mac block
    /// @param hmac the mac object
    TlsCblock (TlsBlock* data, TlsBlock* bmac, Hmac* hmac);
    
    /// copy construct this block
    /// @param that the object to copy
    TlsCblock (const TlsCblock& that);

    /// destroy this message
    ~TlsCblock (void);

    /// assign a block to this one
    /// @param that the header to assign
    TlsCblock& operator = (const TlsCblock& that);

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

    /// @return a clone of the data block
    TlsBlock* getdata (void) const;
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
