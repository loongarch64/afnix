// ---------------------------------------------------------------------------
// - Infix.cpp                                                               -
// - afnix:mth module - infix notation oarser class implementation           -
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

// ---------------------------------------------------------------------------
// - Notes on the infix notation                                             -
// ---------------------------------------------------------------------------
// - vector syntax                                                           -
// -  v0, v1, ...                                                            -
// ---------------------------------------------------------------------------



#include "Infix.hpp"
#include "Strvec.hpp"
#include "Rvector.hpp"
#include "Utility.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // parse a string as a real vector

  Rvi* Infix::torvi (const String& s) {
    // cneck for nil first
    if (s.isnil () == true) return nullptr;
    // split the input string
    Strvec sv = Strvec::split (s, ",");
    long slen = sv.length ();
    if (slen == 0) return nullptr;
    // create a result vector
    Rvi* result = new Rvector (slen);
    for (long k = 0; k < slen; k++) {
      result->set(k, Utility::toreal (sv.get(k)));
    }
    return result;
  }

  // convert a real vector into an infix string

  String Infix::tostring (const Rvi& v) {
    // get the vector size and check for null
    long size = v.getsize();
    if (size  == 0) return "";
    // set the initial value
    String result = Utility::tostring (v.get(0));
    // loop in the vector value
    for (long k = 1; k < size; k++) {
      result += ", ";
      result += Utility::tostring (v.get(k));
    }
    return result;
  }
}

