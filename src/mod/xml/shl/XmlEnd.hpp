// ---------------------------------------------------------------------------
// - XmlEnd.hpp                                                              -
// - afnix:xml module - xml end node class definition                        -
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

#ifndef  AFNIX_XMLEND_HPP
#define  AFNIX_XMLEND_HPP

#ifndef  AFNIX_NODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlEnd class is an internal class used to represent a xml end tag.
  /// This class is used by the reader to mark a end tag which is used to
  /// pop a node from the reader stack. This node is not part of the xml
  /// tree. When a xml end tag is reached, the popped node name must match
  /// the end node name. Internally, the end tag is seen as an empty node.
  /// @author amaury darsch

  class XmlEnd : public XmlNode {
  protected:
    /// the node name
    String d_name;

  public:
    /// create an end node by name
    /// @param name the tag name to use
    XmlEnd (const String& name);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// set the parent node - this method throw an exception
    /// @param node the parent node to set
    void setparent (XmlNode* node);

    /// @return true if a node name is valid
    bool isname (const String& name) const;

    /// set the end tag node name
    /// @param name the tag node name to set
    void setname (const String& name);

    /// @return the end tag node name
    String getname (void) const;

    /// write a end tag node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a end tag node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlEnd (const XmlEnd&);
    // make the assignment operator private
    XmlEnd& operator = (const XmlEnd&);

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
