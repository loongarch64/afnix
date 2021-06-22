// ---------------------------------------------------------------------------
// - XmlEntity.hpp                                                           -
// - afnix:xml module - xml entity node class definition                     -
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

#ifndef  AFNIX_XMLENTITY_HPP
#define  AFNIX_XMLENTITY_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlEntity class is the base class for the xml entity representation.
  /// A xml entity can be either a general entity or a parameter entity. They
  /// differ initialy with the preence of the '%' character. Both entity model
  /// have a name whihc is path of the base class.
  /// @author amaury darsch

  class XmlEntity : public XmlNode {
  protected:
    /// the entity name
    String d_name;

  public:
    /// create an xml entity node by name
    /// @param name the entity name 
    XmlEntity (const String& name);

    /// @return the class name
    String repr (void) const;

    /// @return true if a node name is valid
    bool isname (const String& name) const;

    /// set the entity node name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// @return the entity name
    virtual String getname (void) const;

  private:
    // make the copy constructor private
    XmlEntity (const XmlEntity&);
    // make the assignment operator private
    XmlEntity& operator = (const XmlEntity&);

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
