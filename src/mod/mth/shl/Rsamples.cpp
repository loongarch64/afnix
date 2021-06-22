// ---------------------------------------------------------------------------
// - Rsamples.cpp                                                            -
// - afnix:mth module - real array samples class implementation              -
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
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Rsamples.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default columns size
  static const long RS_COLS_DEF = 1;
  // the default row size
  static const long RS_SIZE_DEF = 1024;
  // the default number precision
  static const long RS_PSIZ_DEF = 0;
  // the default scientific flag
  static const long RS_SFLG_DEF = false;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default array

  Rsamples::Rsamples (void) {
    d_cols = RS_COLS_DEF;
    d_size = 0;
    d_rows = 0;
    d_psiz = RS_PSIZ_DEF;
    d_sflg = RS_SFLG_DEF;
    p_time = nullptr;
    p_data = nullptr;
  }

  // create an array by columns

  Rsamples::Rsamples (const long cols) {
    if (cols <= 0) {
      throw Exception ("rsample-error", "invalid column size");
    }
    d_cols = cols;
    d_size = 0;
    d_rows = 0;
    d_psiz = RS_PSIZ_DEF;
    d_sflg = RS_SFLG_DEF;
    p_time = nullptr;
    p_data = nullptr;
  }

  // create an array by name, info and columns

  Rsamples::Rsamples (const String& name, const String& info,
		      const long cols) : Taggable (name, info) {
    if (cols <= 0) {
      throw Exception ("rsample-error", "invalid column size");
    }
    d_cols = cols;
    d_size = 0;
    d_rows = 0;
    d_psiz = RS_PSIZ_DEF;
    d_sflg = RS_SFLG_DEF;
    p_time = nullptr;
    p_data = nullptr;
  }

  // copy construct this array

  Rsamples::Rsamples (const Rsamples& that) {
    that.rdlock ();
    try {
      // copy base object
      Taggable::operator = (that);
      // copy locally
      d_cols = that.d_cols;
      d_size = that.d_size;
      d_rows = that.d_rows;
      d_psiz = that.d_psiz;
      d_sflg = that.d_sflg;
      p_time = (that.p_time == nullptr) ? nullptr : new t_real[d_size];
      p_data = (d_size == 0) ? nullptr : new t_real*[d_size];
      // copy the array
      for (long i = 0; i < d_size; i++) {
	// copy the time stamp
	if (p_time != nullptr) p_time[i] = that.p_time[i];
	// copy the samples
	if (that.p_data[i] != nullptr) {
	  p_data[i] = new t_real[d_cols];
	  for (t_long j = 0; j < d_cols; j++) p_data[i][j] = that.p_data[i][j];
	} else p_data[i] = nullptr;
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this array samples

  Rsamples::~Rsamples (void) {
    for (t_long i = 0; i < d_size; i++) delete [] p_data[i];
    delete [] p_time;
    delete [] p_data;
  }

  // assign an array samples to this one

  Rsamples& Rsamples::operator = (const Rsamples& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // delete the old array
      for (long i = 0; i < d_size; i++) delete [] p_data[i];
      delete [] p_data; p_data = nullptr;
      // assign base object
      Taggable::operator = (that);
      // assign locally
      d_cols = that.d_cols;
      d_size = that.d_size;
      d_rows = that.d_rows;
      d_psiz = that.d_psiz;
      d_sflg = that.d_sflg;
      p_time = (that.p_time == nullptr) ? nullptr : new t_real[d_size];
      p_data = (d_size == 0) ? nullptr : new t_real*[d_size];
      // copy the array
      for (long i = 0; i < d_size; i++) {
	// copy the time stamp
	if (p_time != nullptr) p_time[i] = that.p_time[i];
	// copy the samples
	if (that.p_data[i] != nullptr) {
	  p_data[i] = new t_real[d_cols];
	  for (t_long j = 0; j < d_cols; j++) p_data[i][j] = that.p_data[i][j];
	} else p_data[i] = nullptr;
      }
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Rsamples::repr (void) const {
    return "Rsamples";
  }

  // return a clone of this object

  Object* Rsamples::clone (void) const {
    return new Rsamples (*this);
  }

  // return the serial did

  t_word Rsamples::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Rsamples::getsid (void) const {
    return SRL_RSPL_SID;
  }
 
  // serialize this object
  
  void Rsamples::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base object
      Taggable::wrstream (os);
      // write the object data
      Serial::wrlong (d_cols, os);
      Serial::wrlong (d_size, os);
      Serial::wrlong (d_rows, os);
      Serial::wrlong (d_psiz, os);
      Serial::wrbool (d_sflg, os);
      // eventually write the time stamps
      if (p_time != nullptr) {
	Serial::wrbool (true, os);
	Serial::Array arry (RS_SIZE_DEF, Serial::Array::ATYP_REAL);
	for (long row = 0; row < d_rows; row++) {
	  arry.add (p_time[row]);
	  if (arry.full () == false) continue;
	  Serial::wrarry (arry, os);
	  arry.clear ();
	}
	if (arry.empty () == false) Serial::wrarry (arry, os);
      } else {
	Serial::wrbool (false, os);
      }
      // write the data
      if (p_data != nullptr) {
	Serial::wrbool (true, os);
	long dlen = d_rows * d_cols;
	Serial::Array arry (RS_SIZE_DEF, Serial::Array::ATYP_REAL);
	for (long k = 0; k < dlen; k++) {
	  long row = k / d_cols;
	  long col = k % d_cols;
	  arry.add (p_data[row][col]);
	  if (arry.full () == false) continue;
	  Serial::wrarry (arry, os);
	  arry.clear ();
	}
	if (arry.empty () == false) Serial::wrarry (arry, os);
      } else {
	Serial::wrbool (false, os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this object

  void Rsamples::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base object
      Taggable::rdstream (is);
      // get the object data
      d_cols = Serial::rdlong (is);
      d_size = Serial::rdlong (is);
      d_rows = Serial::rdlong (is);
      d_psiz = Serial::rdlong (is);
      d_sflg = Serial::rdbool (is);
      // check for time stamps
      if (Serial::rdbool (is) == true) {
	p_time = new t_real[d_size];
	for (long i = 0; i < d_rows; i++) {
	  Array arry = Serial::rdarry (is);
	  long  blen = arry.length ();
	  for (long k = 0; k < blen; k++) p_time[i+k] = arry.getreal (k);
	  i += (blen -1L);
	}
	for (long i = d_rows; i < d_size; i++) p_time[i] = Math::CV_NAN;
      }
      // check for data
      if (Serial::rdbool (is) == true) {
	p_data = new t_real*[d_size];
	for (long i = 0; i < d_rows; i++) p_data[i] = new t_real[d_cols];
	long dlen = d_rows * d_cols;
	for (long i = 0; i < dlen; i++) {
	  Array arry = Serial::rdarry (is);
	  long  blen = arry.length ();
	  for (long k = 0; k < blen; k++) {
	    long row = (i + k) / d_cols;
	    long col = (i + k) % d_cols;
	    if (row >= d_rows) {
	      i = dlen - blen;
	      break;
	    }
	    p_data[row][col] = arry.getreal (k);
	  }
	  i += (blen - 1L);
	}
	for (long i = d_rows; i < d_size; i++) p_data[i] = nullptr;
      }      
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this array samples

  void Rsamples::clear (void) {
    wrlock ();
    try {
      for (t_long i = 0; i < d_size; i++) delete [] p_data[i];
      delete [] p_time; p_time = nullptr;
      delete [] p_data; p_data = nullptr;
      d_rows = 0;
      d_size = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of rows
  
  long Rsamples::getrows (void) const {
    rdlock ();
    try {
      long result = d_rows;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the number of colimns

  void Rsamples::setcols (const long cols) {
    wrlock ();
    try {
      // check for valid columns
      if (cols <= 0) {
	throw Exception ("rsample-error", "invalid column size");
      }
      // clear the samples
      clear ();
      // set a new columns
      d_cols = cols;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of columns
  
  long Rsamples::getcols (void) const {
    rdlock ();
    try {
      long result = d_cols;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the number precision

  void Rsamples::setpsiz (const long psiz) {
    wrlock ();
    try {
      if (psiz < 0) {
        throw Exception ("rsamples-error", "invalid negative number precision");
      }
      d_psiz = psiz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number precision

  long Rsamples::getpsiz (void) const {
    rdlock ();
    try {
      long result = d_psiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the scientific flag

  void Rsamples::setsflg (const bool sflg) {
    wrlock ();
    try {
      d_sflg = sflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  } 

  // get the scientific flag

  bool Rsamples::getsflg (void) const {
    rdlock ();
    try {
      bool result = d_sflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the samples are stamped

  bool Rsamples::stamped (void) const {
    rdlock ();
    try {
      bool result = (p_time != nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a time value by position

  t_real Rsamples::gettime (const long row) const {
    rdlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rows)) {
	throw Exception ("sample-error", "invalid row position");
      }
      t_real result = (p_time == nullptr) ? Math::CV_NAN : p_time[row];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a sample by position and value

  void Rsamples::set (const long row, const long col, const t_real val) {
    wrlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rows)) {
	throw Exception ("sample-error", "invalid row position");
      }
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("sample-error", "invalid column position");
      }
      // set sample value
      p_data[row][col] = val;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sample by position

  t_real Rsamples::get (const long row, const long col) const {
    rdlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rows)) {
	throw Exception ("sample-error", "invalid row position");
      }
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("sample-error", "invalid column position");
      }
      // get sample value
      t_real result = p_data[row][col];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new unitialized row

  long Rsamples::newrow (void) {
    wrlock ();
    try {
      // eventually resize the array
      if ((d_rows + 1) > d_size) resize ((d_size == 0) ? 1 : (d_size * 2));
      // save result and allocate
      long result = d_rows;
      p_data[d_rows++] = new t_real[d_cols];
      // initialized as nan
      for (long i = 0; i < d_cols; i++) p_data[result][i] = Math::CV_NAN;
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new uninitialized row with a time stamp

  long Rsamples::newrow (const t_real tval) {
    wrlock ();
    try {
      // allocate the new row
      long result = newrow ();
      // check for non-allocated time
      if ((p_time == nullptr) && (tval == Math::CV_NAN)) {
	unlock ();
	return result;
      }
      // eventually allocate the time stamps
      if (p_time == nullptr) {
	p_time = new t_real[d_size];
	for (long i = 0; i < d_size; i++) p_time[i] = Math::CV_NAN;
      }
      // set the time stamp
      p_time[result] = tval;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this array samples

  void Rsamples::resize (const long size) {
    wrlock ();
    try {
      // check for resizing
      if (size <= d_size) {
	unlock ();
	return;
      }
      // allocate a new time stamp
      t_real* tptr = (p_time == nullptr) ? nullptr : new t_real[size];
      // allocate new array
      t_real** data = new t_real*[size];
      // copy the data
      for (long i = 0; i < d_rows; i++) {
	if (tptr != nullptr) tptr[i] = p_time[i];
	data[i] = p_data[i];
      }
      for (long i = d_rows; i < size; i++) {
	if (tptr != nullptr) tptr[i] = Math::CV_NAN;
	data[i] = nullptr;
      }
      // clean old and adjust
      delete [] p_time;
      delete [] p_data;
      p_time = tptr;
      p_data = data;
      d_size = size;
      // that's all
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the minimum signed time

  t_real Rsamples::minst (void) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      if (p_time != nullptr) {
	for (long i = 0; i < d_rows; i++) {
	  t_real tval = p_time[i];
	  if (Math::isnan (tval) == true) continue;
	  if ((Math::isnan (result) == true) || (tval < result)) result = tval;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the maximum signed time

  t_real Rsamples::maxst (void) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      if (p_time != nullptr) {
	for (long i = 0; i < d_rows; i++) {
	  t_real tval = p_time[i];
	  if (Math::isnan (tval) == true) continue;
	  if ((Math::isnan (result) == true) || (tval > result)) result = tval;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the minimum column value

  t_real Rsamples::minsc (const long col) const {
    rdlock ();
    try {
      // check for valid column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("sample-error", "invalid column position");
      }
      t_real result = Math::CV_NAN;
      if (p_data!= nullptr) {
	for (long i = 0; i < d_rows; i++) {
	  t_real cval = p_data[i][col];
	  if (Math::isnan  (cval)   == true) continue;
	  if ((Math::isnan (result) == true) || (cval < result)) result = cval;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the maximum column value

  t_real Rsamples::maxsc (const long col) const {
    rdlock ();
    try {
      // check for valid column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("sample-error", "invalid column position");
      }
      t_real result = Math::CV_NAN;
      if (p_data != nullptr) {
	for (long i = 0; i < d_rows; i++) {
	  t_real cval = p_data[i][col];
	  if (Math::isnan (cval)    == true) continue;
	  if ((Math::isnan (result) == true) || (cval > result)) result = cval;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // smooth the sample array upto a relative error

  Rsamples* Rsamples::smooth (void) const {
    rdlock ();
    try {
      Rsamples* result = smooth (Math::d_reps);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // smooth the sample array upto a relative error

  Rsamples* Rsamples::smooth (const t_real reps) const {
    rdlock ();
    Rsamples* result = nullptr;
    try {
      // check for valid rows
      if (d_rows == 0) {
	unlock ();
	return result;
      }
      // create a result sample array
      result = new Rsamples (d_cols);
      result->d_psiz = d_psiz;
      result->d_sflg = d_sflg;
      // prepare the working columns
      t_real time = Math::CV_NAN;
      t_real data[d_cols];
      if (p_time != nullptr) time = p_time[0];
      if (p_data != nullptr) {
	for (long col = 0; col < d_cols; col++) data[col] = p_data[0][col];
      }
      // loop in the rows
      long rows = d_rows - 1;
      for (long row = 1; row < rows; row++) {
	// prepare operating row
	t_real tval = Math::CV_NAN;
	t_real dval[d_cols];
	if (p_time != nullptr) tval = p_time[row];
	if (p_data != nullptr) {
	  for (long col = 0; col < d_cols; col++) dval[col] = p_data[row][col];
	}
	// check for relative error
	bool status = 
	  ((Math::isnan (time) == true) && (Math::isnan (tval) == true)) ||
	  ((Math::isnan (time) != true) && (Math::isnan (tval) != true));
	for (long col = 0; col < d_cols; col++) {
	  if (Math::rcmp (data[col], dval[col], reps) == false) {
	    status = false;
	    break;
	  }
	}
	// check for data push
	if (status == false) {
	  long nr = result->newrow (time);
	  time = tval;
	  for (long col = 0; col < d_cols; col++) {
	    result->p_data[nr][col] = data[col];
	    data[col] = dval[col];
	  }
	} 
      }
      // push last record
      if (rows > 0) {
	time = (p_time == nullptr) ? Math::CV_NAN : p_time[rows];
	long nr = result->newrow (time);
	for (long col = 0; col < d_cols; col++) {
	  result->p_data[nr][col] = p_data[rows][col];
	}
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // create a print table with the samples
  
  PrintTable* Rsamples::toptbl (const bool tflg) const {
    rdlock ();
    PrintTable* result = nullptr;
    try {
      // compute the effective columns
      long ecol = (tflg == true) ? d_cols + 1 : d_cols;
      // create the print table
      result = (d_rows == 0) ? new PrintTable (ecol) :
	new PrintTable (ecol, d_rows);
      // set the style
      Style gstl = result->getstyle ();
      gstl.setpsiz (d_psiz);
      gstl.setsflg (d_sflg);
      result->setstyle (gstl);
      // loop in the data samples
      for (long row = 0; row < d_rows; row++) {
	// create a new row in the table
	if (result->add () != row) {
	  throw Exception ("internal-error", 
			   "inconsistent rsample row index in print-table");
	}
	// set the time if requested
	if (tflg == true) {
	  t_real tval = (p_time == nullptr) ? Math::CV_NAN : p_time[row];
	  result->set (row, 0, tval);
	  // set the samples
	  for (long col = 0; col < d_cols; col++) {
	    result->set (row, col+1, p_data[row][col]);
	  }
	} else {
	  // set the samples
	  for (long col = 0; col < d_cols; col++) {
	    result->set (row, col, p_data[row][col]);
	  }
	}
      }
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 20;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_MINST   = zone.intern ("min-signed-time");
  static const long QUARK_MAXST   = zone.intern ("max-signed-time");
  static const long QUARK_MINSC   = zone.intern ("min-signed-column");
  static const long QUARK_MAXSC   = zone.intern ("max-signed-column");
  static const long QUARK_SMOOTH  = zone.intern ("smooth");
  static const long QUARK_NEWROW  = zone.intern ("new-row");
  static const long QUARK_RESIZE  = zone.intern ("resize");
  static const long QUARK_TOPTBL  = zone.intern ("to-print-table");
  static const long QUARK_STAMPED = zone.intern ("stamped-p");
  static const long QUARK_GETTIME = zone.intern ("get-time");
  static const long QUARK_GETROWS = zone.intern ("get-rows");
  static const long QUARK_SETCOLS = zone.intern ("set-columns");
  static const long QUARK_GETCOLS = zone.intern ("get-columns");
  static const long QUARK_SETPSIZ = zone.intern ("set-number-precision");
  static const long QUARK_GETPSIZ = zone.intern ("get-number-precision");
  static const long QUARK_SETSFLG = zone.intern ("set-scientific-notation");
  static const long QUARK_GETSFLG = zone.intern ("get-scientific-notation");

  // create a new object in a generic way

  Object* Rsamples::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rsamples;
    // check for 1 argument
    if (argc == 1) {
      long cols = argv->getlong (0);
      return new Rsamples (cols);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real samples object");
  }

  // return true if the given quark is defined

  bool Rsamples::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rsamples::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_MINST)   return new Real    (minst ());
      if (quark == QUARK_MAXST)   return new Real    (maxst ());
      if (quark == QUARK_SMOOTH)  return smooth ();
      if (quark == QUARK_GETROWS) return new Integer (getrows ());
      if (quark == QUARK_GETCOLS) return new Integer (getcols ());
      if (quark == QUARK_NEWROW)  return new Integer (newrow  ());
      if (quark == QUARK_GETPSIZ) return new Integer (getpsiz ());
      if (quark == QUARK_GETSFLG) return new Boolean (getsflg ());
      if (quark == QUARK_STAMPED) return new Boolean (stamped ());
      if (quark == QUARK_TOPTBL)  return toptbl (stamped ());
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCOLS) {
        long cols = argv->getlong (0);
	setcols (cols);
	return nullptr;
      }
      if (quark == QUARK_NEWROW) {
        t_real tval = argv->getrint (0);
	return new Integer (newrow (tval));
      }
      if (quark == QUARK_RESIZE) {
        long size = argv->getlong (0);
	resize (size);
	return nullptr;
      }
      if (quark == QUARK_GETTIME) {
	long row = argv->getlong (0);
	return new Real (gettime (row));
      }
      if (quark == QUARK_TOPTBL) {
	bool tflg = argv->getbool (0);
	return toptbl (tflg);
      }
      if (quark == QUARK_MINSC) {
	long col = argv->getlong (0);
	return new Real (minsc (col));
      }
      if (quark == QUARK_MAXSC) {
	long col = argv->getlong (0);
	return new Real (maxsc (col));
      }
      if (quark == QUARK_SMOOTH) {
	t_real reps = argv->getreal (0);
	return smooth (reps);
      }
      if (quark == QUARK_SETPSIZ) {
        long psiz = argv->getlong (0);
        setpsiz (psiz);
        return nullptr;
      }
      if (quark == QUARK_SETSFLG) {
        bool sflg = argv->getbool (0);
        setsflg (sflg);
        return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_GET) {
        long row = argv->getlong (0);
        long col = argv->getlong (1);
        return new Real (get (row, col));
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
        long   row = argv->getlong (0);
        long   col = argv->getlong (1);
        t_real val = argv->getrint (2);
        set (row, col, val);
	return nullptr;
      }
    }
    // call the taggable object
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
