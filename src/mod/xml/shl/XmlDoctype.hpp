// ---------------------------------------------------------------------------
// - XmlDoctype.hpp                                                          -
// - afnix:xml module - xml document type node class definition              -
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

#ifndef  AFNIX_XMLDOCTYPE_HPP
#define  AFNIX_XMLDOCTYPE_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlDoctype class is the xml document type node. In its simplest 
  /// form, the document type has just a name which acts the starting tag
  /// for the document. The document tye can also be associated with a
  /// system or a public identifier. Note also that a local root node
  /// can be attached to this node.
  /// @author amaury darsch

  class XmlDoctype : public XmlNode {
  protected:
    /// the doctype value
    String   d_xval;
    /// the public literal
    String   d_publ;
    /// the system literal
    String   d_sysl;
    /// the local root node
    XmlNode* p_node;

  public:
    /// create a xml doctype by value
    /// @param xval the doctype value
    XmlDoctype (const String& xval);

    /// create a xml doctype by value and system id
    /// @param xval the doctype value
    /// @param sysl the system literal
    XmlDoctype (const String& xval, const String& sysl);

    /// create a xml doctype by value and system and public id
    /// @param xval the doctype value
    /// @param publ the public literal
    /// @param sysl the system literal
    XmlDoctype (const String& xval, const String& publ, const String& sysl);

    /// destroy this document type
    ~XmlDoctype (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return the node value 
    virtual String getxval (void) const;

    /// @return the node public literal
    virtual String getpubl (void) const;

    /// @return the node system literal
    virtual String getsysl (void) const;

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
    XmlDoctype (const XmlDoctype&);
    // make the assignment operator private
    XmlDoctype& operator = (const XmlDoctype&);

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
