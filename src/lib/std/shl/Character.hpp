// ---------------------------------------------------------------------------
// - Character.hpp                                                           -
// - standard object library - unicode character class definition            -
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

#ifndef AFNIX_CHARACTER_HPP
#define AFNIX_CHARACTER_HPP

#ifndef  AFNIX_LITERAL_HPP
#include "Literal.hpp"
#endif

namespace afnix {

  /// The Character class is the object version of the unicode character.
  /// The Character class is derived from the literal object and is built
  /// with a quad representation.
  /// @author amaury darsch

  class Character: public Literal {
  private:
    /// the character representation
    t_quad d_value;

  public:
    /// create a new default character
    Character (void);

    /// create a new character from a base character
    /// @param value the value to create
    Character (const char value);

    /// create a new character from a unicode character
    /// @param value the value to create
    Character (const t_quad value);

    /// create a new character from a string representation
    /// @param s the string to convert
    Character (const String& s);

    /// copy constructor for this character
    /// @param that the character to copy
    Character (const Character& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    // clear this character
    void clear (void) override;

    /// @return a literal representation of this character
    String toliteral (void) const override;

    /// @return a string representation of this character
    String tostring (void) const override;

    /// @return the character did
    t_word getdid (void) const override;

    /// @return the character sid
    t_word getsid (void) const override;

    /// serialize this character to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a character from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// @return a native character
    virtual char tochar (void) const;

    /// @return a native quad representation
    virtual t_quad toquad (void) const;

    /// assign a unicode character with a native value
    /// @param value the value to assign
    Character& operator = (const char value);

    /// assign a character with a native value
    /// @param value the value to assign
    Character& operator = (const t_quad value);

    /// assign a character to this one
    /// @param that the character to assign
    Character& operator = (const Character& that);

    /// add a character with a native value
    /// @param value the value to add
    Character operator + (const long value) const;

    /// sub a character with a native value
    /// @param value the value to add
    Character operator - (const long value) const;

    /// add a native value to this character
    /// @param value the value to add
    Character& operator += (const long value);

    /// sub a native value to this character
    /// @param value the value to add
    Character& operator -= (const long value);

    /// prefix add one to the character
    Character& operator ++ (void);

    /// postfix add one to the character
    Character operator ++ (int);

    /// prefix sub one to the character
    Character& operator -- (void);

    /// postfix sub one to the character
    Character operator -- (int);

    /// compare this character with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const char value) const;

    /// compare this character with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const char value) const;

    /// compare this character with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const t_quad value) const;

    /// compare this character with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const t_quad value) const;

    /// compare two characters
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const Character& value) const;

    /// compare two characters
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const Character& value) const;

    /// compare two characters
    /// @param value the value to compare
    /// @return true if they are less
    bool operator < (const Character& value) const;

    /// compare two characters
    /// @param value the value to compare
    /// @return true if they are less or equal
    bool operator <= (const Character& value) const;

    /// compare two characters
    /// @param value the value to compare
    /// @return true if they are greater
    bool operator > (const Character& value) const;

    /// compare two characters
    /// @param value the value to compare
    /// @return true if they are greater or equal    
    bool operator >= (const Character& value) const;

    /// @return true if the character is a letter
    virtual bool isletter (void) const;

    /// @return true if the character is a digit
    virtual bool isdigit (void) const;

    /// @return true if the character is an alphanumeric
    virtual bool isalpha (void) const;

    /// @return true if the character is a blank or tab
    virtual bool isblank (void) const;

    /// @return true if the character is an ascii character
    virtual bool isascii (void) const;

    /// @return true if the character is a latin character
    virtual bool islatin (void) const;

    /// @return true if the character is an eol
    virtual bool iseol (void) const;

    /// @return true if the character is an eos
    virtual bool iseos (void) const;

    /// @return true if the character is nil
    virtual bool isnil (void) const;

  public:
    /// evaluate an object to a unicode value
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param object the object to evaluate
    static t_quad evalto (Evaluable* zobj, Nameset* nset, Object* object);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;

    /// set an object to this character
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object) override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the name to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
