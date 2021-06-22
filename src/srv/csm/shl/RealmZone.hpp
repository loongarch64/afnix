// ----------------------------------------------------------------------------
// - RealmZone.hpp                                                            -
// - evrix:csm service - workzone realm class definition                      -
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

#ifndef  AFNIX_REALMZONE_HPP
#define  AFNIX_REALMZONE_HPP

#ifndef  AFNIX_REALM_HPP
#include "Realm.hpp"
#endif

#ifndef  AFNIX_WORKZONE_HPP
#include "WorkZone.hpp"
#endif

namespace afnix {

  /// The RealmZone class is a workzone based realm class designed for the
  /// storage and management of authoritys. Authoritys are placed in a
  /// workzone  which is derived from the application workspace.
  /// @author amaury darsch

  class RealmZone : public Realm {
  private:
    /// the realm zone
    WorkZone* p_zone;

  public:
    /// create a default realm
    RealmZone (void);

    /// create a realm by root
    /// @param root the workzone root
    RealmZone (const String& root);

    /// create a realm by workzone
    /// @param zone the realm zone
    RealmZone (WorkZone* zone);

    /// create a realm by root and name
    /// @param root the workzone root
    /// @param name the workzone name
    RealmZone (const String& root, const String& name);

    /// create a realm by root, name and info
    /// @param root the workzone root
    /// @param name the workzone name
    /// @param info the workzone name
    RealmZone (const String& root, const String& name, const String& info);

    /// destroy this realm
    ~RealmZone (void);

    /// @return the class name
    String repr (void) const;

    /// @return the realm name
    String getname (void) const;

    /// @return the realm info
    String getinfo (void) const;
    
    /// check if an authority kid exists
    /// @param kid the authority kid to check
    bool exists (const String& kid) const;

    /// update the workzone with an authority
    /// @param auth the authority to update
    void update (const Authority& auth);

    /// create an authority by name and credential
    /// @param name the authority name
    /// @param cred the authority credential
    void create (const String& name, const Credential& cred);
    
    /// validate an authority by kid and credential
    /// @param kid  the authority name
    /// @param cred the authority credential
    bool validate (const String& kid, const Credential& cred) const;

    /// @return the realm root
    virtual String getroot (void) const;
    
  private:
    // make the copy constructor private
    RealmZone (const RealmZone&) =delete;
    // make the assignment operator private
    RealmZone& operator = (const RealmZone&) =delete;

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
