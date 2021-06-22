// ---------------------------------------------------------------------------
// - TlsChello.cpp                                                           -
// - afnix:tls service - tls client hello class implementation               -
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
#include "Integer.hpp"
#include "TlsTypes.hxx"
#include "TlsUtils.hpp"
#include "TlsChello.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  static t_byte* tls_new_rand (const t_byte vmaj, const t_byte vmin) {
    // check for version below
    bool tflg = false;
    if (vmaj == TLS_VMAJ_3XX) {
      if (vmin == TLS_VMIN_301) tflg = true;
      if (vmin == TLS_VMIN_302) tflg = true;
    }
    return TlsUtils::random (TLS_SIZE_RND, tflg);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty block

  TlsChello::TlsChello (void) {
    p_rand = TlsUtils::random (TLS_SIZE_RND, false);
    p_sess = nullptr;
    p_suit = nullptr;
    p_cmth = nullptr;
    p_exts = nullptr;
    reset ();
  }

  // create a client hello by version

  TlsChello::TlsChello (const t_byte vmaj, const t_byte vmin) {
    p_rand = nullptr;
    p_sess = nullptr;
    p_suit = nullptr;
    p_cmth = nullptr;
    p_exts = nullptr;
    reset ();
    // check valid version
    if (tls_vers_valid (vmaj, vmin) == false) {
      String vers = tls_vers_tostring (vmaj, vmin);
      throw Exception ("tls-error", "invalid tls version", vers);
    }
    d_vmaj = vmaj;
    d_vmin = vmin;
    // initialize by version
    p_rand = tls_new_rand (vmaj, vmin);
    Object::iref (p_suit = new TlsSuite (vmaj, vmin)); p_suit->mksuit ();
    d_clen = 1L;
    p_cmth = new t_byte[d_clen]; p_cmth[0] = nilb;
  }

  // create a block by record, position, type and length

  TlsChello::TlsChello (TlsHblock* hblk) {
    p_rand = new t_byte[TLS_SIZE_RND];
    p_sess = nullptr;
    p_suit = nullptr;
    p_cmth = nullptr;
    p_exts = nullptr;
    decode (hblk);
  }

  // destroy this  block
  
  TlsChello::~TlsChello (void) {
    reset ();
    delete [] p_rand;
    delete [] p_sess;
    Object::dref (p_suit);
    Object::dref (p_exts);
  }

  // return the class name
  
  String TlsChello::repr (void) const {
    return "TlsChello";
  }

  // reset the block

  void TlsChello::reset (void) {
    wrlock ();
    try {
      d_vmaj = nilc;
      d_vmin = nilc;
      if (p_rand != nullptr) {
	for (long k = 0L; k < TLS_SIZE_RND; k++) p_rand[k] = nilc;
      }
      d_slen = 0L;
      delete [] p_sess; p_sess = nullptr;
      Object::dref (p_suit); p_suit = nullptr;
      d_clen = 0L;
      delete [] p_cmth; p_cmth = nullptr;
      Object::dref (p_exts); p_exts = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block info as a plist

  Plist TlsChello::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // set the header info
      plst.add ("TLS-CHL-VMAJ", "TLS CLIENT HELLO MAJOR VERSION", 
		(t_long) d_vmaj);
      plst.add ("TLS-CHL-VMIN", "TLS CLIENT HELLO MINOR VERSION",
		(t_long) d_vmin);
      plst.add ("TLS-CHL-SLEN", "TLS CLIENT SESSION LENGTH",
		(t_long) d_slen);
      plst.add ("TLS-CHL-CLEN", "TLS CLIENT COMPRESSION LENGTH",
		(t_long) d_clen);
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls major version

  t_byte TlsChello::getvmaj (void) const {
    rdlock ();
    try {
      t_byte result = d_vmaj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls minor version

  t_byte TlsChello::getvmin (void) const {
    rdlock ();
    try {
      t_byte result = d_vmin;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the client random buffer

  Buffer TlsChello::getrand (void) const {
    rdlock ();
    try {
      Buffer result;
      result.add ((char*) p_rand, TLS_SIZE_RND);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls cipher suite

  TlsSuite* TlsChello::getsuit (void) const {
    rdlock ();
    try {
      TlsSuite* result = p_suit;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the extensions vector

  Vector* TlsChello::getexts (void) const {
    rdlock ();
    try {
      Vector* result = p_exts;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // decode the hblock into a client hello

  void TlsChello::decode (TlsHblock* hblk) {
    wrlock ();
    try {
      // reset eveything
      reset ();
      if (hblk == nullptr) {
	unlock ();
	return;
      }
      // the block offset
      long boff = 0L;
      // save version
      d_vmaj = hblk->getbyte (boff++);
      d_vmin = hblk->getbyte (boff++);
      // get the random array
      for (long k = 0L; k < TLS_SIZE_RND; k++) {
	p_rand[k] = hblk->getbyte (boff++);
      }
      // get the session size and data
      d_slen = (long) hblk->getbyte (boff++);
      if (d_slen > 0L) {
	p_sess = new t_byte[d_slen];
	for (long k = 0L; k < d_slen; k++) {
	  p_sess[k] = hblk->getbyte (boff++);
	}
      }	  
      // get the cipher suite size
      long cssz = hblk->getbyte (boff++);
      cssz = cssz << 8; cssz += hblk->getbyte (boff++);
      if ((cssz < 0L) || (cssz > 65535)) {
	throw Exception ("tls-error", 
			 "invalid cipher suite size during decode");
      }
      // create a new cipher suite and load
      Object::iref (p_suit = new TlsSuite (d_vmaj, d_vmin));
      for (long k = 0L; k < cssz; k+=2) {
	t_byte ucod = hblk->getbyte (boff++);
	t_byte lcod = hblk->getbyte (boff++);
	p_suit->add (ucod, lcod);
      }
      // get the compression method size and data
      d_clen = hblk->getbyte (boff++);
      if (d_clen > 0) {
	p_cmth = new t_byte[d_clen];
	for (long k = 0L; k < d_clen; k++) {
	  p_cmth[k] = hblk->getbyte (boff++);
	}
      }
      // get the extra data
      long xlen = hblk->length () - boff;
      // build the extension vector
      if (xlen > 0L) {
	t_byte data[xlen];
	for (long k = 0L; k < xlen; k++) data[k] =  hblk->getbyte(boff++);
	Object::iref(p_exts = TlsExtension::toexts (xlen, data));
      }
      // done
      unlock ();
    } catch (...) {
      reset ();
      unlock ();
      throw;
    }
  }

  // map a client hello to a chunk block

  TlsChunk TlsChello::tochunk (void) const {
    rdlock ();
    try {
      // the result chunk
      TlsChunk result;
      // add the version
      result.add (d_vmaj);
      result.add (d_vmin);
      // add the random array
      for (long k = 0L; k < TLS_SIZE_RND; k++) result.add (p_rand[k]);
      // add the session code
      result.add (nilc);
      // add the cipher suite - length in bytes
      long slen = p_suit->length ();
      result.add (TlsUtils::toucod (slen << 1));
      result.add (TlsUtils::tolcod (slen << 1));
      for (long k = 0L; k < slen; k++) {
	t_word cifr = p_suit->getcifr (k);
	result.add (TlsUtils::toucod (cifr));
	result.add (TlsUtils::tolcod (cifr));
      }
      // add the compression methods
      result.add (d_clen);
      for (long k = 0L; k < d_clen; k++) result.add (p_cmth[k]);
      // here it is
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

  // create a new object in a generic way

  Object* TlsChello::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsChello;
    // check for 1 argument
    if (argc == 1) {
      Object*     obj = argv->get (0);
      TlsHblock* hblk = dynamic_cast<TlsHblock*> (obj);
      if ((hblk == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object as tls client hello handshake block",
			 Object::repr (obj));
      }
      return new TlsChello (hblk);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls client hello");
  }
}
