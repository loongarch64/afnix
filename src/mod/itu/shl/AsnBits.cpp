// ---------------------------------------------------------------------------
// - AsnBits.cpp                                                             -
// - afnix:itu module - asn bit string node class implementation             -
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
#include "AsnNode.hxx"
#include "AsnBits.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBuffer.hpp"
#include "InputMapped.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the cer mode indefinite threshold in bytes
  static const long ASN_ICLF_SIZE = 1000;
  // the cer mode indefinite threshold in bits
  static const long ASN_ICLF_BITS = ASN_ICLF_SIZE * 8;

  // this procedure computes the number of extra bits needed
  static inline long asn_bits_xlen (const Bitset& bits) {
    // get the number of original bits
    long blen = bits.length ();
    // get the number of extra bits to add
    return (8 - (blen % 8)) % 8;
  }

  // this procedure compute the bit string length
  static long asn_bits_clen (const Bitset& bits) {
    // get the number of original bits
    long blen = bits.length ();
    // get the number of extra bits to add
    long xlen = asn_bits_xlen (bits);
    // get the number of content octets
    long olen = (blen + xlen) / 8;
    // add the initial zeroing octet
    return olen + 1;
  }

  // this procedure returns a normalized bitset
  static Bitset asn_bits_norm (const Bitset& bits) {
    // get the number of original bits
    long blen = bits.length ();
    // get the number of extra bits
    long xlen = asn_bits_xlen (bits);
    // set the bitset size
    long size = blen + xlen;
    // create a bitset by size
    Bitset bs (size);
    // copy by marking
    for (long i = 0; i < blen; i++) bs.mark (i+xlen, bits.ismark (i));
    // here it is
    return bs;
  }

  // this procedure converts a bit string vector into a bitset
  static Bitset asn_bits_cvtn (const Vector& vnod) {
    // get the node length
    long vlen = vnod.length ();
    // the result bitset
    Bitset result;
    // loop in the bit string vector
    for (long i = 0; i < vlen; i++) {
      Object* obj = vnod.get (i);
      // check for an asn bit string
      AsnBits* node = dynamic_cast <AsnBits*> (obj);
      if (node == nullptr) {
	throw Exception ("asn-error", "invalid object in asn bit vector",
			 Object::repr (obj));
      }
      // add into the final bitset
      result = result + node->tobits ();
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnBits::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get the number of extra bits
      t_byte xlen = (t_byte) asn_bits_xlen (d_bits);
      // get the normalized bitset
      Bitset bs = asn_bits_norm (d_bits);
      // get the number of bits
      long blen = bs.length ();
      // write the content
      switch (encr) {
      case ASN_BER:
	// write the zeroing byte
	buf.add ((char) xlen);
	// add the data
	for (long i = blen - 8; i >= 0; i -= 8) {
	  buf.add ((char) bs.tobyte (i));
	}
	break;
      case ASN_DER:
	// check for valid mode
	if ((d_cstf == true) || (d_iclf == true)) {
	  throw Exception ("asn-error", "invalid asn node state in der mode");
	}
	// write the zeroing byte
	buf.add ((char) xlen);
	// add the data
	for (long i = blen - 8; i >= 0; i -= 8) {
	  buf.add ((char) bs.tobyte (i));
	}
	break;
      case ASN_CER:
	// check for valid mode
	if (((d_cstf == false) || (d_iclf == false)) && 
	    (blen > ASN_ICLF_BITS)) {
	  throw Exception ("asn-error", "invalid asn node state in cer mode");
	}
	if (((d_cstf == true) || (d_iclf == true)) && 
	    (blen <= ASN_ICLF_BITS)) {
	  throw Exception ("asn-error", "invalid asn node state in cer mode");
	}
	// check for primitive mode
	if (d_cstf == false) {
	  // write the zeroing byte
	  buf.add ((char) xlen);
	  // add the data
	  for (long i = blen - 8; i >= 0; i -= 8) {
	    buf.add ((char) bs.tobyte (i));
	  }
	  break;
	}
	// process in indefinite mode
	while (blen > 0) {
	  // compute subset size
	  long size = (blen >= ASN_ICLF_BITS) ? ASN_ICLF_BITS-8 : blen;
	  blen -= size;
	  // extract bitset
	  Bitset sb = bs.subset (size, blen);
	  // create a new node and write
	  AsnBits abn (sb);
	  abn.write (encr, buf);
	}
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnBits::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get the number of extra bits
      t_byte xlen = (t_byte) asn_bits_xlen (d_bits);
      // get the normalized bitset
      Bitset bs = asn_bits_norm (d_bits);
      // get the number of bits
      long blen = bs.length ();
      // write the content
      switch (encr) {
      case ASN_BER:
	// write the zeroing byte
	os.write ((char) xlen);
	// add the data
	for (long i = blen - 8; i >= 0; i -= 8) {
	  os.write ((char) bs.tobyte (i));
	}
	break;
      case ASN_DER:
	// check for valid mode
	if ((d_cstf == true) || (d_iclf == true)) {
	  throw Exception ("asn-error", "invalid asn node state in der mode");
	}
	// write the zeroing byte
	os.write ((char) xlen);
	// add the data
	for (long i = blen - 8; i >= 0; i -= 8) {
	  os.write ((char) bs.tobyte (i));
	}
	break;
      case ASN_CER:
	// check for valid mode
	if (((d_cstf == false) || (d_iclf == false)) && 
	    (blen > ASN_ICLF_BITS)) {
	  throw Exception ("asn-error", "invalid asn node state in cer mode");
	}
	if (((d_cstf == true) || (d_iclf == true)) && 
	    (blen <= ASN_ICLF_BITS)) {
	  throw Exception ("asn-error", "invalid asn node state in cer mode");
	}
	// check for primitive mode
	if (d_cstf == false) {
	  // write the zeroing byte
	  os.write ((char) xlen);
	  // add the data
	  for (long i = blen - 8; i >= 0; i -= 8) {
	    os.write ((char) bs.tobyte (i));
	  }
	  break;
	}
	// process in indefinite mode
	while (blen > 0) {
	  // compute subset size
	  long size = (blen >= ASN_ICLF_BITS) ? ASN_ICLF_BITS-8 : blen;
	  blen -= size;
	  // extract bitset
	  Bitset sb = bs.subset (size, blen);
	  // create a new node and write
	  AsnBits abn (sb);
	  abn.write (encr, os);
	}
	break;
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

  // create a default asn bit string

  AsnBits::AsnBits (void) : AsnNode (ASN_UNIV_BITS) {
    reset ();
  }

  // create an asn bit string by string

  AsnBits::AsnBits (const String& sval) : AsnNode (ASN_UNIV_BITS) {
    d_bits = Bitset (sval);    
  }

  // create an asn bit string by bitset

  AsnBits::AsnBits (const Bitset& bset) : AsnNode (ASN_UNIV_BITS) {
    d_bits = bset;    
  }

  // create a bit string node by node and buffer

  AsnBits::AsnBits (const AsnNode& node, const Buffer& cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_BITS) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn bit string node state");
    }
    // process in primitive mode
    if (d_cstf == false) {
      // create a buffer copy
      Buffer buf = cbuf;
      // get the initial octet
      long ioct = (long) buf.read ();
      if (ioct > 7) {
	throw Exception ("asn-error", "invalid asn bit string initial octet");
      }
      // get the buffer size and check
      long bsiz = buf.length ();
      if ((bsiz == 0) && (ioct != 0x00)) {
	throw Exception ("asn-error", "invalid null asn bit string");
      }
      if (bsiz > 0) {
	// create a working bitset
	Bitset bs (buf, false);
	// compute the final biset size
	long size = (bsiz * 8) - ioct;
	if (size <= 0) {
	  throw Exception ("internal-error", "invalid  asn bit size");
	} 
	// set the final bitset by size
	d_bits = Bitset (size);
	// copy the bitset value
	for (long i = 0; i < size; i++) d_bits.mark (i, bs.ismark (i+ioct));
      }
    } else {
     // parse the buffer as a node vector
      Vector vnod = AsnBuffer::tovnod (cbuf);
      // create the bitset from the node vector
      d_bits = asn_bits_cvtn (vnod);
    }
  }

  // create a bit string node by node and content node vector

  AsnBits::AsnBits (const AsnNode& node, const Vector& vnod) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_BITS) || (d_cstf != true) || (d_iclf != true)) {
      throw Exception ("asn-error", "invalid asn bit string node state");
    }
    // create the bitset from the node vector
    d_bits = asn_bits_cvtn (vnod);
  }

  // copy construct this asn bit string

  AsnBits::AsnBits (const AsnBits& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_bits = that.d_bits;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnBits::repr (void) const {
    return "AsnBits";
  }

  // return a clone of this object

  Object* AsnBits::clone (void) const {
    return new AsnBits (*this);
  }
  
  // assign an asn bit string to this one

  AsnBits& AsnBits::operator = (const AsnBits& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_bits = that.d_bits;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this asn bit string node

  void AsnBits::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_BITS;
      d_bits.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn bit string content length

  t_long AsnBits::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // preset the initial result
      t_long result = asn_bits_clen (d_bits);
      // adjust with the der mode - force to 0 (i.e indefinite length)
      if ((encr == ASN_CER) && (result > ASN_ICLF_SIZE)) result = 0LL;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into a buffer by encoding rule

  void AsnBits::write (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // check for the ber mode
      if (encr == ASN_BER) AsnNode::write (encr, buf);
      // check for the der mode
      if (encr == ASN_DER) {
	// check if we need to adjust the node
	if ((d_cstf == true) || (d_iclf == true)) {
	  // fix the asn bit string node
	  AsnBits abn = *this;
	  abn.d_cstf = false;
	  abn.d_iclf = false;
	  // write the node
	  abn.write (encr, buf);
	} else {
	  AsnNode::write (encr, buf);
	}
      }
      // check for cer mode
      if (encr == ASN_CER) {
	// get the content length
	long clen = asn_bits_clen (d_bits);
	// check for adjustment
	if (((d_cstf == false) || (d_iclf == false)) && 
	    (clen > ASN_ICLF_SIZE)) {
	  // fix the asn bit string node
	  AsnBits abn = *this;
	  abn.d_cstf = true;
	  abn.d_iclf = true;
	  // write the node
	  abn.write (encr, buf);
	} else {
	  if (((d_cstf == true) || (d_iclf == true)) && 
	      (clen <= ASN_ICLF_SIZE)) {
	    // fix the asn bit string node
	    AsnBits abn = *this;
	    abn.d_cstf = false;
	    abn.d_iclf = false;
	    // write the node
	    abn.write (encr, buf);
	  } else {
	    AsnNode::write (encr, buf);
	  }
	}
      }
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into an output stream by encoding rule

  void AsnBits::write (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // check for the ber mode
      if (encr == ASN_BER) AsnNode::write (encr, os);
      // check for the der mode
      if (encr == ASN_DER) {
	// check if we need to adjust the node
	if ((d_cstf == true) || (d_iclf == true)) {
	  // fix the asn bit string node
	  AsnBits abn = *this;
	  abn.d_cstf = false;
	  abn.d_iclf = false;
	  // write the node
	  abn.write (encr, os);
	} else {
	  AsnNode::write (encr, os);
	}
      }
      // check for cer mode
      if (encr == ASN_CER) {
	// get the content length
	long clen = asn_bits_clen (d_bits);
	// check for adjustment
	if (((d_cstf == false) || (d_iclf == false)) && 
	    (clen > ASN_ICLF_SIZE)) {
	  // fix the asn bit string node
	  AsnBits abn = *this;
	  abn.d_cstf = true;
	  abn.d_iclf = true;
	  // write the node
	  abn.write (encr, os);
	} else {
	  if (((d_cstf == true) || (d_iclf == true)) && 
	      (clen <= ASN_ICLF_SIZE)) {
	    // fix the asn bit string node
	    AsnBits abn = *this;
	    abn.d_cstf = false;
	    abn.d_iclf = false;
	    // write the node
	    abn.write (encr, os);
	  } else {
	    AsnNode::write (encr, os);
	  }
	}
      }
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node bitset value

  Bitset AsnBits::tobits (void) const {
    rdlock ();
    try {
      Bitset result = d_bits;
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TOBITS = zone.intern ("to-bits");

  // create a new object in a generic way
 
  Object* AsnBits::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnBits;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new AsnBits (*sobj);
      // check for a bitset
      Bitset* bobj = dynamic_cast <Bitset*> (obj);
      if (bobj != nullptr) return new AsnBits (*bobj);
      // invalid type
      throw Exception ("type-error", 
		       "invalid object with asn bit string constructor",
		       Object::repr (obj));
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn bit string constructor");
  }

  // return true if the given quark is defined

  bool AsnBits::isquark (const long quark, const bool hflg) const {
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

  Object* AsnBits::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOBITS) return new Bitset (tobits ());
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
