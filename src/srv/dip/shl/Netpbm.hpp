// ---------------------------------------------------------------------------
// - Netpbm.hpp                                                              -
// - afnix:dip service - netpbm class definition                             -
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

#ifndef  AFNIX_NETPBM_HPP
#define  AFNIX_NETPBM_HPP

#ifndef  AFNIX_URI_HPP
#include "Uri.hpp"
#endif

#ifndef  AFNIX_IMAGE_HPP
#include "Image.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {
  
  /// The Netpbm class is a pbm/pgm/ppm file format reader/writer with 6
  /// formats, 3 beeing text and 3 beeing binary. Given an image, the writer
  /// can either operates in raw mode (aka binary) or text mode.
  /// @author amaury darsch

  class Netpbm {
  public:
    /// read a stream to an image
    /// @param is the input stream to read
    static Image* read (InputStream* is);

    /// read a stream to an image by uri
    /// @param uri the uri to read
    static Image* read (const Uri& uri);

    /// read a stream to an image by string uri
    /// @param suri the string uri to read
    static Image* read (const String& suri);

    /// write an image to an output stream
    /// @param os  the output stream to write
    /// @param img the image to write
    /// @param raw the raw flag
    static bool write (OutputStream* os, Image* img, const bool raw);

    /// write an image by uri
    /// @param uri the uri to write
    /// @param img the image to write
    /// @param raw the raw flag
    static bool write (const Uri& uri, Image* img, const bool raw);

    /// write an image by string uri
    /// @param suri the string uri to write
    /// @param img  the image to write
    /// @param raw  the raw flag
    static bool write (const String& suri, Image* img, const bool raw);
  };
}

#endif
