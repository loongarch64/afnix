// ---------------------------------------------------------------------------
// - TlsFinish.hpp                                                           -
// - afnix:tls service - tls finished message class definition               -
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

#ifndef  AFNIX_TLSFINISH_HPP
#define  AFNIX_TLSFINISH_HPP

#ifndef  AFNIX_TLSHBLOCK_HPP
#include "TlsHblock.hpp"
#endif

namespace afnix {

  /// The TlsFinish class is the tls client finished message class. The
  /// class is built by decoding a handshake message block. This is the first
  /// protected message which demonstrates that the negotiated parameters
  /// are valid.
  /// @author amaury darsch

  class TlsFinish : public TlsInfos {
  private:
    /// the verify buffer
    Buffer d_vbuf;
    
  public:
    /// create an empty client hello
    TlsFinish (void);

    /// create a client hello by handshake block
    /// @aparam hblk the handshake block
    TlsFinish (TlsHblock* hblk); 

    /// destroy this block
    ~TlsFinish (void);

    /// @return the class name
    String repr (void) const;

    /// reset this block
    void reset (void);
    
    /// @return the block info as a plist
    Plist getinfo (void) const;

    /// decode the hanshake block
    /// @param hblk the block to decode
    virtual void decode (TlsHblock* hblk);

  private:
    // make the copy constructor private
    TlsFinish (const TlsFinish&) =delete;
    // make the assignment operator private
    TlsFinish& operator = (const TlsFinish&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
