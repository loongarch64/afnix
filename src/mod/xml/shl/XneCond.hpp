// ---------------------------------------------------------------------------
// - XneCond.hpp                                                             -
// - afnix:xml module - xne condition class definition                       -
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

#ifndef  AFNIX_XNECOND_HPP
#define  AFNIX_XNECOND_HPP

#ifndef  AFNIX_XNE_HPP
#include "Xne.hpp"
#endif

namespace afnix {

  /// The XneCond is xne condition class. The sole purpose of this class
  /// is to define one or several condition that a node must satisfy in
  /// order to be elected. The condition are accumulated in a list and
  /// later checked for a particular node. Note that an empty condition
  /// always succeds.
  /// @author amaury darsch

  class XneCond : public Object {
  private:
    /// the condition list
    struct t_cond* p_cond;

  public:
    /// create an empty condition
    XneCond (void);

    /// destroy this condition
    ~XneCond (void);

    /// @return the class name
    String repr (void) const;

    /// reset this condition
    void reset (void);

    /// @return true if the node satifies this condition
    bool valid (const XmlNode* node) const;

    /// add a condition by type
    /// @param type the condition type
    void add (const Xne::t_xsel type);

    /// add a condition by type and name
    /// @param type the condition type
    /// @param name the condition name
    void add (const Xne::t_xsel type, const String& name);

    /// add a condition by type and index
    /// @param type the condition type
    /// @param indx the condition index
    void add (const Xne::t_xsel type, const long indx);

  private:
    // make the copy constructor private
    XneCond (const XneCond&);
    // make the assignment operator private
    XneCond& operator = (const XneCond&);

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
