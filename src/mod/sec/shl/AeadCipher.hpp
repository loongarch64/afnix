// ---------------------------------------------------------------------------
// - AeadCipher.hpp                                                          -
// - afnix:sec module - aead cipher class definition                         -
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

#ifndef  AFNIX_AEADCIPHER_HPP
#define  AFNIX_AEADCIPHER_HPP

#ifndef  AFNIX_BLOCKCIPHER_HPP
#include "BlockCipher.hpp"
#endif

namespace afnix {

  /// The AeadCipher class is an abstract class that is used to implement 
  /// a symmetric block cipher augmented with an authentication tag
  /// mechanism. Similar to the block cipher, the aead cipher does not define
  /// an operating mode, not a padding mechanism. As such, it is a much
  /// more generic cipher with an authentication tag generation.
  /// @author amaury darsch

  class AeadCipher : public BlockCipher {
  protected:
    /// the initial vector
    Buffer d_iv;
    /// the authentication data
    Buffer d_auth;
    /// the authentication tag
    Buffer d_atag;
    
  public:
    /// create a cipher by name and size
    /// @param name the cipher name
    /// @param cbsz the cipher block size
    AeadCipher (const String& name, const long cbsz);

    /// reset this aead cipher
    void reset (void) override;

    /// clear this aead cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
    /// set the initial vector by octet string
    /// @param ivs the initial vector string
    void setiv (const String& ivs) override;

    /// set the initial vector by buffer
    /// @param kiv the initial vector buffer
    void setiv (const Buffer& kiv) override;

    /// @return the initial vector as an octet string
    String getiv (void) const override;

    /// add an explicit iv by string
    /// @param evs the explicit iv string
    virtual void addiv (const String& evs) =0;

    /// add an explicit iv by buffer
    /// @param eiv the explicit iv buffer
    virtual void addiv (const Buffer& eiv) =0;
    
    /// set the authentication data by octet string
    /// @param auth the authentication data string
    virtual void setauth (const String& auth);

    /// set the authentication data by buffer
    /// @param kiv the authentication data buffer
    virtual void setauth (const Buffer& auth);

    /// @return the authentication data as an octet string
    virtual String getauth (void) const;

    /// @return the authentication tag size
    virtual long getatsz (void) const;
    
    /// @return the authentication tag
    virtual Buffer getatag (void) const;
        
  private:
    // make the copy constructor private
    AeadCipher (const AeadCipher&) =delete;
    // make the assignment operator private
    AeadCipher& operator = (const AeadCipher&) =delete;

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
