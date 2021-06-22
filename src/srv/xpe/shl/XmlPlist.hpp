// ---------------------------------------------------------------------------
// - XmlPlist.hpp                                                            -
// - afnix:xpe module - xml plist class definition                           -
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

#ifndef  AFNIX_XMLPLIST_HPP
#define  AFNIX_XMLPLIST_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_XMLDOCUMENT_HPP
#include "XmlDocument.hpp"
#endif

namespace afnix {

  /// The XmlPlist class is a xml document class designed to hold a property 
  /// list in the form of a xml tree. Normally the xml tree is created with the
  /// help of a property list object.
  /// @author amaury darsch

  class XmlPlist : public XmlDocument {
  private:
    /// the plist tag name
    String   d_pltn;
    /// the property tag name
    String   d_potn;
    /// the property name attribute
    String   d_natt;
    /// the property value attribute
    String   d_vatt;

  public:
    /// create a default xml plist
    XmlPlist (void);

    /// create a xml plist by plist
    /// @param plst the property list
    XmlPlist (const Plist& plst);

    /// create a xml plist by name and plist
    /// @param name the xml plist name
    /// @param plst the property list
    XmlPlist (const String& name, const Plist& plst);

    /// copy construct this xml plist
    /// @param that the xml plist to copy
    XmlPlist (const XmlPlist& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// set the plist tag name
    /// @param pltn the tag name to set
    void setpltn (const String& pltn);

    /// @return the plist tag name
    String getpltn (void) const;

    /// set the property tag name
    /// @param potn the tag name to set
    void setpotn (const String& potn);

    /// @return the property tag name
    String getpotn (void) const;

    /// set the property name attribute
    /// @param natt the attribute name to set
    void setnatt (const String& natt);

    /// @return the property name attribute
    String getnatt (void) const;

    /// set the property value attribute
    /// @param vatt the attribute value to set
    void setvatt (const String& vatt);

    /// @return the property value attribute
    String getvatt (void) const;

    /// set the xml plist by plist
    /// @param plst the plist to set
    void setplist (const Plist& plst);

  private:
    // make the assignment operator private
    XmlPlist& operator = (const XmlPlist&);

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
