// ---------------------------------------------------------------------------
// - XsoInfo.hpp                                                             -
// - afnix:xml module - xml/xsm node info class definition                   -
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

#ifndef  AFNIX_XSOINFO_HPP
#define  AFNIX_XSOINFO_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The XsoInfo class is a xml/xsm information node used to carry simple
  /// information about a tag. The node is constructed by name, with a 
  /// set of property and eventually a text associated with the node. The
  /// text information is generally the one associated between the start
  /// tag and the end tag. In the case of complex tree, such text data
  /// might be empty.
  /// @author amaury darsch

  class XsoInfo : public Nameable {
  private:
    /// the tag name
    String d_name;
    /// the attribute list
    Plist  d_alst;
    /// the tag text
    String d_xval;

  public:
    /// create an empty info object
    XsoInfo (void);

    /// create an info object by name
    /// @param name the info name
    XsoInfo (const String& name);

    /// create an info object by name and text
    /// @param name the info name
    /// @param xval the info text
    XsoInfo (const String& name, const String& xval);

    /// create an info object by name, attribute and text
    /// @param name the info name
    /// @param alst the attribute list
    /// @param xval the info text
    XsoInfo (const String& name, const Plist& alst, const String& xval);

    /// copy construct this info object
    /// @param that the node to copy
    XsoInfo (const XsoInfo& that);

    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this node
    Object* clone (void) const;

    /// assign a info object to this one
    /// @param that the info object to assign
    XsoInfo& operator = (const XsoInfo& that);

    /// set the info object name
    /// @param name the name to set
    void setname (const String& name);

    /// @return the info name
    String getname (void) const;

    /// @return true if an attribute exists
    bool exists (const String& name) const;

    /// set an attribute by name and value
    /// @param name the attribute name
    /// @param lval the attribute value to set
    void setattr (const String& name, const Literal& pval);

    /// @return the info attribute list
    Plist getattr (void) const;

    /// get an attribute value by name
    /// @param name the attribute name
    String getpval (const String& name) const;

    /// set the info object text
    /// @param xval the text to set
    void setxval (const String& xval);

    /// @return the info text
    String getxval (void) const;

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
