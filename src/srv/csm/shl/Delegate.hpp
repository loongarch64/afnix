// ---------------------------------------------------------------------------
// - Delegate.hpp                                                            -
// - afnix:csm service - delegate blob class definition                     -
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

#ifndef  AFNIX_DELEGATE_HPP
#define  AFNIX_DELEGATE_HPP

#ifndef  AFNIX_CARRIER_HPP
#include "Carrier.hpp"
#endif

namespace afnix {

  /// The Delegate class is a carrier blob which delegates its transport to
  /// another object. Such approach is used when the carried object needs to
  /// remains locally (aka it cannot be serialized) but a reference to it can
  /// be sent to the remote peer.
  /// @author amaury darsch

  class Delegate : public Carrier {
  protected:
    /// the delegate address
    String d_addr;

  public:
    /// create nil delegate
    Delegate (void);
    
    /// create an anonymous delegate
    /// @param aobj the blob object 
    Delegate (Object* aobj);

    /// create a delegate object by name
    /// @param aobj the blob object
    /// @param name the blob name
    Delegate (Object* aobj, const String& name);

    /// create a delegate object by name and info
    /// @param aobj the blob object
    /// @param name the blob name
    /// @param info the blob info
    Delegate (Object* aobj, const String& name, const String& info);

    /// create a delegate object by rid, name and info
    /// @param aobj the blob object
    /// @param rid  the registration id
    /// @param name the blob name
    /// @param info the blob info
    Delegate (Object* aobj, const String& rid, const String& name,
	     const String& info);

    /// create a delegate object by rid, name, info and address
    /// @param aobj the blob object
    /// @param rid  the registration id
    /// @param name the blob name
    /// @param info the blob info
    /// @param addr the delegate address
    Delegate (Object* aobj, const String& rid, const String& name,
	      const String& info, const String& addr);

    /// copy construct this delegate object
    /// @param that the object to copy
    Delegate (const Delegate& that);

    /// assign a delegate to this one
    /// @param that the object to assign
    Delegate& operator = (const Delegate& that);
    
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

    /// set the delegate address
    /// @param addr the address to set
    virtual void setaddr (const String& addr);

    /// @return the delegate address
    virtual String getaddr (void) const;
    
  public:
    /// create a object in a generic way
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
