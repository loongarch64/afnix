// ---------------------------------------------------------------------------
// - SvgLine.hpp                                                             -
// - afnix:svg service - svg line node class definition                      -
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

#ifndef  AFNIX_SVGLINE_HPP
#define  AFNIX_SVGLINE_HPP

#ifndef  AFNIX_IPPD_HPP
#include "Ippd.hpp"
#endif

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgLine class is a basic shape svg node for line object.
  /// The line is defined by two positions. Optional attribute include 
  /// standard styling although here the fill attribute cannot be used.
  /// @author amaury darsch

  class SvgLine : public SvgStyling {
  public:
    /// create a line by point
    /// @param p1 the line point 1
    /// @param p2 the line point 2
    SvgLine (const Ippd& p1, const Ippd& p2);

    /// create a line by coordinates
    /// @param x1 the line x point 1
    /// @param y1 the line y point 1
    /// @param x2 the line x point 2
    /// @param y2 the line y point 2
    SvgLine (const long x1, const long y1, const long x2, const long y2);

    /// @return the class name
    String repr (void) const;

    /// set the line first point
    /// @param p the point to set
    void setp1 (const Ippd& p);

    /// set the line first point
    /// @param x the point x position
    /// @param y the point x position
    void setp1 (const long x, const long y);

    /// set the line second point
    /// @param p the point to set
    void setp2 (const Ippd& p);

    /// set the line second point
    /// @param x the point x position
    /// @param y the point x position
    void setp2 (const long x, const long y);

  private:
    // make the copy constructor private
    SvgLine (const SvgLine&);
    // make the assignment operator private
    SvgLine& operator = (const SvgLine&);

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
