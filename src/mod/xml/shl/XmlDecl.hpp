// ---------------------------------------------------------------------------
// - XmlDecl.hpp                                                             -
// - afnix:xml module - xml declaration node class definition                -
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

#ifndef  AFNIX_XMLDECL_HPP
#define  AFNIX_XMLDECL_HPP

#ifndef  AFNIX_XMLPI_HPP
#include "XmlPi.hpp"
#endif

namespace afnix {

  /// The XmlDecl class is the xml declaration node. It is derived from
  /// the xml processing instruction (pi) node. A declaration node is
  /// defined with a version id, an encoding string and a standalone
  /// flag.
  /// @author amaury darsch

  class XmlDecl : public XmlPi {
  private:
    /// the version id
    String d_xvid;
    /// the encoding mode
    String d_emod;
    /// the standalong flag
    bool   d_stnd;

  public:
    /// create a default xml declaration
    XmlDecl (void);
    
    /// create a xml declaration node
    /// @param xvid the xml version
    XmlDecl (const String& xvid);

    /// create a xml declaration node
    /// @param xvid the xml version
    /// @param emod the encoding mode
    XmlDecl (const String& xvid, const String& emod);

    /// create a xml declaration node
    /// @param xvid the xml version
    /// @param emod the encoding mode
    /// @param stnd the standalone value
    XmlDecl (const String& xvid, const String& emod, const String& stnd);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return the xml version
    String getxvid (void) const;

    /// @return the xml encoding
    String getemod (void) const;

    /// @return the standalone flag
    bool getstnd (void) const;

  private:
    // make the copy constructor private
    XmlDecl (const XmlDecl&);
    // make the assignment operator private
    XmlDecl& operator = (const XmlDecl&);

  public:
    /// create an object in a generic way
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
