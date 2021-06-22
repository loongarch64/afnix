// ---------------------------------------------------------------------------
// - Encoding.hpp                                                            -
// - standard object library - encoding system class definition              -
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

#ifndef  AFNIX_ENCODING_HPP
#define  AFNIX_ENCODING_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The encoding class is a collection of definitions and convenient
  /// functions used by the standard library to manage the various
  /// encoding system available on earth and accepted by the afnix 
  /// platform. The two natural encoding is the byte mode which acts as
  /// a plain mode for the stream reader and the utf encoding that serves
  /// the unicode decoder.
  /// @author amaury darsch

  class Encoding {
  public:
    /// the encoding modes
    enum t_emod {
      EMOD_BYTE, // byte  encoding mode
      EMOD_UTF8  // utf-8 encoding mode
    };

    /// the trancoding mode
    enum t_tmod {
      TMOD_NONE,  // transparent  transcoding
      TMOD_5901, // ISO-88559-01 transcoding
      TMOD_5902, // ISO-88559-02 transcoding
      TMOD_5903, // ISO-88559-03 transcoding
      TMOD_5904, // ISO-88559-04 transcoding
      TMOD_5905, // ISO-88559-05 transcoding
      TMOD_5906, // ISO-88559-06 transcoding
      TMOD_5907, // ISO-88559-07 transcoding
      TMOD_5908, // ISO-88559-08 transcoding
      TMOD_5909, // ISO-88559-09 transcoding
      TMOD_5910, // ISO-88559-10 transcoding
      TMOD_5911, // ISO-88559-11 transcoding
      TMOD_5913, // ISO-88559-13 transcoding
      TMOD_5914, // ISO-88559-14 transcoding
      TMOD_5915, // ISO-88559-15 transcoding
      TMOD_5916  // ISO-88559-16 transcoding
    };

  public:
    /// get the encoding code by mode
    /// @param emod the mode to code
    static t_byte tocode (const t_emod emod);
    
    /// get the encoding mode by byte code
    /// @param code the code to map
    static t_emod toemod (const t_byte code);
    
    /// get the encoding mode by name
    /// @param mode the mode to map
    static t_emod toemod (const String& mode);

    /// get the transcoding mode by name
    /// @param mode the mode to map
    static t_tmod totmod (const String& mode);

    /// @return the system encoding mode
    static t_emod getsem (void);

    /// @return the system transcoding mode
    static t_tmod getstm (void);

    /// @return the default encoding mode
    static String getdem (void);

    /// @return the natural encoding mode
    static String getnem (void);
  };
}

#endif
