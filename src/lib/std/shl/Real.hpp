// ---------------------------------------------------------------------------
// - Real.hpp                                                                -
// - standard object library - real class definition                         -
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

#ifndef AFNIX_REAL_HPP
#define AFNIX_REAL_HPP

#ifndef  AFNIX_INTEGER_HPP
#include "Integer.hpp"
#endif

namespace afnix {

  /// The Real class is a the object version of the native floating point
  /// number. The real number is implemented like the integer class and
  /// is derived from the Literal class. The class implements also a bunch of
  /// method for floating point operations.
  /// @author amaury darsch

  class Real : public Number {
  public:
    /// compute the opposite of the real
    /// @param x the real to oppose
    /// @return a new real opposite of the argument
    friend Real operator - (const Real& x);

    /// add two reals together
    /// @param x the first argument to add
    /// @param y the second argument to add
    /// @return a new sum of the previous one
    friend Real operator + (const Real& x, const Real& y);

    /// subtract two reals together
    /// @param x the first argument to subtract
    /// @param y the second argument to subtract
    /// @return a new real as the difference
    friend Real operator - (const Real& x, const Real& y);

    /// multiply two reals together
    /// @param x the first argument to multiply
    /// @param y the second argument to multiply
    /// @return a new real product
    friend Real operator * (const Real& x, const Real& y);

    /// divide two reals together
    /// @param x the numerator
    /// @param y the denumerator
    /// @return the division of the arguments
    friend Real operator / (const Real& x, const Real& y);

  private:
    /// the real representation
    t_real d_value;

  public:
    /// create a new default real
    Real (void);

    /// create a new real from a native value
    /// @param value the value to create
    Real (const t_real value);

    /// create a new real from an integer class
    /// @param value the value to create
    Real (const Integer& value);

    /// create a new real from a string
    /// @param value the value to convert
    Real (const String& value);

    /// copy constructor for this real
    /// @param that the real class to copy
    Real (const Real& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// clear this real
    void clear (void) override;

    /// @return a literal representation of this real
    String toliteral (void) const override;

    /// @return a string representation of this real
    String tostring (void) const override;

    /// @return a styled string of this real
    String format (const Style& lstl) const override;

    /// @return the real did
    t_word getdid (void) const override;

    /// @return the real sid
    t_word getsid (void) const override;

    /// serialize this real to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a real from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// assign an real with a native value
    /// @param value the value to assign
    Real& operator = (const t_real value);

    /// assign an real to this one
    /// @param that the real to assign
    Real& operator = (const Real& that);

    /// add a real to this one
    /// @param x the argument to add
    /// @return this added real
    Real& operator += (const Real& x);

    /// substract a real to this one
    /// @param x the argument to substract
    /// @return this added real
    Real& operator -= (const Real& x);

    /// multiply a real with this one
    /// @param x the argument to multiply
    /// @return this multiplied real
    Real& operator *= (const Real& x);

    /// multiply a real with this one
    /// @param x the argument to multiply
    /// @return this multiplied real
    Real& operator /= (const Real& x);

    /// prefix add one to the real
    Real& operator ++ (void);

    /// postfix add one to the real
    Real operator ++ (int);

    /// prefix sub one to the real
    Real& operator -- (void);

    /// postfix sub one to the real
    Real operator -- (int);

    /// compare this real with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const t_long value) const;

    /// compare this real with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const t_long value) const;

    /// compare this real with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const t_real value) const;

    /// compare this real with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const t_real value) const;

    /// compare two reals
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const Real& value) const;

    /// compare two reals
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const Real& value) const;

    /// compare two reals
    /// @param value the value to compare
    /// @return true if they are less
    bool operator < (const Real& value) const;

    /// compare two reals
    /// @param value the value to compare
    /// @return true if they are less or equal
    bool operator <= (const Real& value) const;

    /// compare two reals
    /// @param value the value to compare
    /// @return true if they are greater
    bool operator > (const Real& value) const;

    /// compare two reals
    /// @param value the value to compare
    /// @return true if they are greater or equal
    bool operator >= (const Real& value) const;

    /// compare this real with another one upto epsilon
    /// @param value the real value to compare
    bool cmp (const t_real value) const;

    /// @return true if this real is zero
    bool iszero (void) const;

    /// @return true if the number is nan
    bool isnan (void) const;

    /// @return true if the number is infinite
    bool isinf (void) const;
  
    /// @return the floor of this number
    Real floor (void) const;

    /// @return the ceiling of this number
    Real ceiling (void) const;

    /// @return the round of this number
    Real round (void) const;

    /// @return the absolute value of this number
    Real abs (void) const;

    /// @return the remainder of this value with the argument
    Real mod (const Real& x) const;

    /// @return the square root of this real
    Real sqrt (void) const;

    /// @return the natural logarithm of this number
    Real ln (void) const;

    /// @return the exponential of this number
    Real exp (void) const;

    /// @return the power of this number with its argument
    Real pow (const Real& x) const;

    /// @return the sine of this number
    Real sin (void) const;

    /// @return the cosine of this number
    Real cos (void) const;

    /// @return the tangent of this number
    Real tan (void) const;

    /// @return the arc sine of this number
    Real asin (void) const;

    /// @return the arc cosine of this number
    Real acos (void) const;

    /// @return the arc tangent of this number
    Real atan (void) const;

    /// @return the hyperbolic sine of this number
    Real sinh (void) const;

    /// @return the hyperbolic cosine of this number
    Real cosh (void) const;

    /// @return the hyperbolic tangent of this number
    Real tanh (void) const;

    /// @return the hyperbolic arc sine of this number
    Real asinh (void) const;

    /// @return the hyperbolic arc cosine of this number
    Real acosh (void) const;

    /// @return the hyperbolic arc tangent of this number
    Real atanh (void) const;

    /// @return a native value for this real
    t_real toreal (void) const;

    /// @return an integer representation from this real
    t_long tolong (void) const;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;

    /// @return the viewable data
    const t_byte* tobyte (void) const override;
    
  public:
    /// evaluate an object to a real value
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param object the object to evaluate
    static t_real evalto (Evaluable* zobj, Nameset* nset, Object* object);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;

    /// set an object to this real
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object) override;

    /// apply this real with a set of arguments and a quark
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset    
    /// @param quark  the quark to apply these arguments
    /// @param argv   the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
