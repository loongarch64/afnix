// ---------------------------------------------------------------------------
// - XsmReader.cpp                                                           -
// - afnix:xml module - xsm reader class implementation                      -
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
#include "Boolean.hpp"
#include "Unicode.hpp"
#include "Evaluable.hpp"
#include "XsmBuffer.hpp"
#include "XsmReader.hpp"
#include "XsoStream.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the standard entity marker
  static const t_quad XSM_SREF_MC = 0x7FFFFFFFUL;
  // the xml special characters
  static const t_quad XSM_CHAR_LT = 0x0000003CUL; // <
  static const t_quad XSM_CHAR_GT = 0x0000003EUL; // >
  static const t_quad XSM_CHAR_AM = 0x00000026UL; // &
  static const t_quad XSM_CHAR_SC = 0x0000003BUL; // ;
  static const t_quad XSM_CHAR_SL = 0x0000002FUL; // /
  static const t_quad XSM_CHAR_EP = 0x00000021UL; // !
  static const t_quad XSM_CHAR_LB = 0x0000005BUL; // [
  static const t_quad XSM_CHAR_RB = 0x0000005DUL; // ]
  static const t_quad XSM_CHAR_MN = 0x0000002DUL; // -

  // this function parse a xml text node
  static XsmNode* parse_xsm_txt (XsmBuffer& xbuf) {
    // create a text node by string
    XsmNode* node = new XsmNode (xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function parse a xsm tag node
  static XsmNode* parse_xsm_tag (XsmBuffer& xbuf) {
    // create a tag node by string
    XsmNode* node = new XsmNode (XsmNode::TAG_NODE, xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function parse a xsm reference node
  static XsmNode* parse_xsm_ref (XsmBuffer& xbuf) {
    // create a reference node by name
    XsmNode* node = new XsmNode (XsmNode::REF_NODE, xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the eref node
    return node;
  }

  // this function parse a xsm end node
  static XsmNode* parse_xsm_end (XsmBuffer& xbuf) {
    // create a end node by name
    XsmNode* node = new XsmNode (XsmNode::END_NODE, xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function clear a node buffer
  static void clear_xsm_node (XsoStream& xis) {
    // loop until we have an exit condition
    while (xis.valid () == true) {
      try {
	t_quad c = xis.getu ();
	switch (c) {
	case XSM_CHAR_LT:
	  xis.pushback (c);
	  return;
	case nilq:
	case eosq:
	case XSM_CHAR_GT:
	  return;
	default:
	  break;
	}
      } catch (...) {
	continue;
      }
    }
  }

  // this function parse a xsm node
  static XsmNode* parse_xsm_node (XsoStream& xis) {
    // create a xml buffer
    XsmBuffer xbuf;

    // state: s_helo
    // get the first character for dispatch
  s_helo:
    t_quad c = xis.getu ();
    switch (c) {
    case XSM_CHAR_LT:
      xbuf.begin (xis.getlnum ());
      goto s_stag;
    case XSM_CHAR_AM:
      xbuf.begin (xis.getlnum ());
      goto s_sref;
    case nilq:
    case eosq:
      return nullptr;
    default:
      c &= XSM_SREF_MC;
      if (xbuf.isspc (c) == true) {
	goto s_helo;
      } else {
	xbuf.begin (xis.getlnum ());
	xbuf.add   (c);
	goto s_text;
      }
    }

    // state: s_text
    // read some text and accumulate
  s_text:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_LT:
    case XSM_CHAR_AM:
    case eosq:
      xis.pushback (c);
      return parse_xsm_txt (xbuf);
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_text;
    }

    // state: s_stag
    // start a tag with anything inside
  s_stag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_GT:
      return parse_xsm_tag (xbuf);
    case XSM_CHAR_SL:
      goto s_etag;
    case XSM_CHAR_EP:
      xbuf.add (c);
      goto s_ptag;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_ntag
    // read a tag with anything inside
  s_ntag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_GT:
      return parse_xsm_tag (xbuf);
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_etag
    // read an end tag
  s_etag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_GT:
      return parse_xsm_end (xbuf);
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_etag;
    }

    // state: s_ptag
    // read a special tag with !
  s_ptag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_LB:
      xbuf.add (c);
      goto s_btag;
    case XSM_CHAR_MN:
      xbuf.add (c);
      goto s_bgcm;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_btag
    // read a bracket tag 
  s_btag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_RB:
      xbuf.add (c);
      goto s_rtag;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_btag;
    }

    // state: s_rtag
    // read a bracket tag end
  s_rtag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_GT:
      return parse_xsm_tag (xbuf);
    case XSM_CHAR_RB:
      xbuf.add (c);
      goto s_rtag;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_btag;
    }

    // state: s_sref
    // start a reference node
  s_sref:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_SC:
      return parse_xsm_ref (xbuf);
    case XSM_CHAR_LT:
    case eosq:
      xbuf.pushback (XSM_CHAR_AM);
      xis.pushback  (c);
      return parse_xsm_txt (xbuf);
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_sref;
    }

    // state: s_bgcm
    // begin a comment node
  s_bgcm:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_MN:
      xbuf.add (c);
      goto s_ctag;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_ctag
    // read a comment node
  s_ctag:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_MN:
      xbuf.add (c);
      goto s_encm;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ctag;
    }

    // state: s_encm
    // start the end comment node
  s_encm:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_MN:
      xbuf.add (c);
      goto s_fncm;
    case eosq:
      goto s_error;
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ctag;
    }

    // state: s_fncm
    // finish the end comment node
  s_fncm:
    c = xis.getu ();
    switch (c) {
    case XSM_CHAR_GT:
      return parse_xsm_tag (xbuf);
    case eosq:
      throw Exception ("xsm-error", "unterminated comment node");
    default:
      c &= XSM_SREF_MC;
      xbuf.add (c);
      goto s_ctag;
    }

    // state: s_error
    // return a syntax error
  s_error:
    throw Exception ("xsm-error", "syntax error while reading node",
		     xbuf.tostring ());
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty reader

  XsmReader::XsmReader (void) {
    d_erf = false;
    p_is  = nullptr;
  }

  // create a xsm reader by input stream

  XsmReader::XsmReader (InputStream* is) {
    d_erf = false;
    p_is  = nullptr;
    setis (is);
  }

  // create a xsm reader by string

  XsmReader::XsmReader (const String& xval) {
    d_erf = false;
    p_is  = nullptr;
    setis (xval);
  }
  
  // create a xsm reader by input stream and error flag

  XsmReader::XsmReader (InputStream* is, const bool erf) {
    d_erf = erf;
    p_is  = nullptr;
    setis (is);
  }

  // destroy this reader

  XsmReader::~XsmReader (void) {
    Object::dref (p_is);
  }

  // return the document class name

  String XsmReader::repr (void) const {
    return "XsmReader";
  }

  // set the erf flag

  void XsmReader::seterf (const bool erf) {
    wrlock ();
    try {
      d_erf = erf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the erf flag

  bool XsmReader::geterf (void) const {
    rdlock ();
    try {
      bool result = d_erf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the reader input stream

  void XsmReader::setis (InputStream* is) {
    wrlock ();
    try {
      Object::iref (is);
      Object::dref (p_is);
      p_is = is;
      unlock ();
    } catch (...) {
      Object::dref (p_is);
      p_is = nullptr;
      unlock ();
      throw;
    }
  }

  // set the reader input stream by string

  void XsmReader::setis (const String& xval) {
    wrlock ();
    try {
      Object::dref (p_is); p_is = nullptr;
      Object::iref (p_is = new InputString (xval));
      unlock ();
    } catch (...) {
      Object::dref (p_is);
      p_is = nullptr;
      unlock ();
      throw;
    }
  }

  // get the next available node

  XsmNode* XsmReader::getnode (void) {
    wrlock ();
    // check the input stream
    if (p_is == nullptr) {
      unlock ();
      return nullptr;
    }
    // create a xso stream
    XsoStream xis (p_is);
    try {
      // parse the xml stream
      XsmNode* result = parse_xsm_node (xis);
      unlock ();
      return result;
    } catch (Exception& e) {
      if (d_erf == true) {
	// clear the stream
	clear_xsm_node (xis);
	// try to get the next node
	XsmNode* result = getnode ();
	unlock ();
	return result;
      } else {
	e.updlnum (xis.getlnum ());
	unlock ();
	throw e;
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETIS   = zone.intern ("set-input-stream");
  static const long QUARK_SETERF  = zone.intern ("set-error-resume");
  static const long QUARK_GETERF  = zone.intern ("get-error-resume");
  static const long QUARK_GETNODE = zone.intern ("get-node");

  // create a new object in a generic way

  Object* XsmReader::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XsmReader;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new XsmReader (is);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new XsmReader (*sobj);
      // invalid object
      throw Exception ("type-error", 
		       "invalid object with xsm reader constructor",
		       Object::repr (obj));
    }
    // argument error
    throw Exception ("argument-error",
                     "too many argument with xsm reader constructor");
  }

  // return true if the given quark is defined

  bool XsmReader::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XsmReader::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETERF)  return new Boolean (geterf ());
      if (quark == QUARK_GETNODE) return getnode ();
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETERF) {
	bool erf = argv->getbool (0);
	seterf (erf);
	return nullptr;
      }
      if (quark == QUARK_SETIS) {
	Object* obj = argv->get (0);
	if (obj == nullptr) return nullptr;
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  setis (is);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  setis (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with setis",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
