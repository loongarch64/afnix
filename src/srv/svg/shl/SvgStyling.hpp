// ---------------------------------------------------------------------------
// - SvgStyling.hpp                                                          -
// - afnix:svg service - svg styling attribute node class definition         -
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

#ifndef  AFNIX_SVGSTYLING_HPP
#define  AFNIX_SVGSTYLING_HPP

#ifndef  AFNIX_SVGFORMING_HPP
#include "SvgForming.hpp"
#endif

namespace afnix {

  /// The SvgStyling class is a base class for the svg node that holds the
  /// styling attributes. The class offers numerous methods that can be
  /// used specify the object presentation such like color, stroke or filling.
  /// @author amaury darsch

  class SvgStyling : public SvgForming {
  public:
    /// create a svg node by name
    /// @param name the node name
    SvgStyling (const String& name);

    /// set the object fill attribute
    /// @param color the filling color
    void setfill (const String& color);

    /// set the object stroke attribute
    /// @param color the filling color
    void setstrk (const String& color);

    /// set the object stroke width attribute
    /// @param width the stroke width attribute
    void setswth (const long width);

  private:
    // make the copy constructor private
    SvgStyling (const SvgStyling&);
    // make the assignment operator private
    SvgStyling& operator = (const SvgStyling&);

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
