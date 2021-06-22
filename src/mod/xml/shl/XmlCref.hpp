// ---------------------------------------------------------------------------
// - XmlCref.hpp                                                             -
// - afnix:xml module - xml character reference node class definition        -
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

#ifndef  AFNIX_XMLCREF_HPP
#define  AFNIX_XMLCREF_HPP

#ifndef  AFNIX_XMLREF_HPP
#include "XmlRef.hpp"
#endif

namespace afnix {

  /// The XmlCref class is the character reference node. The character value
  /// is stored inside the object at construction.
  /// @author amaury darsch

  class XmlCref : public XmlRef {
  protected:
    /// the character value
    t_quad d_cval;

  public:
    /// create a default cref node
    XmlCref (void);

    /// create a cref node with a value
    /// @param cval the cref value
    XmlCref (const t_quad cval);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// set the cref node value
    /// @param cval the cref value to set
    void setcval (const t_quad cval);

    /// @return the cref node value
    t_quad getcval (void) const;

    /// @return the eref node text value
    String totext (void) const;

    /// write a cref node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a cref node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlCref (const XmlCref&);
    // make the assignment operator private
    XmlCref& operator = (const XmlCref&);

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
