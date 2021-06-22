// ---------------------------------------------------------------------------
// - Numeral.hpp                                                             -
// - standard object library - numeral class definition                      -
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

#ifndef AFNIX_NUMERAL_HPP
#define AFNIX_NUMERAL_HPP

#ifndef  AFNIX_ITEM_HPP
#include "Item.hpp"
#endif

#ifndef  AFNIX_REAL_HPP
#include "Real.hpp"
#endif

namespace afnix {

  /// The Numeral class is a dedicated class for implementation specific
  /// number. The class is designed to manipulate short integer or float
  /// throw a set of specific operations. It is not intended to replace
  /// the standard integer and numeral class, but does provide the necessary
  /// functionalities to operate correctly. A numeral is of course treated
  /// as a number.
  /// @author amaury darsch

  class Numeral : public Number {
  public:
    /// compute the opposite of the dnumeral
    /// @param x the numeral to oppose
    /// @return a new numeral opposite of the argument
    friend Numeral operator - (const Numeral& x);

    /// add two numerals together
    /// @param x the first argument to add
    /// @param y the second argument to add
    /// @return a new sum of the previous one
    friend Numeral operator + (const Numeral& x, const Numeral& y);

    /// subtract two numerals together
    /// @param x the first argument to subtract
    /// @param y the second argument to subtract
    /// @return a new numeral as the difference
    friend Numeral operator - (const Numeral& x, const Numeral& y);

    /// multiply two numerals together
    /// @param x the first argument to multiply
    /// @param y the second argument to multiply
    /// @return a new numeral product
    friend Numeral operator * (const Numeral& x, const Numeral& y);

    /// divide two numerals together
    /// @param x the numerator
    /// @param y the denumerator
    /// @return the division of the arguments
    friend Numeral operator / (const Numeral& x, const Numeral& y);

  public:
    /// the numeral type
    enum t_numt {
      NUMT_SINT = 0x00U, // single integer (32 bits)
      NUMT_SFLT = 0x01U  // single float   (32 bits)
    };

    /// @return a numeral type by object
    static t_numt tonumt (const Item& item);

    /// @return an item by type
    static Item* toitem (const t_numt numt);

    /// @return the byte size of a numeral
    static long tobsiz (const t_numt numt);
    
  private:
    /// the numeral type
    t_numt d_numt;
    /// the numeral value
    union {
      int   d_ival;
      float d_fval;
    };

  public:
    /// create a new default numeral
    Numeral (void);

    /// create a numeral by type
    /// @param numt the numeral type
    Numeral (const t_numt numt);
    
    /// create a new numeral from a native value
    /// @param value the native value
    Numeral (const t_long value);

    /// create a new numeral from a native value
    /// @param value the native value
    Numeral (const t_real value);

    /// create a new numeral from an integer object
    /// @param value the value to create
    Numeral (const Integer& value);

    /// create a new numeral from a real object
    /// @param value the value to create
    Numeral (const Real& value);

    /// create a new numeral from a string
    /// @param value the value to create
    Numeral (const String& value);

    /// copy constructor for this numeral
    /// @param that the numeral class to copy
    Numeral (const Numeral& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// clear this numeral
    void clear (void) override;

    /// @return a literal representation of this numeral
    String toliteral (void) const override;

    /// @return a string representation of this numeral
    String tostring (void) const override;

    /// @return a styled string of this numeral
    String format (const Style& lstl) const override;

    /// @return the numeral did
    t_word getdid (void) const override;

    /// @return the numeral sid
    t_word getsid (void) const override;

    /// serialize this numeral to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a numeral from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// assign an numeral with a native value
    /// @param value the value to assign
    Numeral& operator = (const t_long value);

    /// assign an numeral with a native value
    /// @param value the value to assign
    Numeral& operator = (const t_real value);

    /// assign an numeral to this one
    /// @param that the numeral to assign
    Numeral& operator = (const Numeral& that);

    /// add a numeral to this one
    /// @param x the argument to add
    /// @return this added numeral
    Numeral& operator += (const Numeral& x);

    /// substract a numeral to this one
    /// @param x the argument to substract
    /// @return this added numeral
    Numeral& operator -= (const Numeral& x);

    /// multiply a numeral with this one
    /// @param x the argument to multiply
    /// @return this multiplied numeral
    Numeral& operator *= (const Numeral& x);

    /// multiply a numeral with this one
    /// @param x the argument to multiply
    /// @return this multiplied numeral
    Numeral& operator /= (const Numeral& x);

    /// prefix add one to the numeral
    Numeral& operator ++ (void);

    /// postfix add one to the numeral
    Numeral operator ++ (int);

    /// prefix sub one to the numeral
    Numeral& operator -- (void);

    /// postfix sub one to the numeral
    Numeral operator -- (int);

    /// compare this numeral with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const t_long value) const;

    /// compare this numeral with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const t_long value) const;

    /// compare this numeral with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const t_real value) const;

    /// compare this numeral with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const t_real value) const;

    /// compare two numerals
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const Numeral& value) const;

    /// compare two numerals
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const Numeral& value) const;

    /// compare two numerals
    /// @param value the value to compare
    /// @return true if they are less
    bool operator < (const Numeral& value) const;

    /// compare two numerals
    /// @param value the value to compare
    /// @return true if they are less or equal
    bool operator <= (const Numeral& value) const;

    /// compare two numerals
    /// @param value the value to compare
    /// @return true if they are greater
    bool operator > (const Numeral& value) const;

    /// compare two numerals
    /// @param value the value to compare
    /// @return true if they are greater or equal
    bool operator >= (const Numeral& value) const;

    /// compare this numeral with another one
    /// @param value the numeral value to compare
    bool cmp (const t_long value) const;

    /// compare this numeral with another one upto epsilon
    /// @param value the numeral value to compare
    bool cmp (const t_real value) const;

    /// @return true if this numeral is zero
    bool iszero (void) const;

    /// @return true if this integer is even
    bool iseven (void) const;

    /// @return true if this integer is odd
    bool isodd (void) const;
    
    /// @return true if the numeral is nan
    bool isnan (void) const;

    /// @return true if the numeral is infinite
    bool isinf (void) const;
  
    /// @return the floor of this number
    Numeral floor (void) const;

    /// @return the ceiling of this number
    Numeral ceiling (void) const;

    /// @return the round of this number
    Numeral round (void) const;

    /// @return the absolute value of this number
    Numeral abs (void) const;

    /// @return the remainder of this value with the argument
    Numeral mod (const Numeral& x) const;

    /// @return the square root of this numeral
    Numeral sqrt (void) const;

    /// @return the natural logarithm of this number
    Numeral ln (void) const;

    /// @return the exponential of this number
    Numeral exp (void) const;

    /// @return the power of this number with its argument
    Numeral pow (const Numeral& x) const;

    /// @return the sine of this number
    Numeral sin (void) const;

    /// @return the cosine of this number
    Numeral cos (void) const;

    /// @return the tangent of this number
    Numeral tan (void) const;

    /// @return the arc sine of this number
    Numeral asin (void) const;

    /// @return the arc cosine of this number
    Numeral acos (void) const;

    /// @return the arc tangent of this number
    Numeral atan (void) const;

    /// @return the hyperbolic sine of this number
    Numeral sinh (void) const;

    /// @return the hyperbolic cosine of this number
    Numeral cosh (void) const;

    /// @return the hyperbolic tangent of this number
    Numeral tanh (void) const;

    /// @return the hyperbolic arc sine of this number
    Numeral asinh (void) const;

    /// @return the hyperbolic arc cosine of this number
    Numeral acosh (void) const;

    /// @return the hyperbolic arc tangent of this number
    Numeral atanh (void) const;

    /// @return the numeral type
    t_numt getnumt (void) const;
    
    /// @return a native value for this numeral
    t_real toreal (void) const;

    /// @return an integer representation from this numeral
    t_long tolong (void) const;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;

    /// @return the viewable data
    const t_byte* tobyte (void) const override;

    /// convert a numeral to a type
    t_numt convert (const t_numt numt);
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;

    /// set an object to this numeral
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object) override;

    /// apply this numeral with a set of arguments and a quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param quark  the quark to apply these arguments
    /// @param argv   the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
