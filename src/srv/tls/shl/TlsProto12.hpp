// ---------------------------------------------------------------------------
// - TlsProto12.hpp                                                          -
// - afnix:tls service - tls protocol 1.2 class definition                   -
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

#ifndef  AFNIX_TLSPROTO12_HPP
#define  AFNIX_TLSPROTO12_HPP

#ifndef  AFNIX_TLSPROTO_HPP
#include "TlsProto.hpp"
#endif

namespace afnix {

  /// The TlsProto12 class is the tls protocol extension starting at version
  /// 1.2. The protocol is merely the same, except the prf function which is
  /// based here on sha256.
  /// @author amaury darsch

  class TlsProto12 : public TlsProto {
  protected:
    /// the prf hasher
    Hasher* p_hash;
    
  public:
    /// create a default protocol
    TlsProto12 (void);

    /// destroy this protocol
    ~TlsProto12 (void) override;
    
    /// @return the class name
    String repr (void) const override;

    /// compute a hash buffer by prf
    /// @param sbuf the secret buffer
    /// @param labl the prf label
    /// @param seed the prf seed
    /// @param rlen the prf result size
    Buffer xprf (const Buffer& sbuf, const String& labl, const Buffer& seed,
		 const long    rlen) const override;

    /// compute the server verify buffer
    /// @param mbuf the master secret
    /// @param vbuf the verify buffer 
    Buffer sfin (const Buffer& mbuf, const Buffer& vbuf) const override;
    
    /// compute the client verify buffer
    /// @param mbuf the master secret
    /// @param vbuf the verify buffer 
    Buffer cfin (const Buffer& mbuf, const Buffer& vbuf) const override;

    /// set the protocol hasher
    virtual void sethash (Hasher* hash);
    
  private:
    // make the copy constructor private
    TlsProto12 (const TlsProto12&) =delete;
    // make the assignment operator private
    TlsProto12& operator = (const TlsProto12&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
