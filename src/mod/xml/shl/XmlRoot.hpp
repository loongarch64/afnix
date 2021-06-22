// ---------------------------------------------------------------------------
// - XmlRoot.hpp                                                             -
// - afnix:xml module - xml root node class definition                       -
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

#ifndef  AFNIX_XMLROOT_HPP
#define  AFNIX_XMLROOT_HPP

#ifndef  AFNIX_XMLDECL_HPP
#include "XmlDecl.hpp"
#endif

namespace afnix {

  /// The XmlRoot class is the top level root instanciated by the xml
  /// reader when starting to parse a stream. There should be only one
  /// root node in a tree. The root node does not have a parent node.
  /// @author amaury darsch

  class XmlRoot : public XmlNode {
  public:
    /// create a default root node
    XmlRoot (void);

    /// create a root node with a declaration node
    /// @param dflg the declaration flag
    XmlRoot (const bool dflg);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return the root node without the declaration
    XmlRoot* bdup (void) const;

    /// @return true if a declaration node exists
    bool isdecl (void) const;

    /// @return the declaration node
    XmlDecl* getdecl (void) const;

    /// @return the root encoding mode
    String getemod (void) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;
    
    /// write a node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlRoot (const XmlRoot&);
    // make the assignment operator private
    XmlRoot& operator = (const XmlRoot&);

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
