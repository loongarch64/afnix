// ---------------------------------------------------------------------------
// - XmlElement.hpp                                                           -
// - afnix:xml module - xml element node class definition                     -
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

#ifndef  AFNIX_XMLELEMENT_HPP
#define  AFNIX_XMLELEMENT_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlElement class is the class for the xml element representation.
  /// A xml element is represented with a name and a value. It is during the
  /// processing phase that the element value is interpreted.
  /// @author amaury darsch

  class XmlElement : public XmlNode {
  protected:
    /// the element name
    String d_name;
    /// the element value
    String d_xval;

  public:
    /// create an xml element node by name and value
    /// @param name the element name 
    /// @param xval the element value
    XmlElement (const String& name, const String& xval);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return true if a node name is valid
    bool isname (const String& name) const;

    /// set the element node name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// @return the element name
    virtual String getname (void) const;

    /// set the element node value
    /// @param xval the value to set
    virtual void setxval (const String& xval);

    /// @return the element value
    virtual String getxval (void) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a node into an output steram
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlElement (const XmlElement&);
    // make the assignment operator private
    XmlElement& operator = (const XmlElement&);

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
