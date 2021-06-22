// ---------------------------------------------------------------------------
// - XmlPi.hpp                                                               -
// - afnix:xml module - xml processing instruction node class definition     -
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

#ifndef  AFNIX_XMLPI_HPP
#define  AFNIX_XMLPI_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlPi class is the processing instruction node. A pi node is a node
  /// directly built by the xml reader. The processing instruction node is
  /// defined by a name and a string value.
  /// @author amaury darsch

  class XmlPi : public XmlNode {
  protected:
    /// the pi name
    String d_name;
    /// the pi value
    String d_xval;

  public:
    /// create an xml pi node by name
    /// @param name the instruction name 
    XmlPi (const String& name);

    /// create an xml pi node by name and value
    /// @param name the instruction name 
    /// @param xval the instruction value 
    XmlPi (const String& name, const String& xval);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return true if the node name is valid
    bool isname (const String& name) const;

    /// set the node name
    /// @param name the name to set
    virtual void setname (const String& xval);

    /// @return the node name
    virtual String getname (void) const;

    /// set the node value
    /// @param xval the value to set
    virtual void setxval (const String& xval);

    /// @return the node value 
    virtual String getxval (void) const;

    /// map the node value to an attribute list
    virtual Plist mapxval (void) const;

    /// map the node value to an attribute list
    /// @param xvid the xml version id
    virtual Plist mapxval (const String& xvid) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a node into an output steram
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlPi (const XmlPi&);
    // make the assignment operator private
    XmlPi& operator = (const XmlPi&);

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
