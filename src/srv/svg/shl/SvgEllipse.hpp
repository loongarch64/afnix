// ---------------------------------------------------------------------------
// - SvgEllipse.hpp                                                           -
// - afnix:svg service - svg ellipse node class definition                    -
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

#ifndef  AFNIX_SVGELLIPSE_HPP
#define  AFNIX_SVGELLIPSE_HPP

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgEllipse class is a basic shape svg node for ellipse object.
  /// The ellipse is defined by position and size like the circle but include
  /// tow radius, one in each directon. Optional attribute include standard
  /// styling.
  /// @author amaury darsch

  class SvgEllipse : public SvgStyling {
  public:
    /// create a ellipse by radius
    /// @param rx the ellipse x radius
    /// @param ry the ellipse y radius
    SvgEllipse (const long rx, const long ry);

    /// create a ellipse by radius and position
    /// @param rx the ellipse x radius
    /// @param ry the ellipse x radius
    /// @param cx the ellipse x position
    /// @param cy the ellipse y position
    SvgEllipse (const long rx, const long ry, const long cx, const long cy);

    /// @return the class name
    String repr (void) const;

    /// set the ellipse x position
    /// @param cx the ellipse x position
    void setcx (const long cx);

    /// set the ellipse y position
    /// @param cy the ellipse cy position
    void setcy (const long cy);

    /// set the ellipse position at once
    /// @param cx the ellipse x position
    /// @param cy the ellipse cy position
    void setec (const long cx, const long cy);

    /// set the ellipse x radius attribute
    /// @param rx the x width to set
    void setrx (const long rx);

    /// set the ellipse y radius attribute
    /// @param ry the y width to set
    void setry (const long ry);

    /// set the ellipse radius attribute
    /// @param rx the x width to set
    /// @param ry the y width to set
    void seter (const long rx, const long ry);

  private:
    // make the copy constructor private
    SvgEllipse (const SvgEllipse&);
    // make the assignment operator private
    SvgEllipse& operator = (const SvgEllipse&);

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
