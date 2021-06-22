// ---------------------------------------------------------------------------
// - Rc5.hpp                                                                 -
// - afnix:sec module - RC5 block cipher class definition                    -
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

#ifndef  AFNIX_RC5_HPP
#define  AFNIX_RC5_HPP

#ifndef  AFNIX_MODECIPHER_HPP
#include "ModeCipher.hpp"
#endif

namespace afnix {

  /// The Rc5 class is a block cipher class that implements the RC5 
  /// encryption algorithm. This is an original implementation that
  /// is designed to operate with a 32 bits word. Although, RC5 is
  /// designed to support also 16 and 64 bits implementation, this
  /// class provides only the flexibility for the number of rounds
  /// and the key size as described in the original RC5 specification.
  /// The default implementation conforms to RC5-32/12/16.
  /// @author amaury darsch

  class Rc5 : public ModeCipher {
  private:
    /// the number of rounds
    long d_nrnd;
    /// the secret key size
    long d_sksz;
    /// the expanded key size
    long d_eksz;
    /// the expanded key
    t_quad* p_ekey;

  public:
    /// create a new cipher by key
    /// @param key the cipher key
    Rc5 (const Key& key);

    /// create a new cipher by key and flag
    /// @param key the cipher key
    /// @param rflg the reverse flag
    Rc5 (const Key& key, const bool rflg);

    /// destroy this cipher
    ~Rc5 (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
    /// set the number of rounds
    /// @param nrnd the number of rounds
    virtual void setnrnd (const long nrnd);

    /// @return the number of rounds
    virtual long getnrnd (void) const;

    /// set the secret key size
    /// @param sksz the secret key size to set
    virtual void setsksz (const long sksz);

    /// @return the secret key size
    virtual long getsksz (void) const;

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
    Rc5 (const Rc5&) =delete;
    // make the assignment operator private
    Rc5& operator = (const Rc5&) =delete;

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
