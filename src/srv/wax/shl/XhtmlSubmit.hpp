// ---------------------------------------------------------------------------
// - XhtmlSubmit.hpp                                                         -
// - afnix:wax service - xhtml input submit node class definition            -
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

#ifndef  AFNIX_XHTMLSUBMIT_HPP
#define  AFNIX_XHTMLSUBMIT_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlSubmit class is a xhtml input submit node used in the html form
  /// node. The submit input is a button with a value. An optional button size
  /// is also available for that input submit.
  /// @author amaury darsch

  class XhtmlSubmit : public XhtmlBase {
  public:
    /// create a xhtml submit by value
    /// @param sval the submit value
    XhtmlSubmit (const String& sval);

    /// create a xhtml submit by value and size
    /// @param sval the submit value
    /// @param size the submit size
    XhtmlSubmit (const String& sval, const String& size);

    /// @return the class name
    String repr (void) const;

    /// set the submit size
    /// @param size the size attribute to set
    void setsize (const String& size);

  private:
    // make the copy constructor private
    XhtmlSubmit (const XhtmlSubmit&);
    // make the assignment operator private
    XhtmlSubmit& operator = (const XhtmlSubmit&);

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
