// ---------------------------------------------------------------------------
// - ArgsList.cpp                                                            -
// - afnix engine - argument list class implementation                       -
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

#include "ArgsList.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty argument list

  ArgsList::ArgsList (void) {
    p_array  = nullptr;
    p_const  = nullptr;
    reset ();
  }
  
  // destroy this argument list
  
  ArgsList::~ArgsList (void) {
    delete [] p_array;
    delete [] p_const;
  }

  // reset this argument list

  void ArgsList::reset (void) {
    delete [] p_array;
    delete [] p_const;
    d_size   = 0;
    p_array  = nullptr;
    p_const  = nullptr;
    d_length = 0;
  }

  // return the number of arguments

  long ArgsList::length (void) const {
    return d_length;
  }

  // check that a quark exists in this list

  bool ArgsList::exists (const long quark) const {
    if (d_length == 0) return false;
    for (long i = 0; i < d_length; i++)
      if (p_array[i] == quark) return true;
    return false;
  }

  // add a new argument in this argument list
  
  void ArgsList::add (const long quark, const bool cflag) {
    // check if we have to resize the array
    if (d_length >= d_size) {
      long  size  = (d_size <= 0) ? 1 : d_size * 2;
      long* array = new long[size];
      bool* cdata = new bool[size];
      for (long i = 0; i < d_length; i++) {
	array[i] = p_array[i];
	cdata[i] = p_const[i];
      }
      delete [] p_array;
      delete [] p_const;
      d_size  = size;
      p_array = array;
      p_const = cdata;
    }
    p_array[d_length] = quark;
    p_const[d_length++] = cflag;
  }

  // get a quark at a certain position

  long ArgsList::getquark (const long index) const {
    // check that we are bounded
    if ((index < 0) || (index >= d_length)) {
      throw Exception ("index-error","invalid argument index");
    }
    // get the quark
    return p_array[index];
  }

  // get a quark flag at a certain position

  bool ArgsList::getconst (const long index) const {
    // check that we are bounded
    if ((index < 0) || (index >= d_length)) {
      throw Exception ("index-error","invalid argument index");
    }
    // get the flag
    return p_const[index];
  }
  
  // find the index of a quark or -1

  long ArgsList::find (const long quark) const {
    for (long i = 0; i < d_length; i++) {
      if (p_array[i] == quark) return i;
    }
    return -1;
  }

  // find the index of a quark or throw an exception

  long ArgsList::lookup (const long quark) const {
    for (long i = 0; i < d_length; i++) {
      if (p_array[i] == quark) return i;
    }
    throw Exception ("quark-error","quark not found with mapping",
		     String::qmap (quark));
  }

  // return true if the quark is const

  bool ArgsList::isconst (const long quark) const {
    for (long i = 0; i < d_length; i++) {
      if (p_array[i] == quark) return p_const[i];
    }
    return false;
  }
}
