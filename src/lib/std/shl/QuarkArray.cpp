// ---------------------------------------------------------------------------
// - QuarkArray.cpp                                                          -
// - standard object library - quark dynamic array class implementation      -
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

#include "Exception.hpp"
#include "QuarkArray.hpp"
#include "array.tcc"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default quark array

  QuarkArray::QuarkArray (void) {
    p_lary = new t_array<long>;
  }
  
  // create a quark array with an original size

  QuarkArray::QuarkArray (const long size) {
    if (size < 0) {
      throw Exception ("array-error", "invalid negative quark array size");
    }
    p_lary = new t_array<long>(size);
  }

  // copy constructor for this quark array

  QuarkArray::QuarkArray (const QuarkArray& that) {
    p_lary = new t_array<long> (*p_lary);
  }

  // destroy this quark array

  QuarkArray::~QuarkArray (void) {
    delete p_lary;
  }
  
  // reset this quark array

  void QuarkArray::reset (void) {
    p_lary->reset ();
  }

  // return the number of elements

  long QuarkArray::length (void) const {
    return p_lary->length ();
  }

  // add a new element in this quark array
  
  void QuarkArray::add (const long quark) {
    p_lary->push (quark);
  }

  // set a quark at a certain position

  void QuarkArray::set (const long index, const long quark) {
    // check that we are bounded
    if (index >= p_lary->length()) {
      throw Exception ("index-error","out-of-bound array index in array set");
    }
    // set the object
    p_lary->set (index, quark);
  }
  
  // get a quark at a certain position

  long QuarkArray::get (const long index) const {
    // check that we are bounded
    if ((index < 0) || (index >= p_lary->length())) {
      throw Exception ("index-error","out-of-bound array index in quark array");
    }
    // get the object
    return p_lary->get (index);
  }

  // check that a quark exists in this array

  bool QuarkArray::exists (const long quark) const {
    long alen = p_lary->length ();
    if (alen == 0) return false;
    for (long k = 0L; k < alen; k++) {
      if (p_lary->get(k) == quark) return true;
    }
    return false;
  }
  
  // return the index of a quark or -1

  long QuarkArray::find (const long quark) const {
    long alen = p_lary->length ();
    for (long k = 0L; k < alen; k++) {
      if (p_lary->get (k) == quark) return k;
    }
    return -1L;
  }

  // return the index of a quark in this quark array

  long QuarkArray::lookup (const long quark) const {
    long index = find (quark);
    if (index == -1L) {
      throw Exception ("quark-error","quark not found by index",
		       String::qmap (quark));
    }
    return index;
  }
}
