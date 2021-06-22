// ---------------------------------------------------------------------------
// - Byte.hpp                                                                -
// - standard object library - byte class definition                         -
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

#ifndef AFNIX_BYTE_HPP
#define AFNIX_BYTE_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

namespace afnix {

  /// The Byte class is the object version of the basic 8-bit byte. The Byte 
  /// class support the serialization mechanism but is not a literal object.
  /// The reason that it is not a literal object is that the byte object cannot
  /// be constructed by the reader and thus does not have a literal
  /// representation. In order to operate with a literal, the byte must be 
  /// converted to a character object.
  /// @author amaury darsch

  class Byte: public virtual Serial {
  private:
    /// the byte value
    t_byte d_value;

  public:
    /// create a new default byte
    Byte (void);

    /// create a new byte from a character
    /// @param value the value to create
    Byte (const char value);

    /// create a new byte from a byte
    /// @param value the value to create
    Byte (const t_byte value);

    /// copy constructor for this byte class
    /// @param that the byte class to copy
    Byte (const Byte& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the byte did
    t_word getdid (void) const override;

    /// @return the byte sid
    t_word getsid (void) const override;

    /// serialize this byte to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a byte from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// @return a native byte
    virtual t_byte tobyte (void) const;

    /// @return a native quad
    virtual t_quad toquad (void) const;

    /// @return a hexadecimal representation
    virtual String tohexa (void) const;

    /// @return a hexadecimal string
    virtual String tohstr (void) const;

    /// assign a byte with a native value
    /// @param value the value to assign
    Byte& operator = (const char value);

    /// assign a byte to this byte
    /// @param that the byte to assign
    Byte& operator = (const Byte& that);

    /// add a byte with a native value
    /// @param value the value to add
    Byte operator + (const long value) const;

    /// sub a byte with a native value
    /// @param value the value to add
    Byte operator - (const long value) const;

    /// add a native value to this byte
    /// @param value the value to add
    Byte& operator += (const long value);

    /// sub a native value to this byte
    /// @param value the value to add
    Byte& operator -= (const long value);

    /// prefix add one to the byte
    Byte& operator ++ (void);

    /// postfix add one to the byte
    Byte operator ++ (int);

    /// prefix sub one to the byte
    Byte& operator -- (void);

    /// postfix sub one to the byte
    Byte operator -- (int);

    /// compare this byte with a native value
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const char value) const;

    /// compare this byte with a native value
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const char value) const;

    /// compare two bytes
    /// @param value the value to compare
    /// @return true if they are equals
    bool operator == (const Byte& value) const;

    /// compare two bytes
    /// @param value the value to compare
    /// @return true if they are not equals
    bool operator != (const Byte& value) const;

    /// compare two bytes
    /// @param value the value to compare
    /// @return true if they are less
    bool operator < (const Byte& value) const;

    /// compare two bytes
    /// @param value the value to compare
    /// @return true if they are less or equal
    bool operator <= (const Byte& value) const;

    /// compare two bytes
    /// @param value the value to compare
    /// @return true if they are greater
    bool operator > (const Byte& value) const;

    /// compare two bytes
    /// @param value the value to compare
    /// @return true if they are greater or equal    
    bool operator >= (const Byte& value) const;

    /// @return true if the byte is an eos
    virtual bool iseos (void) const;

    /// @return true if the byte is nil
    virtual bool isnil (void) const;

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

    /// set an object to this byte
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
