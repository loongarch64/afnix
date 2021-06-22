// ---------------------------------------------------------------------------
// - SvgPolygon.hpp                                                          -
// - afnix:svg service - svg polygon node class definition                   -
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

#ifndef  AFNIX_SVGPOLYGON_HPP
#define  AFNIX_SVGPOLYGON_HPP

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgPolygon class is a basic shape svg node for polygon object.
  /// The polygonme is defined by a vector of integer plane points. Optional
  /// attribute include standard styling.
  /// @author amaury darsch

  class SvgPolygon : public SvgStyling {
  public:
    /// create a polygon by vector point
    /// @param pv the vector point
    SvgPolygon (const Vector& pv);

    /// @return the class name
    String repr (void) const;

    /// set the polygon by vector points
    /// @param pv the vector point to use
    void setpv (const Vector& pv);

  private:
    // make the copy constructor private
    SvgPolygon (const SvgPolygon&);
    // make the assignment operator private
    SvgPolygon& operator = (const SvgPolygon&);

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
