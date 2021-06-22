// ---------------------------------------------------------------------------
// - Object.hpp                                                              -
// - standard object library - base object declaration                       -
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

#ifndef  AFNIX_OBJECT_HPP
#define  AFNIX_OBJECT_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {

  /// The Object class is the foundation of the standard object library .
  /// The object class is bound to a control structure that manages a
  /// reference count. The reference count is used to control the life of 
  /// a particular object. When an object is created, the reference count 
  /// is set to 0. Such object is said to be transient. The "iref" static
  /// method increments the reference count. The "dref" method decrements
  /// and eventually destroy the object. The "cref" method eventually destroy
  /// an object if its reference count is null. The object class is an 
  /// abstract class. For each derived object, the "repr" method
  /// is defined to return the class name. Additionally, the object class
  /// defines a set of methods which are used by the evaluable to virtually
  /// modify or evaluate an object. There are two sets of methods. The first
  /// set operates directly on the object. The second set operates by name
  /// on the object. Working by name is equivalent to access a member of a
  /// a particular object. The "cdef" method creates or set a constant object
  /// to the calling object. The "vdef" method create or set an object to the
  /// calling object. The "eval" method evaluates an object in the current
  /// evaluable nameset. The "apply" method evaluates a set of arguments
  /// and apply them to the calling object. It is somehow equivalent to a 
  /// function call. When called by name, it is equivalent to a method call.
  /// @author amaury darsch

  class Object {
  public:
    /// the operator enumeration
    enum t_oper {
      OPER_ADD, // add 
      OPER_SUB, // subtract
      OPER_MUL, // multiply
      OPER_DIV, // divide
      OPER_UMN, // unary minus
      OPER_EQL, // equal
      OPER_QEQ, // equal compare
      OPER_NEQ, // not equal
      OPER_GEQ, // greater equal
      OPER_LEQ, // less equal
      OPER_GTH, // greater than
      OPER_LTH  // less than
    };

    /// increment the object reference count
    /// @param object the object to process
    static Object* iref (Object* object);

    /// decrement the reference count and destroy the object if null
    /// @param object the object to process
    static void dref (Object* object);

    /// clean this object if the reference count is null
    /// @param object the object to process
    static void cref (Object* object);

    /// decrement the object reference count but do not detroy if null
    /// @param object the object to process
    static void tref (Object* object);

    /// return true if the object has a reference count of 0 or 1
    /// @param object the object to process
    static bool uref (Object* object);

  protected:
    /// the object access control
    struct s_oacs* p_oacs;
    /// the locking control
    class Lockrw* p_lock;
    
  public:
    /// create a new object
    Object (void);

    /// copy move this object
    /// @param that the object to move
    Object (Object&& that) noexcept;

    /// destroy this object.
    virtual ~Object (void);

    /// move an object into this one
    /// @param that the object to move
    Object& operator = (Object&& that) noexcept;
    
    /// @return the class name
    virtual class String repr (void) const =0;

    /// @return an object class name or nil
    static const class String repr (Object* object);
      
    /// @return a clone of this object
    virtual Object* clone (void) const;

    /// get a read lock for this object
    virtual void rdlock (void) const;

    /// get a write lock for this object
    virtual void wrlock (void) const;

    /// get an adaptative read lock for this object
    virtual void arlock (void) const;

    /// unlock this object
    virtual void unlock (void) const;

    /// reduce this object
    virtual Object* reduce (void) const;

  private:
    // disable copy constructor
    Object (const Object&) =delete;
    // disable assignment operator
    Object& operator =(const Object&) =delete;
    
  public:
    /// @return true if the given quark is defined
    virtual bool isquark (const long quark, const bool hflg) const;
    
    /// operate this object with another one
    /// @param type   the operator type
    /// @param object the operand object
    virtual Object* oper (t_oper type, Object* object);

    /// set an object as a const object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    virtual Object* cdef (class Evaluable* zobj, class Nameset* nset, 
			  Object* object);

    /// set an object as a const object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to define as const
    /// @param object the object to set
    virtual Object* cdef (class Evaluable* zobj, class Nameset* nset, 
			  const long quark, Object* object);

    /// set an object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    virtual Object* vdef (class Evaluable* zobj, class Nameset* nset, 
			  Object* object);

    /// set an object to this object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to set this object
    /// @param object the object to set
    virtual Object* vdef (class Evaluable* zobj, class Nameset* nset, 
			  const long quark, Object* object);

    /// unreference an object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    virtual Object* udef (class Evaluable* zobj, class Nameset* nset);

    /// unreference an object by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to unref
    virtual Object* udef (class Evaluable* zobj, class Nameset* nset, 
			  const long quark);

    /// evaluate an object in the current nameset
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    virtual Object* eval (class Evaluable* zobj, class Nameset* nset);

    /// evaluate an object in the current nameset by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param quark  the quark to evaluate in this object
    virtual Object* eval (class Evaluable* zobj, class Nameset* nset, 
			  const long quark);

    /// apply an object with a set of arguments
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset 
    /// @param args   the arguments to apply
    virtual Object* apply (class Evaluable* zobj, class Nameset* nset, 
			   class Cons* args);

    /// apply an object by quark with a set of arguments
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param quark  the quark to apply this arguments
    /// @param args   the arguments to apply
    virtual Object* apply (class Evaluable* zobj, class Nameset* nset, 
			   const long quark, class Cons* args);

    /// apply an object by object with a set of arguments
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param object the object to apply this arguments
    /// @param args   the arguments to apply
    virtual Object* apply (class Evaluable* zobj, class Nameset* nset, 
			   Object* object, class Cons* args);

    /// apply an object by object with a vector of arguments
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param object the object to apply this arguments
    /// @param argv   the vector arguments to apply
    virtual Object* apply (class Evaluable* zobj, class Nameset* nset, 
			   Object* object, class Vector* argv);

    /// apply an object with a vector of arguments by quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param quark  the quark to apply these arguments
    /// @param argv   the vector arguments to apply
    virtual Object* apply (class Evaluable* zobj, class Nameset* nset, 
			   const long quark, class Vector* argv);

  public:
    // the memory allocation
    void* operator new       (const t_size size);
    void* operator new    [] (const t_size size);
    void  operator delete    (void* handle);
    void  operator delete [] (void* handle);
    // enable/disable the memory debug (for internal use please...)
    static void setmdbg (const bool flag);
    // set the memory label            (for internal use please...)
    static void setmlbl (const class String& label);
  };

  /// The transient class is a simple object used local transient object
  /// reference counting. The object is reference counted at creation and
  /// dereference at destruction, thus providing a local transient protection
  /// mechanism.

  class Transient {
  private:
    /// the transient object
    Object* p_tobj;

  public:
    /// create a null transient
    Transient (void);

    /// create a transient object
    /// @param tobj the object argument
    Transient (Object* tobj);

    /// copy construct this transient
    /// @param that the transient to copy
    Transient (const Transient& that);

    /// copy move this transient
    /// @param that the transient to copy
    Transient (Transient&& that) noexcept;

    /// destroy this transient
    ~Transient (void);

    /// assign a transient to this one
    /// @param that the transient to assign
    Transient& operator = (const Transient& that);

    /// move a transient to this one
    /// @param that the transient to mobe
    Transient& operator = (Transient&& that) noexcept;

    /// @return the transient object
    Object* operator * (void);

    /// @return the transient object
    const Object* operator * (void) const;
  };
}

#endif
