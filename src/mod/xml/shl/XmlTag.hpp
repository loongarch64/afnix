// ---------------------------------------------------------------------------
// - XmlTag.hpp                                                              -
// - afnix:xml module - xml tag node class definition                        -
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

#ifndef  AFNIX_XMLTAG_HPP
#define  AFNIX_XMLTAG_HPP

#ifndef  AFNIX_NODE_HPP
#include "XmlNode.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The XmlTag class is the base class used to represent a xml tag.
  /// A tag is defined with a name and an attribute list. The tag is
  /// derived from the xml node class and is not marked empty by default.
  /// @author amaury darsch

  class XmlTag : public XmlNode {
  protected:
    /// the node name
    String d_name;
    /// the attributes list
    Plist  d_alst;

  public:
    /// create a tag node by name
    /// @param name the tag name to use
    XmlTag (const String& name);

    /// create a tag node by name and empty flag
    /// @param name the tag name to use
    /// @param eflg the node empty flag
    XmlTag (const String& name, const bool eflg);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return true if a node name is valid
    bool isname (const String& name) const;

    /// @return true if the node attribute exists
    bool isattr (const String& name) const;

    /// @return true if the node attribute is valid
    bool isattr (const String& name, const String& pval) const;

    /// @return the tag node name
    virtual String getname (void) const;

    /// set the tag node name
    /// @param name the tag node name to set
    virtual void setname (const String& name);

    /// clear the attribute list
    virtual void clrattr (void);

    /// @return the number of attributes
    virtual long lenattr (void) const;

    /// add a node attribute by property
    /// @param prop the property to add
    virtual void addattr (Property* prop);

    /// set a node attribute by name and value
    /// @param name the attribute name to set
    /// @param lval the attribute value to set
    virtual void setattr (const String& name, const Literal& lval);

    /// set a node attribute by name and integer value
    /// @param name the attribute name to set
    /// @param xval the attribute value to set
    virtual void setattr (const String& name, const t_long xval);

    /// set a node attribute by name and integer value
    /// @param name the attribute name to set
    /// @param xval the attribute value to set
    virtual void setattr (const String& name, const t_octa xval);

    /// @return an attribute property by index
    virtual Property* getattr (const long index) const;

    /// @return an attribute property by name
    virtual Property* getattr (const String& name) const;

    /// @return an attribute property by name or throw an exception
    virtual Property* lookattr (const String& name) const;

    /// @return an attribute value by name
    virtual String getpval (const String& name) const;

    /// add an attribute list
    /// @param alst the attribute list to add
    virtual void addalst (const Plist& alst);

    /// set an attribute list
    /// @param alst the attribute list to set
    virtual void setalst (const Plist& alst);

    /// @return an attribute property list
    virtual Plist getalst (void) const;

    /// merge a node tag into this one
    /// @param node the node to merge
    virtual void merge (const XmlTag& node);
    
    /// write a tag  node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a tag node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlTag (const XmlTag&);
    // make the assignment operator private
    XmlTag& operator = (const XmlTag&);

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
