// ---------------------------------------------------------------------------
// - XsmBuffer.cpp                                                           -
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
#include "XsmBuffer.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // return true if the character a valid space
  static inline bool is_spcc (const t_quad c) {
    if (c == 0x00000020) return true;
    if (c == 0x00000009) return true;
    if (c == 0x0000000D) return true;
    if (c == 0x0000000A) return true;
    return false;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new buffer class with a default size of 1024 characters

  XsmBuffer::XsmBuffer (void) {}

  // create a new buffer with a string

  XsmBuffer::XsmBuffer (const String& xval) : XsoBuffer (xval) {}

  // copy construct this buffer

  XsmBuffer::XsmBuffer (const XsmBuffer& that) : XsoBuffer (that) {}

  // assign a buffer to this one

  XsmBuffer& XsmBuffer::operator = (const XsmBuffer& that){
    // check for equality
    if (this == &that) return *this;
    // copy the base class
    XsoBuffer::operator = (that);
    // here it is
    return *this;
  }

  // return true if the buffer is not empty after striping

  bool XsmBuffer::isnext (void) {
    stripl ();
    return (d_blen > 0);
  }

  // return true if the character is a space

  bool XsmBuffer::isspc (const t_quad c) const {
    return is_spcc (c);
  }

  // strip the buffer with leading space

  void XsmBuffer::stripl (void) {
    while (empty () == false) {
      t_quad c = getu ();
      if (is_spcc (c) == true) continue;
      pushback (c);
      break;
    }
  }

  // strip the buffer with trailing blank

  void XsmBuffer::stripr (void) {
    while (d_blen > 0) {
      t_quad c = p_ubuf[d_blen-1];
      if (is_spcc (c) == false) break;
      d_blen--;
    }
  }

  // strip the buffer in the middle

  void XsmBuffer::stripm (void) {
    // do nothing if negligeable
    if (d_blen == 0) return;
    // get a new buffer
    t_quad* ubuf = new t_quad[d_blen];
    // loop and accumulate with one space only
    long index = 0;
    for(long i = 0; i < d_blen; i++) {
      // get the character
      t_quad c = p_ubuf[i];
      // add it if valid
      if (is_spcc (c) == false) {
	ubuf[index++] = c;
	continue;
      }
      // always add one at the beginning
      if (index == 0) {
	ubuf[index] = blkq;
	continue;
      }
      // add a space if preceding it nos a space
      if (is_spcc (ubuf[index-1]) == false) ubuf[index++] = blkq;
    }
    // clean old buffer and update
    delete [] p_ubuf;
    p_ubuf = ubuf;
    d_blen = index;
  }

  // strip the buffer with leading and trealing blanks

  void XsmBuffer::strip (void) {
    stripl ();
    stripr ();
  }

  // get the next available general string in this buffer

  String XsmBuffer::getnstr (void) {
    // create a working buffer
    XsmBuffer buf;
    // strip left the bufefr
    stripl ();
    // read character from the buffer until it is empty
    while (empty () == false) {
      t_quad c = getu ();
      if (is_spcc (c) == true) break;
      buf.add (c);
    }
    // get the string name
    return buf.tostring ();
  }

  // get the next available word

  String XsmBuffer::getword (void) {
    // create a working buffer
    XsmBuffer buf;
    // read character from the buffer until it is empty
    while (empty () == false) {
      t_quad c = getu ();
      // check for word constituent
      if (Unicode::iswcc (c) == false) {
	if (buf.empty () == false) break;
	continue;
      }
      // check for non combining at the beginning
      if (Unicode::isncc (c) == false) {
	if (buf.empty () == true) continue;
      }
      // add the character
      buf.add (c);
    }
    // get the string name
    return buf.tostring ();
  }

  // get the next available attribute in this buffer

  Property XsmBuffer::getattr (void) {
    // create a working buffer
    XsmBuffer buf;
    // initialize property name and value
    String name = "";
    String pval = "";
    // strip left the buffer
    stripl ();

    // get the name first
  s_name:
    t_quad c = getu ();
    if (c == nilq) {
      if (buf.empty () == true) goto s_serr;
      name = buf.tostring ();
      buf.reset ();
      goto s_done;
    }
    if (is_spcc (c) == true) goto s_echk;
    if (c == Unicode::toquad ('=')) {
      pushback (c);
      goto s_echk;
    }
    buf.add (c);
    goto s_name;

    // check the equal (=) character
  s_echk:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (is_spcc (c) == true) goto s_echk;
    if (c == Unicode::toquad ('=')) {
      name = buf.tostring ();
      buf.reset ();
      stripl ();
      goto s_pval;
    }
    goto s_serr;

    // get the " or ' starting character
  s_pval:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (is_spcc (c) == true) goto s_serr;
    if (c == Unicode::toquad ('\'')) goto s_sqav;
    if (c == Unicode::toquad ('"'))  goto s_dqav;
    goto s_nqav;

    // single quoted attribute value
  s_sqav:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (c == Unicode::toquad ('\'')) goto s_done;
    buf.add (c);
    goto s_sqav;

    // double quoted attribute value
  s_dqav:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (c == Unicode::toquad ('"')) goto s_done;
    buf.add (c);
    goto s_dqav;

    // non quoted attribute value
  s_nqav:
    c = getu ();
    if (c == nilq) goto s_done;
    if (is_spcc (c) == true) goto s_done;
    buf.add (c);
    goto s_nqav;

    // syntax error
  s_serr:
    throw Exception ("xsm-error", "syntax error while reading attribute",
		     buf.tostring ());

    // get attribute value
  s_done:
    pval = buf.tostring ();
    return Property (name, pval);
  }
}
