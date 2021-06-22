// ---------------------------------------------------------------------------
// - TlsCipher.hpp                                                           -
// - afnix:tls service - tls cipher class definition                         -
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

#ifndef  AFNIX_TLSCIPHER_HPP
#define  AFNIX_TLSCIPHER_HPP

#ifndef  AFNIX_TLSCODEC_HPP
#include "TlsCodec.hpp"
#endif

#ifndef  AFNIX_TLSSTATE_HPP
#include "TlsState.hpp"
#endif

namespace afnix {

  /// The TlsCipher class is the tls cipher object which bind a cipher
  /// with the record block. The class encodes and decode record block
  /// with respect to the cipher type as well as providing to a certain
  /// extend the support for authentication in the cases of aead cipher.
  /// @author amaury darsch

  class TlsCipher : public TlsCodec {    
  protected:
    /// the tls state
    TlsState* p_ssta;
    
  public:
    /// create a nil tls cipher
    TlsCipher (void);

    /// create a tls cipher by state
    /// @param ssta the tls state
    TlsCipher (TlsState* ssta);

    /// destroy this tls cipher
    ~TlsCipher (void);
    
    /// @return the class name
    String repr (void) const override;

    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long encode (TlsBuffer& ob, TlsBuffer& ib) override;
    
    /// decode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long decode (TlsBuffer& ob, TlsBuffer& ib) override;
    
    /// @return the tls state
    virtual TlsState* getssta (void) const;
    
  private:
    // make the copy constructor private
    TlsCipher (const TlsCipher&) =delete;
    // make the assignment operator private
    TlsCipher& operator = (const TlsCipher&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
