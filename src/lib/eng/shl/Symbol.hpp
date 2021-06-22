// ---------------------------------------------------------------------------
// - Symbol.hpp                                                              -
// - afnix engine - symbol class definition                                  -
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

#ifndef  AFNIX_SYMBOL_HPP
#define  AFNIX_SYMBOL_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Symbol class is the fundamental class for all names in the afnix
  /// engine. A symbol binds a name to an object. A symbol can hold some
  /// flags as well. Typically one is the const flag. When a symbol is
  /// created, it is bound by default to the nil object. A symbol is by
  /// default mutable, that is, it can be changed.
  /// @author amaury darsch

  class Symbol : public Literal {
  private:
    /// the symbol quark
    long    d_quark;
    /// the symbol object
    Object* p_object;
    /// the const flag
    bool    d_const;

  public:
    /// create a new symbol by name
    /// @param name the symbol name
    Symbol (const String& name);

    /// create a new symbol by quark
    /// @param quark the symbol quark
    Symbol (const long quark);

    /// create a new symbol with a name and an object
    /// @param name   the symbol name
    /// @param object the symbol object
    Symbol (const String& name, Object* object);

    /// create a new symbol with a quark and an object
    /// @param quark  the symbol quark
    /// @param object the symbol object
    Symbol (const long quark, Object* object);

    /// create a new symbol with a name, a constant flag and an object
    /// @param name   the symbol name
    /// @param cflag  the constant flag
    /// @param object the symbol object
    Symbol (const String& name, const bool cflag, Object* object);

    /// create a new symbol with a quark, a constant flag and an object
    /// @param quark  the symbol quark
    /// @param cflag  the constant flag
    /// @param object the symbol object
    Symbol (const long quark, const bool cflag, Object* object);

    /// copy constructor for this symbol
    /// @param that the symbol to copy
    Symbol (const Symbol& that);

    /// destroy this symbol
    ~Symbol (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this symbol
    void clear (void);

    /// @return a literal representation of this symbol
    String toliteral (void) const;

    /// @return a string representation of this symbol
    String tostring (void) const;

    /// @return the symbol quark
    long toquark (void) const;

    /// set the const flag for this symbol
    /// @param flag the flag to set
    void setconst (const bool flag);

    /// @return the const flag for this symbol
    bool getconst (void) const;

    /// set the symbol object
    /// @param object the object to set
    void setobj (Object* object);

    /// @return the symbol object
    Object* getobj (void) const;

  private:
    // make the assignment operator private
    Symbol& operator = (const Symbol&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// set a constant object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* cdef (Evaluable* zobj, Nameset* nset, Object* object);

    /// set an object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object);

    /// unreference an object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    Object* udef (Evaluable* zobj, Nameset* nset);

    /// evaluate this object
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @return the evaluated object
    Object* eval (Evaluable* zobj, Nameset* nset);

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
