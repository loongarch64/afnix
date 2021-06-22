// ---------------------------------------------------------------------------
// - XmlEref.hpp                                                             -
// - afnix:xml module - xml entity reference node class definition           -
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

#ifndef  AFNIX_XMLEREF_HPP
#define  AFNIX_XMLEREF_HPP

#ifndef  AFNIX_XMLREF_HPP
#include "XmlRef.hpp"
#endif

namespace afnix {

  /// The XmlEref class is the entity reference node. The node stores the
  /// reference value.
  /// @author amaury darsch

  class XmlEref : public XmlRef {
  public:
    /// create a default eref node
    XmlEref (void);

    /// create a eref node with a value
    /// @param xref the eref value
    XmlEref (const String& xref);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the eref node text value
    String totext (void) const;

    /// write a eref node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a eref node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlEref (const XmlEref&);
    // make the assignment operator private
    XmlEref& operator = (const XmlEref&);

  public:
    /// create an object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
