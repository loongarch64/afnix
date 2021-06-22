// ---------------------------------------------------------------------------
// - Rsa.hpp                                                                 -
// - afnix:sec module - rsa public cipher class definition                   -
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

#ifndef  AFNIX_RSA_HPP
#define  AFNIX_RSA_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

#ifndef  AFNIX_PUBLICCIPHER_HPP
#include "PublicCipher.hpp"
#endif

namespace afnix {

  /// The RSA class is a public cipher class that implements the RSA algorithm
  /// as described by PKCS 2.1, RFC 2437 and ISO 18033-2. The class implements
  /// also some padding mechanism described in PKCS 1.5, 2.1 and ISO 18033-2.
  /// The RSA algorithm is a public cryptographic cipher based on a secret
  /// and public keys. The class operates in crypting mode by default and uses
  /// the public key to do the encryption while the secret key is used in 
  /// reverse (decryption) mode. By default, the PKCS 1.5 type 2 padding is
  /// used. The ISO RSA-REM1 padding with a key derivation function (KDF1) is
  /// equivalent to PKCS 2.1 padding with the mask generation function (MGF1).
  /// The ISO RSA-REM1 padding with KDF2 is not described in the PKCS 2.1.
  /// @author amaury darsch

  class Rsa : public PublicCipher {
  public:
    /// the padding mode
    enum t_pmod
      {
       PAD_PKCS11, // PKCS 1.5 padding type 1
       PAD_PKCS12, // PKCS 1.5 padding type 2
       PAD_OAEPK1, // ISO RSA-REM1 (KDF1) padding (aka PKCS 2.1 RSA-OAEP)
       PAD_OAEPK2  // ISO RSA-REM1 (KDF2) padding
      };
    
  private:
    /// the rsa modulus
    Relatif d_kmod;
    /// the rsa exponent
    Relatif d_kexp;
    /// the rsa padding
    t_pmod  d_pmod;
    /// the rsa oaep label
    String  d_labl;
    /// the rsa oaep seed
    String  d_seds;
    /// the rsa oaep hasher
    Hasher* p_hobj;

  public:
    /// create a default rsa cipher
    Rsa (void);

    /// create a rsa cipher by key
    /// @param key the cipher key
    Rsa (const Key& key);

    /// create a rsa cipher by key, hasher and label
    /// @param key  the cipher key
    /// @param hobj the hasher object
    /// @param labl the oaep padding label
    Rsa (const Key& key, Hasher* hobj, const String& labl);

    /// create a rsa cipher by key and flag
    /// @param key the cipher key
    /// @param rflg the reverse flag
    Rsa (const Key& key, const bool rflg);

    /// destroy this rsa cipher
    ~Rsa (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the cipher key to set
    void setkey (const Key& key) override;

    /// set the cipher reverse flag
    /// @param rflg the flag to set
    void setrflg (const bool rflg) override;

    /// set the cipher padding mode
    /// @param pmod the padding mode to set
    virtual void setpmod (const t_pmod pmod);

    /// @return the cipher padding mode
    virtual t_pmod getpmod (void) const;

    /// set the oaep padding label
    /// @param labl the label to set
    virtual void setlabl (const String& labl);

    /// @return the oaep padding label
    virtual String getlabl (void) const;

    /// set the oaep padding seed
    /// @param seds the seed octet string
    virtual void setseds (const String& labl);

    /// @return the oaep padding seed
    virtual String getseds (void) const;

    /// set the oaep hasher object
    /// @param hobj the hasher to set
    virtual void sethobj (Hasher* hobj);

    /// @return the oaep hasher object
    virtual Hasher* gethobj (void) const;

    /// pkcs encryption/decryption primitive
    /// @param m the message representative
    virtual Relatif pkcsed (const Relatif& m) const;

  protected:
    /// encode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    /// @param cc the input count
    long encode (t_byte* bo, const t_byte* bi, const long cc) override;
    
    /// decode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    /// @param cc the input count
    long decode (t_byte* bo, const t_byte* bi, const long cc) override;

  private:
    // make the copy constructor private
    Rsa (const Rsa&) =delete;
    // make the assignment operator private
    Rsa& operator = (const Rsa&) =delete;

  public:
    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
