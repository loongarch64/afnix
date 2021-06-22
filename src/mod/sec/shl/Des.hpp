// ---------------------------------------------------------------------------
// - Des.hpp                                                                 -
// - afnix:sec module - DES (data encryption standard) class definition      -
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

#ifndef  AFNIX_DES_HPP
#define  AFNIX_DES_HPP

#ifndef  AFNIX_MODECIPHER_HPP
#include "ModeCipher.hpp"
#endif

namespace afnix {

  /// The DES class is a block cipher class that implements the 'data 
  /// encryption standard' (DES). This is an original implementation that
  /// conforms to the standard FIPS PUB 46-3. Among many things, DES operates
  /// with 64 bits block and 64 bits key and run in 16 rounds.
  /// @author amaury darsch

  class Des : public ModeCipher {
  private:
    /// the round keys
    t_octa* p_rkey;

    /// encode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    void encode (t_byte* bo, const t_byte* bi) override;

    /// decode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    void decode (t_byte* bo, const t_byte* bi) override;

  public:
    /// create a new des cipher by key
    /// @param key the cipher key
    Des (const Key& key);

    /// create a new des cipher by key and flag
    /// @param key the cipher key
    /// @param rflg the reverse flag
    Des (const Key& key, const bool rflg);

    /// destroy this cipher
    ~Des (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;

  private:
    // make the copy constructor private
    Des (const Des&) =delete;
    // make the assignment operator private
    Des& operator = (const Des&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
