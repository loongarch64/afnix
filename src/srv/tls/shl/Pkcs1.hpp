// ---------------------------------------------------------------------------
// - Pkcs1.hpp                                                               -
// - afnix:tls service - public key cryptographic standard class definition  -
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

#ifndef  AFNIX_PKCS1_HPP
#define  AFNIX_PKCS1_HPP

#ifndef  AFNIX_PKI_HPP
#include "Pki.hpp"
#endif

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

namespace afnix {

  /// The Pkcs1 is the rsa cryptography standard. Originally defined by
  /// RSA Security Inc, it is now a standard track referenced as RFC3447.
  /// @author amaury darsch

  class Pkcs1 : public Pki {
  private:
    /// the pkcs key
    Key* p_pkey;

  public:
    /// create a default pkcs key
    Pkcs1 (void);

    /// create a pkcs by key
    /// @param pkey the key to set
    Pkcs1 (Key* pkey);
    
    /// create a pkcs key by path
    /// @param path the input file path
    Pkcs1 (const String& path);
    
    /// copy construct a pkcs key object
    /// @param that the object to copy
    Pkcs1 (const Pkcs1& that);

    /// destroy this pkcs key
    ~Pkcs1 (void);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a pkcs key to this one
    /// @param that the object to copy
    Pkcs1& operator = (const Pkcs1& that);

    /// reset this pkcs object
    void reset (void);

    /// @return the pkcs key
    virtual Key* getkey (void) const;

  protected:
    /// encode a pki buffer
    bool encode (void);

    /// decode a pki buffer
    bool decode (void);
    
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
