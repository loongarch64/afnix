// ---------------------------------------------------------------------------
// - Algebra.cpp                                                             -
// - afnix:mth module - algebraic algorithm implementation                   -
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

#include "Real.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Algebra.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                        -
  // -------------------------------------------------------------------------

  // this procedure compute the absolute value
  static inline t_real abs (const t_real x) {
    return x < 0.0 ? -x : x;
  }

  // -------------------------------------------------------------------------
  // - vector public section                                                 -
  // -------------------------------------------------------------------------

  // check if a vector is nil

  bool Algebra::isnil (const Ivi& v) {
    // check size compatibility
    t_long size = v.getsize ();
    for (t_long k = 0L; k < size; k++) {
      if (v.nlget (k) != 0) return false;
    }
    return true;
  }

  // check if a vector is nil

  bool Algebra::isnil (const Nvi& v) {
    // check size compatibility
    t_long size = v.getsize ();
    for (t_long k = 0L; k < size; k++) {
      if (v.nlget(k).iszero () == false) return false;
    }
    return true;
  }
  
  // check if a vector is nil

  bool Algebra::isnil (const Rvi& v) {
    // check size compatibility
    t_long size = v.getsize ();
    for (t_long k = 0L; k < size; k++) {
      if (Math::acmp (v.nlget (k), 0.0) == false) return false;
    }
    return true;
  }
  
  // copy a vector into another one

  void Algebra::cpy (Ivi& r, const Ivi& x) {
    // check size compatibility
    t_long size = r.getsize ();
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector copy");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i));
  }
  
  // copy a vector into another one

  void Algebra::cpy (Rvi& r, const Rvi& x) {
    // check size compatibility
    t_long size = r.getsize ();
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector copy");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i));
  }
  
  // compare two vectors

  bool Algebra::eql (const Ivi& x, const Ivi& y) {
    // check size compatibility
    t_long size = x.getsize ();
    if (y.getsize () != size) {
      throw Exception ("vector-error",
		       "incompatible vector size with compare");
    }
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < size; i++) {
      long xi = x.nlget (i);
      long yi = y.nlget (i);
      if (xi != yi) result &= false;
    }
    return result;
  }
  
  // compare two vectors

  bool Algebra::eql (const Rvi& x, const Rvi& y) {
    // check size compatibility
    t_long size = x.getsize ();
    if (y.getsize () != size) {
      throw Exception ("vector-error",
		       "incompatible vector size with compare");
    }
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < size; i++) {
      t_real xi = x.nlget (i);
      t_real yi = y.nlget (i);
      if (xi != yi) result &= false;
    }
    return result;
  }
  
  // compare two vectors by precision

  bool Algebra::cmp (const Rvi& x, const Rvi& y) {
    // check size compatibility
    t_long size = x.getsize ();
    if (y.getsize () != size) {
      throw Exception ("vector-error",
		       "incompatible vector size with compare");
    }
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < size; i++) {
      t_real xi = x.nlget (i);
      t_real yi = y.nlget (i);
      if (Math::acmp (xi, yi) == false) result &= false;
    }
    return result;
  }

  // compute the vector dot product

  t_real Algebra::dot (const Rvi& x, const Rvi& y) {
    // check size compatibility
    t_long size = x.getsize ();
    if (y.getsize () != size) {
      throw Exception ("vector-error", 
		       "incompatible vector size with dot product");
    }
    // loop in locked mode
    t_real result = 0.0;
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < size; i++) {
      result += x.nlget (i) * y.nlget (i);
    }
    return result;
  }

  // compute the vector dot with Kahan's algorithm

  t_real Algebra::kdot (const Rvi& x, const Rvi& y) {
    // check size compatibility
    t_long size = x.getsize ();
    if (y.getsize () != size) {
      throw Exception ("vector-error", 
		       "incompatible vector size with kahan dot product");
    }
    // initialize result
    t_real result = 0.0;
    // loop in locked mode with kahan algorithm
    volatile t_real c = 0.0;
    for (t_long i = 0; i < size; i++) {
      volatile t_real d = x.nlget (i) * y.nlget (i) - c;
      volatile t_real t = result + d;
      c = (t - result) - d;
      result = t;
    }
    return result;
  }

  // compute the vector norm

  t_real Algebra::norm (const Nvi& x) {
    // check size compatibility
    t_long size = x.getsize ();
    // loop in locked mode
    t_real result = 0.0;
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < size; i++) {
      t_real xi = x.nlget(i).toreal();
      result += xi * xi;
    }
    return Math::sqrt(result);
  }

  // compute the vector norm

  t_real Algebra::norm (const Rvi& x) {
    // check size compatibility
    t_long size = x.getsize ();

    // loop in locked mode
    t_real result = 0.0;
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < size; i++) {
      t_real xi = x.nlget (i);
      result += xi * xi;
    }
    return Math::sqrt(result);
  }

  // add a vector with a scalar

  void Algebra::add (Ivi& r, const Ivi& x, const long s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i) + s);
  }

  // add a vector with a scalar

  void Algebra::add (Rvi& r, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i) + s);
  }

  // add a vector with another one

  void Algebra::add (Ivi& r, const Ivi& x, const Ivi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) + y.nlget (i));
    }
  }
  
  // add a vector with another one

  void Algebra::add (Rvi& r, const Rvi& x, const Rvi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) + y.nlget (i));
    }
  }

  // add a vector with another scaled one

  void Algebra::add (Ivi& r, const Ivi& x, const Ivi& y, const long s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) + (y.nlget (i) * s));
    }
  }
  
  // add a vector with another scaled one

  void Algebra::add (Rvi& r, const Rvi& x, const Rvi& y, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) + (y.nlget (i) * s));
    }
  }
  
  // substract a vector with a scalar
  
  void Algebra::sub (Ivi& r, const Ivi& x, const long s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector sub");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i) - s);
  }

  // substract a vector with a scalar
  
  void Algebra::sub (Rvi& r, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector sub");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i) - s);
  }

  // substract a vector with another one

  void Algebra::sub (Ivi& r, const Ivi& x, const Ivi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector sub");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) - y.nlget (i));
    }
  }
  
  // substract a vector with another one

  void Algebra::sub (Rvi& r, const Rvi& x, const Rvi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector sub");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) - y.nlget (i));
    }
  }
  
  // multiply a vector with a scalar
  
  void Algebra::mul (Ivi& r, const Ivi& x, const long s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector mul");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i) * s);
  }
  
  // multiply a vector with a scalar
  
  void Algebra::mul (Rvi& r, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector mul");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) r.nlset (i, x.nlget (i) * s);
  }
  
  // multiply a vector with another one

  void Algebra::mul (Ivi& r, const Ivi& x, const Ivi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector mul");
    }
    if (size == 0) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) * y.nlget (i));
    }
  }

  // multiply a vector with another one

  void Algebra::mul (Rvi& r, const Rvi& x, const Rvi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector mul");
    }
    if (size == 0) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) * y.nlget (i));
    }
  }

  // divide a vector with another one

  void Algebra::div (Rvi& r, const Rvi& x, const Rvi& y) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if ((x.getsize () != size) || (y.getsize () != size)) {
      throw Exception ("algebra-error", "incompatible size in vector div");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, x.nlget (i) / y.nlget (i));
    }
  }
  
  // add equal with a vector
  
  void Algebra::aeq (Ivi& r, const Ivi& x) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, r.nlget (i) + x.nlget (i));
    }
  }
  
  // add equal with a vector
  
  void Algebra::aeq (Rvi& r, const Rvi& x) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector add");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, r.nlget (i) + x.nlget (i));
    }
  }
  
  // add equal with a scaled vector
  
  void Algebra::aeq (Ivi& r, const Ivi& x, const long s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector aeq");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, r.nlget (i) + (x.nlget (i) * s));
    }
  }

  // add equal with a scaled vector
  
  void Algebra::aeq (Rvi& r, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector aeq");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, r.nlget (i) + (x.nlget (i) * s));
    }
  }

  // rescale equal with a vector

  void Algebra::req (Ivi& r, const Ivi& x, const long s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector req");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, (r.nlget (i) * s) + x.nlget (i));
    }
  }
  
  // rescale equal with a vector

  void Algebra::req (Rvi& r, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector req");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, (r.nlget (i) * s) + x.nlget (i));
    }
  }
  
  // normalize a vector

  void Algebra::normalize (Rvi& r, const Rvi& x) {
    // extract operating size
    t_long size = r.getsize ();
    // check target size
    if (x.getsize () != size) {
      throw Exception ("algebra-error", "incompatible size in vector req");
    }
    // get the vector norm
    t_real n = x.norm ();
    if (n == 0.0) {
      r.clear ();
      return;
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      r.nlset (i, (x.nlget (i) / n));
    }
  }
  
  // -------------------------------------------------------------------------
  // - matrix public section                                                 -
  // -------------------------------------------------------------------------

  // copy a matrix into another one

  void Algebra::cpy (Rmi& mr, const Rmi& mx) {
    // extract operating size
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check target size
    if ((mx.getrsiz () != rsiz) || (mx.getcsiz () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix cpy");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real s = mx.nlget (i, j);
	mr.nlset (i, j, s);
      }
    }
  }

  // copy a matrix row into a vector

  void Algebra::cpr (Rvi& r, const Rmi& m, const t_long row) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((r.getsize () != csiz) || (row < 0LL) || (row >= rsiz)) {
      throw Exception ("algebra-error", "incompatible size in row copy");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0; k < csiz; k++) r.nlset (k, m.nlget (row, k));
  }

  // copy a vector as a matrix row

  void Algebra::cpr (Rmi& m, const Rvi& x, const t_long row) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((x.getsize () != csiz) || (row < 0LL) || (row >= rsiz)) {
      throw Exception ("algebra-error", "incompatible size in row copy");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0; k < csiz; k++) m.nlset (row, k, x.nlget (k));
  }

  // copy a matrix column into a vector

  void Algebra::cpc (Rvi& r, const Rmi& m, const t_long col) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((r.getsize () != rsiz) || (col < 0LL) || (col >= csiz)) {
      throw Exception ("algebra-error", "incompatible size in column copy");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0; k < rsiz; k++) r.nlset (k, m.nlget (k, col));
  }

  // copy a vector as matrix column

  void Algebra::cpc (Rmi& m, const Rvi& x, const t_long col) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((x.getsize () != rsiz) || (col < 0LL) || (col >= csiz)) {
      throw Exception ("algebra-error", "incompatible size in column copy");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0; k < rsiz; k++) m.nlset (k, col, x.nlget (k));
  }
  
  // clear the matrix lower triangular part

  void Algebra::clt (Rmi& m) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // nullify the lower part
    for (t_long i = 0LL; i < rsiz; i++) {
      for (t_long j = 0LL; j < csiz; j++) {
	if (j < i) m.nlset (i, j, 0.0);
      }
    }
  }

  // compare two matrices

  bool Algebra::eql (const Rmi& mx, const Rmi& my) {
    // extract operating size
    t_long rsiz = mx.getrsiz ();
    t_long csiz = mx.getcsiz ();
    // check target size
    if ((my.getrsiz () != rsiz) || (my.getcsiz () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix compare");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return true;
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real xij = mx.nlget (i, j);
	t_real yij = my.nlget (i, j);
	if (xij!= yij) result &= false;
      }
    }
    return result;
  }
  
  // compare two matrices by precision

  bool Algebra::cmp (const Rmi& mx, const Rmi& my) {
    // extract operating size
    t_long rsiz = mx.getrsiz ();
    t_long csiz = mx.getcsiz ();
    // check target size
    if ((my.getrsiz () != rsiz) || (my.getcsiz () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix compare");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return true;
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real xij = mx.nlget (i, j);
	t_real yij = my.nlget (i, j);
	if (Math::acmp (xij, yij) == false) result &= false;
      }
    }
    return result;
  }

  // check if a matrix row is null

  bool Algebra::isnrow (const Rmi& m, const t_long row) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check for null
    if ((rsiz == 0LL) && (csiz == 0LL)) return true;
    // check valid row
    if ((row < 0LL) || (row >= rsiz)) {
      throw Exception ("algebra-error", "invalid row index in isnrow");
    }
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long j = 0; j < csiz; j++) {
      t_real val = m.nlget (row, j);
      if (val != 0.0) result &= false;
    }
    return result;
  }

  // check if a matrix column is null

  bool Algebra::isncol (const Rmi& m, const t_long col) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check for null
    if ((rsiz == 0LL) && (csiz == 0LL)) return true;
    // check valid row
    if ((col < 0LL) || (col >= csiz)) {
      throw Exception ("algebra-error", "invalid column index in isncol");
    }
    bool result = true;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      t_real val = m.nlget (i, col);
      if (val != 0.0) result &= false;
    }
    return result;
  }

  // compute the frobenius norm of a matrix

  t_real Algebra::norm (const Rmi& m) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // loop in locked mode
    t_real sum = 0.0;
    #ifdef _OPENMP
    #pragma omp parallel for private(sum)
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real mij = m.nlget (i, j);
	sum += (mij * mij);
      }
    }
    t_real result = Math::sqrt (sum);
    return result;
  }

  // check if a matrix has null diagonal elements

  bool Algebra::nulld (const Rmi& m) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    bool result = false;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for private(result)
    #endif
    for (t_long k = 0LL; k < rsiz; k++) {
      if ((k < csiz) && (m.nlget (k,k) == 0.0)) result |= true;
    }
    return result;
  }
  
  // add a matrix with another one

  void Algebra::add (Rmi& mr, const Rmi& mx, const Rmi& my) {
    // extract operating size
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check target size
    if ((mx.getrsiz () != rsiz) || (my.getrsiz () != rsiz) ||
	(mx.getcsiz () != csiz) || (my.getcsiz () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix add");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real s = mx.nlget (i, j) + my.nlget (i, j);
	mr.nlset (i, j, s);
      }
    }
  }

  // substract a matrix with another one

  void Algebra::sub (Rmi& mr, const Rmi& mx, const Rmi& my) {
    // extract operating size
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check target size
    if ((mx.getrsiz () != rsiz) || (my.getrsiz () != rsiz) ||
	(mx.getcsiz () != csiz) || (my.getcsiz () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix sub");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real s = mx.nlget (i, j) - my.nlget (i, j);
	mr.nlset (i, j, s);
      }
    }
  }
  
  // multiply two vectors as a matrix

  void Algebra::mul (Rmi& m, const Rvi& u, const Rvi& v) {
    // extract operating size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((u.getsize () != rsiz) || (v.getsize () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix mul");
    }
    if ((rsiz == 0LL) || (csiz == 0LL)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0LL; i < rsiz; i++) {
      t_real ui = u.nlget (i);
      for (t_long j = 0LL; j < csiz; j++) {
	t_real vj = v.nlget (j);
	m.nlset (i, j, ui * vj);
      }
    }
  }

  // multiply a matrix with a vector and a scaling factor

  void Algebra::mul (Rvi& r, const Rmi& m, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((size != rsiz) || (x.getsize () != csiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix mul");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      t_real v = 0.0;
      for (t_long j = 0; j < csiz; j++) v += m.nlget (i, j) * x.nlget (j);
      r.nlset (i, v*s);
    }
  }
  
  // multiply two matrices
  
  void Algebra::mul (Rmi& mr, const Rmi& mx, const Rmi& my) {
    // extract operating size
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    t_long size = my.getrsiz ();
    // check target size
    if ((mx.getrsiz () != rsiz) || (my.getcsiz () != csiz) ||
	(mx.getcsiz () != size)) {
      throw Exception ("algebra-error", "incompatible size in matrix mul");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real s = 0.0;
	for (t_long k = 0; k < size; k++) 
	  s += mx.nlget (i, k) * my.nlget (k, j);
	mr.nlset (i, j, s);
      }
    }
  }
  
  // multiply a transposed matrix with a vector and a scaling factor

  void Algebra::tmul (Rvi& r, const Rmi& m, const Rvi& x, const t_real s) {
    // extract operating size
    t_long size = r.getsize ();
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    // check target size
    if ((size != csiz) || (x.getsize () != rsiz)) {
      throw Exception ("algebra-error", "incompatible size in matrix tmul");
    }
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < csiz; i++) {
      t_real v = 0.0;
      for (t_long j = 0; j < rsiz; j++) v += m.nlget (j, i) * x.nlget (j);
      r.nlset (i, v*s);
    }
  }

  // add equal a matrix with a scaled vector by column position

  void Algebra::cva (Rmi& m, const Rvi& v, const t_real s, const t_long col) {
    // check target size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    if ((rsiz != v.getsize ()) || (col < 0LL) || (col >= csiz)) {
      throw Exception ("algebra-error", "inconsistent size in cva");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0; k < rsiz; k++) {
      // get the value to update
      t_real rval = m.nlget (k, col) + s * v.get (k);
      // update the matrix
      m.nlset (k, col, rval);
    }
  }

  // add equal a matrix with a scaled vector by row position

  void Algebra::rva (Rmi& m, const Rvi& v, const t_real s, const t_long row) {
    // check target size
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getcsiz ();
    if ((csiz != v.getsize ()) || (row < 0LL) || (row >= rsiz)) {
      throw Exception ("algebra-error", "inconsistent size in rva");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0; k < csiz; k++) {
      // get the value to update
      t_real rval = m.nlget (row, k) + s * v.get (k);
      // update the matrix
      m.nlset (row, k, rval);
    }
  }

  // initialize an identity matrix

  void Algebra::eye (Nmi& mr) {
    // extract operating size
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    t_long size = (rsiz <= csiz) ? rsiz : csiz;
    // reset and set
    mr.clear ();
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0L; k < size; k++) mr.nlset (k, k, 1.0);
  }
  
  // initialize an identity matrix

  void Algebra::eye (Rmi& mr) {
    // extract operating size
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    t_long size = (rsiz <= csiz) ? rsiz : csiz;
    // reset and set
    mr.clear ();
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = 0L; k < size; k++) mr.nlset (k, k, 1.0);
  }

  // generate a random vector
  
  void Algebra::random (Ivi& r, const long rmin, const long rmax) {
    // check size
    t_long size = r.getsize ();
    if (size == 0) return;
    // check bound
    if ((rmin < 0L) || (rmax < 0L) || (rmax < rmin)) {
      throw Exception ("vector-error", "invalid random ordering");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      long rval = rmin + Utility::longrnd (rmax - rmin);
      r.nlset (i, rval);
    }
  }

  // generate a random vector
  
  void Algebra::random (Nvi& r, const Numeral& rmin, const Numeral& rmax) {
    // check size
    t_long size = r.getsize ();
    if (size == 0) return;
    // check for integer numeral
    if (rmin.getnumt () == Numeral::NUMT_SINT) {
      // check bound type
      if (rmax.getnumt() != Numeral::NUMT_SINT) {
	throw Exception ("vector-error", "invalid numeral bound type");
      }
      // collect bounds
      long nmin = rmin.tolong();
      long nmax = rmax.tolong();
      // check bound
      if ((nmin < 0L) || (nmax < 0L) || (nmax < nmin)) {
	throw Exception ("vector-error", "invalid random ordering");
      }
      // loop in locked mode
      #ifdef _OPENMP
      #pragma omp parallel for
      #endif
      for (t_long i = 0; i < size; i++) {
	long rval = nmin + Utility::longrnd (nmax - nmin);
	r.nlset (i, Numeral((t_long) rval));
      }
    }
    // check for float numeral
    if (rmin.getnumt () == Numeral::NUMT_SFLT) {
      // check bound type
      if (rmax.getnumt() != Numeral::NUMT_SFLT) {
	throw Exception ("vector-error", "invalid numeral bound type");
      }
      // collect bounds
      t_real nmin = rmin.toreal();
      t_real nmax = rmax.toreal();
      // check bound
      if ((nmin < 0.0) || (nmax < 0.0) || (nmax < nmin)) {
	throw Exception ("vector-error", "invalid random ordering");
      }
      // loop in locked mode
      #ifdef _OPENMP
      #pragma omp parallel for
      #endif
      for (t_long i = 0; i < size; i++) {
	t_real rval = nmin + Utility::realrnd (true) * (nmax - nmin);
	r.nlset (i, Numeral (rval));
      }
    }
  }

  // generate a random vector
  
  void Algebra::random (Rvi& r, const t_real rmin, const t_real rmax) {
    // check size
    t_long size = r.getsize ();
    if (size == 0) return;
    // check bound
    if ((rmin < 0.0) || (rmax < 0.0) || (rmax < rmin)) {
      throw Exception ("vector-error", "invalid random ordering");
    }
    // loop in locked mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < size; i++) {
      t_real rval = rmin + Utility::realrnd (true) * (rmax - rmin);
      r.nlset (i, rval);
    }
  }

  // generate a random matrix

  void Algebra::random (Nmi& mr, const Numeral& rmin, const Numeral& rmax) {
    // extract rsiz and csiz
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // check for integer numeral
    if (rmin.getnumt () == Numeral::NUMT_SINT) {
      // check bound type
      if (rmax.getnumt() != Numeral::NUMT_SINT) {
	throw Exception ("vector-error", "invalid numeral bound type");
      }
      // collect bounds
      long nmin = rmin.tolong();
      long nmax = rmax.tolong();
      // check bound
      if ((nmin < 0L) || (nmax < 0L) || (nmax < nmin)) {
	throw Exception ("vector-error", "invalid random ordering");
      }
      // loop in locked mode
      for (t_long i = 0L; i < rsiz; i++) {
	for (t_long j = 0L; j < csiz; j++) {
	  long rval = nmin + Utility::longrnd (nmax - nmin);
	  mr.nlset (i, j, Numeral((t_long) rval));
	}
      }
    }
    // check for float numeral
    if (rmin.getnumt () == Numeral::NUMT_SFLT) {
      // check bound type
      if (rmax.getnumt() != Numeral::NUMT_SFLT) {
	throw Exception ("vector-error", "invalid numeral bound type");
      }
      // collect bounds
      t_real nmin = rmin.toreal();
      t_real nmax = rmax.toreal();
      // check bound
      if ((nmin < 0.0) || (nmax < 0.0) || (nmax < nmin)) {
	throw Exception ("vector-error", "invalid random ordering");
      }
      // loop in locked mode
      for (t_long i = 0L; i < rsiz; i++) {
	for (t_long j = 0L; j < csiz; j++) {
	  t_real rval = nmin + Utility::realrnd (true) * (nmax - nmin);
	  mr.nlset (i,j, Numeral(rval));
	}
      }
    }
  }

  // generate a random matrix
  
  void Algebra::random (Rmi& mr, const t_real rmin, const t_real rmax) {
    // extract rsiz and csiz
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // check bound
    if ((rmin < 0.0) || (rmax < 0.0) || (rmax < rmin)) {
      throw Exception ("matrix-error", "invalid random ordering");
    }
    // loop in locked mode
    for (t_long i = 0; i < rsiz; i++) {
      for (t_long j = 0; j < csiz; j++) {
	t_real rval = rmin + Utility::realrnd (true) * (rmax - rmin);
	mr.nlset (i, j, rval);
      }
    }
  }

  // generate a sparse random matrix

  void Algebra::sparse (Rmi& mr, const t_real rmin, const t_real rmax,
			const t_long nzsz) {
    // extract rsiz and csiz
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // check bound
    if (rmax < rmin) {
      throw Exception ("matrix-error", "invalid random ordering");
    }
    // check for valid non zero size
    if ((nzsz < 0) || (nzsz > (rsiz * csiz))) {
      throw Exception ("matrix-error", "invalid non-zero size in sparse");
    }
    // clear the matrix
    mr.clear ();
    // loop in locked mode
    for (t_long k = 0; k < nzsz; k++) {
      // generate coordinates
      t_long i = Utility::longrnd (rsiz);
      t_long j = Utility::longrnd (csiz);
      // check for non nil
      if (mr.nlget (i, j) != 0.0) {
	k--;
	continue;
      }
      // generate value and set
      t_real val = rmin + Utility::realrnd (true) * (rmax - rmin);
      mr.nlset (i, j, val);
    }
  }
  
  // force a matrix to be diagonaly dominant

  void Algebra::toddom (Rmi& mr, const t_real df) {
    // extract rsiz and csiz
    t_long rsiz = mr.getrsiz ();
    t_long csiz = mr.getcsiz ();
    // check for null
    if ((rsiz == 0) || (csiz == 0)) return;
    // loop in locked mode
    for (t_long i = 0; i < rsiz; i++) {
      t_real s = 0.0;
      for (t_long j = 0; j < csiz; j++) {
	if (i != j) s+= abs (mr.nlget (i,j));
      }
      if (s >= mr.nlget (i, i)) mr.nlset (i, i, s + df);
    }
  }

  // perform a givens vector update

  void Algebra::givens (Rvi& r, const t_long i, const t_long j, 
			const t_real c, const t_real s) {
    // get the vector sizes
    t_long size = r.getsize ();
    if (size == 0LL) return;
    // check coordinates
    if ((i < 0LL) || (i >= size) || (j < 0LL) || (j >= size)) {
      throw Exception ("algebra-error", "invalid givens update coordinates");
    }
    // get the vector values
    t_real vi = r.nlget (i);
    t_real vj = r.nlget (j); 
    // update the target vector
    r.nlset (j,  (c * vj) + (s * vi));
    r.nlset (i, (-s * vj) + (c * vi));
  }

  // perform a givens matrix update

  void Algebra::givens (Rmi& m, const t_long i, const t_long j, 
			const t_real c, const t_real s, const bool pflg) {
    // get the matrix sizes
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check for null
    if ((rsiz == 0LL) || (csiz == 0LL)) return;
    // check coordinates
    if ((i < 0LL) || (i >= rsiz) || (j < 0LL) || (j >= csiz)) {
      throw Exception ("algebra-error", "invalid givens update coordinates");
    }
    // select partial or full update
    t_long l = pflg ? j : 0LL;
    // loop for j to end
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long k = l; k < csiz; k++) {
      // get matrix element
      t_real mjk = m.nlget (j, k); 
      t_real mik = m.nlget (i, k);
      // update at position k
      m.nlset (j, k,  (c * mjk) + (s * mik));
      m.nlset (i, k, (-s * mjk) + (c * mik));
    }
  }

  // -------------------------------------------------------------------------
  // - extension public section                                              -
  // -------------------------------------------------------------------------

  // perform an upper triangular multiplication

  void Algebra::utmul (Rvi& r, const Rmi& m, const Rvi& x) {
    // get the matrix sizes
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check vector size
    if ((rsiz != csiz) || (r.getsize () != csiz) || (x.getsize () != csiz)) {
      throw Exception ("algebra-error", 
		       "invalid size in upper triangular multiplication");
    }
    // loop in row mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      t_real ri = 0.0;
      for (t_long j = i; j < csiz; j++) {
	ri += m.nlget (i, j) * x.nlget (j);
      }
      r.nlset (i, ri);
    }
  }

  // perform an upper triangular division

  void Algebra::utdiv (Rvi& r, const Rmi& m, const Rvi& x) {
    // get the matrix sizes
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check vector size
    if ((rsiz != csiz) || (r.getsize () != csiz) || (x.getsize () != csiz)) {
      throw Exception ("algebra-error", 
		       "invalid size in upper triangular division");
    }
    // loop in reverse mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = rsiz-1LL; i >= 0LL; i--) {
      t_real xi = x.nlget (i);
      for (t_long j = i+1LL; j < csiz; j++) {
	xi -= (m.nlget (i, j) * r.nlget (j));
      }
      xi /= m.nlget (i,i);
      r.nlset (i, xi);
    }
  }
  
  // perform a lower triangular multiplication
  
  void Algebra::ltmul (Rvi& r, const Rmi& m, const Rvi& x) {
    // get the matrix sizes
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check vector size
    if ((rsiz != csiz) || (r.getsize () != csiz) || (x.getsize () != csiz)) {
      throw Exception ("algebra-error", 
		       "invalid size in lower triangular multiplication");
    }
    // loop in row mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      t_real ri = 0.0;
      for (t_long j = 0; j <= i; j++) {
	ri += m.nlget (i, j) * x.nlget (j);
      }
      r.nlset (i, ri);
    }
  }

  // perform a lower triangular division

  void Algebra::ltdiv (Rvi& r, const Rmi& m, const Rvi& x) {
    // get the matrix sizes
    t_long rsiz = m.getrsiz ();
    t_long csiz = m.getrsiz ();
    // check vector size
    if ((rsiz != csiz) || (r.getsize () != csiz) || (x.getsize () != csiz)) {
      throw Exception ("algebra-error", 
		       "invalid size in lower triangular division");
    }
    // loop in row mode
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (t_long i = 0; i < rsiz; i++) {
      t_real xi = x.nlget (i);
      for (t_long j = 0; j < i; j++) {
	xi -= (m.nlget (i, j) * r.nlget (j));
      }
      xi /= m.nlget (i,i);
      r.nlset (i, xi);
    }
  }

  // perform a vector permutation

  void Algebra::permutate (Ivi& r, const Ivi& x, const Cpi& p) {
    // check target size
    t_long size = r.getsize ();
    if ((x.getsize () != size) || (p.getsize () != size)) {
      throw Exception ("algebra-error", "invalid size vector permutate");
    }
    // permutate the vector
    for (t_long k = 0LL; k < size; k++) {
      t_long pos = p.get (k);
      r.nlset (k, x.nlget (pos));
    }
  }

  // perform a vector permutation

  void Algebra::permutate (Rvi& r, const Rvi& x, const Cpi& p) {
    // check target size
    t_long size = r.getsize ();
    if ((x.getsize () != size) || (p.getsize () != size)) {
      throw Exception ("algebra-error", "invalid size vector permutate");
    }
    // permutate the vector
    for (t_long k = 0LL; k < size; k++) {
      t_long pos = p.get (k);
      r.nlset (k, x.nlget (pos));
    }
  }

  // perform a matrix permutation

  void Algebra::permutate (Rmi& r, const Rmi& m, const Cpi& p) {
    // check target size
    t_long rsiz = r.getrsiz ();
    t_long csiz = r.getcsiz ();
    if ((m.getrsiz () != rsiz) || (m.getcsiz () != csiz) ||
	(p.getsize () != rsiz)) {
      throw Exception ("algebra-error", "invalid size matrix permutate");
    }
    // permutate the matrix
    for (t_long row = 0LL; row < rsiz; row++) {
      t_long pos = p.get (row);
      for (t_long col = 0LL; col < csiz; col++) {
	r.nlset (row, col, m.nlget (pos, col));
      }
    }
  }

  // perform a vector reverse permutation

  void Algebra::reverse (Ivi& r, const Ivi& x, const Cpi& p) {
    // check target size
    t_long size = r.getsize ();
    if ((x.getsize () != size) || (p.getsize () != size)) {
      throw Exception ("algebra-error", "invalid size vector permutate");
    }
    // permutate the vector
    for (t_long k = 0LL; k < size; k++) {
      t_long pos = p.rget (k);
      r.nlset (k, x.nlget (pos));
    }
  }

  // perform a vector reverse permutation

  void Algebra::reverse (Rvi& r, const Rvi& x, const Cpi& p) {
    // check target size
    t_long size = r.getsize ();
    if ((x.getsize () != size) || (p.getsize () != size)) {
      throw Exception ("algebra-error", "invalid size vector permutate");
    }
    // permutate the vector
    for (t_long k = 0LL; k < size; k++) {
      t_long pos = p.rget (k);
      r.nlset (k, x.nlget (pos));
    }
  }

  // perform a matrix reverse permutation

  void Algebra::reverse (Rmi& r, const Rmi& m, const Cpi& p) {
    // check target size
    t_long rsiz = r.getrsiz ();
    t_long csiz = r.getcsiz ();
    if ((m.getrsiz () != rsiz) || (m.getcsiz () != csiz) ||
	(p.getsize () != rsiz)) {
      throw Exception ("algebra-error", "invalid size matrix permutate");
    }
    // permutate the matrix
    for (t_long row = 0LL; row < rsiz; row++) {
      t_long pos = p.rget (row);
      for (t_long col = 0LL; col < csiz; col++) {
	r.nlset (row, col, m.nlget (pos, col));
      }
    }
  }
}
