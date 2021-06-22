// ---------------------------------------------------------------------------
// - Serial.hpp                                                              -
// - standard object library - serializable object abstract class definition -
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

#ifndef  AFNIX_SERIAL_HPP
#define  AFNIX_SERIAL_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Serial class is an abstract class that defines the object 
  /// interface for serialization. An object serialization is performed 
  /// with the "wrstream" virtual method. The deserialization is performed
  /// with the "rdstream" virtual method.
  /// @author amaury darsch

  class Serial : public virtual Object {
  public:
    /// The Array class is a array serialization class that
    /// abstract the processus of serializing a fixed array of data
    class Array {
    public:
      /// the array enumeration
      enum t_btyp {
	ATYP_BYTE, // byte array
	ATYP_BOOL, // bool array
	ATYP_LONG, // long array
	ATYP_REAL, // real array
	ATYP_RPT2  // real point 2D
      };

    private:
      /// the array type
      t_btyp d_btyp;
      /// the array data size
      long   d_size;
      /// the single data size
      long   d_dsiz;
      /// the array byte size
      long   d_bsiz;
      /// the array length
      long   d_blen;
      /// the array array
      t_byte* p_byte;

    public:
      /// create a default empty array
      Array (void);

      /// create an empty array by id
      /// @param sid the serial id
      Array (const t_word sid);

      /// create a serial array by size and type
      /// @param size the array size
      /// @param btyp the array type
      Array (const long size, const t_btyp btyp);

      /// copy construct this array
      /// @param that the array to copy
      Array (const Array& that);

      /// move construct this array
      /// @param that the array to move
      Array (Array&& that) noexcept;

      /// destroy this serial array
      ~Array (void);

      /// assign a array to this one
      /// @param that the array to assign
      Array& operator = (const Array& that);

      /// move a array to this one
      /// @param that the array to move
      Array& operator = (Array&& that) noexcept;

      /// clear the serial array
      void clear (void);

      /// @return true if the array is empty
      bool empty (void) const;

      /// @return true if the array is full
      bool full (void) const;

      /// @return the array length
      long length (void) const;

      /// add a byte to the array
      /// @param bval  the byte value
      void add (const t_byte bval);

      /// get a byte value by index
      /// @param index the array index
      t_byte getbyte (const long index) const;

      /// add a boolean to the array
      /// @param bval  the boolean value
      void add (const bool lval);

      /// get a boolean value by index
      /// @param index the array index
      bool getbool (const long index) const;

      /// add an integer to the array
      /// @param lval  the integer value
      void add (const t_long lval);

      /// get an integer value by index
      /// @param index the array index
      t_long getlong (const long index) const;

      /// add a real to the array
      /// @param rval  the real value
      void add (const t_real rval);

      /// add a real point by coordinates
      /// @param xval the x coordinate
      /// @param yval the y coordinate
      /// @param rval the real value
      void add (const t_long xval, const t_long yval, const t_real rval);

      /// get a real value by index
      /// @param index the array index
      t_real getreal (const long index) const;

      /// @return the array did
      t_word getdid (void) const;

      /// @return the array sid
      t_word getsid (void) const;

      /// serialize this array
      /// @param os the output stream to write
      void wrstream (class OutputStream& os) const;

      // deserialize this array
      /// @param ss the input stream to read
      void rdstream (class InputStream& is);
    };

  public:
    /// the deserialize object dispatcher
    using t_deod = Serial* (*) (const t_word);

    /// check if a serial dispatcher is valid
    /// @param did the dispatcher id
    static bool isvdid (const t_word did);
    
    /// add a serial dispatcher
    /// @param did the dispatcher id
    /// @param deod the object dispatcher
    static t_word addsd (const t_word did, t_deod deod);

    /// create a new serial object by dispatch/serial id
    /// @param did the dispatch id
    /// @param sid the serial id
    static Serial* newso (const t_word did, const t_word sid);
    
    /// @return true if a nil object is serialized
    static bool isnilid (class InputStream& is);

    /// write a nil id to an output stream
    /// serialize an object to an output stream
    static void wrnilid (class OutputStream& os);

    /// serialize a boolean to an output stream
    /// @param value the boolean value
    /// @param os    the output stream
    static void wrbool (const bool value, class OutputStream& os);

    /// serialize a boolean array with a array
    /// @param size the array size
    /// @param data the array data
    /// @param os   the output stream
    static void wrbool (const long size, const bool* data, OutputStream& os);

    /// deserialize a boolean
    /// @param is the input stream to read
    static bool rdbool (class InputStream& is);

    /// deserialize a boolean data array
    /// @param is the input stream
    /// @param size the array size
    static bool* rdbool (InputStream& is, const long size);

    /// serialize a byte to an output stream
    /// @param value the byte value
    /// @param os    the output stream
    static void wrbyte (const t_byte value, class OutputStream& os);

    /// deserialize a byte
    /// @param is the input stream to read
    static t_byte rdbyte (class InputStream& is);
    
    /// serialize a character to an output stream
    /// @param value the character value
    /// @param os    the output stream
    static void wrchar (const t_quad value, class OutputStream& os);

    /// deserialize a character
    /// @param is the input stream to read
    static t_quad rdchar (class InputStream& is);

    /// serialize an integer to an output stream
    /// @param value the integer value
    /// @param os    the output stream
    static void wrlong (const t_long value, class OutputStream& os);

    /// serialize an integer array with a array
    /// @param size the array size
    /// @param data the array data
    /// @param os   the output stream
    static void wrlong (const long size, const long* data, OutputStream& os);

    /// deserialize an integer
    /// @param is the input stream to read
    static t_long rdlong (class InputStream& is);

    /// deserialize an integer data array
    /// @param is the input stream
    /// @param size the array size
    static long* rdlong (InputStream& is, const long size);

    /// serialize a real to an output stream
    /// @param value the real value
    /// @param os    the output stream
    static void wrreal (const t_real value, class OutputStream& os);

    /// serialize a real array with a array
    /// @param size the array size
    /// @param data the array data
    /// @param os   the output stream
    static void wrreal (const long size, const t_real* data, OutputStream& os);

    /// deserialize a real
    /// @param is the input stream to read
    static t_real rdreal (class InputStream& is);

    /// deserialize a real data array
    /// @param is the input stream
    /// @param size the array size
    static t_real* rdreal (InputStream& is, const long size);

    /// serialize a array to an output stream
    /// @param arry the array to serialize
    /// @param os    the output stream
    static void wrarry (const Array& arry, class OutputStream& os);

    /// deserialize a array
    /// @param is the input stream to read
    static Array rdarry (class InputStream& is);

    /// identify an object
    /// @param is the input stream to read
    static String identify (class InputStream& is);

    /// deserialize an object
    /// @param is the input stream to read
    /// @return an object by deserialization
    static Object* deserialize (class InputStream& is);

  public:
    /// create a default serial object
    Serial (void) =default;
    
    /// copy move this serial object
    /// @param that the serial to move
    Serial (Serial&& that) noexcept;

    /// move an serial object into this one
    /// @param that the serial to move
    Serial& operator = (Serial&& that) noexcept;
    
    /// @return the object did
    virtual t_word getdid (void) const;

    /// @return the object sid
    virtual t_word getsid (void) const;

    /// serialize an object to an output stream
    /// @param os the output stream to write
    virtual void wrstream (class OutputStream& os) const;

    /// deserialize an object from an input stream
    /// @param is the input steam to read in
    virtual void rdstream (class InputStream& is);

    /// serialize an object with its serial id
    /// @param os the output stream to write
    virtual void serialize (class OutputStream& os) const;

    /// deserialize an object by input stream
    /// @param is the input steam to read in
    virtual void unserialize (class InputStream& is);
    
  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
    
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
