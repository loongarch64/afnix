// ---------------------------------------------------------------------------
// - XmlAttlist.hpp                                                          -
// - afnix:xml module - xml attribute list node class definition             -
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

#ifndef  AFNIX_XMLATTLIST_HPP
#define  AFNIX_XMLATTLIST_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlAttlist class is the class that handles the xml attribute list.
  /// A xml attribute list is primarily defined with two names. The first
  /// name is the element name and the second name is the attribute name. 
  /// There are 3 types of attribute list. The string type, the token type
  /// and the enumeration type. The class manages each type by associating 
  /// a type descriptor which is detected at construction.
  /// @author amaury darsch

  class XmlAttlist : public XmlNode {
  public:
    /// the attribute type
    enum t_attr {
      CDATA,    // string type
      ID,       // id type
      IDREF,    // idref type
      IDREFS,   // idref types
      ENTITY,   // entity type
      ENTITIES, // entity types
      NMTOKEN,  // name token
      NMTOKENS, // name tokens
      ENUMTYPE, // enumeration
      NOTATION  // notation type
    };

    /// the default value type
    enum t_xdef {
      VALUE,     // default value
      FIXED,     // #FIXED
      IMPLIED,   // #IMPLIED
      REQUIRED   // #REQUIRED
    };
  
  private:
    /// the element name
    String d_name;
    /// the attribute name
    String d_attr;
    /// the attribute type
    t_attr d_type;
    /// the enumeration vector
    Strvec d_xenm;
    /// the default value type
    t_xdef d_adef;
    /// the default value
    String d_xdef;

  public:
    /// create an xml attribute list by names and value
    /// @param name the element name 
    /// @param attr the attribute name
    XmlAttlist (const String& name, const String& attr);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// set the element node name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// @return the element name
    virtual String getname (void) const;

    /// set the attribute name
    /// @param attr the name to set
    virtual void setattr (const String& name);

    /// @return the attribute name
    virtual String getattr (void) const;

    /// set the attribute type
    /// @param type the attribute type to parse
    virtual void settype (const String& type);

    /// set the attribute enumeration type
    /// @param xenm the enumeration vector
    /// @param xflg the notation flag
    virtual void settype (const Strvec& xenm, const bool xflg);

    /// set the attribute enumeration type
    /// @param xvec the enumeration vector
    /// @param xflg the notation flag
    virtual void settype (Vector* xvec, const bool xflg);

    /// set the attribute default value
    /// @param xdef the default value
    virtual void setxdef (const String& xdef);

    /// set the fixed attribute default value
    /// @param xdef the fixed default value
    virtual void setfixd (const String& xdef);

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a node into an output steram
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlAttlist (const XmlAttlist&);
    // make the assignment operator private
    XmlAttlist& operator = (const XmlAttlist&);

  public:
    /// create an object in a generic way
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
