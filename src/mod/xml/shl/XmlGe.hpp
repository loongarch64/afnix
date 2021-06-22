// ---------------------------------------------------------------------------
// - XmlGe.hpp                                                               -
// - afnix:xml module - xml general entity node class definition             -
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

#ifndef  AFNIX_XMLGE_HPP
#define  AFNIX_XMLGE_HPP

#ifndef  AFNIX_XMLENTITY_HPP
#include "XmlEntity.hpp"
#endif

namespace afnix {

  /// The XmlGe class is the xml general entity node node. In its simplest 
  /// form, the general entity has a name and a value. The entity type can
  /// also be associated with a system or a public identifier with or without
  /// an extra type name.
  /// @author amaury darsch

  class XmlGe : public XmlEntity {
  protected:
    /// the entity value
    String d_xval;
    /// the public literal
    String d_publ;
    /// the system literal
    String d_sysl;
    /// the ndata value
    String d_data;

  public:
    /// create a general entity by name and value
    /// @param name the entity name
    /// @param xval the entity value
    XmlGe (const String& name, const String& xval);

    /// create a general entity by name and public/system literals
    /// @param name the entity name
    /// @param publ the public literal
    /// @param sysl the system literal
    XmlGe (const String& name, const String& publ, const String& sysl);

    /// create a general entity by name, public/system literals and data
    /// @param name the entity name
    /// @param publ the public literal
    /// @param sysl the system literal
    /// @param data the entity ndata
    XmlGe (const String& name, const String& publ, const String& sysl, 
	   const String& data);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// @return the entity value 
    virtual String getxval (void) const;

    /// @return the node public literal
    virtual String getpubl (void) const;

    /// @return the node system literal
    virtual String getsysl (void) const;

    /// @return the entity data value
    virtual String getdata (void) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a node into an output steram
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlGe (const XmlGe&);
    // make the assignment operator private
    XmlGe& operator = (const XmlGe&);

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
