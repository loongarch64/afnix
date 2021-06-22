// ---------------------------------------------------------------------------
// - Carrier.hpp                                                             -
// - afnix:csm service - object carrier blob class definition                -
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

#ifndef  AFNIX_CARRIER_HPP
#define  AFNIX_CARRIER_HPP

#ifndef  AFNIX_BLOB_HPP
#include "Blob.hpp"
#endif

namespace afnix {

  /// The Carrier class is a blob used to transport an object. The object
  /// transported by the carrier must be serializable. 
  /// @author amaury darsch

  class Carrier : public Blob {
  protected:
    /// the blob object
    Object* p_aobj;

  public:
    /// create nil carrier
    Carrier (void);

    /// create an anonymous carrier
    /// @param aobj the blob object 
    Carrier (Object* aobj);

    /// create a carrier object by name
    /// @param aobj the blob object
    /// @param name the blob name
    Carrier (Object* aobj, const String& name);

    /// create a carrier object by name and info
    /// @param aobj the blob object
    /// @param name the blob name
    /// @param info the blob info
    Carrier (Object* aobj, const String& name, const String& info);

    /// create a carrier object by rid, name and info
    /// @param aobj the blob object
    /// @param name the blob name
    /// @param info the blob info
    /// @param rid  the registration id
    Carrier (Object* aobj, const String& rid, const String& name,
	     const String& info);

    /// copy construct this carrier object
    /// @param that the object to copy
    Carrier (const Carrier& that);

    /// destroy this carrier object
    ~Carrier (void);

    /// assign a carrier to this one
    /// @param that the object to assign
    Carrier& operator = (const Carrier& that);
    
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
    
    /// @return the blob object
    virtual Object* getobj (void) const;

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
