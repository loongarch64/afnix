// ---------------------------------------------------------------------------
// - Rc2.hpp                                                                 -
// - afnix:sec module - RC2 block cipher class definition                    -
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

#ifndef  AFNIX_RC2_HPP
#define  AFNIX_RC2_HPP

#ifndef  AFNIX_MODECIPHER_HPP
#include "ModeCipher.hpp"
#endif

namespace afnix {

  /// The Rc2 class is a block cipher class that implements the RC2 
  /// encryption algorithm. This is an original implementation that
  /// conforms to RFC 2268. Among many things, RC2 operates
  /// with 64 bits block and 1024 bits key and run in 18 rounds.
  /// @author amaury darsch

  class Rc2 : public ModeCipher {
  private:
    /// the effective key length
    long d_klen;
    /// the round keys
    t_word* p_rkey;

  public:
    /// create a new cipher by key
    /// @param key the cipher key
    Rc2 (const Key& key);

    /// create a new cipher by key and flag
    /// @param key the cipher key
    /// @param rflg the reverse flag
    Rc2 (const Key& key, const bool rflg);

    /// destroy this cipher
    ~Rc2 (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
    /// set the effective key length
    /// @param klen the effective key length
    virtual void setklen (const long klen);

    /// @return the effective key length
    virtual long getklen (void) const;

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
    Rc2 (const Rc2&) =delete;
    // make the assignment operator private
    Rc2& operator = (const Rc2&) =delete;

  public:
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
