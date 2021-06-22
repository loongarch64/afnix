// ---------------------------------------------------------------------------
// - Closure.hpp                                                             -
// - afnix engine - closure class definition                                 -
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

#ifndef  AFNIX_CLOSURE_HPP
#define  AFNIX_CLOSURE_HPP

#ifndef  AFNIX_ARGSLIST_HPP
#include "ArgsList.hpp"
#endif

#ifndef  AFNIX_LOCALSET_HPP
#include "Localset.hpp"
#endif

namespace afnix {

  /// The Closure class is the class used to model lambda and gamma 
  /// expressions. When the closure is created, a local set of arguments and
  /// closed variables is generated. Note that such local set is dynamic.
  /// There is no restriction to reconfigure a particular lambda at run-time.
  /// The difference between a lambda and a gamma expression resides in the
  /// nameset binding when the closure is called. With a lambda, the closure
  /// nameset parent is the calling nameset. With a gamma expression, the
  /// parent nameset is always the top-level interpreter nameset. Note also,
  /// that the symbol "self" is automatically binded for this closure.
  /// @author amaury darsch

  class Closure : public Object {
  private:
    /// the lambda flag
    bool d_lflg;
    /// the runtime form
    Object* p_form;
    /// the closure nameset
    Localset* p_cset;
    /// the argument list
    ArgsList d_argl;

  public:
    /// create a new default closure 
    /// @param type the closure type
    /// @param argl the argument list
    /// @param form the execution object 
    Closure (void);

    /// create a new closure by type
    /// @param type the closure type
    Closure (const bool type);

    /// create a new closure  by type, argument list and form
    /// @param type the closure type
    /// @param argl the argument list
    /// @param form the execution object 
    Closure (const bool type, Cons* argl, Object* form);

    /// destroy this closure
    ~Closure (void);

    /// @return the class name
    String repr (void) const;

    /// add a new argument to this closure
    /// @param quark the argument quark
    /// @param cflag the constant flag
    void addarg (const long quark, const bool cflag);

    /// add a new argument to this closure
    /// @param object the argument object
    void addarg (Object* object);

    /// add a closed variable to this closure
    /// @param quark the closed variable quark
    /// @param cflag the constant flag
    /// @param object the object to bind
    void addclv (const long quark, const bool cflag, Object* object);

    /// set the closure form object
    /// @param form the form object to set
    void setform (Object* form);

    /// @return true if the closure is a lambda expression
    bool islambda (void) const;

  private:
    // make the copy constructor private
    Closure (const Closure&);
    // make the assignment operator private
    Closure& operator = (const Closure&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// set an object as a const object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to bind this constant
    /// @param object the object to set
    Object* cdef (Evaluable* zobj, Nameset* nset, const long quark, 
		  Object* object);

    /// set an object to this object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to bind this object
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, const long quark, 
		  Object* object);

    /// unreference an object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to unreference
    Object* udef (Evaluable* zobj, Nameset* nset, const long quark); 

    /// evaluate an object in this object
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    /// @return the evaluated object
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);

    /// apply this object with a set of arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply with
    Object* apply (Evaluable* zobj, Nameset* nset, Cons* args);

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
