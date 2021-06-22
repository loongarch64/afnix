// ---------------------------------------------------------------------------
// - Crypto.hpp                                                              -
// - afnix:sec module - cryptographic function definitions                   -
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

#ifndef  AFNIX_CRYPTO_HPP
#define  AFNIX_CRYPTO_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

#ifndef  AFNIX_SIGNER_HPP
#include "Signer.hpp"
#endif

#ifndef  AFNIX_BLOCKCIPHER_HPP
#include "BlockCipher.hpp"
#endif

namespace afnix {

  /// the Crypto class is a collection of static convenient functions that
  /// are part of the standard cryptographic interface for the afnix engine.
  /// the available functions permits to create vrious hasher or stream cipher
  /// along with their keys. As a class, the Crypto bject provides information
  /// about about a particular cryptographic object such like hash function,
  /// mac and cipher.
  /// @author amaury darsch

  class Crypto : public Object {
  public:
    /// the supported key
    enum t_xkey : t_byte {
      XKEY_NIL = 0x00U, // nil key
      XKEY_RSA = 0x01U, // rsa key
      XKEY_DSA = 0x02U, // dsa key
      XKEY_ASM = 0x03U, // asymetric key
      XKEY_SGN = 0x04U, // signing key
    };
    
    /// create a new key by type
    /// @param type the key type to create
    static Key* mkkey (const t_xkey type);
    
    /// create a new key by name
    /// @param name the key name to use
    static Key* mkkey (const String& name);
    
    /// the supported hasher
    enum t_hash : t_byte
      {
       HASH_NIL       = 0x00U, // nil hasher
       HASH_MD2       = 0x01U, // message digest - 2
       HASH_MD4       = 0x02U, // message digest - 4
       HASH_MD5       = 0x03U, // message digest - 5
       HASH_SHA1      = 0x04U, // secure hash algorithm - 1
       HASH_SHA224    = 0x05U, // secure hash algorithm - 2 - 224
       HASH_SHA256    = 0x06U, // secure hash algorithm - 2 - 256
       HASH_SHA384    = 0x07U, // secure hash algorithm - 2 - 384
       HASH_SHA512    = 0x08U, // secure hash algorithm - 2 - 512
       HASH_SHA512224 = 0x09U, // secure hash algorithm - 2 - 512/224
       HASH_SHA512256 = 0x0AU, // secure hash algorithm - 2 - 512/256
       HASH_SHA3224   = 0x0BU, // secure hash algorithm - 3 - 224
       HASH_SHA3256   = 0x0CU, // secure hash algorithm - 3 - 256
       HASH_SHA3384   = 0x0DU, // secure hash algorithm - 3 - 384
       HASH_SHA3512   = 0x0EU  // secure hash algorithm - 3 - 512
      };
    
    /// create a default hasher type
    static Hasher* mkhasher (void);

    /// create a new hasher by type
    /// @param type the hasher type to create
    static Hasher* mkhasher (const t_hash type);
    
    /// create a new hasher by name
    /// @param name the hasher name to use
    static Hasher* mkhasher (const String& name);

    /// create a new hasher by bits size
    /// @param bits the requested hasher bits size
    static Hasher* mkhasher (const long bits);

    /// @return true for a valid hasher by name
    static bool ishasher (const String& name);
    
    /// the supported cipher
    enum t_cifr : t_byte {
      CIFR_NIL = 0x00U, // nil cipher
      CIFR_AES = 0x01U, // advanced encryption standard
      CIFR_RSA = 0x02U, // PKCS 2.1 encryption standard
      CIFR_DES = 0x03U, // data encryption standard
      CIFR_RC2 = 0x04U, // RC2 cipher
      CIFR_RC4 = 0x05U, // RC4 cipher
      CIFR_RC5 = 0x06U  // RC5 cipher
    };
    
    /// create a new block cipher by type and key
    /// @param type the cipher type to create
    /// @param ckey the cipher key to use
    static Cipher* mkcipher (const t_cifr type, const Key& ckey);

    /// create a new block cipher by name
    /// @param name the cipher name to use
    /// @param ckey the cipher key to use
    static Cipher* mkcipher (const String& name, const Key& ckey);

    /// create a new block cipher by type and key
    /// @param type the cipher type to create
    /// @param ckey the cipher key to use
    /// @param rflg the reverse flag
    static Cipher* mkcipher (const t_cifr type, const Key& ckey,
			     const bool rflg);
    
    /// create a new block cipher by name
    /// @param name the cipher name to use
    /// @param ckey the cipher key to use
    /// @param rflg the reverse flag
    static Cipher* mkcipher (const String& name, const Key& ckey,
			     const bool rflg);

    /// @return true for a valid cipher by name
    static bool iscipher (const String& name);

    /// the supported signer
    enum t_sign : t_byte {
      SIGN_NIL = 0x00U, // nil signature
      SIGN_DSA = 0x01U, // digital signature standard
      SIGN_RSA = 0x02U  // PKCS signature standard
    };

    // create a new signer by type
    /// @param type the signer type
    static Signer* mksigner (const t_sign type);
    
    // create a new signer by key
    /// @param skey the signer key
    static Signer* mksigner (const Key& skey);
    
    // create a new signer by signature
    /// @param sign the signer signature
    static Signer* mksigner (const Signature& sign);
    
    // create a new signer by name
    /// @param name the signer key
    static Signer* mksigner (const String& name);

    /// @return true for a valid signer by name
    static bool issigner (const String& name);

  private:
    /// the default hasher
    Hasher* p_hash;
    
  public:
    /// create an empty crypto info
    Crypto (void);
    
    /// copy this crypto object
    /// @param that the object to copy
    Crypto (const Crypto& that);

    /// copy move this crypto object
    /// @param the the object to copy-move
    Crypto (Crypto&& that);

    /// destroy this crypto object
    ~Crypto (void);
    
    /// assign a crypto object to this one
    /// @param that the object to assign
    Crypto& operator = (const Crypto& that);

    /// move a crypto object to this one
    /// @param that the object to move
    Crypto& operator = (Crypto&& that);

    /// @return the class name
    String repr (void) const;

    /// hash a message into a string
    /// @param msg the message to hash
    String hash (const String& msg);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}
  
#endif
