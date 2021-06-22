// ---------------------------------------------------------------------------
// - SvgForming.hpp                                                          -
// - afnix:svg service - svg forming attribute node class definition         -
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

#ifndef  AFNIX_SVGFORMING_HPP
#define  AFNIX_SVGFORMING_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

#ifndef  AFNIX_TRANSFORM_HPP
#include "Transform.hpp"
#endif

namespace afnix {

  /// The SvgForming class is a base class for the svg node that holds the
  /// forming attributes. The transformation attribute is the principal
  /// forming attribute defined by this class.
  /// @author amaury darsch

  class SvgForming : public XmlTag {
  public:
    /// create a svg node by name
    /// @param name the node name
    SvgForming (const String& name);

    /// set the transformation attribute
    /// @param ts the transformation string
    void settrf (const String& ts);

    /// set the transformation attribute
    /// @param to the transformation string
    void settrf (const Transform& to);

  private:
    // make the copy constructor private
    SvgForming (const SvgForming&);
    // make the assignment operator private
    SvgForming& operator = (const SvgForming&);

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
