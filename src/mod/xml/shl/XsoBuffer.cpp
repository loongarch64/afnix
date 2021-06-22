// ---------------------------------------------------------------------------
// - XsoBuffer.cpp                                                           -
// - afnix:xml module - xsm character buffer class implementation            -
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

#include "Vector.hpp"
#include "Utility.hpp"
#include "Unicode.hpp"
#include "XsoBuffer.hpp"
#include "XmlSystem.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default buffer size
  static const long XSO_BUFFER_SIZE = 1024;

  // the xso special characters
  static const t_quad XSO_CHAR_AM = 0x00000026; // &
  static const t_quad XSO_CHAR_NS = 0x00000023; // #
  static const t_quad XSO_CHAR_SC = 0x0000003B; // ;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new buffer class with a default size of 1024 characters

  XsoBuffer::XsoBuffer (void) {
    d_size = XSO_BUFFER_SIZE;
    p_ubuf = new t_quad[d_size];
    d_blen = 0;
    d_lnum = 0;
    for (long i = 0; i < d_size; i++) p_ubuf[i] = nilq;
  }

  // create a new buffer with a string

  XsoBuffer::XsoBuffer (const String& xval) {
    d_size = XSO_BUFFER_SIZE;
    p_ubuf = new t_quad[d_size];
    d_blen = 0;
    d_lnum = 0;
    for (long i = 0; i < d_size; i++) p_ubuf[i] = nilq;
    add (xval);
  }

  // copy construct this buffer

  XsoBuffer::XsoBuffer (const XsoBuffer& that){
    d_size = that.d_size;
    p_ubuf = new t_quad[d_size];
    d_blen = that.d_blen;
    d_lnum = that.d_lnum;
    for (long i = 0; i < d_blen; i++) p_ubuf[i] = that.p_ubuf[i];
  }

  // destroy this buffer
  
  XsoBuffer::~XsoBuffer (void) {
    delete [] p_ubuf;
  }

  // assign a buffer to this one

  XsoBuffer& XsoBuffer::operator = (const XsoBuffer& that){
    // check for self-assignation
    if (this == &that) return *this;
    // clean the old buffer
    delete [] p_ubuf;
    // copy the data
    d_size = that.d_size;
    p_ubuf = new t_quad[d_size];
    d_blen = that.d_blen;
    d_lnum = that.d_lnum;
    for (long i = 0; i < d_blen; i++) p_ubuf[i] = that.p_ubuf[i];
    // this the new buffer
    return *this;
  }

  // reset this buffer but do not change the size
  
  void XsoBuffer::reset (void) {
    d_blen = 0;
    d_lnum = 0;
  }

  // clear this buffer content
  
  void XsoBuffer::clear (void) {
    d_blen = 0;
    for (long i = 0; i < d_size; i++) p_ubuf[i] = nilq;
  }


  // begin a new session with a line number

  void XsoBuffer::begin (const long lnum) {
    reset   ();
    setlnum (lnum);
  }

  // set the current line number

  void XsoBuffer::setlnum (const long lnum) {
    d_lnum = lnum;
  }

  // return the current line number

  long XsoBuffer::getlnum (void) const {
    return d_lnum;
  }

  // return the length of this buffer
  
  long XsoBuffer::length (void) const {
    return d_blen;
  }

  // return true if the buffer is empty

  bool XsoBuffer::empty (void) const {
    return (d_blen == 0);
  }

  // add a character in this buffer
  
  void XsoBuffer::add (const t_quad c) {
    // first check if we are at the buffer end
    if (d_blen == d_size) {
      long size = d_size * 2;
      t_quad* buf = new t_quad[size];
      for (long i = 0; i < d_blen; i++) buf[i] = p_ubuf[i];
      delete [] p_ubuf;
      d_size = size;
      p_ubuf = buf;
    }
    p_ubuf[d_blen++] = c;
  }

  // add a string to this buffer

  void XsoBuffer::add (const String& s) {
    long len = s.length ();
    for (long i = 0; i < len; i++) add (s[i]);
  }

  // get the next unicode character but do not remove it
  
  t_quad XsoBuffer::get (void) const {
    return (d_blen == 0) ? nilq : p_ubuf[0];
  }

  // get the last unicode character but do not remove it

  t_quad XsoBuffer::last (void) const {
    return (d_blen == 0) ? nilq : p_ubuf[d_blen-1];
  }
  
  // read a character in this buffer
  
  t_quad XsoBuffer::getu (void) {
    // check for empty buffer character
    if (d_blen == 0) return nilq;
    // get value and shift
    long     ulen = d_blen - 1;
    t_quad result = p_ubuf[0];
    for (long i = 0; i < ulen; i++) p_ubuf[i] = p_ubuf[i+1];
    p_ubuf[ulen] = nilq;
    d_blen = ulen;
    return result;
  }
  
  // pushback a character in this buffer
  
  void XsoBuffer::pushback (const t_quad c) {
    // first check if we are at the buffer end
    if (d_blen == d_size) {
      long size = d_size * 2;
      t_quad* buf = new t_quad[size];
      for (long i = 0; i < d_blen; i++) buf[i] = p_ubuf[i];
      delete [] p_ubuf;
      d_size = size;
      p_ubuf = buf;
    }
    // shift the buffer by one
    for (long i = d_blen; i > 0; i--) p_ubuf[i] = p_ubuf[i-1];
    // pushback the character
    p_ubuf[0] = c;
    d_blen++;
  }

  // pushback a string in this buffer

  void XsoBuffer::pushback (const String& s) {
    long len = s.length ();
    if (len == 0) return;
    for (long i = len - 1; i >= 0; i--) pushback (s[i]);
  }

  // return the buffer content as a valid string
  
  String XsoBuffer::tostring (void) const {
    // mark the end of string
    t_quad* ubuf = Unicode::strdup (p_ubuf, d_blen);
    // convert the string
    String result = ubuf;
    delete [] ubuf;
    return result;
  }
  
  // check if the buffer is a standard reference

  bool XsoBuffer::issref (void) const {
    // map the buffer as a string
    String s = tostring ();
    // check for an standard entity character
    return XmlSystem::issref (s);
  }

  // return the buffer content as a standard reference

  t_quad XsoBuffer::tosref (void) const {
    // map the buffer as a string
    String s = tostring ();
    // check for an standard entity character
    return XmlSystem::tosref (s);
  }
  
  // check if the buffer is a character reference

  bool XsoBuffer::iscref (void) const {
    // map the buffer as a string
    String s = tostring ();
    // check for an standard entity character
    if (XmlSystem::issref (s) == true) return true;
    // check for character reference
    if (d_blen < 3) return false;
    if (p_ubuf[0] != XSO_CHAR_AM) return false;
    if (p_ubuf[1] != XSO_CHAR_NS) return false;
    if (p_ubuf[d_blen-1] != XSO_CHAR_SC) return false;
    // potential cref - could be malformed
    return true;
  }
  
  // return the buffer content as a character reference

  t_quad XsoBuffer::tocref (void) const {
    // map the buffer as a string
    String s = tostring ();
    // check for an standard entity character
    if (XmlSystem::issref (s) == true) return XmlSystem::tosref (s);
    // check for valid buffer starter
    if (d_blen < 3) {
      throw Exception ("xso-error", "invalid character reference buffer");
    }
    if (p_ubuf[0] != XSO_CHAR_AM) {
      throw Exception ("xso-error", "invalid character reference buffer");
    }
    if (p_ubuf[1] != XSO_CHAR_NS) {
      throw Exception ("xml-error", "invalid character reference buffer");
    }
    if (p_ubuf[d_blen-1] != XSO_CHAR_SC) {
      throw Exception ("xml-error", "invalid character reference buffer");
    }
    // mark the end of string
    t_quad* ubuf = Unicode::strdup (&p_ubuf[2], d_blen-3);
    // get the string and check
    String xref = ubuf;
    delete [] ubuf;
    // check for non nil buffer
    if (xref.isnil () == true) {
      throw Exception ("xso-error", "invalid nil character reference");
    }
    // prepare the value string
    String xbuf;
    // check for hexadecimal notation
    if (xref[0] == Unicode::toquad ('x')) {
      xbuf  = "0";
      xbuf += xref;
    } else {
      xbuf = xref;
    }
    // convert the string
    t_quad cval = (t_quad) Utility::tolong (xbuf);
    return cval;
  }

  // compare the last characater

  bool XsoBuffer::islast (const t_quad c) const {
    // check for nil
    if (d_blen == 0) return false;
    return (p_ubuf[d_blen-1] == c);
  }
}
