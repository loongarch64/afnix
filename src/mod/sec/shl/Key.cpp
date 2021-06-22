// ---------------------------------------------------------------------------
// - Key.cpp                                                                 -
// - afnix:sec module - key class implementation                             -
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

#include "Key.hpp"
#include "Byte.hpp"
#include "Item.hpp"
#include "Ascii.hpp"
#include "Prime.hpp"
#include "Secsid.hxx"
#include "Vector.hpp"
#include "Crypto.hpp"
#include "Unicode.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // 128 bits key size
  static const long    KEY_128_BITS = 128;

  // the default symmetric key size in bits
  static const long    KEY_SYM_BITS = KEY_128_BITS;
  static const long    KEY_SYM_SIZE = KEY_SYM_BITS / 8;

  // the default rsa key size in bits
  static const long    KEY_RSA_BITS = 1024;
  // the recommended rsa key exponent (NIST)
  static const long    KEY_RSA_REXP = 65537;

  // the default mac key size in bits
  static const long    KEY_MAC_BITS = KEY_128_BITS;
  static const long    KEY_MAC_SIZE = KEY_MAC_BITS / 8;

  // the default dsa key size in bits
  static const long    KEY_DSA_BITS = 1024;
  // the default dsa hash length
  static const long    KEY_DSA_HBIT = 160;
  // the base constant
  static const Relatif KEY_DSA_BTWO = 1;

  // this procedure extract a relatif from an object
  static inline Relatif torel (Object* obj) {
    // check for nil
    if (obj == nullptr) {
      throw Exception ("type-error", "invalid nil object as relatif");
    }
    // check for an integer
    Integer* iobj = dynamic_cast <Integer*> (obj);
    if (iobj != nullptr) return iobj->tolong ();
    // check fo a relatif
    Relatif* zobj = dynamic_cast <Relatif*> (obj);
    if (zobj != nullptr) return *zobj;
    // invalid object
    throw Exception ("type-error", "invalid object as a relatif",
		     Object::repr (obj));    
  }

  // the symmetric cipher key
  struct s_ksym {
    // the number of bytes
    long    d_size;
    // the key buffer
    t_byte* p_kbuf;
    // create a default key
    s_ksym (void) {
      d_size = 0;
      p_kbuf = nullptr;
    }
    // create a key by octet string
    s_ksym (const String& ostr) {
      p_kbuf = Unicode::stob (d_size, ostr);
    }
    // create a key by size and data
    s_ksym (const long size, const t_byte* kval) {
      d_size = size;
      p_kbuf = new t_byte[d_size];
      for (long i = 0; i < d_size; i++) p_kbuf[i] = kval[i];
    }
    // copy construct this key
    s_ksym (const s_ksym& that) {
      d_size = that.d_size;
      p_kbuf = new t_byte[d_size];
      for (long i = 0; i < d_size; i++) p_kbuf[i] = that.p_kbuf[i];
    }
    // destroy this key
    ~s_ksym (void) {
      delete [] p_kbuf;
    }
    // serialize the key
    void wrstream (OutputStream& os) {
      // write the number of bytes
      Serial::wrlong (d_size, os);
      // write the bytes
      for (long k = 0L; k < d_size; k++) os.write ((char) p_kbuf[k]);
    }
    // deserialize the stream
    void rdstream (InputStream& is) {
      // read the number of bytes
      d_size = Serial::rdlong (is);
      // get the bytes
      p_kbuf = new t_byte[d_size];
      for (long k = 0L; k < d_size; k++) p_kbuf[k] = is.read ();
    }
    // return the key size in bits
    long getbits (void) const {
      return d_size * 8;
    }
    // return the key size in byts
    long getsize (void) const {
      return d_size;
    }
    // format the key as a string
    String format (void) const {
      return Ascii::btos (p_kbuf, d_size);
    }
    // load by size with random values
    void ldrand (const long size) {
      // delete old key
      d_size = size;
      delete [] p_kbuf;
      // create buffer and initialize
      p_kbuf = new t_byte[d_size];
      for (long i = 0; i < d_size; i++) p_kbuf[i] = Utility::byternd (); 
    }
    // publicize this key
    s_ksym* topublic (void) const {
      return new s_ksym;
    }
  };

  // the rsa key structure
  struct s_krsa {
    // the modulus
    Relatif d_pmod;
    // the public exponent
    Relatif d_pexp;
    // the secret exponent
    Relatif d_sexp;
    // the secret prime p
    Relatif d_spvp;
    // the secret prime q
    Relatif d_spvq;
    // the crt p exponent
    Relatif d_crtp;
    // the crt q exponent
    Relatif d_crtq;
    // the crt q inverse
    Relatif d_crti;
    // create a null key
    s_krsa (void) {
      d_pmod = 0;
      d_pexp = 0;
      d_sexp = 0;
      d_spvp = 0;
      d_spvq = 0;
      d_crtp = 0;
      d_crtq = 0;
      d_crti = 0;
      
    }
    // copy construct this rsa key
    s_krsa (const s_krsa& that) {
      d_pmod = that.d_pmod;
      d_pexp = that.d_pexp;
      d_sexp = that.d_sexp;
      d_spvp = that.d_spvp;
      d_spvq = that.d_spvq;
      d_crtp = that.d_crtp;
      d_crtq = that.d_crtq;
      d_crti = that.d_crti;
    }
    void wrstream (OutputStream& os) {
      d_pmod.wrstream (os);
      d_pexp.wrstream (os);
      d_sexp.wrstream (os);
      d_spvp.wrstream (os);
      d_spvq.wrstream (os);
      d_crtp.wrstream (os);
      d_crtq.wrstream (os);
      d_crti.wrstream (os);
    }
    void rdstream (InputStream& is) {
      d_pmod.rdstream (is);
      d_pexp.rdstream (is);
      d_sexp.rdstream (is);
      d_spvp.rdstream (is);
      d_spvq.rdstream (is);
      d_crtp.rdstream (is);
      d_crtq.rdstream (is);
      d_crti.rdstream (is);
    }
    // return the key size in bits
    long getbits (void) const {
      return d_pmod.getmsb ();
    }
    // return the key size in bytes
    long getsize (void) const {
      return d_pmod.getmsb () / 8;
    }
    // return a formatted key by type
    String format (const Key::t_ikey type) const {
      if (type == Key::KRSA_PMOD) return d_pmod.tohexa ();
      if (type == Key::KRSA_PEXP) return d_pexp.tohexa ();
      if (type == Key::KRSA_SEXP) return d_sexp.tohexa ();
      throw Exception ("key-error", "invalid rsa key accessor");
    }
    // return a relatif key by type
    Relatif getrkey (const Key::t_ikey type) const {
      if (type == Key::KRSA_PMOD) return d_pmod;
      if (type == Key::KRSA_PEXP) return d_pexp;
      if (type == Key::KRSA_SEXP) return d_sexp;
      throw Exception ("key-error", "invalid rsa key accessor");
    }
    // create a random key by size
    void ldrand (const long bits, const Relatif& e) {
      // check the size
      if (bits < 32) {
	throw Exception ("key-error", "random rsa key size is to small");
      }
      // check for parity
      if ((bits % 2) != 0) {
	throw Exception ("key-error", "random rsa key size is not even");
      }
      do {
	// adjust to even
	long pbit = (bits + 1) / 2;
	long qbit = bits - pbit;
	// get two prime numbers
	d_spvp = Prime::random (pbit);
	d_spvq = Prime::random (qbit);
	// compute the modulus
	d_pmod = d_spvp * d_spvq;
      } while ((d_spvp == d_spvq) || (d_pmod.getmsb () != bits));
      // swap p and q if p > q
      if (d_spvp < d_spvq) {
	Relatif t = d_spvp;
	d_spvp = d_spvq;
	d_spvq = t;
      }
      // get the lcm (originally the totient)
      Relatif m = Relatif::lcm (d_spvp - 1, d_spvq - 1);
      // verify that e is coprime with m
      if (Relatif::gcd (m, e) != 1) {
	throw Exception ("key-error", "random rsa key exponent not valid");
      }
      // update key values
      d_pexp = e;
      d_sexp = Relatif::mmi (e, m);
      // compute crt values
      d_crtp = d_sexp % (d_spvp - 1);
      d_crtq = d_sexp % (d_spvq - 1);
      d_crti = Relatif::mmi (d_spvq, d_spvp);
    }
    // load a key by a number vector
    void ldnvec (const Vector& nvec) {
      // check vector length
      long vlen = nvec.length ();
      if ((vlen != 2) && (vlen != 3) && (vlen != 8)) {
	throw Exception ("key-error", "invalid rsa key vector size");
      }
      // set base elements
      d_pmod = torel (nvec.get (0));
      d_pexp = torel (nvec.get (1));
      // set secret key element
      if ((vlen == 3) || (vlen == 8)) {
	d_sexp = torel (nvec.get (2));
      }
      // set extra elements
      if (vlen == 8) {
	d_spvp = torel (nvec.get (3));
	d_spvq = torel (nvec.get (4));
	d_crtp = torel (nvec.get (5));
	d_crtq = torel (nvec.get (6));
	d_crti = torel (nvec.get (7));
      }
    }
    // publicize this key
    s_krsa* topublic (void) const {
      s_krsa* result = new s_krsa;
      result->d_pmod = d_pmod;
      result->d_pexp = d_pexp;
      return result;
    }
  };

  // the mac key
  struct s_kmac {
    // the number of bytes
    long    d_size;
    // the key buffer
    t_byte* p_kbuf;
    // create a default key
    s_kmac (void) {
      d_size = 0;
      p_kbuf = nullptr;
    }
    // create a key by octet string
    s_kmac (const String& ostr) {
      if (Number::ishnum (ostr) == true) {
	p_kbuf = Unicode::stob (d_size, ostr);
      } else {
	d_size = ostr.length ();
	p_kbuf = (d_size == 0L) ? nullptr : (t_byte*) ostr.tochar ();
      }
    }
    // create a key by size and data
    s_kmac (const long size, const t_byte* kval) {
      d_size = size;
      p_kbuf = new t_byte[d_size];
      for (long i = 0; i < d_size; i++) p_kbuf[i] = kval[i];
    }
    // copy construct this key
    s_kmac (const s_kmac& that) {
      d_size = that.d_size;
      p_kbuf = new t_byte[d_size];
      for (long i = 0; i < d_size; i++) p_kbuf[i] = that.p_kbuf[i];
    }
    // destroy this key
    ~s_kmac (void) {
      delete [] p_kbuf;
    }
    // serialize the key
    void wrstream (OutputStream& os) {
      // write the number of bytes
      Serial::wrlong (d_size, os);
      // write the bytes
      for (long k = 0L; k < d_size; k++) os.write ((char) p_kbuf[k]);
    }
    // deserialize the stream
    void rdstream (InputStream& is) {
      // read the number of bytes
      d_size = Serial::rdlong (is);
      // get the bytes
      p_kbuf = new t_byte[d_size];
      for (long k = 0L; k < d_size; k++) p_kbuf[k] = is.read ();
    }
    // return the key size in bits
    long getbits (void) const {
      return d_size * 8;
    }
    // return the key size in byts
    long getsize (void) const {
      return d_size;
    }
    // format the key as a string
    String format (void) const {
      return Ascii::btos (p_kbuf, d_size);
    }
    // load by size with random values
    void ldrand (const long size) {
      // delete old key
      d_size = size;
      delete [] p_kbuf;
      // create buffer and initialize
      p_kbuf = new t_byte[d_size];
      for (long i = 0; i < d_size; i++) p_kbuf[i] = Utility::byternd (); 
    }
    // publicize this key
    s_kmac* topublic (void) const {
      return new s_kmac;
    }
  };

  // validate the dsa prime p size according to FIPS PUB 186-4
  static inline bool dsa_check_size_p (const long psiz) {
    if (psiz == 1024) return true;
    if (psiz == 2048) return true;
    if (psiz == 3072) return true;
    return false;
  }

  // validate the dsa prime q size according to FIPS PUB 186-4
  static inline bool dsa_check_size_q (const long psiz, const long qsiz) {
    if ((psiz == 1024) && (qsiz == 160)) return true;
    if (psiz == 2048) {
      if (qsiz == 224) return true;
      if (qsiz == 256) return true;
      return false;
    }
    if ((psiz == 3072) && (qsiz == 256)) return true;
    return false;
  }

  // get the default dsa hash size according to FIPS PUB 186-4
  static inline long dsa_hash_size (const long size) {
    if (size == 1024) return 160;
    if (size == 2048) return 256;
    if (size == 3072) return 256;
    throw Exception ("key-error", "invalid dsa key size");
  }

  // get a dsa hasher by hash length
  static inline Hasher* dsa_get_hasher (const long hlen) {
    if (hlen == 160) return Crypto::mkhasher (Crypto::HASH_SHA1);
    if (hlen == 224) return Crypto::mkhasher (Crypto::HASH_SHA224);
    if (hlen == 256) return Crypto::mkhasher (Crypto::HASH_SHA256);
    return nullptr;
  }

  // this procedure check for a prime number according to FIPS PUB 186-4
  static inline bool dsa_check_prime (const Relatif& n, const long bits,
				      const long hlen) {
    // compute minimum number of iterations
    long mni = 
      ((bits == 1024) && (hlen == 160)) ? 40 :
      ((bits == 2048) && (hlen == 224)) ? 56 :
      ((bits == 2048) && (hlen == 256)) ? 56 : 64;
    // check for primality
    return Prime::isprobable (n, mni);
  }

  // hash the seed and force the msb/lsb result
  static inline Relatif dsa_hash_seed (const Relatif& dsp, Hasher* hash) {
    // check for hash
    if (hash == nullptr) return 0;
    // create the relatif buffer
    long blen = dsp.getbbs ();
    t_byte bbuf[blen];
    dsp.toubuf (bbuf, blen);
    // hash the buffer
    hash->reset  ();
    hash->process (bbuf, blen);
    hash->finish ();
    // extract the hash data
    long hlen = hash->gethlen ();
    t_byte hbuf[hlen];
    for (long i = 0; i < hlen; i++) hbuf[i] = hash->getbyte (i);
    // force the msb and lsb 
    hbuf[0] |= 0x80;
    hbuf[hlen-1] |= 0x01;
    // create the result
    return Relatif (hbuf, hlen);
  }

  // hash the dsp
  static inline Relatif dsa_hash_dps (const Relatif& dsp, Hasher* hash) {
    // check for hash
    if (hash == nullptr) return 0;
    // create the relatif buffer
    long blen = dsp.getbbs ();
    t_byte bbuf[blen];
    dsp.toubuf (bbuf, blen);
    // hash the buffer
    hash->reset  ();
    hash->process (bbuf, blen);
    hash->finish ();
    // extract the hash data
    long hlen = hash->gethlen ();
    t_byte hbuf[hlen];
    for (long i = 0; i < hlen; i++) hbuf[i] = hash->getbyte (i);
    // create the result
    return Relatif (hbuf, hlen);
  }

  // the dsa key structure
  struct s_kdsa {
    // the global prime p
    Relatif d_glop;
    // the global prime q
    Relatif d_gloq;
    // the global generator
    Relatif d_glog;
    // the secret key x
    Relatif d_skey;
    // the public key y
    Relatif d_pkey;
    // create a null key
    s_kdsa (void) {
      d_glop = 0;
      d_gloq = 0;
      d_glog = 0;
      d_skey = 0;
      d_pkey = 0;
    }
    // copy construct this rsa key
    s_kdsa (const s_kdsa& that) {
      d_glop = that.d_glop;
      d_gloq = that.d_gloq;
      d_glog = that.d_glog;
      d_skey = that.d_skey;
      d_pkey = that.d_pkey;
    }
    // serialize this key
    void wrstream (OutputStream& os) {
      d_glop.wrstream (os);
      d_gloq.wrstream (os);
      d_glog.wrstream (os);
      d_skey.wrstream (os);
      d_pkey.wrstream (os);
    }
    // deserialize this key
    void rdstream (InputStream& is) {
      d_glop.rdstream (is);
      d_gloq.rdstream (is);
      d_glog.rdstream (is);
      d_skey.rdstream (is);
      d_pkey.rdstream (is);
    }
    // return the key size in bits
    long getbits (void) const {
      return d_glop.getmsb ();
    }
    // return the key size in bytes
    long getsize (void) const {
      return d_glop.getmsb () / 8;
    }
    // return a formatted key by type
    String format (const Key::t_ikey type) const {
      if (type == Key::KDSA_PPRM) return d_glop.tohexa ();
      if (type == Key::KDSA_QPRM) return d_gloq.tohexa ();
      throw Exception ("key-error", "invalid dsa key accessor");
    }
    // return a relatif key by type
    Relatif getrkey (const Key::t_ikey type) const {
      if (type == Key::KDSA_PPRM) return d_glop;
      if (type == Key::KDSA_QPRM) return d_gloq;
      if (type == Key::KDSA_SKEY) return d_skey;
      if (type == Key::KDSA_PKEY) return d_pkey;
      if (type == Key::KDSA_PGEN) return d_glog;
      throw Exception ("key-error", "invalid dsa key accessor");
    }
    // create a random key by size
    void ldrand (const long bits) {
      // extract the hash size
      long hlen = dsa_hash_size (bits);
      // load the key
      ldrand (bits, hlen);
    }
    // create a random key by size
    void ldrand (const long bits, const long hlen) {
      // set the seed length
      long slen = hlen;
      // generate with a seed length
      ldrand (bits, hlen, slen);
    }
    // create the key according to FIPS PUB 186-4
    void ldrand (const long bits, const long hlen, const long slen) {
      // step 1: check p and q requested size
      if (dsa_check_size_p (bits) == false) {
	throw Exception ("key-error", "invalid dsa key size");
      }
      if (dsa_check_size_q (bits, hlen) == false) {
	throw Exception ("key-error", "invalid dsa hash size");
      }
      // step 2: check the seed length
      if (slen < hlen) {
	throw Exception ("key-error", "invalid hash seed size");
      }
      // create a hasher
      Hasher* hash = dsa_get_hasher (hlen);
      // build the key
      try {
	// step 3 (outlen == hlen)
	long n = bits / hlen;
	if ((bits % hlen) == 0) n-= 1;
	// step 4
	long b = bits - 1 - (n * hlen);
	// usefull constant 2**L-1
	Relatif pl = KEY_DSA_BTWO << (bits - 1);
	// set prime flag
	bool pflg = false;
	// main loop
	while (pflg == false) {
	  // step 5
	  Relatif dps = Relatif::random (slen);
	  // step 6-7
	  d_gloq = dsa_hash_seed (dps, hash);
	  // step 8-9
	  if (dsa_check_prime (d_gloq, bits, hlen) == false) continue;
	  // step 10
	  long off = 1;
	  // step 11
	  long cmx = 4 * bits;
	  for (long cnt = 0; cnt < cmx; cnt++) {
	    Relatif w = 0;
	    // step 11.1
	    for (long j = 0; j < n; j++) {
	      Relatif v = dsa_hash_dps (dps + off + j, hash);
	      w += (j == 0) ? v : v << ((j * hlen)-1);
	    }
	    // step 11.2 - last item
	    Relatif vn = dsa_hash_dps (dps + off + n, hash);
	    w += (vn % (KEY_DSA_BTWO << (b-1))) << ((n * hlen)-1);
	    // step 11.3
	    Relatif x = w + pl;
	    // step 11.4
	    Relatif c = x % (d_gloq * 2);
	    // step 11.5
	    d_glop = x - (c - 1);
	    // step 11.6
	    if (d_glop >= pl) {
	      // step 11.7 - 11.8
	      if (dsa_check_prime (d_glop, bits, hlen) == true) {
		pflg = true;
		break;
	      }
	    }
	    // reset p candidate
	    d_glop = 0;
	    // step 11.9
	    off += (n + 1);
	  }
	}
	// verification
	if (d_glop == 0) {
	  throw Exception ("key-error", "invalid dsa prime key");
	}
	// generation of g - PUB 186-4
	Relatif pm = d_glop - 1;
	if ((pm % d_gloq) != 0) {
	  throw Exception ("key-error", "invalid dsa prime key");
	}      
	// step 1
	Relatif e = pm / d_gloq;
	// set generator flag
	bool gflg = false;
	// main loop
	while (gflg == false) {
	  // step 2
	  Relatif h = Relatif::random (pm);
	  if (h < 1) continue;
	  // step 3
	  d_glog = Relatif::mme (h, e, d_glop);
	  // step 4
	  if (d_glop == 1) continue;
	  // step 5
	  gflg = true;
	}
	// secret key generation
	while (true) {
	  d_skey = Relatif::random (d_gloq - 1);
	  if (d_skey > 1) break;
	}
	// public key generation
	d_pkey = Relatif::mme (d_glog, d_skey, d_glop);
	// clean the hasher
	delete hash;
      } catch (...) {
	delete hash;
	throw;
      }
    }
    // load a key by a number vector
    void ldnvec (const Vector& nvec) {
      // check vector length
      long vlen = nvec.length ();
      if ((vlen != 4) && (vlen != 5)) {
	throw Exception ("key-error", "invalid dsa key vector size");
      }
      // set base element
      d_glop = torel (nvec.get (0));
      d_gloq = torel (nvec.get (1));
      d_glog = torel (nvec.get (2));
      if (vlen == 4) {
	d_pkey = torel (nvec.get (3));
      }
      if (vlen == 5) {
	d_skey = torel (nvec.get (3));
	d_pkey = torel (nvec.get (4));
      }
    }

    // publicize this key
    s_kdsa* topublic (void) const {
      s_kdsa* result = new s_kdsa;
      result->d_glop = d_glop;
      result->d_gloq = d_gloq;
      result->d_glog = d_glog;
      result->d_pkey = d_pkey;
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default random key

  Key::Key (void) {
    // default nil key
    d_type = CKEY_KNIL;
    p_knil = nullptr;
  }

  // create a default key by type

  Key::Key (const t_ckey type) {
    // save the type
    d_type = type;
    // check for nil key
    if (d_type == CKEY_KNIL) {
      p_knil = nullptr;
    }
    // check for symmetric key
    if (d_type == CKEY_KSYM) {
      p_ksym = new s_ksym;
      // load the key with random values
      p_ksym->ldrand (KEY_SYM_SIZE);
    }
    // check for rsa key
    if (d_type == CKEY_KRSA) {
      p_krsa = new s_krsa;
      // load the key with random values
      p_krsa->ldrand (KEY_RSA_BITS, KEY_RSA_REXP);
    }
    // check for mac key
    if (d_type == CKEY_KMAC) {
      p_kmac = new s_kmac;
      // load the key with random values
      p_kmac->ldrand (KEY_MAC_SIZE);
    }
    // check for dsa key
    if (d_type == CKEY_KDSA) {
      p_kdsa = new s_kdsa;
      // load the key with random values
      p_kdsa->ldrand (KEY_DSA_BITS, KEY_DSA_HBIT);
    }
  }

  // create a symmetric key by octet string

  Key::Key (const String& ostr) {
    d_type = CKEY_KSYM;
    p_ksym = new s_ksym (ostr);
  }

  // create a symmetric key by buffer

  Key::Key (const Buffer& kbuf) {
    // get the buffer content
    t_byte* kval = reinterpret_cast <t_byte*> (kbuf.tochar ());
    // set the key
    try {
      d_type = CKEY_KSYM;
      p_ksym = new s_ksym (kbuf.length(), kval);
      delete [] kval;
    } catch (...) {
      delete [] kval;
      throw;
    } 
  }
  
  // create a key by type and bit size

  Key::Key (const t_ckey type, const long bits) {
    // check the bit size
    if (bits <= 0) {
      throw Exception ("size-error", "invalid key bit size");
    }
    // check for nil key
    if (type == CKEY_KNIL) {
      throw Exception ("type-error", "invalid key nil type with bit size");
    }
    // check for symmetric key
    if (type == CKEY_KSYM) {
      if ((bits % 8) != 0) {
	throw Exception ("size-error", 
			 "unaligned byte size for symmetric key");
      }
      // save the type
      d_type = type;
      // create the symmetric key
      p_ksym = new s_ksym;
      // load the key with random byte
      p_ksym->ldrand (bits/8);
    } 
    // check for rsa key
    if (type == CKEY_KRSA) {
      // save the type
      d_type = type;
      // create the rsa key
      p_krsa = new s_krsa;
      // load the key with random primes
      p_krsa->ldrand (bits, KEY_RSA_REXP);
    }
    // check for mac key
    if (type == CKEY_KMAC) {
      if ((bits % 8) != 0) {
	throw Exception ("size-error", 
			 "unaligned byte size for mac key");
      }
      // save the type
      d_type = type;
      // create the mac key
      p_kmac = new s_kmac;
      // load the key with random byte
      p_kmac->ldrand (bits/8);
    }
    // check for dsa key
    if (type == CKEY_KDSA) {
      // save the type
      d_type = type;
      // create the rsa key
      p_kdsa = new s_kdsa;
      // load the key with random primes
      p_kdsa->ldrand (bits);
    } 
  }
  
  // create a key by type and buffer

  Key::Key (const t_ckey type, const Buffer& kbuf) {
    // save the type
    d_type = type;
    // check for nil key
    if (type == CKEY_KNIL) {
      throw Exception ("type-error", "invalid key nil type with byte array");
    }
    // check for symmetric key
    if (d_type == CKEY_KSYM) {
      // get the buffer content
      t_byte* kval = reinterpret_cast <t_byte*> (kbuf.tochar ());
      // create the key
      p_ksym = new s_ksym (kbuf.length(), kval);
      delete [] kval;
    }
    // check for rsa key
    if (type == CKEY_KRSA) {
      throw Exception ("type-error", "invalid key rsa type with byte array");
    }
    // check for mac key
    if (d_type == CKEY_KMAC) {
      // get the buffer content
      t_byte* kval = reinterpret_cast <t_byte*> (kbuf.tochar ());
      // create the key
      p_kmac = new s_kmac (kbuf.length (), kval);
      delete [] kval;
    }
    // check for rsa key
    if (type == CKEY_KDSA) {
      throw Exception ("type-error", "invalid key dsa type with byte array");
    }
  }
  
  // create a key by type and octet string

  Key::Key (const t_ckey type, const String& ostr) {
    // save the type
    d_type = type;
    // check for nil key
    if (type == CKEY_KNIL) {
      throw Exception ("type-error", "invalid key nil type with octet string");
    }
    // check for symmetric key
    if (d_type == CKEY_KSYM) {
      p_ksym = new s_ksym (ostr);
    }
    // check for rsa key
    if (type == CKEY_KRSA) {
      throw Exception ("type-error", "invalid key rsa type with octet string");
    }
    // check for mac key
    if (d_type == CKEY_KMAC) {
      p_kmac = new s_kmac (ostr);
    }
    // check for rsa key
    if (type == CKEY_KDSA) {
      throw Exception ("type-error", "invalid key type by octet string");
    }
  }

  // create a key by type and byte buffer

  Key::Key (const t_ckey type, const long size, const t_byte* kval) {
    // save the type
    d_type = type;
    // check for symmetric key
    if (d_type == CKEY_KSYM) {
      p_ksym = new s_ksym (size, kval);
    }
    // check for rsa key
    if (type == CKEY_KRSA) {
      throw Exception ("type-error", "invalid key type by byte array");
    }
    // check for mac key
    if (d_type == CKEY_KMAC) {
      p_kmac = new s_kmac (size, kval);
    }
    // check for rsa key
    if (type == CKEY_KDSA) {
      throw Exception ("type-error", "invalid key type by byte array");
    }
  }

  // create a key by type and object vector
  
  Key::Key (const t_ckey type, const Vector& ovec) {
    // check for nil key
    if (type == CKEY_KNIL) {
      throw Exception ("type-error", "invalid key nil type with object vector");
    }
    // check for symmetric key
    if (type == CKEY_KSYM) {
      throw Exception ("type-error", "invalid key sym type with object vector");
    }
    // check key type
    if (type == CKEY_KRSA) {
      // save the type
      d_type = type;
      // create the rsa key
      p_krsa = new s_krsa;
      // load the key with a number vector
      p_krsa->ldnvec (ovec);
    }
    // check for mac key
    if (type == CKEY_KMAC) {
      throw Exception ("type-error", "invalid key type by object vector");
    }
    // check for dsa key
    if (type == CKEY_KDSA) {
      // save the type
      d_type = type;
      // create the rsa key
      p_kdsa = new s_kdsa;
      // load the key with a number vector
      p_kdsa->ldnvec (ovec);
    }
  }

  // copy construct this key

  Key::Key (const Key& that) {
    that.rdlock ();
    try {
      // copy base key
      Taggable::operator = (that);
      // copy locally
      d_type = that.d_type;
      switch (d_type) {
      case CKEY_KNIL:
	p_knil = that.p_knil;
	break;
      case CKEY_KSYM:
	p_ksym = new s_ksym (*that.p_ksym);
	break;
      case CKEY_KRSA:
	p_krsa = new s_krsa (*that.p_krsa);
	break;
      case CKEY_KMAC:
	p_kmac = new s_kmac (*that.p_kmac);
	break;
      case CKEY_KDSA:
	p_kdsa = new s_kdsa (*that.p_kdsa);
	break;
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this key

  Key::~Key (void) {
    reset ();
  }

  // assign a key to this one

  Key& Key::operator = (const Key& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base key
      Taggable::operator = (that);
      // assign locally
      if (this != &that) {
	// clean the old key
	switch (d_type) {
	case CKEY_KSYM:
	  delete p_ksym;
	  p_ksym = nullptr;
	  break;
	case CKEY_KRSA:
	  delete p_krsa;
	  p_krsa = nullptr;
	  break;
	case CKEY_KMAC:
	  delete p_kmac;
	  p_kmac = nullptr;
	  break;
	case CKEY_KDSA:
	  delete p_kdsa;
	  p_kdsa = nullptr;
	  break;
	default:
	  break;
	}
	d_type = that.d_type;
	// set the new key
	switch (d_type) {
	case CKEY_KNIL:
	  p_knil = that.p_knil;
	  break;
	case CKEY_KSYM:
	  p_ksym = new s_ksym (*that.p_ksym);
	  break;
	case CKEY_KRSA:
	  p_krsa = new s_krsa (*that.p_krsa);
	  break;
	case CKEY_KMAC:
	  p_kmac = new s_kmac (*that.p_kmac);
	  break;
	case CKEY_KDSA:
	  p_kdsa = new s_kdsa (*that.p_kdsa);
	  break;
	}
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

  // return the class name

  String Key::repr (void) const {
    return "Key";
  }

  // return a clone of this object

  Object* Key::clone (void) const {
    return new Key (*this);
  }

  // return the serial did

  t_word Key::getdid (void) const {
    return SRL_DEOD_SEC;
  }

  // return the serial sid

  t_word Key::getsid (void) const {
    return SRL_SKEY_SID;
  }
  
  // serialize this key

  void Key::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base object
      Taggable::wrstream (os);
      // serialize locally
      os.write ((char) d_type);
      switch (d_type) {
      case CKEY_KSYM:
	p_ksym->wrstream (os);
	break;
      case CKEY_KRSA:
	p_krsa->wrstream (os);
	break;
      case CKEY_KMAC:
	p_kmac->wrstream (os);
	break;
      case CKEY_KDSA:
	p_kdsa->wrstream (os);
	break;
      default:
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this key

  void Key::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base object
      Taggable::rdstream (is);
      // deserialize locally
      switch (is.read ()) {
      case CKEY_KNIL:
	d_type = CKEY_KNIL;
	break;
      case CKEY_KSYM:
	d_type = CKEY_KSYM;
	p_ksym = new s_ksym; p_ksym->rdstream (is);
	break;
      case CKEY_KRSA:
	d_type = CKEY_KRSA;
	p_krsa = new s_krsa; p_krsa->rdstream (is);
	break;
      case CKEY_KMAC:
	d_type = CKEY_KMAC;
	p_kmac = new s_kmac; p_kmac->rdstream (is);
	break;
      case CKEY_KDSA:
	d_type = CKEY_KDSA;
	p_kdsa = new s_kdsa; p_kdsa->rdstream (is);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this key

  void Key::reset (void) {
    wrlock ();
    try {
      // destroy this key
      switch (d_type) {
      case CKEY_KSYM:
	delete p_ksym; p_ksym = nullptr;
	break;
      case CKEY_KRSA:
	delete p_krsa; p_krsa = nullptr;
	break;
      case CKEY_KMAC:
	delete p_kmac; p_kmac = nullptr;
	break;
      case CKEY_KDSA:
	delete p_kdsa; p_kdsa = nullptr;
	break;
      default:
	break;
      }
      d_type = CKEY_KNIL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the key is a block cipher key

  bool Key::isbk (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_type) {
      case CKEY_KSYM:
	result = true;
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the key is a public cipher key

  bool Key::ispk (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_type) {
      case CKEY_KRSA:
	result = true;
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the key is a mac key

  bool Key::ismk (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_type) {
      case CKEY_KMAC:
	result = true;
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the key is a serial cipher key

  bool Key::isck (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_type) {
      case CKEY_KSYM:
	result = true;
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the key is a signature key

  bool Key::issk (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_type) {
      case CKEY_KDSA:
	result = true;
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the key type

  Key::t_ckey Key::gettype (void) const {
    rdlock ();
    try {
      t_ckey result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the key size in bits

  long Key::getbits (void) const {
    rdlock ();
    try {
      long result = 0;
      switch (d_type) {
      case CKEY_KNIL:
	result = 0L;
	break;
      case CKEY_KSYM:
	result = p_ksym->getbits ();
	break;
      case CKEY_KRSA:
	result = p_krsa->getbits ();
	break;
      case CKEY_KMAC:
	result = p_kmac->getbits ();
	break;
      case CKEY_KDSA:
	result = p_kdsa->getbits ();
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the key size in bytes

  long Key::getsize (void) const {
    rdlock ();
    try {
      long result = 0;
      switch (d_type) {
      case CKEY_KNIL:
	result = 0L;
	break;
      case CKEY_KSYM:
	result = p_ksym->getsize ();
	break;
      case CKEY_KRSA:
	result = p_krsa->getsize ();
	break;
      case CKEY_KMAC:
	result = p_kmac->getsize ();
	break;
      case CKEY_KDSA:
	result = p_kdsa->getsize ();
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a formatted key representation
  
  String Key::format (void) const {
    rdlock ();
    try {
      // check for symmetric key
      if (d_type == CKEY_KSYM) {
	String result = p_ksym->format ();
	unlock ();
	return result;
      }
      // check for mac key
      if (d_type == CKEY_KMAC) {
	String result = p_kmac->format ();
	unlock ();
	return result;
      }
      // invalid key type in format
      throw Exception ("type-error", "invalid key type in format");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a formatted key representation by type

  String Key::format (const t_ikey type) const {
    rdlock ();
    try {
      // initialize result
      String result;
      // select the key
      switch (d_type) {
      case CKEY_KRSA:
	result = p_krsa->format (type);
	break;
      case CKEY_KDSA:
	result = p_kdsa->format (type);
	break;
      default:
	throw Exception ("key-error", "unsupported key type accessor");
	break;
      }
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a relatif key by type

  Relatif Key::getrkey (const t_ikey type) const {
    rdlock ();
    try {
      // initialize result
      Relatif result = 0;
      // select the key
      switch (d_type) {
      case CKEY_KRSA:
	result = p_krsa->getrkey (type);
	break;
      case CKEY_KDSA:
	result = p_kdsa->getrkey (type);
	break;
      default:
	throw Exception ("key-error", "unsupported key type accessor");
	break;
      }
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a key byte by index

  t_byte Key::getbyte (const long index) const {
    rdlock ();
    try {
      // check for symmetric key
      if (d_type == CKEY_KSYM) {
	// get key size and check
	if ((index < 0) || (index >= p_ksym->d_size)) {
	  throw Exception ("key-error","index is out of bound");
	}
	// key key byte
	t_byte result = p_ksym->p_kbuf[index];
	unlock ();
	return result;
      }
      // check for mac key
      if (d_type == CKEY_KMAC) {
	// get key size and check
	if ((index < 0) || (index >= p_kmac->d_size)) {
	  throw Exception ("key-error","index is out of bound");
	}
	// key key byte
	t_byte result = p_kmac->p_kbuf[index];
	unlock ();
	return result;
      }
      // invalid key type in format
      throw Exception ("key-error", "invalid key type in get byte");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // publicize this key by removing the secret part
  
  Key Key::topublic (void) const {
    rdlock ();
    try {
      Key result;
      result.d_type = d_type;
      switch (d_type) {
      case CKEY_KNIL:
	result.p_knil = nullptr;
	break;
      case CKEY_KSYM:
	result.p_ksym = p_ksym->topublic ();
	break;
      case CKEY_KRSA:
	result.p_krsa = p_krsa->topublic ();
	break;
      case CKEY_KMAC:
	result.p_kmac = p_kmac->topublic ();
	break;
      case CKEY_KDSA:
	result.p_kdsa = p_kdsa->topublic ();
	break;
      }
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

  // the object eval quarks
  static const long QUARK_KEY      = String::intern ("Key");
  static const long QUARK_CKEYKNIL = String::intern ("KNIL");
  static const long QUARK_CKEYKSYM = String::intern ("KSYM");
  static const long QUARK_CKEYKRSA = String::intern ("KRSA");
  static const long QUARK_CKEYKMAC = String::intern ("KMAC");
  static const long QUARK_CKEYKDSA = String::intern ("KDSA");
  static const long QUARK_KRSAPMOD = String::intern ("RSA-MODULUS");
  static const long QUARK_KRSAPEXP = String::intern ("RSA-PUBLIC-EXPONENT");
  static const long QUARK_KRSASEXP = String::intern ("RSA-SECRET-EXPONENT");
  static const long QUARK_KDSAPPRM = String::intern ("DSA-P-PRIME");
  static const long QUARK_KDSAQPRM = String::intern ("DSA-Q-PRIME");
  static const long QUARK_KDSASKEY = String::intern ("DSA-SECRET-KEY");
  static const long QUARK_KDSAPKEY = String::intern ("DSA-PUBLIC-KEY");
  static const long QUARK_KDSAPGEN = String::intern ("DSA-PUBLIC-GENERATOR");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TOPUB    = zone.intern ("to-public");
  static const long QUARK_FORMAT   = zone.intern ("format");
  static const long QUARK_GETBITS  = zone.intern ("get-bits");
  static const long QUARK_GETSIZE  = zone.intern ("get-size");
  static const long QUARK_GETTYPE  = zone.intern ("get-type");
  static const long QUARK_GETBYTE  = zone.intern ("get-byte");
  static const long QUARK_GETRKEY  = zone.intern ("get-relatif-key");
  
  // map an item to a key type
  static inline Key::t_ckey item_to_ckey (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_KEY)
      throw Exception ("item-error", "item is not a key item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_CKEYKNIL) return Key::CKEY_KNIL;
    if (quark == QUARK_CKEYKSYM) return Key::CKEY_KSYM;
    if (quark == QUARK_CKEYKRSA) return Key::CKEY_KRSA;
    if (quark == QUARK_CKEYKMAC) return Key::CKEY_KMAC;
    if (quark == QUARK_CKEYKDSA) return Key::CKEY_KDSA;
    throw Exception ("item-error", "cannot map item to key type");
  }

  // map a key type to an item
  static inline Item* ckey_to_item (const Key::t_ckey type) {
    switch (type) {
    case Key::CKEY_KNIL:
      return new Item (QUARK_KEY, QUARK_CKEYKNIL);
      break;
    case Key::CKEY_KSYM:
      return new Item (QUARK_KEY, QUARK_CKEYKSYM);
      break;
    case Key::CKEY_KRSA:
      return new Item (QUARK_KEY, QUARK_CKEYKRSA);
      break;
    case Key::CKEY_KMAC:
      return new Item (QUARK_KEY, QUARK_CKEYKMAC);
      break;
    case Key::CKEY_KDSA:
      return new Item (QUARK_KEY, QUARK_CKEYKDSA);
      break;
    }
    return nullptr;
  }

  // map an item to a key accessor
  static inline Key::t_ikey item_to_ikey (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_KEY)
      throw Exception ("item-error", "item is not a key item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_KRSAPMOD) return Key::KRSA_PMOD;
    if (quark == QUARK_KRSAPEXP) return Key::KRSA_PEXP;
    if (quark == QUARK_KRSASEXP) return Key::KRSA_SEXP;
    if (quark == QUARK_KDSAPPRM) return Key::KDSA_PPRM;
    if (quark == QUARK_KDSAQPRM) return Key::KDSA_QPRM;
    if (quark == QUARK_KDSASKEY) return Key::KDSA_SKEY;
    if (quark == QUARK_KDSAPKEY) return Key::KDSA_PKEY;
    if (quark == QUARK_KDSAPGEN) return Key::KDSA_PGEN;
    throw Exception ("item-error", "cannot map item to key index accesoor");
  }

  // evaluate an object data member

  Object* Key::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    // key type part
    if (quark == QUARK_CKEYKNIL)
      return new Item (QUARK_KEY, QUARK_CKEYKNIL);
    if (quark == QUARK_CKEYKSYM)
      return new Item (QUARK_KEY, QUARK_CKEYKSYM);
    if (quark == QUARK_CKEYKRSA)
      return new Item (QUARK_KEY, QUARK_CKEYKRSA);
    if (quark == QUARK_CKEYKMAC)
      return new Item (QUARK_KEY, QUARK_CKEYKMAC);
    if (quark == QUARK_CKEYKDSA)
      return new Item (QUARK_KEY, QUARK_CKEYKDSA);
    // key accessor
    if (quark == QUARK_KRSAPMOD)
      return new Item (QUARK_KEY, QUARK_KRSAPMOD);
    if (quark == QUARK_KRSAPEXP)
      return new Item (QUARK_KEY, QUARK_KRSAPEXP);
    if (quark == QUARK_KRSASEXP)
      return new Item (QUARK_KEY, QUARK_KRSASEXP);
    if (quark == QUARK_KDSAPPRM)
      return new Item (QUARK_KEY, QUARK_KDSAPPRM);
    if (quark == QUARK_KDSAQPRM)
      return new Item (QUARK_KEY, QUARK_KDSAQPRM);
    if (quark == QUARK_KDSASKEY)
      return new Item (QUARK_KEY, QUARK_KDSASKEY);
    if (quark == QUARK_KDSAPKEY)
      return new Item (QUARK_KEY, QUARK_KDSAPKEY);
    if (quark == QUARK_KDSAPGEN)
      return new Item (QUARK_KEY, QUARK_KDSAPGEN);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }
  
  // create a new object in a generic way
  
  Object* Key::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Key;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Key (*sobj);
      // check for an item type
      Item* iobj = dynamic_cast <Item*> (obj);
      if (iobj != nullptr) {
        t_ckey type = item_to_ckey (*iobj);
        return new Key (type);
      }
      throw Exception ("type-error", "invalid object with key constructor",
                       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      // load first object
      Object* obj = argv->get (0);
      // check for an item type
      Item*  iobj = dynamic_cast <Item*> (obj);
      if (iobj == nullptr) {
	throw Exception ("type-error", "invalid object with key constructor",
			 Object::repr (obj));
      }
      t_ckey type = item_to_ckey (*iobj);
      // load second object
      obj = argv->get (1);
      // check fo an integer
      Integer* nobj = dynamic_cast <Integer*> (obj);
      if (nobj != nullptr) {     
	// get the number of bits
	long bits = nobj->tolong ();
	// create the key
	return new Key (type, bits);
      }
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Key (type, *sobj);
      // check for a vector
      Vector* vobj = dynamic_cast <Vector*> (obj);
      if (vobj != nullptr) return new Key (type, *vobj);
      // invalid type
      throw Exception ("type-error", "invalid object with key constructor",
		       Object::repr (obj));
    }
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with key");
  }

  // return true if the given quark is defined

  bool Key::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Key::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOPUB)   return new Key      (topublic ());
      if (quark == QUARK_FORMAT)  return new String   (format   ());
      if (quark == QUARK_GETBITS) return new Integer  (getbits  ());
      if (quark == QUARK_GETSIZE) return new Integer  (getsize  ());
      if (quark == QUARK_GETTYPE) return ckey_to_item (gettype  ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETBYTE) {
	long index = argv->getlong (0);
	return new Byte (getbyte (index));
      }
      if (quark == QUARK_FORMAT) {
	// check for an item type
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with format",
			   Object::repr (obj));
	}
	t_ikey type = item_to_ikey (*iobj);
	// get the relatif
	return new String (format (type));
      }
      if (quark == QUARK_GETRKEY) {
	// check for an item type
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object with get-relatif-key",
			   Object::repr (obj));
	}
	t_ikey type = item_to_ikey (*iobj);
	// get the relatif
	return new Relatif (getrkey (type));
      }      
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
