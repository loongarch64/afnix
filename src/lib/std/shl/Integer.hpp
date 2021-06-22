// ---------------------------------------------------------------------------
// - Integer.hpp                                                             -
// - standard object library - integer class definition                      -
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

#ifndef  AFNIX_INTEGER_HPP
#define  AFNIX_INTEGER_HPP

#ifndef  AFNIX_NUMBER_HPP
#include "Number.hpp"
#endif

namespace afnix {

  /// The Integer class is the object version of the native integer
  /// operation for the afnix subsystem. All operations are made with
  /// at least a 64 bits representation.
  /// @author amaury darsch

  class Integer : public Number {
  public:
    /// compute the opposite of the integer
    /// @param x the integer to oppose
    /// @return a new integer opposite of the argument
    friend Integer operator - (const Integer& x);
    
    /// add two integers together
    /// @param x the first argument to add
    /// @param y the second argument to add
    /// @return a new integer sum of the previous one
    friend Integer operator + (const Integer& x, const Integer& y);

    /// subtract two integers together
    /// @param x the first argument to subtract
    /// @param y the second argument to subtract
    /// @return a new integer difference of the previous one
    friend Integer operator - (const Integer& x, const Integer& y);

    /// multiply two integers together
    /// @param x the first argument to multiply
    /// @param y the second argument to multiply
    /// @return a new integer product of the previous one
    friend Integer operator * (const Integer& x, const Integer& y);

    /// divide two integers together
    /// @param x the numerator
    /// @param y the denumerator
    /// @return the division of the arguments  
    friend Integer operator / (const Integer& x, const Integer& y);

    /// get the modulo between two integers
    /// @param x the integer value
    /// @param y the modulus
    /// @return the division of the arguments  
    friend Integer operator % (const Integer& x, const Integer& y);

    /// bitwise negate an integer
    /// @param x the integer value
    friend Integer operator ~ (const Integer& x);

    /// bitwise and two integers together
    /// @param x the first argument to and
    /// @param y the second argument to and
    friend Integer operator & (const Integer& x, const Integer& y);

    /// bitwise or two integers together
    /// @param x the first argument to or
    /// @param y the second argument to or
    friend Integer operator | (const Integer& x, const Integer& y);

    /// bitwise xor two integers together
    /// @param x the first argument to xor
    /// @param y the second argument to xor
    friend Integer operator ^ (const Integer& x, const Integer& y);

  private:
    /// the integer representation
    t_long d_value;

  public:
    /// create a new default integer
    Integer (void);

    /// create a new integer from a long long integer
    /// @param value the value to create
    Integer (const t_long value);

    /// create a new integer from a string
    /// @param value the value to convert
    Integer (const String& value);

    /// copy constructor for this integer
    /// @param that the integer class to copy
    Integer (const Integer& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// clear this integer
    void clear (void) override;

    /// @return a literal representation of this integer
    String toliteral (void) const override;

    /// @return a string representation of this integer
    String tostring (void) const override;

    /// @return a styled string of this real
    String format (const Style& lstl) const override;

    /// @return the integer did
    t_word getdid (void) const override;

    /// @return the integer sid
    t_word getsid (void) const override;

    /// serialize this integer to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize an integer from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// assign an integer with a native value
    /// @param value the value to assign
    Integer& operator = (const t_long value);

    /// assign an integer with a native value
    /// @param that the integer to assign
    Integer& operator = (const Integer& that);

    /// add an integer to this one
    /// @param x the argument to add
    /// @return this added integer
    Integer& operator += (const Integer& x);

    /// substract an integer to this one
    /// @param x the argument to substract
    /// @return this added integer
    Integer& operator -= (const Integer& x);

    /// multiply an integer with this one
    /// @param x the argument to multiply
    /// @return this multiplied integer
    Integer& operator *= (const Integer& x);

    /// divide an integer with this one
    /// @param x the argument to divide
    /// @return this multiplied integer
    Integer& operator /= (const Integer& x);

    /// prefix add one to the integer
    Integer& operator ++ (void);

    /// postfix add one to the integer
    Integer operator ++ (int);

    /// prefix sub one to the integer
    Integer& operator -- (void);

    /// postfix sub one to the integer
    Integer operator -- (int);

    /// shift left an integer by a certain amount
    /// @param asl the amount of left shift
    Integer operator << (const long asl) const;

    /// shift right an integer by a certain amount
    /// @param asr the amount of right shift
    Integer operator >> (const long asr) const;

    /// compare this integer with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const t_long value) const;

    /// compare this integer with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const t_long value) const;

    /// compare two integers
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const Integer& value) const;

    /// compare two integers
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const Integer& value) const;

    /// compare two integers
    /// @param value the value to compare
    /// @return true if they are less
    bool operator < (const Integer& value) const;

    /// compare two integers
    /// @param value the value to compare
    /// @return true if they are less or equal
    bool operator <= (const Integer& value) const;

    /// compare two integers
    /// @param value the value to compare
    /// @return true if they are greater
    bool operator > (const Integer& value) const;

    /// compare two integers
    /// @param value the value to compare
    /// @return true if they are greater or equal
    bool operator >= (const Integer& value) const;

    /// @return true if this integer is zero
    bool iszero (void) const;

    /// @return true if this integer is even
    bool iseven (void) const;

    /// @return true if this integer is odd
    bool isodd (void) const;

    /// @return the absolute value of this number
    Integer abs (void) const;

    /// @return a hexadecimal representation
    String tohexa (void) const override;

    /// @return a hexadecimal string
    String tohstr (void) const override;

    /// @return a native integer representation
    t_long tolong (void) const;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;

    /// @return the viewable data
    const t_byte* tobyte (void) const override;

  public:
    /// evaluate an object to an integer value
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param object the object to evaluate
    static t_long evalto (Evaluable* zobj, Nameset* nset, Object* object);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;

    /// set an object to this integer
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
