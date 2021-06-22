// ---------------------------------------------------------------------------
// - Visa.hpp                                                                -
// - afnix:csm service - user identification act class definition            -
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

#ifndef  AFNIX_VISA_HPP
#define  AFNIX_VISA_HPP

#ifndef  AFNIX_ACT_HPP
#include "Act.hpp"
#endif

namespace afnix {

  /// The Visa class is a user pass that has been signed by a notary and
  /// properly identify a user with respect to credentials  that have been
  /// produced. Single of multi-factor authentication can be perform
  /// depending on the number of credentials supplied to the notary.
  /// It is up to the notary to decide whether the credentials are
  /// valid or not and at the end the act contains the identity of the user
  /// who has been authenticated. Note that the credentials are supplied to
  /// the notary and are never stored in the act.
  /// @author amaury darsch

  class Visa : public Act {
  protected:
    /// the user identity
    Identity d_user;
    
  public:
    /// create a null visa
    Visa (void) = default;

    /// create a visa by identities
    /// @param idty the act identity
    /// @param rqst the act requester
    /// @param user the user identity
    Visa (const Identity& idty, const Identity& rqst, const Identity& user);

    /// copy construct this visa
    /// @param that the object to copy
    Visa (const Visa& that);

    /// assign an visa to this one
    /// @param that the object to assign
    Visa& operator = (const Visa& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the user identity
    virtual Identity getuser (void) const;
    
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
