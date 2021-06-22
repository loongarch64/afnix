// ---------------------------------------------------------------------------
// - TlsUtils.hpp                                                            -
// - afnix:tls service - tls utility class definition                        -
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

#ifndef  AFNIX_TLSUTILS_HPP
#define  AFNIX_TLSUTILS_HPP

#ifndef  AFNIX_MAC_HPP
#include "Mac.hpp"
#endif

namespace afnix {

  /// The TlsUtils class is a collection of static convenient functions. Their
  /// purpose is to group these general functions into a single umbrella.
  /// @author amaury darsch

  class TlsUtils {
  public:

    /// convert byte codes into a word
    /// @param ucod the upper byte
    /// @param lcod the lower byte
    static t_word toword (const t_byte ucod, const t_byte lcod);

    /// convert a word into a upper byte code
    /// @param code the code to convert
    static t_byte toucod (const t_word code);

    /// convert a word into a lower byte code
    /// @param code the code to convert
    static t_byte tolcod (const t_word code);

    /// convert a word code into a string
    /// @param code the code tgo convert
    static String toscod (const t_word code);

    /// create a random buffer by size flag
    /// @param size the buffer size
    static Buffer random (const long size);

    /// create a random array by size and time flag
    /// @param size the array size
    /// @param tflg the time flag
    static t_byte* random (const long size, const bool tflg);

    /// the standard p_hash function of the tls
    /// @param hmac the hash mac object
    /// @param seed the mac seed buffer
    /// @param rlen he requested byte length
    static Buffer phash (Mac* hmac, const Buffer& seed, const long rlen);    
  };
}

#endif
