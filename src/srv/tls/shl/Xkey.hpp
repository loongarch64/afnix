// ---------------------------------------------------------------------------
// - Xkey.hpp                                                                -
// - afnix:tls service - X509 public key class definition                    -
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

#ifndef  AFNIX_XKEY_HPP
#define  AFNIX_XKEY_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_XALGO_HPP
#include "Xalgo.hpp"
#endif

#ifndef  AFNIX_BITSET_HPP
#include "Bitset.hpp"
#endif

namespace afnix {

  /// The Xkey class is a generic class design to hold the X509 public
  /// key. The public key type is stored in a key algorithm object and
  /// the key value is a bitset extacted directly from the certificate.
  /// The decoding of the bitset is done automatically and the result
  /// is stored in a key object which is the standard afnix secured key
  /// representation. The key is persistent in the object.
  /// @author amaury darsch

  class Xkey : public virtual Object {
  protected:
    /// the key algorithm
    Xalgo    d_algo;
    /// the key bitset
    Bitset   d_bset;

  public:
    /// create an empty key
    Xkey (void);

    /// create a key by algoritthm and bitset
    /// @param algo the key algorithm
    /// @param bset the key bitset
    Xkey (const Xalgo& algo, const Bitset& bset);

    /// copy construct this object
    /// @param that the object to copy
    Xkey (const Xkey& that);

    /// assign an object to this one
    /// @param that the object to assign
    Xkey& operator = (const Xkey& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this certificate key
    virtual void reset (void);

    /// map the key from the algorithm key
    virtual Key* tokey (void) const;

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
