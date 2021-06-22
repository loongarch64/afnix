// ---------------------------------------------------------------------------
// - Boolean.hpp                                                             -
// - standard object library - boolean class definition                      -
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

#ifndef  AFNIX_BOOLEAN_HPP
#define  AFNIX_BOOLEAN_HPP

#ifndef  AFNIX_LITERAL_HPP
#include "Literal.hpp"
#endif

namespace afnix {

  /// The Boolean class is a builtin object for the boolean native type.
  /// The Boolean class accepts only two values (true or false) and is mostly
  /// used for binary testing. There is no automatic conversion from or to the
  /// boolean type - except for string representation.
  /// @author amaury darsch

  class Boolean : public Literal {
  private:
    /// the native boolean representation
    bool d_value;

  public:
    /// create a new default boolean - by default it is false
    Boolean (void);

    /// create a new boolean from a native boolean
    /// @param value the value to create
    Boolean (const bool value);

    /// create a new boolean from a string
    /// @param value the value to convert
    Boolean (const String& value);

    /// copy constructor for this boolean
    /// @param that the boolean class to copy
    Boolean (const Boolean& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// clear this boolean
    void clear (void) override;

    /// @return a literal representation of this boolean
    String toliteral (void) const override;

    /// @return a string representation of this boolean
    String tostring (void) const override;

    /// @return the boolean did
    t_word getdid (void) const override;

    /// @return the boolean sid
    t_word getsid (void) const override;

    /// serialize this boolean to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const  override;

    /// deserialize a boolean from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// @return the boolean value of this boolean
    virtual bool tobool (void) const;

    /// assign a boolean with a native value
    /// @param value the value to assign
    Boolean& operator = (const bool value);

    /// assign a boolean to this one
    /// @param that the boolean to assign
    Boolean& operator = (const Boolean& that);

    /// compare this boolean with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const bool value) const;

    /// compare this boolean with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const bool value) const;

    /// compare two booleans
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const Boolean& value) const;

    /// compare two boolean
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const Boolean& value) const;

  public:
    /// evaluate an object to a boolean value
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param object the object to evaluate
    static bool evalto (Evaluable* zobj, Nameset* nset, Object* object);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;

    /// set an object to this boolean
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object) override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
