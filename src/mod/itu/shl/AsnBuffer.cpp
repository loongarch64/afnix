// ---------------------------------------------------------------------------
// - AsnBuffer.cpp                                                           -
// - afnix:itu module - asn buffer node class implementation                 -
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

#include "AsnEoc.hpp"
#include "AsnGtm.hpp"
#include "AsnUtc.hpp"
#include "AsnIas.hpp"
#include "AsnOid.hpp"
#include "AsnSet.hpp"
#include "AsnNode.hxx"
#include "AsnNull.hpp"
#include "AsnNums.hpp"
#include "AsnPrts.hpp"
#include "AsnBmps.hpp"
#include "AsnUnvs.hpp"
#include "AsnUtfs.hpp"
#include "AsnRoid.hpp"
#include "AsnBits.hpp"
#include "Boolean.hpp"
#include "Relatif.hpp"
#include "Evaluable.hpp"
#include "AsnUtils.hpp"
#include "AsnBuffer.hpp"
#include "AsnOctets.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBoolean.hpp"
#include "AsnInteger.hpp"
#include "AsnSequence.hpp"
#include "InputMapped.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // check if the node is an asn node
  static bool asn_iseoc (const AsnNode* node, const Buffer& cbuf) {
    // check argument first
    if (node == nullptr) return false;
    // check node tag
    if (node->gettagn () != 0) return false;
    // check for primitive
    if (node->isprm () == false) return false;
    // check for indefinite
    if (node->isicl () == true) return false;
    // check for an empty buffer
    if (cbuf.empty () == true) return true;
    // invalid state
    throw Exception ("asn-error", "non null buffer with null identifier");
  }

  // map an identifier octet to a node class
  static AsnNode::t_ncls asn_to_ncls (const t_byte ido) {
    AsnNode::t_ncls result = AsnNode::CLS_UNIV;
    switch (ido & ASN_MASK_CLS) {
    case ASN_UNIV_CLS:
      break;
    case ASN_APPL_CLS:
      result = AsnNode::CLS_APPL;
      break;
    case ASN_CTXS_CLS:
      result = AsnNode::CLS_CTXS;
      break;
    case ASN_PRIV_CLS:
      result = AsnNode::CLS_PRIV;
      break;
    }
    return result;
  }

  // map an identifier octet to a constructed flag
  static bool asn_to_cstf (const t_byte ido) {
    bool result = ((ido & ASN_MASK_CFL) == ASN_CONS_CLS);
    return result;
  }

  // map an identifier octet to a tag number
  static t_octa asn_to_tagn (const t_byte ido, InputStream* is) {
    // mask the octet identifier and check
    t_byte bval = ido & ASN_MASK_TAG;
    // check for long value
    return (bval == ASN_LTAG_BIN) ? AsnUtils::mapaid (is) : (t_octa) bval;
  }

  // map an octet to a content length
  static t_long asn_to_clen (const t_byte clo, InputStream* is) {
    // check for reserver form first
    if (clo == 0xFF) {
      throw Exception ("asn-error", "invalid content length byte");
    }
    // check for a short form
    t_byte scl = clo & 0x80;
    if (scl == 0x00) return (t_long) (clo & 0x7F);
    // process the long form as a relatif - first is the length
    long clen = (long) (clo & 0x7F);
    // check for null - yes it can exists ...
    if (clen == 0) return 0;
    // now check the stream - might be null
    if (is == nullptr) {
      throw Exception ("asn-error", "nil stream with content length");
    }
    // create a byte array representation
    t_byte rbuf[clen];
    for (long i = 0; i < clen; i++) rbuf[i] = 0x00;
    // now fill the byte array
    for (long i = 0; i < clen; i++) {
      // check for a valid content
      if (is->valid () == false) {
	throw Exception ("asn-error", "cannot parse asn input stream" );
      }
      rbuf[i] = (t_byte) (is->read ());
    }
    // convert it a relatif
    Relatif rlen (rbuf, clen);
    // extract the length
    return rlen.tolong ();
  }

  // map the content length octet to an eoc flaf
  static bool asn_to_iclf (const t_byte clo) {
    bool result = (clo == ASN_ILEN_BIN);
    return result;
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnBuffer::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node footer into an output stream

  void AsnBuffer::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // parse a buffer as a node vector

  Vector AsnBuffer::tovnod (const Buffer& buf) {
    // create an input map stream
    InputStream* is = new InputMapped (buf);
    if (is == nullptr) {
      throw Exception ("asn-error", "cannot map buffer to a stream");
    }
    try {
      // create the result vector
      Vector result;
      // the parsing buffer
      AsnBuffer abuf;
      // loop and parse
      while (is->valid () == true) {
	// parse the input stream
	if (abuf.parse (is) == false) break;
	// now map the node
	AsnNode* node = abuf.mapnode ();
	// add it into the vector
	if (node != nullptr) result.add (node);
      }
      delete is;
      return result;
    } catch (...) {
      delete is;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn buffer

  AsnBuffer::AsnBuffer (void) {
    reset ();
  }

  // create an asn buffer by stream

  AsnBuffer::AsnBuffer (InputStream* is) {
    reset ();
    parse (is);
  }

  // create an asn buffer by buffer

  AsnBuffer::AsnBuffer (const Buffer& buf) {
    reset ();
    parse (buf);
  }

  // create an asn buffer by bitset

  AsnBuffer::AsnBuffer (const Bitset& bset) {
    reset ();
    parse (bset);
  }

  // copy construct this asn buffer

  AsnBuffer::AsnBuffer (const AsnBuffer& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_cbuf = that.d_cbuf;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnBuffer::repr (void) const {
    return "AsnBuffer";
  }

  // return a clone of this object

  Object* AsnBuffer::clone (void) const {
    return new AsnBuffer (*this);
  }

  // assign an asn buffer to this one

  AsnBuffer& AsnBuffer::operator = (const AsnBuffer& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_cbuf = that.d_cbuf;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this asn buffer node

  void AsnBuffer::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // reset the object
      d_cbuf.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node content length

  t_long AsnBuffer::getclen (const t_encr encr) const {
    rdlock ();
    try {
      t_long result = d_cbuf.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse an input stream and fill the buffer node

  bool AsnBuffer::parse (InputStream* is) {
    wrlock ();
    try {
      // start by a node reset
      reset ();
      // check for nil stream
      if ((is == nullptr) || (is->iseos () == true)) {
	unlock ();
	return false;
      }
      // try to get the identifier octet
      if (is->valid () == false) {
	throw Exception ("asn-error", "cannot parse asn input stream" );
      }
      t_byte ido = (t_byte) is->read ();
      // extract the class
      d_ncls = asn_to_ncls (ido);
      // extract the constructed flag
      d_cstf = asn_to_cstf (ido);
      // extract the tag
      d_tagn = asn_to_tagn (ido, is);
      // try to get the length octet
      if (is->valid () == false) {
	throw Exception ("asn-error", "cannot parse asn input stream" );
      }
      t_byte clo = (t_byte) is->read ();
      // extract the content length
      t_long clen = asn_to_clen (clo, is);
      // extract the icl flag
      d_iclf = asn_to_iclf (clo);
      // extract the content
      if (d_iclf == false) {
	for (t_long i = 0LL; i < clen; i++) {
	  if (is->valid () == false) {
	    throw Exception ("asn-error", "unexpected end of stream" );
	  }
	  d_cbuf.add (is->read ());
	}
      } else {
	while (is->valid () == true) {
	  // create a new buffer
	  AsnBuffer abuf;
	  // parse the buffer
	  if (abuf.parse (is) == false) {
	    throw Exception ("asn-error", "cannot parse indefinite stream" );
	  }
	  // map the node
	  AsnNode* node = abuf.mapnode ();
	  if (node == nullptr) {
	    throw Exception ("asn-error", "cannot parse indefinite stream" );
	  }
	  // check for an eoc node
	  if (dynamic_cast <AsnEoc*> (node) != nullptr) {
	    Object::dref (node);
	    break;
	  }
	  // add the node to the node vector
	  d_vnod.add (node);
	}
      }
      // unlock and return
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a buffer

  bool AsnBuffer::parse (const Buffer& buf) {
    // create an input mapped stream
    InputMapped* is = new InputMapped (buf);
    if (is == nullptr) {
      throw Exception ("asn-error", "cannot map buffer to a stream");
    }
    // lock and parse
    wrlock ();
    try {
      bool result = parse  (is);
      delete is;
      unlock ();
      return result;
    } catch (...) {
      delete is;
      unlock ();
      throw;
    }
  }

  // parse a bitset as a buffer

  bool AsnBuffer::parse (const Bitset& bset) {
    // create an input mapped stream
    InputMapped* is = new InputMapped (bset);
    if (is == nullptr) {
      throw Exception ("asn-error", "cannot map bitset to a stream");
    }
    // lock and parse
    wrlock ();
    try {
      bool result = parse  (is);
      delete is;
      unlock ();
      return result;
    } catch (...) {
      delete is;
      unlock ();
      throw;
    }
  }

  // map the buffer node

  AsnNode* AsnBuffer::mapnode (void) const {
    rdlock ();
    try {
      AsnNode* result = nullptr;
      // check first for primitive type
      if ((d_ncls == CLS_UNIV) && (d_iclf == false)) {
	result = mapnode (d_ncls, d_tagn);
      }
      // check for indefinite constructed type
      if ((d_ncls == CLS_UNIV) && (d_cstf == true) && (d_iclf == true)) {
	// check for a bit string
	if (d_tagn == ASN_UNIV_BITS) result = new AsnBits (*this, d_vnod);
	// check for an octet string
	if (d_tagn == ASN_UNIV_OCTS) result = new AsnOctets (*this, d_vnod);
	// check for a numeric string
	if (d_tagn == ASN_UNIV_NUMS) result = new AsnNums (*this, d_vnod);
	// check for a printable string
	if (d_tagn == ASN_UNIV_PRTS) result = new AsnPrts (*this, d_vnod);
	// check for an ias string
	if (d_tagn == ASN_UNIV_IA5S) result = new AsnIas (*this, d_vnod);
	// check for a bmp string
	if (d_tagn == ASN_UNIV_BMPS) result = new AsnBmps (*this, d_vnod);
	// check for a universal string
	if (d_tagn == ASN_UNIV_UNVS) result = new AsnUnvs (*this, d_vnod);
	// check for an utf string
	if (d_tagn == ASN_UNIV_UTFS) result = new AsnUtfs (*this, d_vnod);
	// check for a sequence
	if (d_tagn == ASN_UNIV_SEQO) result = new AsnSequence (*this, d_vnod);
	// check for a set
	if (d_tagn == ASN_UNIV_SETO) result = new AsnSet (*this, d_vnod);
      }
      // default map to an asn buffer
      if (result == nullptr) result = new AsnBuffer (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a buffer node by tag number - this is a context specific class
  
  AsnNode* AsnBuffer::mapnode (const t_octa tagn) const {
    rdlock ();
    try {
      AsnNode* result = mapnode (CLS_CTXS, tagn);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a buffer node by class and tag number
  
  AsnNode* AsnBuffer::mapnode (const t_ncls ncls, const t_octa tagn) const {
    rdlock ();
    try {
      AsnNode* result = nullptr;
      // process only valid class not indefinite constructed
      if ((ncls == d_ncls) && (d_iclf == false)) {
	// special case for eoc
	if (asn_iseoc (this, d_cbuf) == true) result = new AsnEoc;
	// check for boolean
	if (tagn == ASN_UNIV_BOOL) result = new AsnBoolean (*this, d_cbuf);
	// check for integer
	if (tagn == ASN_UNIV_INTG) result = new AsnInteger (*this, d_cbuf);
	// check for an oid
	if (tagn == ASN_UNIV_OIDN) result = new AsnOid (*this, d_cbuf);
	// check for an roid
	if (tagn == ASN_UNIV_ROID) result = new AsnRoid (*this, d_cbuf);
	// check for a bit string
	if (tagn == ASN_UNIV_BITS) result = new AsnBits (*this, d_cbuf);
	// check for an octet string
	if (tagn == ASN_UNIV_OCTS) result = new AsnOctets (*this, d_cbuf);
	// check for a null value
	if (tagn == ASN_UNIV_NULL) result = new AsnNull (*this, d_cbuf);
	// check for a numeric string
	if (tagn == ASN_UNIV_NUMS) result = new AsnNums (*this, d_cbuf);
	// check for a printable string
	if (tagn == ASN_UNIV_PRTS) result = new AsnPrts (*this, d_cbuf);
	// check for an ias string
	if (tagn == ASN_UNIV_IA5S) result = new AsnIas (*this, d_cbuf);
	// check for a bmp string
	if (tagn == ASN_UNIV_BMPS) result = new AsnBmps (*this, d_cbuf);
	// check for a universal string
	if (tagn == ASN_UNIV_UNVS) result = new AsnUnvs (*this, d_cbuf);
	// check for an utf string
	if (tagn == ASN_UNIV_UTFS) result = new AsnUtfs (*this, d_cbuf);
	// check for a utc time string
	if (tagn == ASN_UNIV_UTCS) result = new AsnUtc (*this, d_cbuf);
	// check for a generalized time string
	if (tagn == ASN_UNIV_GTMS) result = new AsnGtm (*this, d_cbuf);
	// check for a sequence
	if (tagn == ASN_UNIV_SEQO) result = new AsnSequence (*this, d_cbuf);
	// check for a set
	if (tagn == ASN_UNIV_SETO) result = new AsnSet (*this, d_cbuf);
      }
      // default map to an asn buffer
      if (result == nullptr) result = new AsnBuffer (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the content buffer

  Buffer* AsnBuffer::getcbuf (void) const {
    rdlock ();
    try {
      Buffer* result = new Buffer (d_cbuf);
      unlock ();
      return result;
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
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_NMAP    = zone.intern ("node-map");
  static const long QUARK_GETCBUF = zone.intern ("get-content-buffer");

  // create a new object in a generic way
 
  Object* AsnBuffer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnBuffer;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      if (obj == nullptr) return new AsnBuffer;
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new AsnBuffer (is);
      // check for a buffer
      Buffer* bobj = dynamic_cast <Buffer*> (obj);
      if (bobj != nullptr) return new AsnBuffer (*bobj);
      // check for a bitset
      Bitset* sobj = dynamic_cast <Bitset*> (obj);
      if (sobj != nullptr) return new AsnBuffer (*sobj);
      // invalid argument
      throw Exception ("type-error", "invalid object with asn buffer",
		       Object::repr (obj));
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn buffer constructor");
  }

  // return true if the given quark is defined

  bool AsnBuffer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? AsnNode::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* AsnBuffer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_NMAP)    return mapnode ();
      if (quark == QUARK_GETCBUF) return getcbuf ();
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	Object* obj = argv->get (0);
	if (obj == nullptr) return nullptr;
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  bool result = parse (is);
	  return new Boolean (result);
	}
	// check for a buffer
	Buffer* bobj = dynamic_cast <Buffer*> (obj);
	if (bobj != nullptr) {
	  bool result = parse (*bobj);
	  return new Boolean (result);
	}
	// check for bitset
	Bitset* sobj = dynamic_cast <Bitset*> (obj);
	if (sobj != nullptr) {
	  bool result = parse (*sobj);
	  return new Boolean (result);
	}
	throw Exception ("type-error", "invalid object with parse",
			 Object::repr (obj));
      }
      if (quark == QUARK_NMAP) {
	t_octa tagn = argv->getocta (0);
	return mapnode (tagn);
      }
    }
    if (argc == 2) {
      if (quark == QUARK_NMAP) {
        // check for an item type
        Object* obj = argv->get (0);
        Item*  iobj = dynamic_cast <Item*> (obj);
        if (iobj == nullptr) {
          throw Exception ("type-error", "invalid object with node-map",
                           Object::repr (obj));
        }
        AsnNode::t_ncls ncls =AsnNode::toncls (*iobj);
        // get the tag number
        t_octa tagn = argv->getocta (1);
        return mapnode (ncls, tagn);
      }
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
