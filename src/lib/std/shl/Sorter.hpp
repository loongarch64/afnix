// ---------------------------------------------------------------------------
// - Sorter.hpp                                                              -
// - standard object library - generic quick sort class definition           -
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

#ifndef  AFNIX_SORTER_HPP
#define  AFNIX_SORTER_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

namespace afnix {

  /// The Sorter class defines a generic implementation that performs a quick
  /// sort operation. The algorithm operates on an object vector and perform
  /// the sorting operation in place. The compare function that operates
  /// between two objects must be supplied by the user.
  /// @author amaury darsch

  class Sorter : public Object {
  public:
    /// the compare function
    /// @param ref the reference object
    /// @param slv the slave object
    using t_cmpf = bool (*) (Object* ref, Object* slv);

  private:
    /// the compare function
    t_cmpf p_cmpf;

  public:
    /// create a default sorter
    Sorter (void);

    /// create a sorter with a compare function
    /// @param cmpf the compare function
    Sorter (t_cmpf cmpf);

    /// @return this class name
    String repr (void) const;

    /// set the compare function
    /// @param cmpf the compare function
    void setcmpf (t_cmpf cmpf);

    /// sort the object vector argument
    /// @param argv the argulent vector
    void qsort (Vector* argv) const;

  private:
    // make the copy constructor private
    Sorter (const Sorter&);
    // make the assignment operator private
    Sorter& operator = (const Sorter&);
  };
}

#endif
