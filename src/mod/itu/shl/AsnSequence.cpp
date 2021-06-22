// ---------------------------------------------------------------------------
// - AsnSequence.cpp                                                         -
// - afnix:itu module - asn sequence node class implementation               -
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

#include "AsnNode.hxx"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBuffer.hpp"
#include "InputMapped.hpp"
#include "AsnSequence.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnSequence::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get the number of nodes
      long slen = d_vseq.length ();
      // get each node in the vector
      for (long i = 0; i < slen; i++) {
	AsnNode* node = dynamic_cast <AsnNode*> (d_vseq.get (i));
	if (node == nullptr) continue;
	node->write (encr, buf);
      }
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnSequence::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get the number of nodes
      long slen = d_vseq.length ();
      // get each node in the vector
      for (long i = 0; i < slen; i++) {
	AsnNode* node = dynamic_cast <AsnNode*> (d_vseq.get (i));
	if (node == nullptr) continue;
	node->write (encr, os);
      }
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn sequence

  AsnSequence::AsnSequence (void) : AsnNode (ASN_UNIV_SEQO, true) {
    reset ();
  }
  
  // create an asn sequence node by node and buffer

  AsnSequence::AsnSequence (const AsnNode& node, 
			    const Buffer&  cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_SEQO) || (d_cstf != true) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn sequence node state");
    }
    // check the content length
    t_long clen = cbuf.length ();
    if (clen < 1) {
      throw Exception ("asn-error", "invalid asn sequence content length");
    }
    // create an input map stream
    InputStream* is = new InputMapped (cbuf);
    if (is == nullptr) {
      throw Exception ("asn-error", "cannot map sequence buffer to a stream");
    }
    try {
      // associate the stream with an asn buffer
      AsnBuffer abuf;
      // loop in the buffer
      while (is->valid () == true) {
	// parse the input stream
	if (abuf.parse (is) == false) break;
	// now map the node
	AsnNode* node = abuf.mapnode ();
	// add it to the sequence
	add (node);
      }
      delete is;
    } catch (...) {
      reset ();
      delete is;
      throw;
    }
  }

  // create an asn sequence node by node and content node vector

  AsnSequence::AsnSequence (const AsnNode& node, 
			    const Vector&  vnod) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_SEQO) || (d_cstf != true) || (d_iclf != true)) {
      throw Exception ("asn-error", "invalid asn sequence node state");
    }
    // copy the node into the vector
    d_vseq = vnod;
  }

  // copy construct this asn sequence

  AsnSequence::AsnSequence (const AsnSequence& that) {
    that.rdlock ();
    try {
      // copy the base node
      AsnNode::operator = (that);
      // get the sequence length
      long slen = d_vseq.length ();
      // copy the nodes
      for (long i = 0; i < slen; i++) {
	AsnNode* vnod = dynamic_cast <AsnNode*> (that.d_vseq.get (i));
	if (vnod == nullptr) continue;
	AsnNode* cnod = dynamic_cast <AsnNode*> (vnod->clone ());
	if (cnod == nullptr) continue;
	add (cnod);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnSequence::repr (void) const {
    return "AsnSequence";
  }

  // return a clone of this object

  Object* AsnSequence::clone (void) const {
    return new AsnSequence (*this);
  }
  
  // assign an asn sequence to this one

  AsnSequence& AsnSequence::operator = (const AsnSequence& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base node
      AsnNode::operator = (that);
      // get the sequence length
      long slen = d_vseq.length ();
      // copy the nodes
      for (long i = 0; i < slen; i++) {
	AsnNode* vnod = dynamic_cast <AsnNode*> (that.d_vseq.get (i));
	if (vnod == nullptr) continue;
	AsnNode* cnod = dynamic_cast <AsnNode*> (vnod->clone ());
	if (cnod == nullptr) continue;
	add (cnod);
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this asn sequence node

  void AsnSequence::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_SEQO;
      d_cstf = true;
      d_vseq.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node content length

  t_long AsnSequence::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // get the number of nodes
      long slen = d_vseq.length ();
      // initialize result
      t_long result = 0LL;
      // get each node in the vector
      for (long i = 0; i < slen; i++) {
	AsnNode* node = dynamic_cast <AsnNode*> (d_vseq.get (i));
	if (node == nullptr) continue;
	result += node->length (encr);
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of nodes

  long AsnSequence::getnlen (void) const {
    rdlock ();
    try {
      long result = d_vseq.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a node by index

  AsnNode* AsnSequence::getnode (const long index) const {
    rdlock ();
    try {
      AsnNode* node = dynamic_cast <AsnNode*> (d_vseq.get (index));
      unlock ();
      return node;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an asn node to the sequence

  void AsnSequence::add (AsnNode* node) {
    // check for nil
    if (node == nullptr) return;
    // lock and add
    wrlock ();
    try {
      // add the asn node
      d_vseq.add (node);
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_NADD = zone.intern ("node-add");
  static const long QUARK_NGET = zone.intern ("node-get");
  static const long QUARK_NLEN = zone.intern ("node-length");

  // create a new object in a generic way
 
  Object* AsnSequence::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnSequence;
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn sequence constructor");
  }

  // return true if the given quark is defined

  bool AsnSequence::isquark (const long quark, const bool hflg) const {
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

  Object* AsnSequence::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_NLEN) return new Integer (getnlen ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_NGET) {
	rdlock ();
	try {
	  long index = argv->getlong (0);
	  AsnNode* result = getnode (index);
	  if (zobj != nullptr) zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_NADD) {
	Object* obj = argv->get (0);
	// check for a node
	AsnNode* node = dynamic_cast <AsnNode*> (obj);
	if (node != nullptr) {
	  add (node);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with add",
			 Object::repr (obj));
      }
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
