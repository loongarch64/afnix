// ---------------------------------------------------------------------------
// - Aes.hpp                                                                 -
// - afnix:sec module - eas (rijndael) class definition                      -
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

#ifndef  AFNIX_AES_HPP
#define  AFNIX_AES_HPP

#ifndef  AFNIX_MODECIPHER_HPP
#include "ModeCipher.hpp"
#endif

namespace afnix {

  /// The AES class is a block cipher class that implements the 'advanced 
  /// encryption standard' (AES) -- originally known as rijndael. This is an
  /// original implementation that conforms to the standard FIPS PUB 197.
  /// It should be noted that the AES standard, unlike rijndael, defines a 
  /// fixed block size of 16 bytes (4 words) and 3 keys sizes (128, 192, 256).
  /// @author amaury darsch

  class Aes : public ModeCipher {
  private:
    /// the number of rounds
    long    d_rnds;
    /// the round key size
    long    d_rksz;
    /// the round key
    t_byte* p_rkey;

  public:
    /// create a new aes cipher by key
    /// @param key the cipher key
    Aes (const Key& key);

    /// create a new aes cipher by key and flag
    /// @param key the cipher key
    /// @param rflg the reverse flag
    Aes (const Key& key, const bool rflg);

    /// destroy this cipher
    ~Aes (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
  protected:
    /// encode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    void encode (t_byte* bo, const t_byte* bi) override;

    /// decode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    void decode (t_byte* bo, const t_byte* bi) override;
    
  private:
    // make the copy constructor private
    Aes (const Aes&) =delete;
    // make the assignment operator private
    Aes& operator = (const Aes&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
