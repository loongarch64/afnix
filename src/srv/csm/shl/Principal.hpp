// ----------------------------------------------------------------------------
// - Principal.hpp                                                            -
// - evrix:csm service - principal class definition                           -
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

#ifndef  AFNIX_PRINCIPAL_HPP
#define  AFNIX_PRINCIPAL_HPP

#ifndef  AFNIX_IDENTITY_HPP
#include "Identity.hpp"
#endif

namespace afnix {

  /// The Principal class is an identity party who has the power of an
  /// apparent authority. Unlike the authority who is identified by credential,
  /// the principal carries its signing key which can be changed at any time.
  /// @author amaury darsch

  class Principal : public Identity {
  protected:
    /// the signing key
    Key* p_skey;
    
  public:
    /// create a nil principal
    Principal (void);

    /// create a principal by name
    /// @param name the principal name
    Principal (const String& name);

    /// create a principal by name and info
    /// @param name the principal name
    /// @param info the principal info
    Principal (const String& name, const String& info);

    /// create a principal by rid, name and info
    /// @param rid  the principal rid
    /// @param name the principal name
    /// @param info the principal info
    Principal (const String& rid, const String& name, const String& info);

    /// create an principal by name and input stream
    /// @param name the xml document name
    /// @param is the input stream
    Principal (const String& name, InputStream* is);

    /// copy construct this principal
    /// @param that the object to copy
    Principal (const Principal& that);

    /// destroy this principal
    ~Principal (void);
    
    /// assign a principal to this one
    /// @param that the object to assign
    Principal& operator = (const Principal& that);
    
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

    /// verify the principal
    bool verify (void) const;

    /// sign this principal by key
    /// @param skey the signing key
    bool sign (const Key& skey);

    /// sign a principal with a new key
    virtual bool sign (void);
    
    /// sign a signable object
    /// @param sgnl the signable to sign
    virtual bool sign (Signable* sgnl) const;
    
    /// @return the principal key
    virtual Key* getskey (void) const;

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

