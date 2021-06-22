// ---------------------------------------------------------------------------
// - XhtmlImg.hpp                                                            -
// - afnix:wax service - xhtml image node class definition                   -
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

#ifndef  AFNIX_XHTMLIMG_HPP
#define  AFNIX_XHTMLIMG_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlImg class is a xhtml image node used in the html body. The
  /// image node is an mpty node with several attributes including the image
  /// source, the image width and height and an alternate string.
  /// @author amaury darsch

  class XhtmlImg : public XmlTag {
  public:
    /// create a xhtml image node by source and alternate
    /// @param src the image src attribute
    /// @param alt the image alt attribute
    XhtmlImg (const String& src, const String& alt);

    /// @return the class name
    String repr (void) const;

    /// set the image width
    /// @param wth the image width to set
    void setwth (const String& wth);

    /// set the image height
    /// @param hgt the image height to set
    void sethgt (const String& hgt);

    /// set the image width and height attributes
    /// @param wth the image width to set
    /// @param hgt the image height to set
    void setgeo (const String& wth, const String& hgt);

  private:
    // make the copy constructor private
    XhtmlImg (const XhtmlImg&);
    // make the assignment operator private
    XhtmlImg& operator = (const XhtmlImg&);

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
