// ---------------------------------------------------------------------------
// - Cipher.hpp                                                              -
// - afnix:sec module - base cipher class definition                         -
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

#ifndef  AFNIX_CIPHER_HPP
#define  AFNIX_CIPHER_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_CODEC_HPP
#include "Codec.hpp"
#endif

namespace afnix {

  /// The Cipher class is a codec class that is used to implement a cipher.
  /// A cipher is used to encrypt or decrypt a message. There are basically
  /// two types of ciphers, namely symmetric cipher and asymmetric ciphers.
  /// For the base class operations, only the cipher name and key are needed.
  /// A reverse flag controls whether or not an encryption operation must
  /// be reversed. A reset method can also be used to reset the internal
  /// cipher state.
  /// @author amaury darsch

  class Cipher : public Codec {
  protected:
    /// the cipher key
    Key d_ckey;

  public:
    /// create a default cipher
    Cipher (void) =default;

    /// create a cipher by name
    /// @param name the cipher name
    Cipher (const String& name);

    /// create a cipher by name and key
    /// @param name the cipher name
    /// @param key  the cipher key
    Cipher (const String& name, const Key& key);

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    virtual void setkey (const Key& key);

    /// @return the cipher key
    virtual Key getkey (void) const;

    /// normalize a data size
    /// @param size the size to normalize
    virtual t_long waist (const t_long size) const;
    
  private:
    // make the copy constructor private
    Cipher (const Cipher&);
    // make the assignment operator private
    Cipher& operator = (const Cipher&);

  public:
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
