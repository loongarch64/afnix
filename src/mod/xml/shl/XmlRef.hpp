// ---------------------------------------------------------------------------
// - XmlRef.hpp                                                              -
// - afnix:xml module - xml reference abstract node class definition         -
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

#ifndef  AFNIX_XMLREF_HPP
#define  AFNIX_XMLREF_HPP

#ifndef  AFNIX_NODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlRef class is an abstact class that is designed to hold reference.
  /// Since the Xml reference defines several reference type, the common
  /// element is the string that represent it. Although, the reprentation
  /// is different from one element to another.
  /// @author amaury darsch

  class XmlRef : public XmlNode {
  protected:
    /// the reference value
    String d_xref;

  public:
    /// create an empty  reference node
    XmlRef (void);

    /// create a reference node by value
    /// @param xref the reference value
    XmlRef (const String& xref);

    /// @return the class name
    String repr (void) const;

    /// set the reference value
    /// @param xref the reference to set
    virtual void setxref (const String& xref);

    /// @return the reference value
    virtual String getxref (void) const;

  private:
    // make the copy constructor private
    XmlRef (const XmlRef&);
    // make the assignment operator private
    XmlRef& operator = (const XmlRef&);

  public:
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
