// ---------------------------------------------------------------------------
// - XsoStream.hpp                                                           -
// - afnix:xml module - xml/xsm input stream class definition                -
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

#ifndef  AFNIX_XSOSTREAM_HPP
#define  AFNIX_XSOSTREAM_HPP

#ifndef  AFNIX_XSOBUFFER_HPP
#include "XsoBuffer.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The XsoStream class is a xml/xsm standard input stream class designed
  /// to handle the inline character reference found with most of sgml/xml
  /// documents.
  /// @author amaury darsch

  class XsoStream {
  private:
    /// the input stream
    InputStream* p_is;
    /// the line number
    long   d_lnum;
    /// the xml buffer
    XsoBuffer d_xbuf;

  public:
    /// create a new xml stream by input stream
    /// @param is the input stream to bind
    XsoStream (InputStream* is);

    /// destroy this xml stream
    ~XsoStream (void);

    /// @return the current line number
    long getlnum (void) const;

    /// set the input steam encoding mode
    /// @param mode the coding mode to set
    void setemod (const String& mode);

    /// @return true if the stream is valid
    bool valid (void) const;

    /// @return the next available unicode character
    t_quad getu (void);

    /// pushback a unicode character in this stream
    /// @param c the unicode character to pushback
    void pushback (const t_quad c);

  private:
    // make the copy constuctor private
    XsoStream (const XsoStream&);
    // make the assignment operator private
    XsoStream& operator = (const XsoStream&);    
  };
}

#endif
