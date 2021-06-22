// ---------------------------------------------------------------------------
// - Vector.hpp                                                              -
// - standard object library - dynamic vector class definition               -
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

#ifndef  AFNIX_VECTOR_HPP
#define  AFNIX_VECTOR_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

#ifndef  AFNIX_COLLECTABLE_HPP
#include "Collectable.hpp"
#endif

namespace afnix {

  /// The Vector class implementes a dynamic array. It is by far less efficent
  /// than an array where the size of the array is known in advance. This
  /// class should be used when an indexed container is required.
  /// @author amaury darsch

  class Vector : public virtual Serial, public Iterable, public Collectable {
  protected:
    /// the vector size
    long d_size;
    /// the vector length
    long d_vlen;
    /// the allocated vector
    Object** p_vobj;

  public:
    /// create an empty vector.
    Vector (void);

    /// create a vector with a predefined allocated size
    /// @param size the requested size
    Vector (const long size);

    /// copy construct this vector
    /// @param that the vector to copy 
    Vector (const Vector& that);

    /// copy move this vector
    /// @param that the vector to move 
    Vector (Vector&& that) noexcept;

    /// destroy this vector
    ~Vector (void);

    /// assign a vector to this one
    /// @param that the vector to assign
    Vector& operator = (const Vector& that);

    /// move a vector to this one
    /// @param that the vector to move
    Vector& operator = (Vector&& that) noexcept;

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the vector did
    t_word getdid (void) const override;

    /// @return the vector sid
    t_word getsid (void) const override;

    /// serialize this vector to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a vector from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// release this object links
    void release (void) override;
    
    /// reset this vector
    virtual void reset (void) ;

    /// @return true if the vector is empty
    virtual bool empty (void) const;

    /// @return true if the object exists
    virtual bool exists (Object* object) const;

    /// @return the number of objects in this vector
    virtual long length (void) const;

    /// add an element in this vector
    /// @param object the object to add
    virtual void add (Object* object);

    /// add an element in this vector at a certain index
    /// @param index  the object index 
    /// @param object the object to add
    virtual void add (const long index, Object* object);

    /// set an object at a given position
    /// @param index the vector index
    /// @param object the object to set
    virtual void set (const long index, Object* object);

    /// get an object at a certain index
    /// @param index the vector index
    /// @return the object at this position
    virtual Object* get (const long index) const;

    /// @return the first object in this vector
    virtual Object* first (void) const;

    /// @return the last object in this vector
    virtual Object* last (void) const;

    /// @return the first object in the vector
    virtual Object* pop (void);

    /// pop the last object from this vector
    virtual Object* rml (void);

    /// find and object in this vector
    /// @param object the object to find
    virtual long find (Object* object);

    /// remove and object by index and repack the vector
    /// @param index the object index to remove
    virtual void remove (const long index);

    /// remove and object and repack the vector
    /// @param object the object to remove
    virtual void remove (Object* object);

    /// shift the vector by one element and return a new one
    virtual Vector* shift (void) const;

    /// merge a vector into this one
    /// @param v the vector to merge
    virtual void merge (const Vector& v);

    /// @return a long integer value by object index
    virtual t_long getlong (const long index) const;

    /// @return an octa value by object index
    virtual t_octa getocta (const long index) const;

    /// @return a real value by object index
    virtual t_real getreal (const long index) const;

    /// @return a real value by object index, even from an integer
    virtual t_real getrint (const long index) const;

    /// @return a boolean value by object index
    virtual bool getbool (const long index) const;

    /// @return a byte value by object index
    virtual t_byte getbyte (const long index) const;

    /// @return a word value by object index
    virtual t_word getword (const long index) const;

    /// @return a character value by object index
    virtual t_quad getchar (const long index) const;

    /// @return a string value by object index
    virtual String getstring (const long index) const;

  private:
    // make the vector iterator a friend
    friend class Vectorit;

  public:
    /// @return a new iterator for this vector
    Iterator* makeit (void) override;

    /// create an iterator at the begining
    class Vectorit begin (void);

    /// create an iterator at the end
    class Vectorit end (void);

  public:
    /// generate an object of evaluated arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to evaluate
    /// @return a vector of evaluated argument
    static Vector* eval (Evaluable*zobj, Nameset* nset, Cons* args);
    
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
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
  
  /// The Vectorit class is the iterator for the vector class. Such 
  /// iterator is constructed with the "makeit" vector method. The iterator
  /// is reset to the beginning of the vector.
  /// @author amaury darsch

  class Vectorit : public Iterator {
  private:
    /// the vector to iterate
    Vector* p_vobj;
    /// the iterator vector index
    long    d_vidx;

  public:
    /// create a new iterator from a vector
    /// @param vobj the vector to iterate
    Vectorit (Vector* vobj);

    /// copy construct this iterator
    /// @param that the object to copy
    Vectorit (const Vectorit& that);
    
    /// destroy this vector iterator
    ~Vectorit (void);

    /// assign an iterator to this one
    /// @param that the object to assign
    Vectorit& operator = (const Vectorit& that);

    /// compare two iterators
    /// @param it the iteraror to compare
    bool operator == (const Vectorit& it) const;

    /// compare two iterators
    /// @param it the iteraror to compare
    bool operator != (const Vectorit& it) const;

    /// move the iterator to the next position
    Vectorit& operator ++ (void);
    
    /// move the iterator to the previous position
    Vectorit& operator -- (void);
    
    /// @return the iterator object
    Object* operator * (void) const;
    
    /// @return the class name
    String repr (void) const override;

    /// reset the iterator to the begining
    void begin (void) override;

    /// reset the iterator to the end
    void end (void) override;

    /// move the vector iterator to the next position
    void next (void) override;

    /// move the vector iterator to the previous position
    void prev (void) override;

    /// @return the object at the current position
    Object* getobj (void) const override;

    /// @return true if the iterator is at the end
    bool isend (void) const override;
  };
}

#endif
