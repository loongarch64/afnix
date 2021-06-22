// ---------------------------------------------------------------------------
// - BlockCipher.hpp                                                         -
// - afnix:sec module - block cipher class definition                        -
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

#ifndef  AFNIX_BLOCKCIPHER_HPP
#define  AFNIX_BLOCKCIPHER_HPP

#ifndef  AFNIX_CIPHER_HPP
#include "Cipher.hpp"
#endif

namespace afnix {

  /// The BlockCipher class is an abstract class that is used to implement 
  /// a symmetric block cipher. It is a base class for the mode and tag
  /// cipher.
  /// @author amaury darsch

  class BlockCipher : public Cipher {
  protected:
    /// the cipher block size
    long d_cbsz;

  public:
    /// create a cipher by name and size
    /// @param name the cipher name
    /// @param cbsz the cipher block size
    BlockCipher (const String& name, const long cbsz);

    /// reset this block cipher
    void reset (void) override;

    /// clear this block cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
    /// @return the cipher block size
    virtual long getcbsz (void) const;

    /// set the initial vector by octet string
    /// @param ivs the initial vector string
    virtual void setiv (const String& ivs) =0;

    /// set the initial vector by buffer
    /// @param kiv the initial vector buffer
    virtual void setiv (const Buffer& kiv) =0;

    /// @return the initial vector as an octet string
    virtual String getiv (void) const =0;

  private:
    // make the copy constructor private
    BlockCipher (const BlockCipher&) =delete;
    // make the assignment operator private
    BlockCipher& operator = (const BlockCipher&) =delete;

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
