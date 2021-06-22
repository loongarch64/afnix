// ----------------------------------------------------------------------------
// - Realm.hpp                                                                -
// - evrix:csm service - realm class definition                               -
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

#ifndef  AFNIX_REALM_HPP
#define  AFNIX_REALM_HPP

#ifndef  AFNIX_AUTHORITY_HPP
#include "Authority.hpp"
#endif

#ifndef  AFNIX_WORKZONE_HPP
#include "WorkZone.hpp"
#endif

#ifndef  AFNIX_CREDENTIAL_HPP
#include "Credential.hpp"
#endif

namespace afnix {

  /// The Realm class is an abstract class design for the storage and 
  /// management of authorities. The class provides the basic methods
  /// to create, check and validate an authority.
  /// @author amaury darsch

  class Realm : public Nameable {
  public:
    /// create a default realm
    Realm(void) =default;
    
    /// @return the realm info
    virtual String getinfo (void) const =0;
    
    /// check if an authority kid exists
    /// @param kid the authority kid to check
    virtual bool exists (const String& kid) const =0;

    /// update the workzone with a authority
    /// @param auth the authority to update
    virtual void update (const Authority& auth) =0;

    /// create an authority by name and credential
    /// @param name the authority name
    /// @param cred the authority credential
    virtual void create (const String& name, const Credential& cred) =0;
    
    /// validate an authority by kid and credential
    /// @param kid  the authority name
    /// @param cred the authority credential
    virtual bool validate (const String& kid, const Credential& cred) const =0;

  private:
    // make the copy constructor private
    Realm (const Realm&) =delete;
    // make the assignment operator private
    Realm& operator = (const Realm&) =delete;

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
