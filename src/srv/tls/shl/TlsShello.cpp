// ---------------------------------------------------------------------------
// - TlsShello.cpp                                                           -
// - afnix:tls service - tls server hello class implementation               -
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
#include "TlsSuite.hpp"
#include "TlsShello.hpp"
#include "TlsHblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty block

  TlsShello::TlsShello (void) {
    p_rand = nullptr;
    p_sess = nullptr;
    reset ();
  }

  // create a server hello by version and cipher code

  TlsShello::TlsShello (const t_byte vmaj, const t_byte vmin, 
			const t_word cifr) {
    // reset localy
    p_rand = nullptr;
    p_sess = nullptr;
    reset ();
    // check valid version
    if (tls_vers_valid (vmaj, vmin) == false) {
      String vers = tls_vers_tostring (vmaj, vmin);
      throw Exception ("tls-error", "invalid tls version", vers);
    }
    d_vmaj = vmaj;
    d_vmin = vmin;
    d_cifr = cifr;
  }

  // create a block by record, position, type and length

  TlsShello::TlsShello (TlsHblock* hblk) {
    // reset localy
    p_rand = nullptr;
    p_sess = nullptr;
    reset ();
    // decode the block
    decode (hblk);
  }

  // destroy this block
  
  TlsShello::~TlsShello (void) {
    reset ();
    delete [] p_rand;
    delete [] p_sess;
  }

  // return the class name
  
  String TlsShello::repr (void) const {
    return "TlsShello";
  }

  // reset the block

  void TlsShello::reset (void) {
    wrlock ();
    try {
      d_vmaj = nilc;
      d_vmin = nilc;
      delete [] p_rand;
      p_rand = TlsUtils::random (TLS_SIZE_RND, true);
      d_slen = 0L;
      delete [] p_sess; p_sess = nullptr;
      d_cifr = nilw;
      d_cmth = nilc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the block info as a plist

  Plist TlsShello::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // set the header info
      plst.add ("TLS-SHL-VMAJ", "TLS SERVER HELLO MAJOR VERSION", 
		(t_long) d_vmaj);
      plst.add ("TLS-SHL-VMIN", "TLS SERVER HELLO MINOR VERSION",
		(t_long) d_vmin);
      plst.add ("TLS-SHL-SLEN", "TLS SERVER SESSION LENGTH",
		(t_long) d_slen);
      plst.add ("TLS-SHL-CIFR", "TLS SERVER CIPHER CODE",
		(t_long) d_cifr);
      plst.add ("TLS-SHL-CMTH", "TLS SERVER COMPRESSION METHOD",
		(t_long) d_cmth);
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls major version

  t_byte TlsShello::getvmaj (void) const {
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

  t_byte TlsShello::getvmin (void) const {
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
  
  // get the tls cipher code

  t_word TlsShello::getcifr (void) const {
    rdlock ();
    try {
      t_word result = d_cifr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the server random buffer

  Buffer TlsShello::getrand (void) const {
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
  
  // decode the hblock into a server hello

  void TlsShello::decode (TlsHblock* hblk) {
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
      // get the cipher code
      t_byte ucod = hblk->getbyte (boff++);
      t_byte lcod = hblk->getbyte (boff++);
      d_cifr = TlsUtils::toword (ucod, lcod);
      // get the compression method
      d_cmth = hblk->getbyte (boff++);
      // check for consistency
      if (hblk->length () != boff) {
	throw Exception ("tls-error", "spurious data in server hello message");
      }
      // done
      unlock ();
    } catch (...) {
      reset ();
      unlock ();
      throw;
    }
  }

  // map a server hello to a chunk block

  TlsChunk TlsShello::tochunk (void) const {
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
      // add the cipher suite
      result.add (TlsUtils::toucod (d_cifr));
      result.add (TlsUtils::tolcod (d_cifr));
      // add the compression
      result.add (nilc);
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

  Object* TlsShello::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsShello;
    // check for 1 argument
    if (argc == 1) {
      Object*     obj = argv->get (0);
      TlsHblock* hblk = dynamic_cast<TlsHblock*> (obj);
      if ((hblk == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", 
			 "invalid object as tls server hello handshake block",
			 Object::repr (obj));
      }
      return new TlsShello (hblk);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls server hello");
  }
}
