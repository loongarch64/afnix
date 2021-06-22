// ---------------------------------------------------------------------------
// - XsoStream.cpp                                                           -
// - afnix:xml module - xml/xsm input stream class implementation            -
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

#include "Unicode.hpp"
#include "XsoStream.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the standard entity marker
  static const t_quad XSO_SREF_MS = 0x80000000UL;
  // the xml character reference
  static const t_quad XSO_CHAR_AM = 0x00000026UL;
  static const t_quad XSO_CHAR_SC = 0x0000003BUL;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new xml stream

  XsoStream::XsoStream (InputStream* is) {
    if (is == nullptr) {
      throw Exception ("xml-error", "invalid nil input stream");
    }
    Object::iref (p_is = is);
    d_lnum = 1;
    d_xbuf.reset ();
 }

  // destroy this xml stream
  
  XsoStream::~XsoStream (void) {
    if (p_is != nullptr) {
      if (d_xbuf.empty () == false) p_is->pushback (d_xbuf.tostring ());
    }
    Object::tref (p_is);
  }

  // return the current line number

  long XsoStream::getlnum (void) const {
    return d_lnum;
  }

  // set the stream encoding mode

  void XsoStream::setemod (const String& mode) {
    p_is->setemod (mode);
  }

  // return true if the stream is valid

  bool XsoStream::valid (void) const {
    return p_is->valid ();
  }
  
  // read the next available character with character reference escape
  // use the marker for an entity reference

  t_quad XsoStream::getu (void) {
    // check the xml buffer first
    if (d_xbuf.empty () == false) {
      t_quad c = d_xbuf.getu ();
      if (c == eolq) d_lnum++;
      return c;
    }

    // check for & character
    t_quad c = p_is->getu ();
    if (c != XSO_CHAR_AM) {
      if (c == eolq) d_lnum++;
      return c;
    }
    d_xbuf.add (c);

    // accumulate characters until a ; character
  s_sref:
    c = p_is->getu ();
    switch (c) {
    case XSO_CHAR_SC:
      d_xbuf.add(c);
      if (d_xbuf.issref () == true) {
	c = d_xbuf.tosref () | XSO_SREF_MS;
	d_xbuf.reset ();
	return c;
      } 
      return getu();
    case blkq:
    case tabq:
    case eolq:
    case eosq:
      p_is->pushback (c);
      c = d_xbuf.getu ();
      if (c == eolq) d_lnum++;
      return c;
    default:
      d_xbuf.add (c);
      goto s_sref;
    }

    // we should never be here
    throw Exception ("internal-error", "out-of-bound unicode xso stream");
  }

  // pushback a character

  void XsoStream::pushback (const t_quad c) {
    if ((c == eolq) && (d_lnum > 1)) d_lnum--;
    d_xbuf.pushback (c);
  }
}
