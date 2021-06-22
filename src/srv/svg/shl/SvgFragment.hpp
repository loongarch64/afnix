// ---------------------------------------------------------------------------
// - SvgFragment.hpp                                                         -
// - afnix:svg service - svg fragment node class definition                  -
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

#ifndef  AFNIX_SVGFRAGMENT_HPP
#define  AFNIX_SVGFRAGMENT_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The SvgFragment class is the primary svg node that acts as a node 
  /// fragment for a complete svg document. Normally, the node is used to 
  /// bind the svg xml namespace as well as some essential attributes like
  /// the width, height or view box. An optional title and description tag 
  /// can be added. 
  /// @author amaury darsch

  class SvgFragment : public XmlTag {
  public:
    /// create a default svg fragment
    SvgFragment (void);

    /// create a svg fragment by width and height
    /// @param width  the node and box width
    /// @param height the node and box height
    SvgFragment (const long width, const long height);

    /// @return the class name
    String repr (void) const;

    /// set the svg embedded x position
    /// @param x the embedded x position
    virtual void setx (const long x);

    /// set the svg embedded y position
    /// @param y the embedded y position
    virtual void sety (const long y);

    /// set the svg width attribute
    /// @param width the width to set
    virtual void setwth (const long width);

    /// set the svg width attribute
    /// @param wth the width string to set
    virtual void setwth (const String& width);

    /// set the svg height attribute
    /// @param height the width to set
    virtual void sethgt (const long height);

    /// set the svg height attribute
    /// @param hgt the height string to set
    virtual void sethgt (const String& hgt);

    /// set the svg viewbox attribute
    /// @param tlx the top left x coordinate
    /// @param tly the top left y coordinate
    /// @param brx the bottom right x coordinate
    /// @param bry the bottom right y coordinate
    virtual void setvbx (const long tlx, const long tly, 
			 const long brx, const long bry);

  private:
    // make the copy constructor private
    SvgFragment (const SvgFragment&);
    // make the assignment operator private
    SvgFragment& operator = (const SvgFragment&);

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
