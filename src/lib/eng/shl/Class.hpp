// ---------------------------------------------------------------------------
// - Class.hpp                                                               -
// - afnix engine - class class definition                                   -
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

#ifndef  AFNIX_CLASS_HPP
#define  AFNIX_CLASS_HPP

#ifndef  AFNIX_LOCALSET_HPP
#include "Localset.hpp"
#endif

namespace afnix {

  /// The Class class is the class used to model afnix class object. The afnix
  /// model is based on a single inheritance mechanism. A class binds only
  /// a local set - which somehow is equivalent to c++ static data member.
  /// When an instance of that class is created, it is the instance initial
  /// method which is responsible to define the local data members.
  /// @author amaury darsch

  class Class : public virtual Object {
  private:
    /// the inference class
    Class* p_infer;
    /// the inference const flag
    bool   d_ctinf;
    /// the deferent class
    Class* p_defer;
    /// the deferent const flag
    bool   d_ctdef;
    /// the class local set
    Localset* p_cset;

  public:
    /// create a new class 
    Class (void);

    /// destroy this class class
    ~Class (void);

    /// @return the class name
    String repr (void) const;

    /// @return an object in the class locally
    Object* get (const long quark) const;

    /// set the class inferent object
    /// @param object the class object to set
    /// @param flag   the class const flag
    Object* setinfer (Object* object, const bool flag);

    /// set the class inferent class
    /// @param infer the class to set
    /// @param flag  the class const flag
    Object* setinfer (Class* infer, const bool flag);

    /// @return the class inferent object
    Object* getinfer (void) const;
    
    /// unset the class inferent class
    Object* uninfer (void);

    /// set the class deferent object
    /// @param object the class object to set
    /// @param flag   the class const flag
    Object* setdefer (Object* object, const bool flag);

    /// set the class deferent class
    /// @param defer the class to set
    /// @param flag  the class const flag
    Object* setdefer (Class* defer, const bool flag);

    /// @return the class deferent object
    Object* getdefer (void) const;
    
    /// unset the class deferent class
    Object* undefer (void);

  private:
    // make the copy constructor private
    Class (const Class&);
    // make the assignment operator private
    Class& operator = (const Class&);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// operate this class with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object);

    /// set a constant object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to define as const
    /// @param object the object to set
    Object* cdef (Evaluable* zobj, Nameset* nset, const long quark, 
		  Object* object);

    /// set an object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to define as trans
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, const long quark, 
		  Object* object);

    /// unreference an object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to unreference
    Object* udef (Evaluable* zobj, Nameset* nset, const long quark); 

    /// evaluate an object member by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the member quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);

    /// apply this object by arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset 
    /// @param args the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, Cons* args);

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param args  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Cons* args);
  };
}

#endif
