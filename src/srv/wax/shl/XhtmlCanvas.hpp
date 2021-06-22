// ---------------------------------------------------------------------------
// - XhtmlCanvas.hpp                                                         -
// - afnix:wax service - xhtml canvas node class definition                  -
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

#ifndef  AFNIX_XHTMLCANVAS_HPP
#define  AFNIX_XHTMLCANVAS_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlCanvas class is a xhtml canvas node used in the html body. The
  /// canvas node is cannot be an empty node and should be defined with a
  /// width and height attribute.
  /// @author amaury darsch

  class XhtmlCanvas : public XmlTag {
  public:
    /// create a default canvas
    XhtmlCanvas (void);

    /// @return the class name
    String repr (void) const;

    /// set the canvas width
    /// @param wth the canvas width to set
    void setwth (const String& wth);

    /// set the canvas height
    /// @param hgt the canvas height to set
    void sethgt (const String& hgt);

    /// set the canvas width and height attributes
    /// @param wth the canvas width to set
    /// @param hgt the canvas height to set
    void setgeo (const String& wth, const String& hgt);

  private:
    // make the copy constructor private
    XhtmlCanvas (const XhtmlCanvas&);
    // make the assignment operator private
    XhtmlCanvas& operator = (const XhtmlCanvas&);

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
