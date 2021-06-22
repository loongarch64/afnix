// ---------------------------------------------------------------------------
// - SvgRoot.hpp                                                             -
// - afnix:svg service - svg root node class definition                    -
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

#ifndef  AFNIX_SVGROOT_HPP
#define  AFNIX_SVGROOT_HPP

#ifndef  AFNIX_MIME_HPP
#include "Mime.hpp"
#endif

#ifndef  AFNIX_XMLROOT_HPP
#include "XmlRoot.hpp"
#endif

#ifndef  AFNIX_SVGFRAGMENT_HPP
#include "SvgFragment.hpp"
#endif

namespace afnix {

  /// The SvgRoot class is a xml root node used for the design of a svg
  /// document page. At construction, the root node is initialized with a 
  /// default xml processing instruction, and optionnaly a svg node with a
  /// specific width and height.
  /// @author amaury darsch

  class SvgRoot : public XmlRoot, public Mime {
  public:
    /// create a svg root node
    SvgRoot (void);

    /// create a svg root node by fragment
    /// @param frag the svg fragment
    SvgRoot (SvgFragment* frag);

    /// create a svg root node by width and height
    /// @param width  the node and box width
    /// @param height the node and box height
    SvgRoot (const long width, const long height);

    /// @return the class name
    String repr (void) const;

    /// @return the svg node
    SvgFragment* getsvg (void) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;
    
    /// write a node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    SvgRoot (const SvgRoot&);
    // make the assignment operator private
    SvgRoot& operator = (const SvgRoot&);

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
