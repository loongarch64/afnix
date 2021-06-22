// ---------------------------------------------------------------------------
// - TlsCodec.hpp                                                            -
// - afnix:tls service - tls codec class definition                          -
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

#ifndef  AFNIX_TLSCODEC_HPP
#define  AFNIX_TLSCODEC_HPP

#ifndef  AFNIX_TLSBUFFER_HPP
#include "TlsBuffer.hpp"
#endif

namespace afnix {

  /// The TlsCodec class is an abstract class for the tls that provides
  /// support for buffer encoding and decoding. The encoding and decoding
  /// might be performed by specific ciphers.
  /// @author amaury darsch

  class TlsCodec : public Object {
  public:
    /// the hmac structure
    struct s_hblk {
      /// the block type
      t_byte  d_type;
      /// the major version
      t_byte  d_vmaj;
      /// the minor version
      t_byte  d_vmin;
      /// the block length
      long    d_blen;
      /// the block data
      t_byte* p_blok;
    };
    
  public:
    /// create a default tls codec
    TlsCodec (void) = default;
    
    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    virtual long encode (TlsBuffer& ob, TlsBuffer& ib) =0;
    
    /// decode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    virtual long decode (TlsBuffer& ob, TlsBuffer& ib) =0;
    
  private:
    // make the copy constructor private
    TlsCodec (const TlsCodec&) =delete;
    // make the assignment operator private
    TlsCodec& operator = (const TlsCodec&) =delete;
  };
}

#endif
