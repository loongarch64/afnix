// ---------------------------------------------------------------------------
// - AsnNode.cpp                                                             -
// - afnix:itu module - asn base node class implementation                   -
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
#include "AsnNode.hpp"
#include "AsnNode.hxx"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Relatif.hpp"
#include "AsnUtils.hpp"
#include "Loopable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the default node class
  static const AsnNode::t_ncls ASN_NCLS_DEF = AsnNode::CLS_UNIV;
  // the default primitive/constructed flag
  static const bool            ASN_CSTF_DEF = false;
  // the default tag number
  static const t_octa          ASN_TAGN_DEF = 0x0000000000000000ULL;
  // the default indefinite content length flag
  static const bool            ASN_ICLF_DEF = false;
  // the end of content length
  static const t_long          ASN_ASNB_LEN = 2LL;

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure maps a class type to a byte
  static t_byte asn_ncls_byte (const AsnNode::t_ncls ncls) {
    t_byte result = nilc;
    switch (ncls) {
    case AsnNode::CLS_UNIV:
      result = ASN_UNIV_CLS;
      break;
    case AsnNode::CLS_APPL:
      result = ASN_APPL_CLS;
      break;
    case AsnNode::CLS_CTXS:
      result = ASN_CTXS_CLS;
      break;
    case AsnNode::CLS_PRIV:
      result = ASN_PRIV_CLS;
      break;
    }
    return result;
  }

  // this procedure generates an identifier octet
  static t_byte asn_make_ido (const AsnNode::t_ncls ncls, const bool cstf,
			      const t_octa tagn) {
    // generate the identifier octet
    t_byte ido = asn_ncls_byte (ncls);
    // map the construted flag
    if (cstf == true) ido |= ASN_CONS_CLS;
    // check for max tag value
    if (tagn < 31) {
      ido |= (t_byte) tagn;
    } else {
      ido |= ASN_LTAG_BIN;
    }
    // here it is
    return ido;
  }

  // this procedure write a definite length into a byte array - the byte
  // array is in reverse order of writing and the minimal length is always
  // generated as recommended in CER and DER encoding...
  static long asn_clen_byte (t_byte buf[9], t_long clen) {
    // check for negative
    if (clen < 0) {
      throw Exception ("asn-error", "invalid negative content length");
    }
    // if we have a content length less than 128, then a single byte
    // is generated (i.e short form)
    if (clen < 128) {
      buf[0] = (t_byte) clen;
      return 1;
    }
    // here we have a content length bigger that 128 - the first buffer byte
    // is the long form length byte followed by the encoded length
    Relatif rlen = clen;
    // fill the byte array at position 1
    long size = rlen.toubuf (&buf[1], 8);
    // long form byte length with msb set to 1
    buf[0] = 0x80 | (t_byte) size;
    // here is the total size
    return size + 1;
  }

  // this proceduire generates a buffer which represents the node header
  static Buffer asn_head_buf (const AsnNode::t_ncls ncls, 
			      const bool cstf, const t_octa tagn, 
			      const bool iclf, const t_long clen) {
    // create the result buffer
    Buffer buf;
    // generate the identifier octet
    t_byte ido = asn_make_ido (ncls, cstf, tagn);
    // write the byte
    buf.add ((char) ido);
    // check if we need to write a large tag
    if ((ido & ASN_LTAG_BIN) == ASN_LTAG_BIN) {
      AsnUtils::aidbuf (buf, tagn);
    }
    // write the content length
    if (iclf == false) {
      // write the content length
      t_byte cbuf[9];
      long size = asn_clen_byte (cbuf, clen);
      for (long i = 0; i < size; i++) buf.add ((char) cbuf[i]);
    } else {
      buf.add ((char) ASN_ILEN_BIN);
    }
    // here it is
    return buf;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_ASNBER  = String::intern ("BER");
  static const long QUARK_ASNDER  = String::intern ("DER");
  static const long QUARK_ASNCER  = String::intern ("CER");
  static const long QUARK_ASNUNIV = String::intern ("UNIVERSAL");
  static const long QUARK_ASNAPPL = String::intern ("APPLICATION");
  static const long QUARK_ASNCTXS = String::intern ("CONTEXT-SPECIFIC");
  static const long QUARK_ASNPRIV = String::intern ("PRIVATE");
  static const long QUARK_ASNNODE = String::intern ("AsnNode");

  // map an item to an encoding rule type
  
  AsnNode::t_encr AsnNode::toencr (const Item& item) {
    // check for an asn node item
    if (item.gettid () != QUARK_ASNNODE)
      throw Exception ("item-error", "item is not an asn node item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_ASNBER) return AsnNode::ASN_BER;
    if (quark == QUARK_ASNCER) return AsnNode::ASN_CER;
    if (quark == QUARK_ASNDER) return AsnNode::ASN_DER;
    throw Exception ("item-error", "cannot map item to asn encoding rule");
  }

  // map an item to a node class type
  
  AsnNode::t_ncls AsnNode::toncls (const Item& item) {
    // check for an asn node item
    if (item.gettid () != QUARK_ASNNODE)
      throw Exception ("item-error", "item is not an asn node item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_ASNUNIV) return AsnNode::CLS_UNIV;
    if (quark == QUARK_ASNAPPL) return AsnNode::CLS_APPL;
    if (quark == QUARK_ASNCTXS) return AsnNode::CLS_CTXS;
    if (quark == QUARK_ASNPRIV) return AsnNode::CLS_PRIV;
    throw Exception ("item-error", "cannot map item to asn node class");
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // create an empty node

  AsnNode::AsnNode (void) {
    d_ncls = ASN_NCLS_DEF;
    d_cstf = ASN_CSTF_DEF;
    d_tagn = 0ULL;
    d_iclf = ASN_ICLF_DEF;
  }

  // create a primitive node by tag number

  AsnNode::AsnNode (const t_octa tagn) {
    d_ncls = ASN_NCLS_DEF;
    d_cstf = ASN_CSTF_DEF;
    d_tagn = tagn;
    d_iclf = ASN_ICLF_DEF;
  }

  // create a primitive node by tag number and flag

  AsnNode::AsnNode (const t_octa tagn, const bool cstf) {
    d_ncls = ASN_NCLS_DEF;
    d_cstf = cstf;
    d_tagn = tagn;
    d_iclf = ASN_ICLF_DEF;
  }
  
  // copy construct an asn node

  AsnNode::AsnNode (const AsnNode& that) {
    that.rdlock ();
    try {
      d_ncls = that.d_ncls;
      d_cstf = that.d_cstf;
      d_tagn = that.d_tagn;
      d_iclf = that.d_iclf;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an asn node to this one

  AsnNode& AsnNode::operator = (const AsnNode& that) {
    if (this == &that) return *this;
    wrlock ();
    that.rdlock ();
    try {
      d_ncls = that.d_ncls;
      d_cstf = that.d_cstf;
      d_tagn = that.d_tagn;
      d_iclf = that.d_iclf;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // write the node header into a buffer

  void AsnNode::whead (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get the content length for definite form
      t_long clen = d_iclf ? 0 : getclen (encr);
      // generate the head buffer
      Buffer hbuf = asn_head_buf (d_ncls, d_cstf, d_tagn, d_iclf, clen);
      // add the head buffer
      buf.add (hbuf);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node header into an output stream

  void AsnNode::whead (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get the content length for definite form
      t_long clen = d_iclf ? 0 : getclen (encr);
      // generate the head buffer
      Buffer hbuf = asn_head_buf (d_ncls, d_cstf, d_tagn, d_iclf, clen);
      // write the header content
      while (hbuf.empty () == false) os.write (hbuf.read ());
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node footer into a buffer

  void AsnNode::wfoot (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // check for the icl flag
      if (d_iclf == true) {
	buf.add (nilc);
	buf.add (nilc);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node footer into an output stream
  
  void AsnNode::wfoot (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // check for the icl flag
      if (d_iclf == true) {
	os.write (nilc);
	os.write (nilc);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // return the node class name

  String AsnNode::repr (void) const {
    return "AsnNode";
  }

  // reset this asn node

  void AsnNode::reset (void) {
    wrlock ();
    try {
      d_ncls = ASN_NCLS_DEF;
      d_cstf = ASN_CSTF_DEF;
      d_tagn = ASN_TAGN_DEF;
      d_iclf = ASN_ICLF_DEF;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn node class

  AsnNode::t_ncls AsnNode::getcls (void) const {
    rdlock ();
    try {
      t_ncls result = d_ncls;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the node is primitive

  bool AsnNode::isprm (void) const {
    rdlock ();
    try {
      bool result = !d_cstf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the node is constructed

  bool AsnNode::iscst (void) const {
    rdlock ();
    try {
      bool result = d_cstf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the icl flag is set

  bool AsnNode::isicl (void) const {
    rdlock ();
    try {
      bool result = d_iclf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node tag number

  t_octa AsnNode::gettagn (void) const {
    rdlock ();
    try {
      t_octa result = d_tagn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node content length

  t_long AsnNode::getclen (void) const {
    rdlock ();
    try {
      t_long result = d_iclf ? 0 : getclen (ASN_BER);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node length

  t_long AsnNode::length (void) const {
    rdlock ();
    try {
      t_long result = length (ASN_BER);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node length

  t_long AsnNode::length (const t_encr encr) const {
    rdlock ();
    try {
      // get the content length
      t_long clen = getclen (encr);
      // generate the head buffer
      Buffer hbuf = asn_head_buf (d_ncls, d_cstf, d_tagn, d_iclf, clen);
      // get the header length
      t_long result = hbuf.length ();
      // add the content length
      result += clen;
      // eventually add the icl value
      if (d_iclf == true) result += ASN_ASNB_LEN;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into a buffer in ber mode

  void AsnNode::write (Buffer& buf) const {
    rdlock ();
    try {
      write (ASN_BER, buf);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into a buffer by encoding rule

  void AsnNode::write (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // write the header
      whead (encr, buf);
      // write the body
      wbody (encr, buf);
      // write the footer
      wfoot (encr, buf);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

 // write a node into an output stream in ber mode

  void AsnNode::write (OutputStream& os) const {
    rdlock ();
    try {
      write (ASN_BER, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into an output stream by encoding rule

  void AsnNode::write (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // write the header
      whead (encr, os);
      // write the body
      wbody (encr, os);
      // write the footer
      wfoot (encr, os);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETCLS  = zone.intern ("get-class");
  static const long QUARK_ISPRMP  = zone.intern ("primitive-p");
  static const long QUARK_ISCSTP  = zone.intern ("constructed-p");
  static const long QUARK_GETTAGN = zone.intern ("get-tag-number");
  static const long QUARK_GETCLEN = zone.intern ("get-content-length");

  // map an asn node type to an item
  static inline Item* type_to_item (const AsnNode::t_ncls type) {
    switch (type) {
    case AsnNode::CLS_UNIV:
      return new Item (QUARK_ASNNODE, QUARK_ASNUNIV);
      break;
    case AsnNode::CLS_APPL:
      return new Item (QUARK_ASNNODE, QUARK_ASNAPPL);
      break;
    case AsnNode::CLS_CTXS:
      return new Item (QUARK_ASNNODE, QUARK_ASNCTXS);
      break;
    case AsnNode::CLS_PRIV:
      return new Item (QUARK_ASNNODE, QUARK_ASNPRIV);
      break;
    }
    // we should never be here
    return nullptr;
  }

  // map an item to an encoding rule
  static inline AsnNode::t_encr item_to_encr (const Item& item) {
    // check for an asn node item
    if (item.gettid () != QUARK_ASNNODE)
      throw Exception ("item-error", "item is not an asn node item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_ASNBER) return AsnNode::ASN_BER;
    if (quark == QUARK_ASNDER) return AsnNode::ASN_DER;
    if (quark == QUARK_ASNCER) return AsnNode::ASN_CER;
    throw Exception ("item-error", "cannot map item to asn encoding rule");
  }

  // evaluate an object data member
  
  Object* AsnNode::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_ASNBER)
      return new Item (QUARK_ASNNODE, QUARK_ASNBER);
    if (quark == QUARK_ASNDER)
      return new Item (QUARK_ASNNODE, QUARK_ASNDER);
    if (quark == QUARK_ASNCER)
      return new Item (QUARK_ASNNODE, QUARK_ASNCER);
    if (quark == QUARK_ASNUNIV)
      return new Item (QUARK_ASNNODE, QUARK_ASNUNIV);
    if (quark == QUARK_ASNAPPL)
      return new Item (QUARK_ASNNODE, QUARK_ASNAPPL);
    if (quark == QUARK_ASNCTXS)
      return new Item (QUARK_ASNNODE, QUARK_ASNCTXS);
    if (quark == QUARK_ASNPRIV)
      return new Item (QUARK_ASNNODE, QUARK_ASNPRIV);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool AsnNode::isquark (const long quark, const bool hflg) const {
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

  Object* AsnNode::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETCLS)  return type_to_item (getcls ());
      if (quark == QUARK_ISPRMP)  return new Boolean (isprm   ());
      if (quark == QUARK_ISCSTP)  return new Boolean (iscst   ());
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETTAGN) return new Integer (gettagn ());
      if (quark == QUARK_GETCLEN) return new Integer (getclen ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) write (*os);
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  write (*os);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  write (*buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with write",
			 Object::repr (obj));
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	// check for an item type
	Item* iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with write",
			   Object::repr (obj));
	}
	AsnNode::t_encr encr = item_to_encr (*iobj);
	// get secondary object
	obj = argv->get (1);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  write (encr, *os);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  write (encr, *buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with write",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
