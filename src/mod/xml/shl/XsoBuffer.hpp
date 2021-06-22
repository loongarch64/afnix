// ---------------------------------------------------------------------------
// - XsoBuffer.hpp                                                           -
// - afnix:xml module - xml/xsm standard character buffer class definition   -
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

#ifndef  AFNIX_XSOBUFFER_HPP
#define  AFNIX_XSOBUFFER_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The XsoBuffer class is a xml/xsm standard object (xso) character buffer
  /// used to accumulate characters during the xml/xsm operations. The buffer
  /// accumulates unicode characters but do not provide at this level the
  /// xml/xsm specific verifications. One feature provided at this level is
  /// the character reference transformation which is needed by the xso 
  /// input stream.
  /// @author amaury darsch

  class XsoBuffer {
  protected:
    /// the buffer size
    long    d_size;
    /// the unicode buffer
    t_quad* p_ubuf;
    /// the buffer length
    long    d_blen;
    /// the line number
    long    d_lnum;

  public:
    /// create a new buffer
    XsoBuffer (void);

    /// create a new buffer by string
    /// @param xval the initial string
    XsoBuffer (const String& xval);

    /// copy construct this buffer
    /// @param that the buffer to copy
    XsoBuffer (const XsoBuffer& that);

    /// destroy this buffer
    virtual ~XsoBuffer (void);

    /// assign a buffer to this one
    /// @param that the buffer to assign
    XsoBuffer& operator = (const XsoBuffer& that);

    /// reset this buffer 
    virtual void reset (void);

    /// clear this buffer 
    virtual void clear (void);

    /// begin a new session with a line number
    /// @param lnum the line number to set
    virtual void begin (const long lnum);

    /// set the current line number
    /// @param lnum the line number to set
    virtual void setlnum (const long lnum);

    /// @return the current line number
    virtual long getlnum (void) const;

    /// @return the length of this buffer
    virtual long length (void) const;

    /// @return true if the buffer is empty
    virtual bool empty (void) const;

    /// add a unicode character to this buffer
    /// @param c the character to add to this buffer
    virtual void add (const t_quad c);

    /// add a string to this buffer
    /// @param s the string to add
    virtual void add (const String& s);

    /// @return the next unicode character but do not remove it
    virtual t_quad get (void) const;

    /// @return the last unicode character but do not remove it
    virtual t_quad last (void) const;

    /// @return the next available unicode character
    virtual t_quad getu (void);

    /// pushback a unicode character in this buffer
    /// @param c the unicode character to pushback
    virtual void pushback (const t_quad c);

    /// pushback a string in this buffer
    /// @param s the string to pushback
    virtual void pushback (const String& s);

    /// @return the buffer contents as a string
    virtual String tostring (void) const;

    /// @return true if the buffer is a standard reference
    virtual bool issref (void) const;
    
    /// @return the buffer content as a standard reference
    virtual t_quad tosref (void) const;

    /// @return true if the buffer is a character reference
    virtual bool iscref (void) const;
    
    /// @return the buffer content as a character reference
    virtual t_quad tocref (void) const;

    /// compare the last character
    /// @param c the character to compare
    virtual bool islast (const t_quad c) const;
  };
}

#endif
