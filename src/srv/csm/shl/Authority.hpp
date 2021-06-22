// ----------------------------------------------------------------------------
// - Authority.hpp                                                            -
// - evrix:csm service - authority class definition                           -
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

#ifndef  AFNIX_AUTHORITY_HPP
#define  AFNIX_AUTHORITY_HPP

#ifndef  AFNIX_URI_HPP
#include "Uri.hpp"
#endif

#ifndef  AFNIX_IDENTITY_HPP
#include "Identity.hpp"
#endif

#ifndef  AFNIX_CREDENTIAL_HPP
#include "Credential.hpp"
#endif

namespace afnix {

  /// The Authority class is an authoritative party who subsumes an identity.
  /// The authority is designed to sign an identity and transports one or more
  /// credentials bound to the undelying identity. The authority is also
  /// designed to be saved in a readable xml form.
  /// @author amaury darsch

  class Authority : public Identity {
  protected:
    /// the authority credentials
    Credential* p_cred;
    
  public:
    /// create a default authority
    Authority (void);

    /// create a authority by uri
    /// @param uri the authority uri
    Authority (const Uri& uri);
    
    /// create a authority by name
    /// @param name the authority name
    Authority (const String& name);

    /// create a authority by name and info
    /// @param name the authority name
    /// @param info the authority info
    Authority (const String& name, const String& info);

    /// create a authority by rid, name and info
    /// @param rid  the authority rid
    /// @param name the authority name
    /// @param info the authority info
    Authority (const String& rid, const String& name, const String& info);

    /// create an authority by name and input stream
    /// @param name the xml document name
    /// @param is the input stream
    Authority (const String& name, InputStream* is);

    /// copy construct this authority
    /// @param that the object to copy
    Authority (const Authority& that);

    /// destroy this authority
    ~Authority (void);
    
    /// assign a authority to this one
    /// @param that the object to assign
    Authority& operator = (const Authority& that);
    
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

    /// @return the blob plist
    Plist getplst (void) const;

    /// set the authority credential value
    /// @param cred the credential to set
    virtual void setcred (const Credential& cred);
    
    /// @return the authority credential
    virtual Credential* getcred (void) const;
    
    /// authority to xml by uri
    /// @param uri the xml uri
    virtual void toxml (const Uri& uri) const;

    /// authority to xml by string uri
    /// @param suri the string uri
    virtual void toxml (const String& suri) const;

    /// authority to xml by output stream
    /// @param os the output stream
    virtual void toxml (OutputStream* os) const;
    
    /// validate an authority by credential
    /// @param cred the authority credential
    virtual bool validate (const Credential& cred) const;

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

