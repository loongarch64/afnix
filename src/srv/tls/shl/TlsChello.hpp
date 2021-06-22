// ---------------------------------------------------------------------------
// - TlsChello.hpp                                                           -
// - afnix:tls service - tls client hello message class definition           -
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

#ifndef  AFNIX_TLSCHELLO_HPP
#define  AFNIX_TLSCHELLO_HPP

#ifndef  AFNIX_TLSHBLOCK_HPP
#include "TlsHblock.hpp"
#endif

#ifndef  AFNIX_TLSSUITE_HPP
#include "TlsSuite.hpp"
#endif

#ifndef  AFNIX_TLSEXTENSION_HPP
#include "TlsExtension.hpp"
#endif

namespace afnix {

  /// The TlsChello class is the tls client hello message class. The class
  /// is built by decoding a handshake message block. The client hello
  /// message contains the client version, a random sequence, a session id,
  /// the supported cipher suite and the supported compression method. It is
  /// permitted for a client hello message to include extra data but this
  /// feature is hidden in the implementation as it is processed by the right
  /// version implementation. Note that upto tls 3.2 the random array
  /// is 4 + 28 bytes. 
  /// @author amaury darsch

  class TlsChello : public TlsInfos {
  private:
    /// the client major
    t_byte    d_vmaj;
    /// the client minor
    t_byte    d_vmin;
    /// the random array
    t_byte*   p_rand;
    /// the session length
    long      d_slen;
    /// the session array
    t_byte*   p_sess;
    /// the cipher suite
    TlsSuite* p_suit;
    /// the compression length
    long      d_clen;
    /// the compression array
    t_byte*   p_cmth;
    /// the extension vector
    Vector*   p_exts;

  public:
    /// create an empty client hello
    TlsChello (void);

    /// create a client hello by version
    /// @param vmaj the major version
    /// @param vmin the minor version
    TlsChello (const t_byte vmaj, const t_byte vmin);
    
    /// create a client hello by handshake block
    /// @aparam hblk the handshake block
    TlsChello (TlsHblock* hblk); 

    /// destroy this block
    ~TlsChello (void);

    /// @return the class name
    String repr (void) const;

    /// reset this block
    void reset (void);

    /// @return the block info as a plist
    Plist getinfo (void) const;

    /// @return the tls major version
    virtual t_byte getvmaj (void) const;

    /// @return the tls minor version
    virtual t_byte getvmin (void) const;

    /// @return the valid cipher code
    virtual TlsSuite* getsuit (void) const;

    /// @return the extensions vector
    virtual Vector* getexts (void) const;
    
    /// @return the client random sequence
    virtual Buffer getrand (void) const;
    
    /// decode the hanshake block
    /// @param hblk the block to decode
    virtual void decode (TlsHblock* hblk);

    /// map the server hello to a chunk
    virtual TlsChunk tochunk (void) const;
    
  private:
    // make the copy constructor private
    TlsChello (const TlsChello&) =delete;
    // make the assignment operator private
    TlsChello& operator = (const TlsChello&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
