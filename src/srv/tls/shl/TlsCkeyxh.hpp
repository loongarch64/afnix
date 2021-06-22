// ---------------------------------------------------------------------------
// - TlsCkeyxh.hpp                                                           -
// - afnix:tls service - tls client key exchange message class definition    -
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

#ifndef  AFNIX_TLSCKEYXH_HPP
#define  AFNIX_TLSCKEYXH_HPP

#ifndef  AFNIX_STATE_HPP
#include "TlsState.hpp"
#endif

#ifndef  AFNIX_TLSHBLOCK_HPP
#include "TlsHblock.hpp"
#endif

namespace afnix {

  /// The TlsCkeyxh class is the tls client key exchange message class. The
  /// class is built by decoding a handshake message block. Since the original
  /// message is encoded, a key is necessary for decoding the message block.
  /// @author amaury darsch

  class TlsCkeyxh : public TlsInfos {
  private:
    /// the premaster length
    long d_mlen;
    /// the premaster key
    t_byte* p_mbuf;
    
  public:
    /// create a client key exchange
    TlsCkeyxh (void);

    /// create a client key exchange by state
    /// @param ssta the tls state
    TlsCkeyxh (TlsState* ssta);

    /// create a client exchange by handshake block
    /// @aparam hblk the handshake block
    TlsCkeyxh (TlsHblock* hblk); 

    /// create a client exchange by handshake block
    /// @aparam hblk the handshake block
    /// @aparam ssta the tls state
    TlsCkeyxh (TlsHblock* hblk, TlsState* ssta); 

    /// destroy this block
    ~TlsCkeyxh (void);

    /// @return the class name
    String repr (void) const;

    /// reset this block
    void reset (void);

    /// @return the premaster key as a buffer
    virtual Buffer getmbuf (void) const;
    
    /// @return the block info as a plist
    Plist getinfo (void) const;

    /// decode the hanshake block
    /// @param hblk the block to decode
    /// @param ssta the tls state
    virtual void decode (TlsHblock* hblk, TlsState* ssta);

    /// map the client key exchange to a chunk
    /// @param sta the tls state
    virtual TlsChunk tochunk (TlsState* sta) const;

  private:
    // make the copy constructor private
    TlsCkeyxh (const TlsCkeyxh&) =delete;
    // make the assignment operator private
    TlsCkeyxh& operator = (const TlsCkeyxh&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
