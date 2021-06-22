// ---------------------------------------------------------------------------
// - XmlInclude.hpp                                                          -
// - afnix:xpe module - xml include feature class definition                 -
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

#ifndef  AFNIX_XMLINCLUDE_HPP
#define  AFNIX_XMLINCLUDE_HPP

#ifndef  AFNIX_XMLFEATURE_HPP
#include "XmlFeature.hpp"
#endif

namespace afnix {

  /// The XmlInclude class is a xml processor feature class designed to
  /// handle the "XInclude" schema that permits to include xml document.
  /// The feature operates recursively by scanning the document for
  /// a "xi:include" tag and replacing the content by the appropriate tree.
  /// The feature operates recursively unless specified otherwise.
  /// @author amaury darsch

  class XmlInclude : public XmlFeature {
  public:
    /// create an xml include feature
    XmlInclude (void);

    /// create an xml include feature with a processing level
    /// @param plvl the processing level to set
    XmlInclude (const long plvl);

    /// @return the class name
    String repr (void) const;

    /// set the recursive flag
    /// @param rflg the flag to set
    void setrflg (const bool rflg);

    /// @return te recursive flag
    bool getrflg (void) const;

    /// process a xml content
    /// @param xcnt the content to process
    XmlContent* process (const XmlContent* xcnt);

  private:
    // make the copy constructor private
    XmlInclude (const XmlInclude&);
    // make the assignment operator private
    XmlInclude& operator = (const XmlInclude&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
