// ---------------------------------------------------------------------------
// - SvgCircle.hpp                                                           -
// - afnix:svg service - svg circle node class definition                    -
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

#ifndef  AFNIX_SVGCIRCLE_HPP
#define  AFNIX_SVGCIRCLE_HPP

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgCircle class is a basic shape svg node for circle object.
  /// The circle is defined by position and size. Optional attributes
  /// include standard styling.
  /// @author amaury darsch

  class SvgCircle : public SvgStyling {
  public:
    /// create a circle by radius
    /// @param radius the circle radius
    SvgCircle (const long radius);

    /// create a circle by radius and position
    /// @param radius the circle radius
    /// @param x      the circle x position
    /// @param y      the circle y position
    SvgCircle (const long radius, const long x, const long y);

    /// @return the class name
    String repr (void) const;

    /// set the circle x position
    /// @param x the circle x position
    void setx (const long x);

    /// set the circle y position
    /// @param y the circle y position
    void sety (const long y);

    /// set the circle radius attribute
    /// @param  the width to set
    void setcr (const long radius);

  private:
    // make the copy constructor private
    SvgCircle (const SvgCircle&);
    // make the assignment operator private
    SvgCircle& operator = (const SvgCircle&);

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
