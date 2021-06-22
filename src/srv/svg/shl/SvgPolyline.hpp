// ---------------------------------------------------------------------------
// - SvgPolyline.hpp                                                         -
// - afnix:svg service - svg polyline node class definition                  -
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

#ifndef  AFNIX_SVGPOLYLINE_HPP
#define  AFNIX_SVGPOLYLINE_HPP

#ifndef  AFNIX_IPPD_HPP
#include "Ippd.hpp"
#endif

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgPolyline class is a basic shape svg node for polyline object.
  /// The polyline is defined by a vector of integer plane points. Optional
  /// attribute include standard styling.
  /// @author amaury darsch

  class SvgPolyline : public SvgStyling {
  public:
    /// create an empty polyline
    /// @param pv the vector point
    SvgPolyline (void);

    /// create a polyline by vector point
    /// @param pv the vector point
    SvgPolyline (const Vector& pv);

    /// @return the class name
    String repr (void) const;

    /// set the polyline by vector points
    /// @param pv the vector point to use
    void setpv (const Vector& pv);

    /// add an integer plane point to the polyline
    /// @param p the point to add
    void addipp (const Ippd& p);

    /// add a point to the polyline
    /// @param x the x point to add
    /// @param y the y point to add
    void addipp (const long x, const long y);

  private:
    // make the copy constructor private
    SvgPolyline (const SvgPolyline&);
    // make the assignment operator private
    SvgPolyline& operator = (const SvgPolyline&);

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
