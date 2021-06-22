// ---------------------------------------------------------------------------
// - XmlFeature.hpp                                                          -
// - afnix:xpe module - xml processor feature class definition               -
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

#ifndef  AFNIX_XMLFEATURE_HPP
#define  AFNIX_XMLFEATURE_HPP

#ifndef  AFNIX_XMLCONTENT_HPP
#include "XmlContent.hpp"
#endif

namespace afnix {

  /// The XmlFeature class is a xml procesor base class that defines a
  /// processing feature. A processing feature is defined by name and
  /// information with a processing level. The default processing level
  /// is null. When the processor is called, it calls sequentially and in
  /// ascending order all features.
  /// @author amaury darsch

  class XmlFeature : public Nameable {
  protected:
    /// the feature name
    String d_name;
    /// the feature info
    String d_info;
    /// the processing level
    long   d_plvl;

  public:
    /// create a feature by name
    /// @param name the feature name
    XmlFeature (const String& name);

    /// create a feature by name and info
    /// @param name the feature name
    /// @param info the feature info
    XmlFeature (const String& name, const String& info);

    /// create a feature by name, info and level
    /// @param name the feature name
    /// @param info the feature info
    /// @param plvl the feature level
    XmlFeature (const String& name, const String& info, const long plvl);

    /// @return the feature name
    String getname (void) const;

    /// @return the feature info
    virtual String getinfo (void) const;

    /// @return true if the feature is at the processing level
    virtual bool isplvl (const long plvl) const;

    /// set the feature processing level
    /// @param plvl the processing level to set
    virtual void setplvl (const long plvl);

    /// @return the feature processing level
    virtual long getplvl (void) const;

    /// process a xml content
    /// @param xcnt the content to process
    virtual XmlContent* process (const XmlContent* xcnt) =0;

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
