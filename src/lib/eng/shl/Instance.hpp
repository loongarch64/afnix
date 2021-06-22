// ---------------------------------------------------------------------------
// - Instance.hpp                                                            -
// - afnix engine - afnix instance class definition                          -
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

#ifndef  AFNIX_INSTANCE_HPP
#define  AFNIX_INSTANCE_HPP

#ifndef  AFNIX_CLASS_HPP
#include "Class.hpp"
#endif

namespace afnix {

  /// The Instance class is used as an instance of the Class class. When an
  /// instance is created, all data members are bounded in the instance local
  /// nameset. By default, all data members are set to nil. If a constructor
  /// is invoked with arguments, the data members are set with them.
  /// @author amaury darsch

  class Instance : public virtual Object {
  private:
    /// the meta class
    Class*  p_meta;
    /// the meta flag
    bool    d_ctmta;
    /// the super instance
    Object* p_super;
    /// the super flag
    bool    d_ctsup;
    /// the instance localset
    Localset* p_iset;

  public:
    /// create a default instance
    Instance (void);

    /// create an instance with a meta class
    /// @param meta the meta class to bind
    Instance (Class* meta);

    /// destroy this instance
    ~Instance (void);

    /// @return the class name
    String repr (void) const;

    /// reset this instance
    virtual void reset (void);

    /// set the instance meta class object
    /// @param object the meta object to set
    /// @param flag   the meta const flag
    virtual Object* setmeta (Object* object, const bool flag);

    /// unset the instance meta class object
    virtual Object* unmeta (void);

    /// set the super instance value
    /// @param object the super object to set
    /// @param flag   the super const flag
    virtual Object* setsuper (Object* object, const bool flag);

    /// unset the super instance value
    virtual Object* unsuper (void);

  private:
    // make the copy constructor private
    Instance (const Instance&);
    // make the assignment operator private
    Instance& operator = (const Instance&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// preset the instance by arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply
    Object* pdef (Evaluable* zobj, Nameset* nset, Cons* args);
    
    /// mute the instance by arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply
    Object* mute (Evaluable* zobj, Nameset* nset, Cons* args);

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
    /// @param quark  the quark to define as const
    Object* udef (Evaluable* zobj, Nameset* nset, const long quark);

    /// evaluate an object member by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);

    /// apply an object method with a set of arguments by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the name to apply these arguments
    /// @param args  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark, 
		   Cons* args);

    /// apply an object method by object with a set of arguments
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param object the object to apply with the arguments
    /// @param args   the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, Object* object, Cons* args);
  };
}

#endif
