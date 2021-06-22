// ---------------------------------------------------------------------------
// - Combo.hpp                                                               -
// - standard object library - combo object class definition                 -
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

#ifndef  AFNIX_COMBO_HPP
#define  AFNIX_COMBO_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Combo class is a generic class which holds an object combination.
  /// The combination can be either quark based or object based. A combo 
  /// object is built with a reference object associated with a combo value
  /// which can be either a quark or another object. In the quark mode, the
  /// combo value can be evaluated by quark with respect to the reference 
  /// object. In the object mode, the combo value can be applied by object 
  /// with respect to the reference object. An optional apply flag controls
  /// whether or not the apply method should be called generically.
  /// @author amaury darsch

  class Combo : public virtual Object {
  private:
    enum {QUARK, OBJECT} d_type;
    /// the reference object
    Object* p_zobj;
    /// the combo value
    union {
      long    qval;
      Object* oval;
    } d_cval;
    /// the apply flag
    bool d_aflg;

  public:
    /// create a new combo with a quark
    /// @param quark the combo quark
    /// @param czobj the combo reference object
    Combo (const long quark, Object* czobj);

    /// create a new combo with a combo object
    /// @param coval the combo object value
    /// @param czobj the combo reference object
    Combo (Object* coval, Object* czobj);

    /// create a new combo with a quark and an apply flag
    /// @param quark the combo quark
    /// @param czobj the combo reference object
    /// @param aflag the apply flag to set
    Combo (const long quark, Object* czobj, const bool aflag);

    /// create a new combo with a combo object and an apply flag
    /// @param coval the combo object value
    /// @param czobj the combo reference object
    /// @param aflag the apply flag to set
    Combo (Object* coval, Object* czobj, const bool aflag);

    /// destroy this combo oject
    ~Combo (void);

    /// @return this class name
    String repr (void) const override;

    /// @return true if a combo is looping
    virtual bool isequal (Object* obj) const;

    /// @return the apply flag
    virtual bool getaflg (void) const;

    /// set the apply flag
    /// @param aflg the apply flag to set
    virtual void setaflg (const bool aflg);

  private:
    // make the copy constructor private
    Combo (const Combo&) =delete;
    // make the assignment operator private
    Combo& operator = (const Combo&) =delete;

  public:
    /// set an object as a const object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to bind this constant
    /// @param object the object to set
    Object* cdef (Evaluable* zobj, Nameset* nset, const long quark,
                  Object* object) override;
    
    /// set an object to this object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to bind this object
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, const long quark,
                  Object* object) override;
    
    /// unreference an object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to unref
    Object* udef (Evaluable* zobj, Nameset* nset, const long quark) override;

    /// evaluate an object member by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    /// @return the evaluated object
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark) override;

    /// apply this object with a set of arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, Cons* args) override;
  };
}

#endif
