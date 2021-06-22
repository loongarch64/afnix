// ---------------------------------------------------------------------------
// - Relatif.hpp                                                             -
// - standard object library - relatif big number class definition           -
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

#ifndef AFNIX_RELATIF_HPP
#define AFNIX_RELATIF_HPP

#ifndef  AFNIX_NUMBER_HPP
#include "Number.hpp"
#endif

namespace afnix {

  /// The Relatif class is a big number integer implementation. The class
  /// behaves like the Integer class except that it can handle any integer
  /// size. The class is a also a literal object. The name 'relatif' comes
  /// from the french 'entier relatif'.
  /// @author amaury darsch

  class Relatif : public Number {
  public:    
    /// compute the greatest common divisor of two relatifs
    /// @param x the first relatif
    /// @param y the first relatif
    static Relatif gcd (const Relatif& x, const Relatif& y);

    /// compute the least common multiple of two relatifs
    /// @param x the first relatif
    /// @param y the first relatif
    static Relatif lcm (const Relatif& x, const Relatif& y);

    /// compute the modular multiplicative inverse
    /// @param x the base relatif
    /// @param m the modulus
    static Relatif mmi (const Relatif& x, const Relatif& m);

    /// compute a fast modular exponentiation
    /// @param x the base relatif
    /// @param e the exponent
    /// @param m the modulus
    static Relatif mme (const Relatif& x, const Relatif& e, const Relatif& m);

    /// generate a random relatif number upto a maximum
    /// @param mrn the maximum relatif
    static Relatif random (const Relatif& mrn);

    /// generate a random n bits relatif number
    /// @param bits the number of bits
    static Relatif random (const long bits);

    /// generate a random n bits odd or even relatif number
    /// @param bits the number of bits
    /// @param oddf the odd number flag flag
    static Relatif random (const long bits, const bool oddf);

    /// change the sign of a relatif
    /// @param x the argument to change
    friend Relatif operator - (const Relatif& x);

    /// add a relatif with an integer
    /// @param x the first argument to add
    /// @param y the second argument to add
    friend Relatif operator + (const Relatif& x, const t_long y);

    /// add two relatifs together
    /// @param x the first argument to add
    /// @param y the second argument to add
    friend Relatif operator + (const Relatif& x, const Relatif& y);

    /// substract a relatif with an integer
    /// @param x the first argument to substract
    /// @param y the second argument to substract
    friend Relatif operator - (const Relatif& x, const t_long y);

    /// substract two relatifs together
    /// @param x the first argument to substract
    /// @param y the second argument to substract
    friend Relatif operator - (const Relatif& x, const Relatif& y);

    /// multiply a relatif with an integer
    /// @param x the first argument to multiply
    /// @param y the second argument to multiply
    friend Relatif operator * (const Relatif& x, const t_long y);

    /// multiply two relatifs together
    /// @param x the first argument to multiply
    /// @param y the second argument to multiply
    friend Relatif operator * (const Relatif& x, const Relatif& y);

    /// divide a relatif with an integer
    /// @param x the first argument to divide
    /// @param y the second argument to divide
    friend Relatif operator / (const Relatif& x, const t_long y);

    /// divide two relatifs together
    /// @param x the first argument to divide
    /// @param y the second argument to divide
    friend Relatif operator / (const Relatif& x, const Relatif& y);

    /// compute the reminder of a relatif with an integer
    /// @param x the first argument to divide
    /// @param y the second argument to divide
    friend Relatif operator % (const Relatif& x, const t_long y);

    /// compute the reminder of two relatifs together
    /// @param x the first argument to divide
    /// @param y the second argument to divide
    friend Relatif operator % (const Relatif& x, const Relatif& y);

    /// bitwise negate this relatif
    /// @param x the argument to negate
    friend Relatif operator ~ (const Relatif& x);

    /// bitwise and two relatifs together
    /// @param x the first argument to and
    /// @param y the second argument to and
    friend Relatif operator & (const Relatif& x, const Relatif& y);

    /// bitwise or two relatifs together
    /// @param x the first argument to or
    /// @param y the second argument to or
    friend Relatif operator | (const Relatif& x, const Relatif& y);

    /// bitwise xor two relatifs together
    /// @param x the first argument to xor
    /// @param y the second argument to xor
    friend Relatif operator ^ (const Relatif& x, const Relatif& y);

  private:
    /// the relatif sign
    bool d_sgn;
    /// the mpi structure
    struct s_mpi* p_mpi;

  public:
    /// create a default relatif
    Relatif (void);

    /// create a relatif by value
    /// @param x the value to convert
    Relatif (const t_long x);

    /// create a relatif by string
    /// @param s the string to convert
    Relatif (const String& s);

    /// create a relatif from a byte array
    /// @param rbuf the byte buffer
    /// @param size the array size
    Relatif (const t_byte* rbuf, const long size);

    /// create a relatif from a byte array in complemented form
    /// @param rbuf the byte buffer
    /// @param size the array size
    /// @param cflg the complemented flag
    Relatif (const t_byte* rbuf, const long size, const bool cflg);

    /// copy construct this relatif
    /// @param that the relatif to copy
    Relatif (const Relatif& that);
    
    /// copy move this relatif
    /// @param that the relatif to move
    Relatif (Relatif&& that) noexcept;

    /// destroy this relatif
    ~Relatif (void);

    /// assign a relatif to this one
    /// @param that the relatif to assign
    Relatif& operator = (const Relatif& that);

    /// move a relatif into this one
    /// @param that the relatif to move
    Relatif& operator = (Relatif&& that) noexcept;
    
    /// add a relatif to this one
    /// @param x the argument to add
    Relatif& operator += (const Relatif& x);

    /// substract a relatif to this one
    /// @param x the argument to substract
    Relatif& operator -= (const Relatif& x);

    /// multiply a relatif to this one
    /// @param x the argument to multiply
    Relatif& operator *= (const Relatif& x);

    /// divide a relatif to this one
    /// @param x the argument to divide
    Relatif& operator /= (const Relatif& x);

    /// prefix increments this relatif
    Relatif& operator ++ (void);

    /// prefix decrements this relatif
    Relatif& operator -- (void);

    /// postfix increments this relatif
    Relatif operator ++ (int);

    /// postfix decrements this relatif
    Relatif operator -- (int);

    /// shift left a relatif by a certain amount
    /// @param asl the amount of left shift
    Relatif operator << (const long asl) const;

    /// shift right a relatif by a certain amount
    /// @param asr the amount of right shift
    Relatif operator >> (const long asr) const;

    /// shift left this relatif by a certain amount
    /// @param asl the amount of left shift
    Relatif& operator <<= (const long asl);

    /// shift right this relatif by a certain amount
    /// @param asr the amount of right shift
    Relatif& operator >>= (const long asr);

    /// compare a relatif with an integer
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator == (const t_long x) const;

    /// compare two relatifs
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator == (const Relatif& x) const;

    /// compare a relatif with an integer
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator != (const t_long x) const;

    /// compare two relatifs
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator != (const Relatif& x) const;

    /// compare a relatif with an integer
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator < (const t_long x) const;

    /// compare two relatifs
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator < (const Relatif& x) const;

    /// compare a relatif with an integer
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator <= (const t_long x) const;

    /// compare two relatifs
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator <= (const Relatif& x) const;

    /// compare a relatif with an integer
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator > (const t_long x) const;

    /// compare two relatifs
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator > (const Relatif& x) const;

    /// compare a relatif with an integer
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator >= (const t_long x) const;

    /// compare two relatifs
    /// @param x the value to compare
    /// @return true if they are equals
    bool operator >= (const Relatif& x) const;

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// clear this relatif
    void clear (void) override;

    /// @return a literal representation of this relatif
    String toliteral (void) const override;

    /// @return a string representation of this relatif
    String tostring (void) const override;

    /// @return the relatif did
    t_word getdid (void) const override;

    /// @return the relatif sid
    t_word getsid (void) const override;

    /// serialize this relatif to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a relatif from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// @return true if this relatif is zero
    virtual bool iszero (void) const;
    
    /// @return true if this relatif is even
    virtual bool iseven (void) const;

    /// @return true if the relatif is odd
    virtual bool isodd (void) const;

    /// @return the absolute value of this relatif
    virtual Relatif abs (void) const;

    /// compute the power of a relatif
    /// @param e the exponent
    virtual Relatif pow (const t_long e) const;

    /// compute the power of a relatif
    /// @param e the exponent
    virtual Relatif pow (const Relatif& e) const;

    /// compute the modular exponentiation of a relatif
    /// @param e the exponent
    /// @param m the modulus
    virtual Relatif pow (const t_long e, const Relatif& m) const;

    /// compute the modular exponentiation of a relatif
    /// @param e the exponent
    /// @param m the modulus
    virtual Relatif pow (const Relatif& e, const Relatif& m) const;

    /// create a reverse relatif by size
    /// @param size the result bit size
    virtual Relatif reverse (const long size) const;
    
    /// @return the lsb of this relatif
    virtual long getlsb (void) const;

    /// @return the msb of this relatif
    virtual long getmsb (void) const;

    /// @return the bit value at a certain position
    virtual bool getbit (const long bpos) const;

    /// set the relatif bit value by position
    /// @param bpos the bit position
    /// @param bval the bit value
    virtual void setbit (const long bpos, const bool bval);

    /// @return the byte buffer size
    virtual long getbbs (void) const;

    /// fill an unsigned buffer representation by size
    /// @param rbuf the relatif buffer
    /// @param size the buffer size
    virtual long toubuf (t_byte* rbuf, const long size) const;

    /// fill a signed buffer representation by size
    /// @param rbuf the relatif buffer
    /// @param size the buffer size
    virtual long tosbuf (t_byte* rbuf, const long size) const;

    /// fill a large buffer with an unsigned representation
    /// @param rbuf the relatif buffer
    /// @param size the buffer size
    virtual long tobyte (t_byte* rbuf, const long size) const;

    /// @return a hexadecimal representation
    String tohexa (void) const override;

    /// @return a hexadecimal string
    String tohstr (void) const override;

    /// @return an integer representation
    t_long tolong (void) const;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;
    
    /// @return the viewable data
    const t_byte* tobyte (void) const override;
    
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
    
    /// set an object to this relatif
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
