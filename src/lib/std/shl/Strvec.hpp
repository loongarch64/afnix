// ---------------------------------------------------------------------------
// - Strvec.hpp                                                              -
// - standard object library - string vector class definition                -
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

#ifndef  AFNIX_STRVEC_HPP
#define  AFNIX_STRVEC_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

namespace afnix {

  /// The Strvec class implements a string vector facility which is faster 
  /// than the standard vector implementation with objects. The string vector
  /// provides the same functionality than the vector class except that it 
  /// works with string only. The class provides also a facility to check if a 
  /// string exists in this vector. The class provides a also a facility to
  /// split a string into multiple substrings. One thing to notice is the
  /// difference of behavior with the zero length flag, depending on the use
  /// of the break sequence.
  /// @author amaury darsch

  class Strvec : public virtual Serial, public Iterable {
  public:
    /// split a string and return a vector of string
    /// @param name the string to split
    /// @param sbrk the break character sequence
    /// @param uniq the unique flag
    /// @param zlen the zero length flag
    static Strvec split (const String& name, const String& sbrk, 
			 const bool uniq, const bool zlen);

    /// split a string and return a vector of string
    /// @param name the string to split
    /// @param sbrk the break character sequence
    /// @param uniq the unique flag
    static Strvec split (const String& name, const String& sbrk, 
			 const bool uniq);

    /// split a string with a default break sequence
    /// @param name the string to split
    /// @param sbrk the break character sequence
    static Strvec split (const String& name, const String& sbrk);

    /// split a string with a default break sequence
    /// @param name the string to split
    static Strvec split (const String& name);

  private:
    /// the size of the allocated vector
    long d_size;
    /// the uniq flag
    bool d_uniq;
    /// the zero length flag
    bool d_zlen;
    /// the current size of this vector
    long d_length;
    /// the allocated vector
    String* p_vector;

  public:
    /// create an empty string vector.
    Strvec (void);

    /// create a string vector with a predefined size
    /// @param size the requested size  
    Strvec (const long size);
  
    /// create a string vector with a uniq flag
    /// @param uniq the uniq flag 
    Strvec (const bool uniq);
  
    /// create a string vector with a predefined size and uniq flag
    /// @param size the requested size  
    /// @param uniq the uniq flag  
    Strvec (const long size, const bool uniq);
  
    /// copy constructor for this string vector
    /// @param that the string vector to copy
    Strvec (const Strvec& that);

    /// destroy this string vector
    ~Strvec (void);

    /// @return the class name
    String repr (void) const;

    /// @return the string vector did
    t_word getdid (void) const;

    /// @return the string vector sid
    t_word getsid (void) const;

    /// serialize this string vector to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a string vector from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// assign a string vector to this one
    /// @param that the string vector to assign
    Strvec& operator = (const Strvec& that);

    /// reset this string vector
    void reset (void);

    /// set the uniq flag
    /// @param uniq the uniq flag to set
    void setuniq (const bool uniq);

    /// @return the uniq flag
    bool getuniq (void) const;
   
    /// set the zero length flag
    /// @param zlen the zero length flag to set
    void setzlen (const bool zlen);

    /// @return the zero length flag
    bool getzlen (void) const;

    /// @return true if the vector is empty
    bool empty (void) const;

    /// @return true if the vector is active
    bool active (void) const;

    /// @return true if the string exists
    bool exists (const String& name) const;

    /// @return the number of elements in this string vector
    long length (void) const;

    /// add a string in this string vector
    /// @param s the string to add
    void add (const String& s);

    /// set a string at a given position
    /// @param index the vector index
    /// @param s   the string to set
    void set (const long index, const String& s);

    /// @return the first string from this vector
    String pop (void);

    /// @return the last string from this vector
    String rml (void);

    /// get a string at a certain index
    /// @param index the vector index
    /// @return the string at this position
    String get (const long index) const;

    /// @return the first string in this vector
    String first (void) const;

    /// @return the last string in this vector
    String last (void) const;
    
    /// search for a key in the string vector
    /// @param key the key to search for
    /// @return the index of the key or -1 if not found
    long find (const String& key) const;

    /// search for a key in the string vector or throw an exception
    /// @param key the key to search for
    /// @return the index of the first string which match the key
    long lookup (const String& key) const;

    /// remove an entry by index and repack the vector
    /// @param index the index to remove
    void remove (const long index);

    /// remove an entry by key and repack the vector
    /// @param key the key to remove
    void remove (const String& key);

    /// @return the maximum string length in this vector
    long maxlen (void) const;

    /// @return the minimum string length in this vector
    long minlen (void) const;

    /// concatenate the string vector without separator
    String concat (void) const;

    /// concatenate the string vector with a separator
    /// @param sc the separation character
    String concat (const t_quad sc) const;

    /// @return an array of quarks mapping this string vector
    long* toquarks (void) const;

    /// @return a vector of strings
    Vector* tovector (void) const;

    /// @return a new iterator for this vector
    Iterator* makeit (void);

  private:
    // make the string vector iterator a friend
    friend class Strvecit;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };

  /// The Strvecit class is the iterator for the vector class. Such 
  /// iterator is constructed with the "makeit" vector method. The iterator
  /// is reset to the beginning of the vector.
  /// @author amaury darsch

  class Strvecit : public Iterator {
  private:
    /// the vector to iterate
    Strvec* p_vobj;
    /// the iterable vector index
    long    d_vidx;

  public:
    /// create a new iterator from a string vector
    /// @param vobj the vector to iterate
    Strvecit (Strvec* vobj);

    /// destroy this vector iterator
    ~Strvecit (void);

    /// @return the class name
    String repr (void) const;

    /// reset the iterator to the begining
    void begin (void);

    /// reset the iterator to the end
    void end (void);

    /// move the vector iterator to the next position
    void next (void);

    /// move the vector iterator to the previous position
    void prev (void);

    /// @return the object at the current position
    Object* getobj (void) const;

    /// @return true if the iterator is at the end
    bool isend (void) const;

  private:
    // make the copy constructor private
    Strvecit (const Strvecit&);
    // make the assignment operator private
    Strvecit& operator = (const Strvecit&);
  };
}

#endif
