// ---------------------------------------------------------------------------
// - XhtmlBase.hpp                                                           -
// - afnix:wax service - xhtml base element class definition                 -
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

#ifndef  AFNIX_XHTMLBASE_HPP
#define  AFNIX_XHTMLBASE_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlBase class is a base class for the xhtml element. It defines
  /// the common attributes for the standard xhtml elements.
  /// @author amaury darsch

  class XhtmlBase : public XmlTag {
  public:
    /// create a xhtml by tag name
    /// @param name the tag name
    XhtmlBase (const String& name);

    /// create a xhtml by tag name and end flag
    /// @param name the tag name
    /// @param eflg the end tag flag
    XhtmlBase (const String& name, const bool eflg);

    /// set the element class
    /// @param sval the attribute value
    virtual void setclass (const String& sval);

    /// set the element style
    /// @param sval the attribute value
    virtual void setstyle (const String& sval);

  private:
    // make the copy constructor private
    XhtmlBase (const XhtmlBase&);
    // make the assignment operator private
    XhtmlBase& operator = (const XhtmlBase&);

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
