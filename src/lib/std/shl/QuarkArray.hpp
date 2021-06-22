// ---------------------------------------------------------------------------
// - Quarkarray.hpp                                                          -
// - standard object library - quark dynamic array class definition          -
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

#ifndef  AFNIX_QUARKARRAY_HPP
#define  AFNIX_QUARKARRAY_HPP

#ifndef  AFNIX_BITS_HPP
#include "Bits.hpp"
#endif

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The QuarkArray class implements a quark dynamic array which is faster 
  /// than the standard vector implementation with objects. The quark array
  /// provides the same functionality than the vector class except that it 
  /// works with quarks only.
  /// @author amaury darsch

  class QuarkArray {
  protected:
    t_array<long>* p_lary;
    
  public:
    /// create an empty quark array
    QuarkArray (void);

    /// create a quark array with a predefined size
    /// @param size the requested size  
    QuarkArray (const long size);
  
    /// copy constructor for this quark array
    /// @param that the quark array to copy
    QuarkArray (const QuarkArray& that);

    /// destroy this quark array
    virtual ~QuarkArray (void);
    
    /// reset this quark array
    virtual void reset (void);

    /// @return the number of elements
    virtual long length (void) const;

    /// add a quark in this array
    /// @param quark the quark to add
    virtual void add (const long quark);

    /// set a quark at a given position
    /// @param index the array index
    /// @param quark the quark to set
    virtual void set (const long index, const long quark);

    /// get a quark at a certain index
    /// @param index the vector index
    virtual long get (const long index) const;

    /// @return true if the quark exists
    virtual bool exists (const long quark) const;

    /// get a quark index by quark or return -1
    /// @param quark the quark to search
    virtual long find (const long quark) const;

    /// get a quark index by quark or throw an exception
    /// @param quark the quark to search
    virtual long lookup (const long quark) const;

  private:
    // make the assignment operator private
    QuarkArray& operator = (const QuarkArray&) =delete;
  };
}

#endif
