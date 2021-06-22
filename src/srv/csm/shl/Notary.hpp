// ----------------------------------------------------------------------------
// - Notary.hpp                                                               -
// - evrix:csm service - notary class definition                              -
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

#ifndef  AFNIX_NOTARY_HPP
#define  AFNIX_NOTARY_HPP

#ifndef  AFNIX_ACT_HPP
#include "Act.hpp"
#endif

#ifndef  AFNIX_REALM_HPP
#include "Realm.hpp"
#endif

#ifndef  AFNIX_PRINCIPAL_HPP
#include "Principal.hpp"
#endif

namespace afnix {

  /// The Notary class is the a class designed to produce authenticated acts.
  /// When an act is produced, it can be used locally or across a session for
  /// remote operations such like authentication.
  /// @author amaury darsch

  class Notary : public Principal {
  private:
    /// the notary realm
    Realm* p_relm;

  public:
    /// create a default notary
    Notary (void);

    /// create a notary by uri
    /// @param uri the notary uri
    Notary (const Uri& uri);

    /// create a notary by name
    /// @param name the notary name
    Notary (const String& name);

    /// create a notary by name and info
    /// @param name the notary name
    /// @param info the notary info
    Notary (const String& name, const String& info);

    /// create a notary by name and input stream
    /// @param name the xml document name
    /// @param is the input stream
    Notary (const String& name, InputStream* is);

    /// copy construct this notary
    /// @param that the notary to copy
    Notary (const Notary& that);
    
    /// destroy this notary
    ~Notary (void);
    
    /// assign a notary to this one
    /// @param that the notary to assign
    Notary& operator = (const Notary& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;
    
    /// set the notary realm
    /// @param relm the realm to set
    virtual void setrelm (Realm* relm);
    
    /// @return the notary realm
    virtual Realm* getrelm (void) const;
    
    /// notary to xml by uri
    /// @param uri the xml uri
    virtual void toxml (const Uri& uri) const;

    /// notary to xml by string uri
    /// @param suri the string uri
    virtual void toxml (const String& suri) const;

    /// notary to xml by output stream
    /// @param os the output stream
    virtual void toxml (OutputStream* os) const;
    
    /// validate an act
    /// @param act the act to validate
    virtual bool validate (const Act& act) const;
    
    /// validate an identity
    /// @param idty the identity to validate
    virtual bool validate (const Identity& idty) const;
    
    /// validate an principal
    /// @param auth the principal to validate
    virtual bool validate (const Principal& auth) const;
    
    /// deliver an act to an identity by name, credential 
    /// @param name the identity name
    /// @param cred the identity credential
    /// @param rqst the requester identity
    virtual Act* deliver (const String& name, const Credential& cred,
			  const Identity& rqst) const;

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
