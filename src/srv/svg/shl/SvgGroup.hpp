// ---------------------------------------------------------------------------
// - SvgGroup.hpp                                                            -
// - afnix:svg service - svg group node class definition                     -
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

#ifndef  AFNIX_SVGGROUP_HPP
#define  AFNIX_SVGGROUP_HPP

#ifndef  AFNIX_SVGSTYLING_HPP
#include "SvgStyling.hpp"
#endif

namespace afnix {

  /// The SvgGroup class is a grouping class for the svg subsystem. Most
  /// of the time, svg objects are added into a group with a transformation
  /// attribute and some global styling attributes.
  /// @author amaury darsch

  class SvgGroup : public SvgStyling {
  public:
    /// create an empty group
    SvgGroup (void);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    SvgGroup (const SvgGroup&);
    // make the assignment operator private
    SvgGroup& operator = (const SvgGroup&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
