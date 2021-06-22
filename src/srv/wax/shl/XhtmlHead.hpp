// ---------------------------------------------------------------------------
// - XhtmlHead.hpp                                                           -
// - afnix:wax service - xhtml head node class definition                    -
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

#ifndef  AFNIX_XHTMLHEAD_HPP
#define  AFNIX_XHTMLHEAD_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlHead class is a xhtml head node used for the design of a xhtml
  /// document page. At construction, the head node is initialized with a 
  /// with a title node. The class is designed to hold as well meta nodes
  /// and style nodes.
  /// @author amaury darsch

  class XhtmlHead : public XmlTag {
  public:
    /// create a xhtml head node with a title
    /// @param title the node title
    XhtmlHead (const String& title);

    /// @return the class name
    String repr (void) const;
    
    /// set the head title by value
    /// @param title the title to set
    void settitle (const String& title);

    /// add charset meta node
    void addcset (void);

    /// add meta data node by name and content
    /// @param name the meta data name
    /// @param cnts the meta data content
    void addmeta (const String& name, const String& cnts);

    /// add a link node as a style reference
    /// @param url the url style reference
    void addstyle (const String& url);

    /// add a script node by type and url
    /// @param type the script type
    /// @param url  the script url
    void addscript (const String& type, const String& url);

  private:
    // make the copy constructor private
    XhtmlHead (const XhtmlHead&);
    // make the assignment operator private
    XhtmlHead& operator = (const XhtmlHead&);

  public:
    /// create a new object in a generic object
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
