// ---------------------------------------------------------------------------
// - ArgsList.hpp                                                            -
// - afnix engine - argument list class definition                           -
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

#ifndef  AFNIX_ARGSLIST_HPP
#define  AFNIX_ARGSLIST_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The ArgsList class implements the functionnality of a quark vector
  /// that is similar to the QuarkArray class. The class adds a boolean
  /// array that controls the constant flags of the arguments.
  /// @author amaury darsch

  class ArgsList {
  private:
    /// current size
    long  d_length;
    /// the size of allocation
    long  d_size;
    /// the quark array
    long* p_array;
    /// the const array
    bool* p_const;

  public:
    /// create an empty argument list
    ArgsList (void);

    /// destroy this argument list
    ~ArgsList (void);

    /// reset this quark array
    void reset (void);

    /// @return the number of arguments
    long length (void) const;

    /// @return true if the quark exists
    bool exists (const long quark) const;

    /// add a new argument in this argument list
    /// @param quark the quark to add
    /// @param cflag the const flag
    void add (const long quark, const bool cflag);

    /// get a quark at a certain index
    /// @param index the vector index
    long getquark (const long index) const;

    /// get a quark flag at a certain index
    /// @param index the vector index
    bool getconst (const long index) const;

    /// get a quark index by quark or return -1
    /// @param quark the quark to search
    long find (const long quark) const;

    /// get a quark index by quark or throw an exception
    /// @param quark the quark to search
    long lookup (const long quark) const;

    /// @return true if a quark is const
    bool isconst (const long quark) const;

  private:
    // make the copy constructor private
    ArgsList (const ArgsList&);
    // make the assignment operator private
    ArgsList& operator = (const ArgsList&);
  };
}

#endif
