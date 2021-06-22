// ----------------------------------------------------------------------------
// - Identity.hpp                                                             -
// - evrix:csm service - identity class definition                            -
// ----------------------------------------------------------------------------
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

#ifndef  AFNIX_IDENTITY_HPP
#define  AFNIX_IDENTITY_HPP

#ifndef  AFNIX_BLOC_HPP
#include "Bloc.hpp"
#endif

#ifndef  AFNIX_SIGNABLE_HPP
#include "Signable.hpp"
#endif

#ifndef  AFNIX_SIGNATURE_HPP
#include "Signature.hpp"
#endif

namespace afnix {

  /// The Identity class is a registrable party who is identified by name,
  /// with an optional information. The identity is associated with a
  /// signature which can be used to verify the identity with the help of the
  /// public signing key. The identity is a parameter blob, any other
  /// parameters can be added to it.
  /// @author amaury darsch

  class Identity : public Bloc, public Signable {
  protected:
    /// the identity signature
    Signature* p_sign;
    
  public:
    /// create a default identity
    Identity (void);

    /// create a identity by name
    /// @param name the identity name
    Identity (const String& name);

    /// create a identity by name and info
    /// @param name the identity name
    /// @param info the identity info
    Identity (const String& name, const String& info);

    /// create a identity by rid, name and info
    /// @param  rid the identity rid
    /// @param name the identity name
    /// @param info the identity info
    Identity (const String& rid, const String& name, const String& info);

    /// copy construct this identity
    /// @param that the object to copy
    Identity (const Identity& that);

    /// destroy this identity
    ~Identity (void);
    
    /// assign a identity to this one
    /// @param that the object to assign
    Identity& operator = (const Identity& that);
    
    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return the blob plist
    Plist getplst (void) const override;

    /// serialize the signable object
    /// @param os the output stream
    void wrsign (OutputStream& os) const override;
    
    /// sign this identity by key
    /// @param skey the signing key
    bool sign (const Key& skey) override;

    /// verify this identity
    bool verify (void) const override;

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
