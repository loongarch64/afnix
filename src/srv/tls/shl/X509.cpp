// ---------------------------------------------------------------------------
// - X509.cpp                                                                -
// - afnix:tls service - X509 certificate class implementation               -
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

#include "X509.hpp"
#include "Vector.hpp"
#include "AsnOid.hpp"
#include "AsnSet.hpp"
#include "AsnUtc.hpp"
#include "AsnGtm.hpp"
#include "AsnBits.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
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

  // get the certificate node from the asn buffer
  static AsnSequence* x509_tocert (const AsnBuffer& abuf) {
    // map the buffer to a node
    AsnNode* node = abuf.mapnode ();
    // check for a sequence
    AsnSequence* cert = dynamic_cast <AsnSequence*> (node);
    if (cert == nullptr) {
      delete node;
      throw Exception ("X509-error", "cannot map certificate sequence node");
    }
    // check the sequence length
    if (cert->getnlen () != 3) {
      delete node;
      throw Exception ("X509-error", "invalid certificate sequence length");
    }
    return cert;
  }

  // get the tbs certificate from the certificate node
  static AsnSequence* x509_toxtbs (const AsnSequence* cert) {
    // check for nil
    if (cert == nullptr) {
      throw Exception ("X509-error", "invalid nil certificate node");
    }
    // get the first node
    AsnNode* node = cert->getnode (0);
    // check for a sequence
    AsnSequence* xtbs = dynamic_cast <AsnSequence*> (node);
    if (xtbs == nullptr) {
      throw Exception ("X509-error", "cannot map certificate tbs node");
    }
    // check the sequence length
    if (xtbs->getnlen () < 7) {
      throw Exception ("X509-error", "invalid certificate tbs length");
    }
    return xtbs;
  }

  // get the certificate version from the tbs certificate
  static long x509_tovers (const AsnSequence* xtbs) {
    // check for nil
    if (xtbs == nullptr) {
      throw Exception ("X509-error", "invalid nil tbs certificate node");
    }
    // get the first node (version) - in x509, the tag is explicit [0]
    // and the node must be constructed with the underlying node as an integer
    AsnNode*   node = xtbs->getnode (0);
    if ((node != nullptr) && (node->iscst () == false)) {
      throw Exception ("X509-error", "invalid implicit version node");
    }
    if (node->gettagn () != 0) {
      throw Exception ("X509-error", "invalid version tag number");
    }
    AsnBuffer* nbuf = dynamic_cast <AsnBuffer*> (node);
    if (nbuf == nullptr) {
      throw Exception ("X509-error", "cannot map version node buffer");
    }
    // get the content buffer
    Buffer* vbuf = nbuf->getcbuf ();
    if (vbuf == nullptr) {
      throw Exception ("X509-error", "cannot get version content buffer");
    }
    // map the version node
    AsnBuffer abuf (*vbuf);
    AsnNode*  vnod = nullptr;
    try {
      vnod = abuf.mapnode ();
    } catch (...) {
      delete vbuf;
      throw;
    }
    AsnInteger* inod = dynamic_cast <AsnInteger*> (vnod);
    if (inod == nullptr) {
      delete vbuf;
      delete vnod;
      throw Exception ("X509-error", "cannot map integer version node");
    }
    Relatif rval = inod->torelatif ();
    delete vbuf;
    delete vnod;
    // this implementation supports only version 1,2 or 3
    long vers = rval.tolong ();
    if ((vers < 0) || (vers > 2)) {
      throw Exception ("X509-error", "invalid certificate version number",
		       Utility::tostring (vers));
    }
    return vers;
  }
  
  // get the certificate serial number from the tbs certificate
  static Relatif x509_tosnum (const AsnSequence* xtbs) {
    // check for nil
    if (xtbs == nullptr) {
      throw Exception ("X509-error", "invalid nil tbs certificate node");
    }
    // get the serial node [1]
    AsnNode* node = xtbs->getnode (1);
    AsnInteger* inod = dynamic_cast <AsnInteger*> (node);
    if (inod == nullptr) {
      throw Exception ("X509-error", "cannot map integer serial number node");
    }
    return inod->torelatif ();
  }

  // get an algorithm from a sequence
  static Xalgo x509_toalgo (AsnNode* node) {
    // check for nil
    if (node == nullptr) {
      throw Exception ("X509-error", "invalid nil sequence as algorithm");
    }
    // map the algorithm node
    AsnSequence* snod = dynamic_cast <AsnSequence*> (node);
    if (snod == nullptr) {
      throw Exception ("X509-error", "cannot map algorithm sequence node");
    }
    long slen = snod->getnlen ();
    if ((slen != 1) && (slen != 2)) {
      throw Exception ("X509-error", "invalid algorithm sequence length");
    }
    // get the signature oid
    AsnOid* soid = dynamic_cast <AsnOid*> (snod->getnode (0));
    if (soid == nullptr) {
      throw Exception ("X509-error", "invalid nil oid in algorithm sequence");
    }
    // get the oid representation
    Oid    oid = soid->getoid ();
    String aid = oid.format   ();
    // get the signature parameters
    AsnNode* aop = (slen == 2) ? snod->getnode (1) : nullptr;
    // create the algorithm identifier
    return Xalgo (aid, aop);
  }

  // get an extension from a sequence
  static Xextn* x509_toextn (AsnNode* node) {
    // check for nil
    if (node == nullptr) {
      throw Exception ("X509-error", "invalid nil sequence as extension");
    }
    // map the extension node
    AsnSequence* snod = dynamic_cast <AsnSequence*> (node);
    if (snod == nullptr) {
      throw Exception ("X509-error", "cannot map extension sequence node");
    }
    long slen = snod->getnlen ();
    if ((slen != 2) && (slen != 3)) {
      throw Exception ("X509-error", "invalid extension sequence length");
    }
    // get the extension oid
    AsnOid* soid = dynamic_cast <AsnOid*> (snod->getnode (0));
    if (soid == nullptr) {
      throw Exception ("X509-error", "invalid nil oid in extension sequence");
    }
    // get the oid representation
    Oid    oid = soid->getoid ();
    String eid = oid.format   ();
    // get the extension flag and object
    bool     ecf = false;
    AsnNode* eon = nullptr;
    if (slen == 2) eon = snod->getnode (1);
    if (slen == 3) {
      AsnBoolean* secf = dynamic_cast <AsnBoolean*> (snod->getnode (1));
      if (secf == nullptr) {
      throw Exception ("X509-error", 
		       "invalid nil boolean in extension sequence");
      }
      ecf = secf->tobool ();
      eon = snod->getnode (2);
    }
    // create the extension object
    return new Xextn (eid, ecf, eon);
  }
 
  // get a rdn name as a property list
  static Plist x509_toname (const AsnSequence* xtbs, const long xidx) {
    // check for nil
    if (xtbs == nullptr) {
      throw Exception ("X509-error", "invalid nil tbs certificate node");
    }
    // get the sequence node
    AsnSequence* node = dynamic_cast <AsnSequence*> (xtbs->getnode (xidx));
    if (node == nullptr) {
      throw Exception ("X509-error", "cannot map name sequence node");
    }
    // get the number of rdn
    long slen = node->getnlen ();
    if (slen == 0) {
      throw Exception ("X509-error", "invalid nil name sequence length");
    }
    // loop and set the plist result
    Plist result;
    for (long k = 0; k < slen; k++) {
      // get the set node
      AsnSet* snod = dynamic_cast <AsnSet*> (node->getnode (k));
      if (snod == nullptr) {
	throw Exception ("X509-error", "cannot map name set node");
      }
      if (snod->getnlen () != 1) {
	throw Exception ("X509-error", "invalid name sequence set length");
      }
      // get the attribute node
      AsnSequence* anod = dynamic_cast <AsnSequence*> (snod->getnode (0));
      if (anod == nullptr) {
	throw Exception ("X509-error", "cannot map name attribute node");
      }
      if (anod->getnlen () != 2) {
	throw Exception ("X509-error", "invalid name attrbute set length");
      }
      // get the oid attribute
      AsnOid* aid = dynamic_cast <AsnOid*> (anod->getnode (0));
      if (aid == nullptr) {
	throw Exception ("X509-error", "cannot map name attribute oid");
      }
      Oid oid = aid->getoid ();
      // get the attribute value
      AsnNode* vnod = anod->getnode (1);
      // check for octets string
      AsnOctets* onod = dynamic_cast <AsnOctets*> (vnod);
      if (onod != nullptr) {
	result.add (oid.format (), onod->tostring ());
	continue;
      }
      // check for boolean
      AsnBoolean* bnod = dynamic_cast <AsnBoolean*> (vnod);
      if (bnod != nullptr) {
	result.add (oid.format (), bnod->tobool ());
	continue;
      }
      // check for integer
      AsnInteger* inod = dynamic_cast <AsnInteger*> (vnod);
      if (inod != nullptr) {
	Relatif rval = inod->torelatif ();
	result.add (oid.format (), rval.tolong ());
	continue;
      }
      // invalid object
      throw Exception ("X509-error", "cannot map name attribute value",
		       Object::repr (vnod));
    }
    return result;
  }

  // this procedure map a node to a time value
  static t_long x509_totime (AsnNode* node) {
    // check for nil first
    if (node == nullptr) {
      throw Exception ("X509-error", "invalid nil time node");
    }
    // check for utc node
    AsnUtc* utcn = dynamic_cast <AsnUtc*> (node);
    if (utcn != nullptr) return utcn->totime ();
    // check for generailized time node
    AsnGtm* gtmn = dynamic_cast <AsnGtm*> (node);
    if (gtmn != nullptr) return gtmn->totime ();
    // invalid node
    throw Exception ("X509-error", "invalid asn object as time node",
		     Object::repr (node));
  }

  // map the validity time sequence
  static void x509_tovldt (t_long& tvlb,t_long& tvla,const AsnSequence* xtbs) {
    // check for nil
    if (xtbs == nullptr) {
      throw Exception ("X509-error", "invalid nil tbs certificate node");
    }
    // get the validity node [4]
    AsnSequence* node = dynamic_cast <AsnSequence*> (xtbs->getnode (4));
    if (node == nullptr) {
      throw Exception ("X509-error", "cannot map validity sequence node");
    }
    // get the number of nodes
    long slen = node->getnlen ();
    if (slen != 2) {
      throw Exception ("X509-error", "invalid validity sequence length");
    }
    // get the validity before time
    tvlb = x509_totime (node->getnode (0));
    // get the validity after time
    tvla = x509_totime (node->getnode (1));
  }

  // get the subject public key sequence
  static AsnSequence* x509_tospks (const AsnSequence* xtbs) {
    // check for nil
    if (xtbs == nullptr) {
      throw Exception ("X509-error", "invalid nil tbs certificate node");
    }
    // get the subject public key node [6]
    AsnSequence* node = dynamic_cast <AsnSequence*> (xtbs->getnode (6));
    if (node == nullptr) {
      throw Exception ("X509-error", 
		       "cannot map subject public key info sequence node");
    }
    // get the number of nodes
    long slen = node->getnlen ();
    if (slen != 2) {
      throw Exception ("X509-error", 
		       "invalid validity subject public key info length");
    }
    return node;
  }

  // get the sequence algorithm
  static Xalgo x509_toxpka (const AsnSequence* snod, const long spos) {
    AsnNode* node = (snod == nullptr) ? nullptr : snod->getnode (spos);
    return x509_toalgo (node);
  }

  // get the sequence bit string
  static Bitset x509_toxpki (const AsnSequence* snod, const long spos) {
    // map the bit string node
    AsnNode* node = (snod == nullptr) ? nullptr : snod->getnode (spos);
    AsnBits* bnod = dynamic_cast <AsnBits*> (node);
    // check for nil
    if (bnod == nullptr) {
      throw Exception ("x509-error", "invalid nil bit string in sequence");
    }
    // map it as a bitset
    return bnod->tobits ();
  }

  // get the issuer/subject unique id
  static Bitset* x509_toxuid (AsnNode* node) {
    // check for nil first
    if (node == nullptr) return nullptr;
    // map as a buffer
    AsnBuffer* xbuf = dynamic_cast <AsnBuffer*> (node);
    if (xbuf == nullptr) {
      throw Exception ("x509-error", "invalid node as unique id");
    }
    AsnNode* bnod = xbuf->mapnode (AsnNode::ASN_UNIV_BITS);
    AsnBits* bits = dynamic_cast <AsnBits*> (bnod);
    if (bits == nullptr) {
      delete bnod;
      throw Exception ("x509-error", "cannot map unique id node");
    }
    Bitset* bset = new Bitset (bits->tobits ());
    delete bits;
    return bset;
  }

  // get the extensions vector
  static Vector x509_toexts (AsnNode* node) {
    if ((node != nullptr) && (node->iscst () == false)) {
      throw Exception ("X509-error", "invalid implicit extension node");
    }
    if (node->gettagn () != 3) {
      throw Exception ("X509-error", "invalid extension node tag number");
    }
    AsnBuffer* nbuf = dynamic_cast <AsnBuffer*> (node);
    if (nbuf == nullptr) {
      throw Exception ("X509-error", "cannot map extension node buffer");
    }
    // get the content buffer
    Buffer* cbuf = nbuf->getcbuf ();
    if (cbuf == nullptr) {
      throw Exception ("X509-error", "cannot get extension content buffer");
    }
    // map the extension node
    AsnBuffer xbuf (*cbuf);
    AsnNode*  xnod = nullptr;
    try {
      xnod = xbuf.mapnode ();
      delete cbuf;
    } catch (...) {
      delete cbuf;
      throw;
    }
    AsnSequence* snod = dynamic_cast <AsnSequence*> (xnod);
    if (snod == nullptr) {
      delete xnod;
      throw Exception ("x509-error", "cannot map extension sequence node");
    }
    // get the number of extensions
    long xlen = snod->getnlen ();
    // prepare result and process sequence
    Vector result;
    try {
      for (long k = 0; k < xlen; k++) {
	// get the asn node
	AsnNode* enod = snod->getnode (k);
	// get the extension object
	Xextn*   extn = x509_toextn (enod);
	// add it as an extension
	result.add (extn);
      }
    } catch (...) {
      delete xnod;
      throw;
    }
    delete xnod;
    return result;
  }

  // clone a vector of extensions
  Vector X509_copy_exts (const Vector& exts) {
    Vector result;
    long vlen = exts.length ();
    for (long k = 0L; k < vlen; k++) {
      Xextn* extn = dynamic_cast <Xextn*> (exts.get (k));
      if (extn != nullptr) result.add (new Xextn (*extn));
    }
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty certificate

  X509::X509 (void) : Pki (Pem::PEMC_CERT) {
    p_iuid = nullptr;
    p_suid = nullptr;
    reset ();
  }

  // create a certificate by path

  X509::X509 (const String& path) : Pki (Pem::PEMC_CERT) {
    // preset objects
    p_iuid = nullptr;
    p_suid = nullptr;
    // read the file
    Pki::read (path);
  }
  
  // create a certificate by buffer
  
  X509::X509 (const Buffer& xbuf) : Pki (Pem::PEMC_CERT) {
    // preset objects
    p_iuid = nullptr;
    p_suid = nullptr;
    d_xbuf = xbuf;
    // decode the certificate
    if (decode () == false) {
      throw Exception ("X509-error", "cannot decode certificate");
    }
  }

  // copy construct this certificate

  X509::X509 (const X509& that) {
    that.rdlock ();
    try {
      // copy base object
      Pki::operator = (that);
      // copy locally
      d_vers = that.d_vers;
      d_snum = that.d_snum;
      d_slgo = that.d_slgo;
      d_ilst = that.d_ilst;
      d_slst = that.d_slst;
      d_tvlb = that.d_tvlb;
      d_tvla = that.d_tvla;
      d_spka = that.d_spka;
      d_spki = that.d_spki;
      p_iuid = (that.p_iuid == nullptr) ? nullptr : new Bitset (*that.p_iuid);
      p_suid = (that.p_suid == nullptr) ? nullptr : new Bitset (*that.p_suid);
      d_exts = X509_copy_exts (that.d_exts);
      d_csgo = that.d_csgo;
      d_sign = that.d_sign;
      // here it is
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this certificate

  X509::~X509 (void) {
    delete p_iuid;
    delete p_suid;
  }

  // assign a certificate into this one

  X509& X509::operator = (const X509& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // locl and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base object
      Pki::operator = (that);
      // copy locally
      d_vers = that.d_vers;
      d_snum = that.d_snum;
      d_slgo = that.d_slgo;
      d_ilst = that.d_ilst;
      d_slst = that.d_slst;
      d_tvlb = that.d_tvlb;
      d_tvla = that.d_tvla;
      d_spka = that.d_spka;
      d_spki = that.d_spki;
      if (p_iuid != nullptr) delete p_iuid;
      p_iuid = (that.p_iuid == nullptr) ? nullptr : new Bitset (*that.p_iuid);
      if (p_suid != nullptr) delete p_suid;
      p_suid = (that.p_suid == nullptr) ? nullptr : new Bitset (*that.p_suid);
      d_exts = X509_copy_exts (that.d_exts);
      d_csgo = that.d_csgo;
      d_sign = that.d_sign;
      // here it is
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the object class name
  
  String X509::repr (void) const {
    return "X509";
  }
  
  // return a clone of this object

  Object* X509::clone (void) const {
    return new X509 (*this);
  }

  // reset the certificate

  void X509::reset (void) {
    wrlock ();
    try {
      // reset base object
      Pki::reset ();
      // reset locally
      d_vers = 0;
      d_snum = 0;
      d_slgo.reset ();
      d_ilst.reset ();
      d_slst.reset ();
      d_tvlb = 0LL;
      d_tvla = 0LL;
      d_spka.reset ();
      d_spki.reset ();
      d_csgo.reset ();
      d_sign.reset ();
      delete p_iuid; p_iuid = nullptr;
      delete p_suid; p_suid = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the certificate version

  long X509::getvers (void) const {
    rdlock ();
    try {
      long result = d_vers;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the certificate serial number

  Relatif X509::getsnum (void) const {
    rdlock ();
    try {
      Relatif result = d_snum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the signature algorithm

  Xalgo X509::getslgo (void) const {
    rdlock ();
    try {
      Xalgo result = d_slgo;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the issuer plist

  Plist X509::getilst (void) const {
    rdlock ();
    try {
      Plist result = d_ilst;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the subject plist

  Plist X509::getslst (void) const {
    rdlock ();
    try {
      Plist result = d_slst;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the validity before time

  t_long X509::gettvlb (void) const {
    rdlock ();
    try {
      t_long result = d_tvlb;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the validity after time

  t_long X509::gettvla (void) const {
    rdlock ();
    try {
      t_long result = d_tvla;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the subject key algorithm

  Xalgo X509::getspka (void) const {
    rdlock ();
    try {
      Xalgo result = d_spka;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the subject key info

  Bitset X509::getspki (void) const {
    rdlock ();
    try {
      Bitset result = d_spki;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the subject key

  Xkey X509::getskey (void) const {
    rdlock ();
    try {
      Xkey result (d_spka, d_spki);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the issuer unique id

  Bitset* X509::getiuid (void) const {
    rdlock ();
    try {
      Bitset* result = (p_iuid == nullptr) ? nullptr : new Bitset (*p_iuid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the subject unique id

  Bitset* X509::getsuid (void) const {
    rdlock ();
    try {
      Bitset* result = (p_suid == nullptr) ? nullptr : new Bitset (*p_suid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the extension vector

  Vector X509::getexts (void) const {
    rdlock ();
    try {
      Vector result = d_exts;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // encode  the certificate

  bool X509::encode (void) {
    wrlock ();
    try {
      bool result = false;
      unlock ();
      return result;;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode the certificate
  
  bool X509::decode (void) {
    wrlock ();
    AsnSequence* cert = nullptr;
    try {
      // create an asn buffer
      AsnBuffer abuf (d_xbuf);
      // map the buffer to a node
      cert = x509_tocert (abuf);
      // get the tbs node
      AsnSequence* xtbs = x509_toxtbs (cert);
      // get the certificate version
      d_vers = x509_tovers (xtbs);
      // get the serial number
      d_snum = x509_tosnum (xtbs);
      // get the signature algorithm
      d_slgo = x509_toxpka (xtbs, 2);
      // get the issuer as a plist
      d_ilst = x509_toname (xtbs, 3);
      // get the subject as a plist
      d_slst = x509_toname (xtbs, 5);
      // get the validity time
      x509_tovldt (d_tvlb, d_tvla, xtbs);
      // get the subject key info
      AsnSequence* spks = x509_tospks (xtbs);
      d_spka = x509_toxpka (spks, 0);
      d_spki = x509_toxpki (spks, 1);
      // starting now, the certificate might contain extra information
      // which depends on the version and optional value (it's a mess)
      long xlen = xtbs->getnlen ();
      // check version 2 or 3 for content
      if ((d_vers == 0) && (xlen > 7)) {
	throw Exception ("X509-error", "invalid certificate content");
      }
      if ((d_vers == 1) && (xlen > 9)) {
	throw Exception ("X509-error", "invalid certificate content");
      }
      if ((d_vers == 2) && (xlen > 10)) {
	throw Exception ("X509-error", "invalid certificate content");
      }
      AsnNode* iuid = nullptr;
      AsnNode* suid = nullptr;
      AsnNode* exts = nullptr;
      for (long k = 7; k < xlen; k++) {
	AsnNode* node = xtbs->getnode (k);
	t_octa tagn = (node == nullptr) ? 0ULL : node->gettagn ();
	switch (tagn) {
	case 1ULL:
	  if (iuid != nullptr) {
	    throw Exception ("X509-error", "invalid certificate content");
	  }
	  iuid = node;
	  break;
	case 2ULL:
	  if (suid != nullptr) {
	    throw Exception ("X509-error", "invalid certificate content");
	  }
	  suid = node;
	  break;
	case 3ULL:
	  if ((exts != nullptr) || (d_vers < 2)) {
	    throw Exception ("X509-error", "invalid certificate content");
	  }
	  exts = node;
	  break;
	default:
	  throw Exception ("X509-error", "invalid certificate content");
	  break;
	}
      }
      // now process the nodes
      if (iuid != nullptr) p_iuid = x509_toxuid (suid);
      if (suid != nullptr) p_suid = x509_toxuid (iuid);
      if (exts != nullptr) d_exts = x509_toexts (exts);
      // finally extract the signature algorithm
      d_csgo = x509_toxpka (cert, 1);
      d_sign = x509_toxpki (cert, 2);
      // clean and return
      delete cert;
      unlock ();
      return true;
    } catch (...) {
      delete cert;
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETVERS = zone.intern ("get-version");
  static const long QUARK_GETILST = zone.intern ("get-issuer-plist");
  static const long QUARK_GETSLST = zone.intern ("get-subject-plist");
  static const long QUARK_GETSNUM = zone.intern ("get-serial-number");
  static const long QUARK_GETSLGO = zone.intern ("get-signature-algorithm");
  static const long QUARK_GETTVLB = zone.intern ("get-validity-before");
  static const long QUARK_GETTVLA = zone.intern ("get-validity-after");
  static const long QUARK_GETSPKA = zone.intern ("get-subject-key-algorithm");
  static const long QUARK_GETSPKI = zone.intern ("get-subject-key-info");
  static const long QUARK_GETSKEY = zone.intern ("get-subject-key");
  static const long QUARK_GETIUID = zone.intern ("get-issuer-unique-id");
  static const long QUARK_GETSUID = zone.intern ("get-subject-unique-id");
  static const long QUARK_GETEXTS = zone.intern ("get-extension-vector");

  // create a new object in a generic way
 
  Object* X509::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // create a default object
    if (argc == 0) return new X509;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      auto sobj = dynamic_cast<String*> (obj);
      if (sobj != nullptr) return new X509 (*sobj);
      // check for a buffer
      auto bobj = dynamic_cast<Buffer*> (obj);
      if (bobj != nullptr) return new X509 (*bobj);
      // invalid object
      throw Exception ("type-error", "invalid object for X509 constructor",
		       Object::repr (obj));
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with X509 constructor");
  }

  // return true if the given quark is defined

  bool X509::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Pki::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* X509::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETVERS) return new Integer (getvers ());
      if (quark == QUARK_GETSNUM) return new Relatif (getsnum ());
      if (quark == QUARK_GETSLGO) return new Xalgo   (getslgo ());
      if (quark == QUARK_GETILST) return new Plist   (getilst ());
      if (quark == QUARK_GETSLST) return new Plist   (getslst ());
      if (quark == QUARK_GETTVLB) return new Integer (gettvlb ());
      if (quark == QUARK_GETTVLA) return new Integer (gettvla ());
      if (quark == QUARK_GETSPKA) return new Xalgo   (getspka ());
      if (quark == QUARK_GETSPKI) return new Bitset  (getspki ());
      if (quark == QUARK_GETSKEY) return new Xkey    (getskey ());
      if (quark == QUARK_GETIUID) return getiuid ();
      if (quark == QUARK_GETSUID) return getsuid ();
      if (quark == QUARK_GETEXTS) return new Vector  (getexts ());
    }
    // call the pki method
    return Pki::apply (zobj, nset, quark, argv);
  }
}
