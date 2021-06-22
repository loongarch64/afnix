// ---------------------------------------------------------------------------
// - Nameset.hpp                                                            -
// - afnix engine - nameset class definition                                -
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

#ifndef  AFNIX_NAMESET_HPP
#define  AFNIX_NAMESET_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Nameset class defines an environment for a set of objects. Since
  /// a nameset is an object, it is possible to create or evaluate by
  /// name. This class is an abstract class.
  /// @author amaury darsch

  class Nameset : public Object {
  protected:
    /// the parent nameset
    Nameset* p_pset;
    /// the context object
    Object* p_whom;
    
  public:
    /// create a new nameset
    Nameset (void);
    
    /// destroy this nameset
    ~Nameset (void);

    /// set the parent nameset
    /// @param nset the parent nameset
    virtual void setparent (Nameset* nset);

    /// @return the parent nameset
    virtual Nameset* getparent (void) const;

    /// set the nameset context object
    /// @param object the object to set
    virtual void setwhom (Object* object);

    /// @return the nameset context object
    virtual Object* getwhom (void) const;
    
    /// reset this nameset
    virtual void reset (void) =0;

    /// @return a child nameset
    virtual Nameset* dup (void) =0;

    /// add a new object by quark
    /// @param quark the object quark
    /// @param object the object to bind
    virtual void bind (const long quark, Object* object) =0;

    /// add a new object by name
    /// @param name the object name
    /// @param object the object to bind
    virtual void bind (const String& name, Object* object);

    /// @return true if the quark exists in this nameset
    virtual bool exists (const long quark) const =0;

    /// @return true if the name exists in this nameset
    virtual bool exists (const String& name) const;

    /// @return true if the quark is valid in the namesets
    virtual bool valid (const long quark) const;

    /// @return true if the name is valid in the namesets
    virtual bool valid (const String& name) const;

    /// @return an object by quark locally
    virtual Object* get (const long quark) const =0;

    /// @return an object by name locally
    virtual Object* get (const String& name) const;

    /// @return an object by quark recursively
    virtual Object* find (const long quark) const;

    /// @return an object by name recursively
    virtual Object* find (const String& name) const;

    /// remove an object by quark in this nameset
    /// @param quark the binding to remove
    virtual void remove (const long quark) =0;

    /// remove an object by name in this nameset
    /// @param name the binding to remove
    virtual void remove (const String& name);

    /// create a child nameset by quark
    /// @param quark the child nameset quark
    virtual Nameset* mknset (const long quark);

    /// create a child nameset by name
    /// @param name the child nameset name
    virtual Nameset* mknset (const String& name);

    /// @return a nameset by quark
    virtual Nameset* getnset (const long quark) const;

    /// @return a nameset by name
    virtual Nameset* getnset (const String& name) const;

    /// create a new constant symbol by quark
    /// @param quark  the symbol quark to create
    /// @param object the object to bind
    virtual void symcst (const long quark, Object* object);

    /// create a new constant symbol by name
    /// @param name   the symbol name to create
    /// @param object the object to bind
    virtual void symcst (const String& name, Object* object);

    /// create a new symbol by quark
    /// @param quark  the symbol quark to create
    /// @param object the object to bind
    virtual void symdef (const long quark, Object* object);

    /// create a new symbol by name
    /// @param name   the symbol name to create
    /// @param object the object to bind
    virtual void symdef (const String& name, Object* object);

    /// create a new symbol by quark
    /// @param quark  the symbol quark to create
    /// @param cflag  the constant flag
    /// @param object the object to bind
    virtual void symset (const long quark, const bool cflag, Object* object);

    /// create a new symbol by name and flag
    /// @param name   the symbol name to create
    /// @param cflg   the constant flag
    /// @param object the object to bind
    virtual void symset (const String& name, const bool cfgl, Object* object);

  private:
    // make the copy constructor private
    Nameset (const Nameset&);
    // make the assignment operator private
    Nameset& operator = (const Nameset&);

  public:
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

    /// evaluate an object by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);
  };
}

#endif
