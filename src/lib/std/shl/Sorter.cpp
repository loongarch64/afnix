// ---------------------------------------------------------------------------
// - Sorter.cpp                                                              -
// - standard object library - generic quick sort class implementation       -
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

#include "Sorter.hpp"
#include "String.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function makes a partition of the array and returns the 
  // pivot index
  static long qsort_part (Vector* argv, Sorter::t_cmpf cmpf,
			  const long first, const long last) {
    // initialize the pivot index, last index and next (unknown) index
    long pidx = first;
    long lidx = first;
    long uidx = lidx + 1;
    // get the pivot object - and protect it
    Object* pvt = Object::iref (argv->get (pidx));
    // swap until we have reached the last element
    while (uidx <= last) {
      Object* ref = Object::iref (argv->get (uidx));
      if (cmpf (ref, pvt) == true) {
	lidx++;
	argv->set (uidx, argv->get (lidx));
	argv->set (lidx, ref);
      }
      Object::tref (ref);
      // the intel is in the other region
      uidx++;
    }
    // place the pivot in proper position
    argv->set (first, argv->get (lidx));
    argv->set (lidx,  pvt);
    Object::tref (pvt);
    // the pivot index is now the last index
    return lidx;
  }

  // this function performs a recursive quick sort
  static void qsort_vector (Vector* argv, Sorter::t_cmpf cmpf,
			    const long first, const long last) {
    if (first >= last) return;
    long pidx = qsort_part (argv, cmpf, first, last);
    qsort_vector (argv, cmpf, first, pidx - 1);
    qsort_vector (argv, cmpf, pidx + 1, last);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default sorter

  Sorter::Sorter (void) {
    p_cmpf = nullptr;
  }

  // create a sorter with a compare method

  Sorter::Sorter (t_cmpf cmpf) {
    p_cmpf = cmpf;
  }

  // return the class name

  String Sorter::repr (void) const {
    return "Sorter";
  }

  // set the compare function

  void Sorter::setcmpf (t_cmpf cmpf) {
    wrlock ();
    p_cmpf = cmpf;
    unlock ();
  }

  // quick sort method

  void Sorter::qsort (Vector* argv) const {
    // check for sorting method
    if (p_cmpf == nullptr) return;
    rdlock ();
    // sort the vector in place
    argv->wrlock ();
    try {
      long argc = (argv == nullptr) ? 0 : argv->length ();
      qsort_vector (argv, p_cmpf, 0, argc - 1);
      argv->unlock ();
      unlock ();
    } catch (...) {
      argv->unlock ();
      unlock ();
      throw;
    }
  }
}
