// ---------------------------------------------------------------------------
// - AsnUtils.hpp                                                            -
// - afnix:itu module - asn utilities definitions                            -
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

#ifndef AFNIX_ASNUTILS_HPP
#define AFNIX_ASNUTILS_HPP

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// This file containes a set of static convenient functions. These
  /// function are mostly designed for the asn reader writer helper.
  /// @author amaury darsch

  class AsnUtils {
  public:
    /// check if a string a numeric string
    /// @param sval the string to check
    static bool isnums (const String& sval);

    /// check if a string a printable string
    /// @param sval the string to check
    static bool isprts (const String& sval);

    /// check if a string an ia string
    /// @param sval the string to check
    static bool isias (const String& sval);

    /// check if a string a bmp string
    /// @param sval the string to check
    static bool isbmps (const String& sval);

    /// check if a string a generalized time
    /// @param sval the string to check
    static bool isgent (const String& sval);

    /// add an asn identifier into a buffer
    /// @param buf the holding buffer
    /// @param aid the asn id to convert
    static void aidbuf (Buffer& buf, const t_octa aid);

    /// extract an asn identifier from a buffer
    /// @param buf the buffer to read
    static t_octa bufaid (Buffer& buf);

    /// extract an asn identifier from an input stream
    /// @param is the input stream to read
    static t_octa mapaid (InputStream* is);
  };
}

#endif
