// ---------------------------------------------------------------------------
// - Hkdf.hpp                                                                -
// - afnix:sec module - hashed key derivation function class definition      -
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

#ifndef  AFNIX_HKDF_HPP
#define  AFNIX_HKDF_HPP

#ifndef  AFNIX_KDF_HPP
#include "Kdf.hpp"
#endif

#ifndef  AFNIX_HASHABLE_HPP
#include "Hashable.hpp"
#endif

namespace afnix {

  /// The Hkdf class is an abstract class used to model key derivation
  /// function  based on hash function. The class maintains a hashable object
  /// that is used to derive the key from an octer string.
  /// @author amaury darsch

  class Hkdf : public Kdf {
  protected:
    /// the hashable object
    Hashable* p_hobj;

  public:
    /// create a hashed kdf object by hashable and size
    /// @param hobj the hashable object
    /// @param name the kdf name
    /// @param kbsz the key buffer size
    Hkdf (Hashable* hobj, const String& name, const long kbsz);

    /// destroy this hashed kdf object
    ~Hkdf (void);

    /// @return the class name
    String repr (void) const;

    /// reset this hkdf
    void reset (void);

    /// @return the hashable object
    virtual Hashable* gethobj (void) const;
 
  private:
    // make the copy constructor private
    Hkdf (const Hkdf&);
    // make the assignment operator private
    Hkdf& operator = (const Hkdf&);

  public:
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
