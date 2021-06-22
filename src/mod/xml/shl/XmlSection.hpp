// ---------------------------------------------------------------------------
// - XmlSection.hpp                                                          -
// - afnix:xml module - xml section node class definition                    -
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

#ifndef  AFNIX_XMLSECTION_HPP
#define  AFNIX_XMLSECTION_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlSection class is the xml section type node. A section node
  /// is used to model conditional section that are part of a dtd. The
  /// section value is a string that is evaluated by the xml processor.
  /// Most of the time, it is a parameter entity reference which corresponds
  /// to the keyword INCLUDE or IGNORE , but it could be anything else. A node
  /// is also attached to this section.
  /// @author amaury darsch

  class XmlSection : public XmlNode {
  protected:
    /// the section value
    String   d_xval;
    /// the local root node
    XmlNode* p_node;

  public:
    /// create a xml section by value
    /// @param xval the section value
    XmlSection (const String& xval);

    /// destroy this section node
    ~XmlSection (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return the node value 
    virtual String getxval (void) const;

    /// attach a node to the document type
    /// @param node the node to attach
    void setnode (XmlNode* node);

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a node into an output steram
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlSection (const XmlSection&);
    // make the assignment operator private
    XmlSection& operator = (const XmlSection&);

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
