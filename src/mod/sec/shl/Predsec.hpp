// ---------------------------------------------------------------------------
// - Predsec.hpp                                                             -
// - afnix:sec module - predicates declaration                               -
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

#ifndef  AFNIX_PREDSEC_HPP
#define  AFNIX_PREDSEC_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {
  
  /// this file contains the predicates associated with the afnix:sec
  /// standard module.
  /// @author amaury darsch

  /// the galois field object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_gfp     (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the ecc point object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_ecpp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the ecc object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_eccp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the secp object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_secpp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the sect object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sectp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_hashp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the key object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_keyp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the signature object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sgnp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the dsa object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_dsap    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the MD2 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_md2p    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the MD4 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_md4p    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the MD5 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_md5p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the SHA-1 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the SHA-224 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha224p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the SHA-256 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha256p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the SHA-384 hasher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha384p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the SHA-512 hasher  object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha512p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the SHA-3-224 hasher  object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha3224p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the SHA-3-256 hasher  object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha3256p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the SHA-3-384 hasher  object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha3384p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the SHA-3-512 hasher  object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_sha3512p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the kdf object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_kdfp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the hashed kdf object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_hkdfp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the kdf1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_kdf1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the kdf2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_kdf2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pbkdf2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_pkdfp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the mac object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_macp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the hmac object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_hmacp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the des cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_desp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the aes cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_aesp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the gcm mode object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_gcmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rc2 cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_rc2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rc4 cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_rc4p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rc5 cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_rc5p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rsa cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_rsap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_cifrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the block cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_bcfrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the mode cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_mcfrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the aead cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_acfrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the public cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_pcfrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the input cipher object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_icfrp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the generic crypto object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_cptop (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the keyring object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sec_krngp (Evaluable* zobj, Nameset* nset, Cons* args);


}

#endif
