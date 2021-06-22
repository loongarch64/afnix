// ---------------------------------------------------------------------------
// - XhtmlOption.hpp                                                         -
// - afnix:wax service - xhtml select option node class definition           -
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

#ifndef  AFNIX_XHTMLSELECT_HPP
#define  AFNIX_XHTMLSELECT_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

#ifndef  AFNIX_XHTMLOPTION_HPP
#include "XhtmlOption.hpp"
#endif

namespace afnix {
  /// The XhtmlSelect class is a xhtml element used in the html form
  /// element. Generally a select is constructed by adding option elements.
  /// @author amaury darsch

  class XhtmlSelect : public XhtmlBase {
  public:
    /// create a xhtml select by name
    /// @param name the select name
    XhtmlSelect (const String& name);

    /// @return the class name
    String repr (void) const;

    /// add an option to the select tag
    /// @param lbal the option label
    /// @param sval the option value
    XhtmlOption* addoption (const String& lval, const String& sval);

  private:
    // make the copy constructor private
    XhtmlSelect (const XhtmlSelect&);
    // make the assignment operator private
    XhtmlSelect& operator = (const XhtmlSelect&);

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
