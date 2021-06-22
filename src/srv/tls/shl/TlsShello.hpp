// ---------------------------------------------------------------------------
// - TlsShello.hpp                                                           -
// - afnix:tls service - tls server hello message class definition           -
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

#ifndef  AFNIX_TLSSHELLO_HPP
#define  AFNIX_TLSSHELLO_HPP

#ifndef  AFNIX_TLSHBLOCK_HPP
#include "TlsHblock.hpp"
#endif

namespace afnix {

  /// The TlsShello class is the tls server hello message class. The class
  /// is derived from the the handshake message class by processing the
  /// message type. The server hello message contains the server version,
  /// a random sequence, a session id, the supported cipher and the
  /// supported compression method.
  /// Note that upto tls 3.2 the random array is 4 + 28 bytes.
  /// @author amaury darsch

  class TlsShello : public TlsInfos {
  private:
    /// the server major
    t_byte    d_vmaj;
    /// the server minor
    t_byte    d_vmin;
    /// the random array
    t_byte*   p_rand;
    /// the session length
    long      d_slen;
    /// the session array
    t_byte*   p_sess;
    /// the cipher code
    t_word    d_cifr;
    /// the compression method
    t_byte    d_cmth;

  public:
    /// create an empty server hello
    TlsShello (void);

    /// create a server hello by version and cipher
    /// @param vmaj the major version
    /// @param vmin the minor version
    /// @param cifr the cipher code
    TlsShello (const t_byte vmaj, const t_byte vmin, const t_word cifr);

    /// create a server hello by handshake block
    /// @aparam hblk the handshake block
    TlsShello (TlsHblock* hblk); 

    /// destroy this block
    ~TlsShello (void);

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

    /// @return the tls cipher code
    virtual t_word getcifr (void) const;
    
    /// @return the server random sequence
    virtual Buffer getrand (void) const;

    /// decode a handshake block
    /// @param hblk the block to decode
    virtual void decode (TlsHblock* hblk);

    /// map the server hello to a chunk
    virtual TlsChunk tochunk (void) const;

  private:
    // make the copy constructor private
    TlsShello (const TlsShello&);
    // make the assignment operator private
    TlsShello& operator = (const TlsShello&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
