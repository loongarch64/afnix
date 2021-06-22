// ---------------------------------------------------------------------------
// - Predsec.cpp                                                             -
// - afnix:sec module - predicates implementation                            - 
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

#include "Dsa.hpp"
#include "Des.hpp"
#include "Aes.hpp"
#include "Rc2.hpp"
#include "Rc4.hpp"
#include "Rc5.hpp"
#include "Rsa.hpp"
#include "Md2.hpp"
#include "Md4.hpp"
#include "Md5.hpp"
#include "Gcm.hpp"
#include "Sha1.hpp"
#include "Kdf1.hpp"
#include "Kdf2.hpp"
#include "Hmac.hpp"
#include "Cons.hpp"
#include "Secp.hpp"
#include "Sect.hpp"
#include "Galois.hpp"
#include "Crypto.hpp"
#include "Pbkdf2.hpp"
#include "Sha224.hpp"
#include "Sha256.hpp"
#include "Sha384.hpp"
#include "Sha512.hpp"
#include "Sha3224.hpp"
#include "Sha3256.hpp"
#include "Sha3384.hpp"
#include "Sha3512.hpp"
#include "Keyring.hpp"
#include "Predsec.hpp"
#include "Boolean.hpp"
#include "Signature.hpp"
#include "Exception.hpp"
#include "InputCipher.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
                                 const String& pname) {
    Object* car = nullptr;
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
                       pname);
    car = args->getcar ();
    return (car == nullptr) ? nullptr : car->eval (zobj,nset);
  }

  // gfp: galois field object predicate

  Object* sec_gfp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "galois-p");
    bool result =  (dynamic_cast <Galois*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ecpp: ecc point object predicate

  Object* sec_ecpp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ecp-p");
    bool result =  (dynamic_cast <Ecp*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // eccp: ecc object predicate

  Object* sec_eccp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ecc-p");
    bool result =  (dynamic_cast <Ecc*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // secpp: secp object predicate

  Object* sec_secpp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "secp-p");
    bool result =  (dynamic_cast <Secp*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sectp: sect object predicate

  Object* sec_sectp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sect-p");
    bool result =  (dynamic_cast <Sect*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hashp: hasher object predicate

  Object* sec_hashp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "hasher-p");
    bool result =  (dynamic_cast <Hasher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // keyp: key object predicate

  Object* sec_keyp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "key-p");
    bool result =  (dynamic_cast <Key*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sgnp: signature object predicate

  Object* sec_sgnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "signature-p");
    bool result =  (dynamic_cast <Signature*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // dsap: dsa object predicate

  Object* sec_dsap (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "dsa-p");
    bool result =  (dynamic_cast <Dsa*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // md2p: MD-2 object predicate

  Object* sec_md2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "md2-p");
    bool result =  (dynamic_cast <Md2*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // md4p: MD-4 object predicate

  Object* sec_md4p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "md4-p");
    bool result =  (dynamic_cast <Md4*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // md5p: MD-5 object predicate

  Object* sec_md5p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "md5-p");
    bool result =  (dynamic_cast <Md5*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha1p: SHA-1 object predicate

  Object* sec_sha1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha1-p");
    bool result =  (dynamic_cast <Sha1*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha224p: SHA-224 object predicate

  Object* sec_sha224p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha224-p");
    bool result =  (dynamic_cast <Sha224*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha256p: SHA-256 object predicate

  Object* sec_sha256p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha256-p");
    bool result =  (dynamic_cast <Sha256*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha384p: SHA-384 object predicate

  Object* sec_sha384p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha384-p");
    bool result =  (dynamic_cast <Sha384*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha512p: SHA-512 object predicate

  Object* sec_sha512p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha512-p");
    bool result =  (dynamic_cast <Sha512*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha3224p: SHA-3-224 object predicate

  Object* sec_sha3224p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha3224-p");
    bool result =  (dynamic_cast <Sha3224*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha3256p: SHA-3-256 object predicate
  
  Object* sec_sha3256p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha3256-p");
    bool result =  (dynamic_cast <Sha3256*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha3384: SHA-3-384 object predicate

  Object* sec_sha3384p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha3384-p");
    bool result =  (dynamic_cast <Sha3384*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sha3512p: SHA-3-512 object predicate

  Object* sec_sha3512p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sha3512-p");
    bool result =  (dynamic_cast <Sha3512*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // kdfp: kdf object predicate

  Object* sec_kdfp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "kdf-p");
    bool result =  (dynamic_cast <Kdf*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hkdfp: hashed kdf object predicate

  Object* sec_hkdfp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "hashed-kdf-p");
    bool result =  (dynamic_cast <Hkdf*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // kdf1p: kdf1 object predicate

  Object* sec_kdf1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "kdf1-p");
    bool result =  (dynamic_cast <Kdf1*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // kdf2p: kdf2 object predicate

  Object* sec_kdf2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "kdf2-p");
    bool result =  (dynamic_cast <Kdf2*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pkdfp: pbkdf2 object predicate

  Object* sec_pkdfp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pbkdf2-p");
    bool result =  (dynamic_cast <Pbkdf2*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // macp: mac object predicate

  Object* sec_macp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "mac-p");
    bool result =  (dynamic_cast <Mac*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hmacp: mac object predicate

  Object* sec_hmacp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "hmac-p");
    bool result =  (dynamic_cast <Hmac*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // desp: des object predicate

  Object* sec_desp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "des-p");
    bool result =  (dynamic_cast <Des*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // aesp: aes object predicate

  Object* sec_aesp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "aes-p");
    bool result =  (dynamic_cast <Aes*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // gcmp: gcm object predicate

  Object* sec_gcmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "gcm-p");
    bool result =  (dynamic_cast <Gcm*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rc2p: rc2 object predicate

  Object* sec_rc2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rc2-p");
    bool result =  (dynamic_cast <Rc2*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rc4p: rc4 object predicate

  Object* sec_rc4p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rc4-p");
    bool result =  (dynamic_cast <Rc4*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rc5p: rc5 object predicate

  Object* sec_rc5p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rc5-p");
    bool result =  (dynamic_cast <Rc5*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rsap: rsa object predicate

  Object* sec_rsap (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rsa-p");
    bool result =  (dynamic_cast <Rsa*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cifrp: cipher object predicate

  Object* sec_cifrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cipher-p");
    bool result =  (dynamic_cast <Cipher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bcfrp: block cipher object predicate

  Object* sec_bcfrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "block-cipher-p");
    bool result =
      (dynamic_cast <BlockCipher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // mcfrp: mode cipher object predicate

  Object* sec_mcfrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "mode-cipher-p");
    bool result =
      (dynamic_cast <ModeCipher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // acfrp: aead cipher object predicate

  Object* sec_acfrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "aead-cipher-p");
    bool result =
      (dynamic_cast <AeadCipher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pccfrp: public cipher object predicate

  Object* sec_pcfrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "public-cipher-p");
    bool result =
      (dynamic_cast <PublicCipher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // icfrp: input cipher object predicate

  Object* sec_icfrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-cipher-p");
    bool result =
      (dynamic_cast <InputCipher*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cptop: generic crypto object predicate

  Object* sec_cptop (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "crypto-p");
    bool result = (dynamic_cast <Crypto*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // krng: keyring  object predicate

  Object* sec_krngp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "keyring-p");
    bool result = (dynamic_cast <Keyring*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  } 
}
