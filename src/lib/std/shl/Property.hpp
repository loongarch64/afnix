// ---------------------------------------------------------------------------
// - Property.hpp                                                            -
// - standard object library - property class definition                     -
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

#ifndef AFNIX_PROPERTY_HPP
#define AFNIX_PROPERTY_HPP

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Property class is a simple name/value pair. The property object
  /// is designed to operate with a property list but this is not mandatory.
  /// @author amaury darsch

  class Property: public Taggable {
  protected:
    /// the property value
    Literal* p_pval;

  public:
    /// create a default property
    Property (void);

    /// create a new property by name
    /// @param name the property name
    Property (const String& name);

    /// create a new property by name and boolean value
    /// @param name the property name
    /// @param bval the property value
    Property (const String& name, const bool bval);

    /// create a new property by name and integer value
    /// @param name the property name
    /// @param ival the property value
    Property (const String& name, const t_long ival);

    /// create a new property by name and real value
    /// @param name the property name
    /// @param rval the property value
    Property (const String& name, const t_real rval);

    /// create a new property by name and value
    /// @param name the property name
    /// @param lval the property value
    Property (const String& name, const Literal& lval);

    /// create a new property by name, info and boolean value
    /// @param name the property name
    /// @param info the property info
    /// @param bval the property value
    Property (const String& name, const String& info, const bool bval);

    /// create a new property by name, info and integer value
    /// @param name the property name
    /// @param info the property info
    /// @param ival the property value
    Property (const String& name, const String& info, const t_long ival);

    /// create a new property by name, info and real value
    /// @param name the property name
    /// @param info the property info
    /// @param rval the property value
    Property (const String& name, const String& info, const t_real rval);

    /// create a new property by name, info and value
    /// @param name the property name
    /// @param info the property info
    /// @param lval the property value
    Property (const String& name, const String& info, const Literal& lval);

    /// copy construct this property
    /// @param that the property class to copy
    Property (const Property& that);

    /// copy move this property
    /// @param that the property class to move
    Property (Property&& that) noexcept;

    /// destroy this property
    ~Property (void);

    /// assign a property to this one
    /// @param that the property to assign
    Property& operator = (const Property& that);

    /// move a property to this one
    /// @param that the property to move
    Property& operator = (Property&& that) noexcept;

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the property did
    t_word getdid (void) const;

    /// @return the property sid
    t_word getsid (void) const;

    /// serialize this property to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a property from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// clear this property
    virtual void clear (void);
    
    /// set the property by boolean value
    /// @param bval the boolean value to set
    virtual void setpval (const bool bval);

    /// set the property by integer value
    /// @param ival the boolean value to set
    virtual void setpval (const t_long ival);

    /// set the property by real value
    /// @param rval the real value to set
    virtual void setpval (const t_real rval);

    /// set the property value
    /// @param lval the literal value to set
    virtual void setpval (const Literal& lval);

    /// @return the property value
    virtual String getpval (void) const;

    /// @return the property as a boolean
    virtual bool tobool (void) const;

    /// @return the property as a literal
    virtual Literal* toliteral (void) const;
    
    /// @return the property as an integer boolean
    virtual bool toboil (void) const;

    /// @return the property as an integer
    virtual t_long tolong (void) const;

    /// @return the property as a real
    virtual t_real toreal (void) const;

    /// set the property pair by boolean value
    /// @param name the property name to set
    /// @param bval the boolean value to set
    virtual void set (const String& name, const bool bval);

    /// set the property pair by integer value
    /// @param name the property name to set
    /// @param ival the integer value to set
    virtual void set (const String& name, const t_long ival);

    /// set the property pair by real value
    /// @param name the property name to set
    /// @param rval the real value to set
    virtual void set (const String& name, const t_real rval);

    /// set the property pair at once
    /// @param name the property name to set
    /// @param lval the literal value to set
    virtual void set (const String& name, const Literal& lval);

    /// format the property value with style
    /// @param lstl the literal style 
    virtual String format (const Style& lstl) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the name to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
