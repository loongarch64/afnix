// ---------------------------------------------------------------------------
// - XmlTexter.hpp                                                           -
// - afnix:xml module - xml text reader class definition                     -
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

#ifndef  AFNIX_XMLTEXTER_HPP
#define  AFNIX_XMLTEXTER_HPP

#ifndef  AFNIX_XMLROOT_HPP
#include "XmlRoot.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The XmlTexter class is a simple xml reader that convert an input
  /// stream into a text node. The xml texter builds a valid xml root
  /// representation by creating a declaration node followed by a text 
  /// node. As a consequence, the root node has only two children.
  /// @author amaury darsch

  class XmlTexter : public Object {
  protected:
    /// the xml version
    String d_xvid;
    /// the root node
    XmlRoot* p_root;

  public:
    /// create an empty texter
    XmlTexter (void);

    /// create a texter with a version
    /// @param xvid the version to use
    XmlTexter (const String& xvid);

    /// destroy this texter
    ~XmlTexter (void);

    /// @return the class name
    String repr (void) const;

    /// reset this texter
    void reset (void);

    /// @return the root node
    virtual XmlRoot* getroot (void) const;

    /// parse an input stream
    /// @param is the input steam to read
    virtual void parse (InputStream* is);

    /// parse a string
    /// @param value the string to parse
    virtual void parse (const String& value);

  private:
    // make the copy constructor private
    XmlTexter (const XmlTexter&);
    // make the assignment operator private
    XmlTexter& operator = (const XmlTexter&);

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
