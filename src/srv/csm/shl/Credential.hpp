// ----------------------------------------------------------------------------
// - Credential.hpp                                                           -
// - evrix:csm service - credential class definition                          -
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

#ifndef  AFNIX_CREDENTIAL_HPP
#define  AFNIX_CREDENTIAL_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Credential class is a collection of user credential which can be
  /// used by a notary to authenticate a party. The standard credential is
  /// the password or passphrase message. More elaborated credentials are
  /// possible to allow for multi-factor authentication. Note that the real
  /// password is never stored here. Password string credential is converted
  /// using a hash function and injecting a hashed value in this object is
  /// certainly not a good idea. All things beeing equal, the credential
  /// object should be considered sealed  when properly created. The class
  /// provides also a 'validate' method where a credential object is checked
  /// as being accepted as a valid credential for this object; in other words
  /// the argument credential contains at least all credentials of the object.
  /// @author amaury darsch

  class Credential : public virtual Serial {
  public:
    /// the credential type
    enum t_cred : t_byte {
      CRED_NONE = 0x00, // no credential
      CRED_PASS = 0x01  // pass string credential
    };
    
  private:
    /// the number of credentials
    long d_size;
    /// the credential array
    struct s_cred* p_cred;

  public:
    /// create an empty credential
    Credential (void);

    /// create a credential by pass string
    /// @param ppse the pass string
    Credential (const String& ppse);

    /// copy construct this credential
    /// @param that the object to copy
    Credential (const Credential& that);

    /// move construct this credential
    /// @param that the object to move
    Credential (Credential&& that);

    /// destroy this credential
    ~Credential (void);
    
    /// assign a credential to this one
    /// @param that the object to assign
    Credential& operator = (const Credential& that);

    /// move a credential to this one
    /// @param that the object to move
    Credential& operator = (Credential&& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this blob
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this blob
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// reset this credential
    virtual void reset (void);
    
    /// @return the number of credentials
    virtual long length (void) const;

    /// add a credential by type and string
    /// @param type the credential type
    /// @param cval the credential value
    virtual void add (const t_cred type, const String& cval);

    /// add a credential by type and string
    /// @param type the credential type
    /// @param cval the credential value
    /// @param nopf the no process flag
    virtual void add (const t_cred type, const String& cval, const bool nopf);

    /// @return a credential type by index
    virtual t_cred gettype (const long cidx) const;
    
    /// @return a credential value by index
    virtual String getcval (const long cidx) const;

    /// validate a credential by type and value
    /// @param type the credential type
    /// @param cval the credential value
    virtual bool validate (const t_cred type, const String& cval) const;

    /// validate a credential for acceptance
    /// @param cred the rcedential to accept
    virtual bool validate (const Credential& cred) const;
    
  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
