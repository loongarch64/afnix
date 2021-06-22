// ---------------------------------------------------------------------------
// - XmlProcessor.hpp                                                        -
// - afnix:xpe module - xml processor class definition                       -
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

#ifndef  AFNIX_XMLPROCESSOR_HPP
#define  AFNIX_XMLPROCESSOR_HPP

#ifndef  AFNIX_XMLFEATURE_HPP
#include "XmlFeature.hpp"
#endif

namespace afnix {

  /// The XmlProcessor class is a global class designed to operate on 
  /// a xml content. The xml processor provides several features that 
  /// can be enabled prior the document processor. Once the features are 
  /// defined, the 'process' method can be called and a new xml
  /// content can be produced.
  /// @author amaury darsch

  class XmlProcessor : public Object {
  private:
    /// the vector of features
    Vector d_xftr;
    /// the maximum level
    long   d_mlvl;

  public:
    /// create a default xml processor
    XmlProcessor (void);

    /// @return the class name
    String repr (void) const;

    /// @return the number of features
    long lenxftr (void) const;

    /// @return a feature by index
    XmlFeature* getxftr (const long index) const;

    /// add a new feature to the processor
    /// @param xftr the xml feature to add
    void addxftr (XmlFeature* xftr);

    /// process a xml content
    /// @param xcnt the content to process
    XmlContent* process (const XmlContent* xcnt) const;

  private:
    // make the copy constructor private
    XmlProcessor (const XmlProcessor&);
    // make the assignment operator private
    XmlProcessor& operator = (const XmlProcessor&);

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
