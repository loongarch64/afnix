// ---------------------------------------------------------------------------
// - SvgRect.hpp                                                             -
// - afnix:svg service - svg rectangle node class definition                 -
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

#ifndef  AFNIX_SVGRECT_HPP
#define  AFNIX_SVGRECT_HPP

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgRect class is a basic shape svg node for rectangle object.
  /// Teh rectangle is defined by position and size. Optional attributes
  /// include rectangle rounding effect and standard styling.
  /// @author amaury darsch

  class SvgRect : public SvgStyling {
  public:
    /// create a rectangle by width and height
    /// @param width  the rectangle width
    /// @param height the rectangle height
    SvgRect (const long width, const long height);

    /// create a rectangle by width, height and position
    /// @param width  the rectangle width
    /// @param height the rectangle height
    /// @param x      the rectangle x position
    /// @param y      the rectangle y position
    SvgRect (const long width, const long height, const long x, const long y);

    /// @return the class name
    String repr (void) const;

    /// set the rectangle x position
    /// @param x the rectangle x position
    void setx (const long x);

    /// set the rectangle y position
    /// @param y the rectangle y position
    void sety (const long y);

    /// set the rectangle width attribute
    /// @param width the width to set
    void setwth (const long width);

    /// set the rectangle width attribute
    /// @param wth the width string to set
    void setwth (const String& width);

    /// set the svg height attribute
    /// @param height the width to set
    void sethgt (const long height);

    /// set the rectangle height attribute
    /// @param hgt the height string to set
    void sethgt (const String& hgt);

    /// set the rectangle rounding x attribute
    /// @param rx the x rounding to set
    void setrx (const long rx);

    /// set the rectangle rounding y attribute
    /// @param ry the y rounding to set
    void setry (const long ry);

  private:
    // make the copy constructor private
    SvgRect (const SvgRect&);
    // make the assignment operator private
    SvgRect& operator = (const SvgRect&);

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
