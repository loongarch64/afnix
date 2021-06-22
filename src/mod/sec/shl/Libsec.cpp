// ---------------------------------------------------------------------------
// - Libsec.cpp                                                              -
// - afnix:sec module - declaration & implementation                         -
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
#include "Meta.hpp"
#include "Secp.hpp"
#include "Sect.hpp"
#include "Secsrl.hxx"
#include "Libsec.hpp"
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
#include "Function.hpp"
#include "Signature.hpp"
#include "InputCipher.hpp"

namespace afnix {

  // initialize the afnix:sec module

  Object* init_afnix_sec (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid eng serial dispatcher");
    }
    // create the afnix:sys nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* nset = aset->mknset   ("sec");
    
    // bind all classes in the afnix:sec nameset 
    nset->symcst ("Key",             new Meta (Key::meval, Key::mknew));
    nset->symcst ("Dsa",             new Meta (Dsa::mknew));
    nset->symcst ("Des",             new Meta (Des::mknew));
    nset->symcst ("Aes",             new Meta (Aes::mknew));
    nset->symcst ("Gcm",             new Meta (Gcm::mknew));
    nset->symcst ("Ecp",             new Meta (Ecp::mknew));
    nset->symcst ("Rc2",             new Meta (Rc2::mknew));
    nset->symcst ("Rc4",             new Meta (Rc4::mknew));
    nset->symcst ("Rc5",             new Meta (Rc5::mknew));
    nset->symcst ("Rsa",             new Meta (Rsa::meval, Rsa::mknew));
    nset->symcst ("Md2",             new Meta (Md2::mknew));
    nset->symcst ("Md4",             new Meta (Md4::mknew));
    nset->symcst ("Md5",             new Meta (Md5::mknew));
    nset->symcst ("Sha1",            new Meta (Sha1::mknew));
    nset->symcst ("Kdf1",            new Meta (Kdf1::mknew));
    nset->symcst ("Kdf2",            new Meta (Kdf2::mknew));
    nset->symcst ("Hmac",            new Meta (Hmac::mknew));
    nset->symcst ("Secp",            new Meta (Secp::mknew));
    nset->symcst ("Sect",            new Meta (Sect::mknew));
    nset->symcst ("Galois",          new Meta (Galois::mknew));
    nset->symcst ("Crypto",          new Meta (Crypto::mknew));
    nset->symcst ("Pbkdf2",          new Meta (Pbkdf2::mknew));
    nset->symcst ("Sha224",          new Meta (Sha224::mknew));
    nset->symcst ("Sha256",          new Meta (Sha256::mknew));
    nset->symcst ("Sha384",          new Meta (Sha384::mknew));
    nset->symcst ("Sha512",          new Meta (Sha512::mknew));
    nset->symcst ("Sha3224",         new Meta (Sha3224::mknew));
    nset->symcst ("Sha3256",         new Meta (Sha3256::mknew));
    nset->symcst ("Sha3384",         new Meta (Sha3384::mknew));
    nset->symcst ("Sha3512",         new Meta (Sha3512::mknew));
    nset->symcst ("Keyring",         new Meta (Keyring::mknew)); 
    nset->symcst ("Signature",       new Meta (Signature::meval, 
					       Signature::mknew));
    nset->symcst ("ModeCipher",      new Meta (ModeCipher::meval));
    nset->symcst ("InputCipher",     new Meta (InputCipher::mknew));

    // bind all predicates in the afnix:sec nameset
    nset->symcst ("key-p",           new Function (sec_keyp));
    nset->symcst ("dsa-p",           new Function (sec_dsap));
    nset->symcst ("kdf-p",           new Function (sec_kdfp));
    nset->symcst ("des-p",           new Function (sec_desp));
    nset->symcst ("aes-p",           new Function (sec_aesp));
    nset->symcst ("gcm-p",           new Function (sec_gcmp));
    nset->symcst ("ecp-p",           new Function (sec_ecpp));
    nset->symcst ("ecc-p",           new Function (sec_eccp));
    nset->symcst ("rc2-p",           new Function (sec_rc2p));
    nset->symcst ("rc4-p",           new Function (sec_rc4p));
    nset->symcst ("rc5-p",           new Function (sec_rc5p));
    nset->symcst ("rsa-p",           new Function (sec_rsap));
    nset->symcst ("md2-p",           new Function (sec_md2p));
    nset->symcst ("md4-p",           new Function (sec_md4p));
    nset->symcst ("md5-p",           new Function (sec_md5p));
    nset->symcst ("sha1-p",          new Function (sec_sha1p));
    nset->symcst ("kdf1-p",          new Function (sec_kdf1p));
    nset->symcst ("kdf2-p",          new Function (sec_kdf2p));
    nset->symcst ("mac-p",           new Function (sec_macp));
    nset->symcst ("hmac-p",          new Function (sec_hmacp));
    nset->symcst ("secp-p",          new Function (sec_secpp));
    nset->symcst ("sect-p",          new Function (sec_sectp));
    nset->symcst ("galois-p",        new Function (sec_gfp));
    nset->symcst ("crypto-p",        new Function (sec_cptop));
    nset->symcst ("pbkdf2-p",        new Function (sec_pkdfp));
    nset->symcst ("sha224-p",        new Function (sec_sha224p));
    nset->symcst ("sha256-p",        new Function (sec_sha256p));
    nset->symcst ("sha384-p",        new Function (sec_sha384p));
    nset->symcst ("sha512-p",        new Function (sec_sha512p));
    nset->symcst ("sha3224-p",       new Function (sec_sha3224p));
    nset->symcst ("sha3256-p",       new Function (sec_sha3256p));
    nset->symcst ("sha3384-p",       new Function (sec_sha3384p));
    nset->symcst ("sha3512-p",       new Function (sec_sha3512p));
    nset->symcst ("hasher-p",        new Function (sec_hashp));
    nset->symcst ("cipher-p",        new Function (sec_cifrp));
    nset->symcst ("keyring-p",       new Function (sec_krngp));
    nset->symcst ("signature-p",     new Function (sec_sgnp));
    nset->symcst ("hashed-kdf-p",    new Function (sec_hkdfp));
    nset->symcst ("mode-cipher-p",   new Function (sec_mcfrp));
    nset->symcst ("aead-cipher-p",   new Function (sec_acfrp));
    nset->symcst ("input-cipher-p",  new Function (sec_icfrp));
    nset->symcst ("block-cipher-p",  new Function (sec_bcfrp));
    nset->symcst ("public-cipher-p", new Function (sec_pcfrp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_sec (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_sec (interp, argv);
  }
}
