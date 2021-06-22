// ---------------------------------------------------------------------------
// - Part.hpp                                                                -
// - afnix:csm service - part class definition                               -
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

#ifndef  AFNIX_PART_HPP
#define  AFNIX_PART_HPP

#ifndef  AFNIX_UUID_HPP
#include "Uuid.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Part class is a taggable object which is bound by a unique key
  /// and provide a plist interface access. The part object is the foundation
  /// of the blob object and is also used to feed a collection. The key is
  /// represented by a uuid object.
  /// @author amaury darsch

  class Part : public Taggable {
  protected:
    /// the part kid
    Uuid d_kid;
    /// the part plist
    Plist* p_plst;

  public:
    /// create nil part
    Part (void);

    /// create a registered part by name
    /// @param name the registration name
    Part (const String& name);

    /// create a registered part by name and info
    /// @param name the registration name
    /// @param info the registration info
    Part (const String& name, const String& info);

    /// copy construct this part
    /// @param that the object to copy
    Part (const Part& that);
    
    /// copy move this part
    /// @param that the object to copy
    Part (Part&& that) noexcept;

    /// destroy this part
    ~Part (void);
    
    /// assign a part to this one
    /// @param that the object to assign
    Part& operator = (const Part& that);

    /// move a part to this one
    /// @param that the object to move
    Part& operator = (Part&& that) noexcept;

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this part
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this part
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// validate a part by kid
    /// @param kid the key id to check
    virtual bool iskid (const Uuid& kid) const;

    /// validate a part by kid
    /// @param kid the key id to check
    virtual bool iskid (const String& kid) const;

    /// set a part kid by uuid
    /// @param kid the key id to set
    virtual void setkid (const Uuid& kid);

    /// set a part kid by string uuid
    /// @param kid the key id to set
    virtual void setkid (const String& kid);

    /// @return the part kid
    virtual Uuid getkid (void) const;
    
    /// @return the part plist
    virtual Plist getplst (void) const;

    /// @return true if a property exits in the plist
    virtual bool isprop (const String& name) const;

    /// add a plist to the part
    /// @param plst the plist to add
    virtual void addplst (const Plist& plst);

    /// add to the plist by name and value
    /// @param name the name to add
    /// @param lval the value to add
    virtual void addprop (const String& name, const Literal& lval);

    /// add to the plist by name, info and literal value
    /// @param name the property name to add
    /// @param info the property info to add
    /// @param bval the property value to add
    virtual void addprop (const String&  name, const String& info, 
			  const Literal& lval);
    
    /// @return a plist literal by name
    virtual Literal* getlval (const String& name) const;
    
    /// @return a plist value by name
    virtual String getpval (const String& name) const;

    /// @return true if this part is swappable
    virtual bool isswap (Part* part) const;
    
    /// @return a subpart by name
    /// @param name the subpart name
    virtual Part* subpart (const String& name) const;
    
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
