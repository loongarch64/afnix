// ---------------------------------------------------------------------------
// - XhtmlText.hpp                                                           -
// - afnix:wax service - xhtml input text node class definition              -
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

#ifndef  AFNIX_XHTMLTEXT_HPP
#define  AFNIX_XHTMLTEXT_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlText class is a xhtml input text node used in the html form
  /// node. The text input is a field which binds a name to a user defined
  /// text. When the form is submitted, the user text is transmitted along
  /// with the associated name.
  /// @author amaury darsch

  class XhtmlText : public XhtmlBase {
  public:
    /// create a xhtml text by name
    /// @param name the text name
    XhtmlText (const String& name);

    /// create a xhtml text by name and size
    /// @param name the text name
    /// @param size the text size
    XhtmlText (const String& name, const String& size);

    /// @return the class name
    String repr (void) const;

    /// set the text size
    /// @param size the size attribute to set
    void setsize (const String& size);

  private:
    // make the copy constructor private
    XhtmlText (const XhtmlText&);
    // make the assignment operator private
    XhtmlText& operator = (const XhtmlText&);

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
