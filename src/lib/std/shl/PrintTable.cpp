// ---------------------------------------------------------------------------
// - PrintTable.cpp                                                          -
// - standard object library - printable table class definition              -
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

#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Loopable.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "PrintTable.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure compute the maximum of two integers
  static inline long max (const long x, const long y) {
    return (x < y) ? y : x;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default print table

  PrintTable::PrintTable (void) {
    d_size = 16;
    d_cols = 1;
    d_rows = 0;
    // initialize the data table
    p_head = new String[d_cols];
    p_foot = new String[d_cols];
    p_body = new String*[d_size];
    p_stag = nullptr;
    for (long k = 0L; k < d_size; k++) p_body[k] = nullptr;
    // initialize the column style and width
    p_cstl = new Style*[d_cols];
    p_cwth = new long[d_cols];
    for (long k = 0; k < d_cols; k++) {
      p_cstl[k] = nullptr;
      p_cwth[k] = 0L;
    }
  }

  // create a print table with a number of columns

  PrintTable::PrintTable (const long cols) {
    // check valid columns
    if (cols <= 0) {
      throw Exception ("table-error", "invalid zero column table");
    }
    d_size = 16;
    d_cols = cols;
    d_rows = 0;
    // initialize the data table
    p_head = new String[d_cols];
    p_foot = new String[d_cols];
    p_body = new String*[d_size];
    p_stag = nullptr;
    for (long k = 0L; k < d_size; k++) p_body[k] = nullptr;
    // initialize the column style and width
    p_cstl = new Style*[d_cols];
    p_cwth = new long[d_cols];
    for (long k = 0L; k < d_cols; k++) {
      p_cstl[k] = nullptr;
      p_cwth[k] = 0L;
    }
  }

  // create a print table with a number of columns and rows

  PrintTable::PrintTable (const long cols, const long rows) {
    // check valid columns
    if ((cols <= 0) || (rows <= 0)) {
      throw Exception ("table-error", "invalid zero row/column table");
    }
    d_size = rows;
    d_cols = cols;
    d_rows = 0;
    // initialize the data table
    p_head = new String[d_cols];
    p_foot = new String[d_cols];
    p_body = new String*[d_size];
    p_stag = nullptr;
    for (long k = 0L; k < d_size; k++) p_body[k] = nullptr;
    // initialize the column style and width
    p_cstl = new Style*[d_cols];
    p_cwth = new long[d_cols];
    for (long k = 0; k < d_cols; k++) {
      p_cstl[k] = nullptr;
      p_cwth[k] = 0L;
    }
  }

  // destroy this print table

  PrintTable::~PrintTable (void) {
    for (long i = 0; i < d_rows; i++) delete [] p_body[i];
    if (p_stag != nullptr) {
      for (long i = 0; i < d_rows; i++) delete [] p_stag[i]; 
    }
    for (long k = 0L; k < d_cols; k++) delete p_cstl[k];
    delete [] p_head;
    delete [] p_foot;
    delete [] p_body;
    delete [] p_stag;
    delete [] p_cstl;
    delete [] p_cwth;
  }

  // return the class name

  String PrintTable::repr (void) const {
    return "PrintTable";
  }

  // return the print table did

  t_word PrintTable::getdid (void) const {
    return SRL_DEOD_STD;
  }
  
  // return the print table sid

  t_word PrintTable::getsid (void) const {
    return SRL_PTBL_SID;
  }

  // serialize this print table

  void PrintTable::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the table size
      Serial::wrlong (d_size, os);
      // write the columns and rows
      Serial::wrlong (d_cols, os);
      Serial::wrlong (d_rows, os);
      // write the global style
      d_gstl.wrstream (os);
      // write the table body
      if (p_body == nullptr) {
	throw Exception ("table-error", "invalid nil table body");
      }
      for (long i = 0; i < d_rows; i++) {
	String* row = p_body[i];
	if (row == nullptr) {
	  throw Exception ("table-error", "invalid nil table row");
	}
	for (long j = 0; j < d_cols; j++) row[j].wrstream (os);
      }
      // write the table tags
      if (p_stag == nullptr) {
	Serial::wrnilid (os);
      } else {
	for (long i = 0; i < d_rows; i++) {
	  String* row = p_stag[i];
	  if (row == nullptr) {
	    throw Exception ("table-error", "invalid nil tag row");
	  }
	  for (long j = 0; j < d_cols; j++) row[j].wrstream (os);
	}
      }
      // write the table head
      if (p_head == nullptr) {
	throw Exception ("table-error", "invalid nil table head");
      }
      for (long i = 0L; i < d_cols; i++) p_head[i].wrstream (os);
      // write the table foot
      if (p_foot == nullptr) {
	throw Exception ("table-error", "invalid nil table foot");
      }
      for (long i = 0L; i < d_cols; i++) p_foot[i].wrstream (os);
      // write the column style
      for (long i = 0; i < d_cols; i++) {
	if (p_cstl[i] == nullptr) {
	  Serial::wrnilid (os);
	} else {
	  p_cstl[i]->serialize (os);
	}
      }
      // write the column width
      for (long i = 0; i < d_cols; i++) Serial::wrlong (p_cwth[i], os);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this print table

  void PrintTable::rdstream (InputStream& is) {
    wrlock ();
    try {
      // start to clear the table
      for (long i = 0; i < d_rows; i++) delete [] p_body[i];
      for (long i = 0; i < d_cols; i++) delete p_cstl[i];
      delete [] p_head;
      delete [] p_foot;
      delete [] p_body;
      delete [] p_cstl;
      delete [] p_cwth;
      // get the table size
      d_size = Serial::rdlong (is);
      p_body = new String*[d_size];
      for (long i = 0; i < d_size; i++) p_body[i] = nullptr;
      // get the table row and column
      d_cols = Serial::rdlong (is);
      d_rows = Serial::rdlong (is);
      // read the global style
      d_gstl.rdstream (is);
      // get the table body
      for (long i = 0; i < d_rows; i++) {
	p_body[i] = new String[d_cols];
	for (long j = 0; j < d_cols; j++) p_body[i][j].rdstream (is);
      }
      // get the table tags
      if (Serial::isnilid (is) == true) {
	p_stag = nullptr;
	if (Serial::deserialize (is) != nullptr) {
	  throw Exception ("internal-error", 
			   "non nil deserialized object in tag list");
	}
      } else {
	p_stag = new String*[d_size];
	for (long i = 0; i < d_rows; i++) {
	  p_stag[i] = new String[d_cols];
	  for (long j = 0; j < d_cols; j++) p_stag[i][j].rdstream (is);
	}
      }
      // get the table head
      p_head = new String[d_cols];
      for (long i = 0L; i < d_cols; i++) p_head[i].rdstream (is);
      // get the table foot
      p_foot = new String[d_cols];
      for (long i = 0L; i < d_cols; i++) p_foot[i].rdstream (is);
      // get the column style
      p_cstl = new Style*[d_cols];
      for (long i = 0; i < d_cols; i++) {
	p_cstl[i] = dynamic_cast < Style*> (Serial::deserialize (is));
      }
      // get the column width
      p_cwth = new long[d_cols];
      for (long i = 0; i < d_cols; i++) p_cwth[i] = Serial::rdlong (is);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the header is defined

  bool PrintTable::ishead (void) const {
    rdlock ();
    try {
      bool result = false;
      for (long i = 0; i < d_cols; i++) result |= !p_head[i].isnil ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the footer is defined

  bool PrintTable::isfoot (void) const {
    rdlock ();
    try {
      bool result = false;
      for (long i = 0L; i < d_cols; i++) result |= !p_foot[i].isnil ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // return the number of rows

  long PrintTable::getrows (void) const {
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

  // return the number of columns

  long PrintTable::getcols (void) const {
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

  // set the global style 

  void PrintTable::setstyle (const Style& gstl) {
    wrlock ();
    try {
      d_gstl = gstl;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the global style

  Style PrintTable::getstyle (void) const {
    rdlock ();
    try {
      Style result = d_gstl;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the column style 

  void PrintTable::setstyle (const long col, const Style& cstl) {
    wrlock ();
    try {
      // check for valid column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index");
      }
      if (p_cstl[col] == nullptr) p_cstl[col] = new Style (d_gstl);
      *(p_cstl[col]) = cstl;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the column style 

  Style PrintTable::getstyle (const long col) const {
    wrlock ();
    try {
      // check for valid column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index");
      }
      if (p_cstl[col] == nullptr) p_cstl[col] = new Style (d_gstl);
      Style result = *(p_cstl[col]);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new row and return the row index

  long PrintTable::add (void) {
    wrlock ();
    try {
      // check if we need to resize
      if ((d_rows + 1) >= d_size) resize (d_size * 2);
      //  create a new row
      long result = d_rows;
      p_body[d_rows++] = new String[d_cols];
      // check if add a tag row
      if (p_stag != nullptr) p_stag[result] = new String[d_cols];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a head element by column

  void PrintTable::sethead (const long col, const String& val) {
    wrlock ();
    try {
      // check for valid column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index");
      }
      // udpate the header
      p_head[col] = val;
      // update the maximum width
      p_cwth[col] = max (p_cwth[col], val.length ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a head element by column

  String PrintTable::gethead (const long col) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index");
      }
      String result = p_head[col];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a foot element by column

  void PrintTable::setfoot (const long col, const String& val) {
    wrlock ();
    try {
      // check for valid column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index");
      }
      // udpate the header
      p_foot[col] = val;
      // update the maximum width
      p_cwth[col] = max (p_cwth[col], val.length ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a foot element by column

  String PrintTable::getfoot (const long col) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index");
      }
      String result = p_foot[col];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a data element at row and column

  void PrintTable::set (const long row, const long col, const String& val) {
    wrlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows) || (col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid row or column index");
      }
      // udpate the table
      String* line = p_body[row];
      line[col] = val;
      // update the maximum width
      p_cwth[col] = max (p_cwth[col], val.length ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a data element at row and column

  void PrintTable::set (const long row, const long col, const long val) {
    wrlock ();
    try {
      String sval = Utility::tostring (val);
      set (row, col, sval);
      unlock ();
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // set a data element at row and column

  void PrintTable::set (const long row, const long col, const t_long val) {
    wrlock ();
    try {
      String sval = Utility::tostring (val);
      set (row, col, sval);
      unlock ();
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // set a data element at row and column

  void PrintTable::set (const long row, const long col, const t_octa val) {
    wrlock ();
    try {
      String sval = Utility::tohexa (val, true, true);
      set (row, col, sval);
      unlock ();
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // set a data element at row and column

  void PrintTable::set (const long row, const long col, const t_real val) {
    wrlock ();
    try {
      Style  styl = (p_cstl[col] == nullptr) ? d_gstl : *p_cstl[col];
      String sval = styl.format (val);
      set (row, col, sval);
      unlock ();
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // set a data element with a literal object

  void PrintTable::set (const long row, const long col, Literal* obj) {
    wrlock ();
    try {
      if (obj == nullptr) return;
      set (row, col, obj->tostring ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a table element by row and column

  String PrintTable::get (const long row, const long col) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows) || (col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid row or column index");
      }
      String* line = p_body[row];
      String result = line[col];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a truncated table element by row and column

  String PrintTable::truncate (const long row, const long col) const {
    rdlock ();
    try {
      // get the original value
      String sval = get (row, col);
      // get the column style
      Style style = (p_cstl[col] == nullptr) ? d_gstl : *p_cstl[col];
      // compute result value
      String result = style.truncate (sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a tag is defined by position

  bool PrintTable::istag (const long row, const long col) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows) || (col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid row or column index");
      }
      bool result = false;
      if (p_stag != nullptr) result = !p_stag[row][col].isnil ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a tag by position and value

  void PrintTable::settag (const long row, const long col, const String& tag) {
    wrlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows) || (col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid row or column index");
      }
      // check if build the tag array
      if (p_stag == nullptr) {
	p_stag = new String*[d_size];
	for (long i = 0;      i < d_rows; i++) p_stag[i] = new String[d_cols];
	for (long i = d_rows; i < d_size; i++) p_stag[i] = nullptr;
      }
      p_stag[row][col] = tag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a tag by position
  
  String PrintTable::gettag (const long row, const long col) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows) || (col < 0) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid row or column index");
      }
      String result;
      if (p_stag != nullptr) result = p_stag[row][col];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add columns to the print table

  void PrintTable::addcols (const long cnum) {
    wrlock ();
    try {
      // check valid column number
      if (cnum < 0L) {
	throw Exception ("table-error", "invalid column resize number");
      }
      // check for null
      if (cnum == 0L) {
	unlock ();
	return;
      }
      long cols = d_cols + cnum;
      // reallocate the header
      String* head = new String[cols];
      for (long i = 0L; i < d_cols; i++) head[i] = p_head[i];
      delete [] p_head; p_head = head;
      // reallocate the footer
      String* foot = new String[cols];
      for (long i = 0L; i < d_cols; i++) foot[i] = p_foot[i];
      delete [] p_foot; p_foot = foot;
      // reallocate the body
      String** body = new String*[d_rows];
      for (long i = 0L; i < d_rows; i++) {
	if (p_body[i] == nullptr) body[i] = nullptr;
	else {
	  String* nl = new String[cols];
	  String* ol = p_body[i];
	  for (long j = 0L; j < d_cols; j++) nl[j] = ol[j];
	  body[i] = nl;
	  delete [] ol; p_body[i] = nullptr;
	}
      }
      delete [] p_body; p_body = body;
      // reallocate the tags
      if (p_stag != nullptr) {
	String** stag = new String*[d_rows];
	for (long i = 0L; i < d_rows; i++) {
	  if (p_stag[i] == nullptr) stag[i] = nullptr;
	  else {
	    String* nl = new String[cols];
	    String* ol = p_stag[i];
	    for (long j = 0L; j < d_cols; j++) nl[j] = ol[j];
	    stag[i] = nl;
	    delete [] ol; p_stag[i] = nullptr;
	  }
	}
	delete [] p_stag; p_stag = stag;
      }
      // realocate the style and column width
      Style** cstl = new Style*[cols];
      long*   cwth = new long[cols];
      for (long k = 0L; k < d_cols; k++) {
	cstl[k] = p_cstl[k];
	cwth[k] = p_cwth[k];
      }
      for (long k = d_cols; k < cols; k++) {
	cstl[k] = nullptr;
	cwth[k] = 0L;
      }
      delete [] p_cstl; p_cstl = cstl;
      delete [] p_cwth; p_cwth = cwth;
      // update the column and unlock
      d_cols = cols;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a formatted row suitable for dumping

  String PrintTable::dump (const long row) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows)) {
	throw Exception ("table-error", "invalid row index");
      }
      // get the line and preapre for result
      String* line = p_body[row];
      String result;
      for (long i = 0; i < d_cols; i++) {
	String data = line[i].toliteral ();
	Style  styl = (p_cstl[i] == nullptr) ? d_gstl : *p_cstl[i];
	long   cwth = p_cwth[i] + 2;
	result = result + styl.format (data, cwth);
	if (i < (d_cols - 1)) result = result + ' ';
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // dump the table into a buffer

  void PrintTable::dump (Buffer& buffer) const {
    rdlock ();
    try {
      for (long i = 0; i < d_rows; i++) {
	buffer.add (dump (i));
	buffer.add (eolc);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // dump the table for an output stream

  void PrintTable::dump (OutputStream& os) const {
    rdlock ();
    try {
      for (long i = 0; i < d_rows; i++)  os.writeln (dump (i));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a formatted head suitable for printing

  String PrintTable::hformat (void) const {
    rdlock ();
    try {
      String result;
      for (long i = 0; i < d_cols; i++) {
	Style style = (p_cstl[i] == nullptr) ? d_gstl : *p_cstl[i];
	result = result + style.format (p_head[i], p_cwth[i]); 
	if (i < (d_cols - 1)) result = result + ' ';
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

 // get a formatted foot suitable for printing

  String PrintTable::fformat (void) const {
    rdlock ();
    try {
      String result;
      for (long i = 0; i < d_cols; i++) {
	Style style = (p_cstl[i] == nullptr) ? d_gstl : *p_cstl[i];
	result = result + style.format (p_foot[i], p_cwth[i]); 
	if (i < (d_cols - 1)) result = result + ' ';
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a formatted row suitable for printing

  String PrintTable::format (const long row) const {
    rdlock ();
    try {
      // check for valid row and column
      if ((row < 0) || (row >= d_rows)) {
	throw Exception ("table-error", "invalid row index");
      }
      // get the line and prepare for result
      String* line = p_body[row];
      String result;
      for (long i = 0; i < d_cols; i++) {
	Style style = (p_cstl[i] == nullptr) ? d_gstl : *p_cstl[i];
	result = result + style.format (line[i], p_cwth[i]);
	if (i < (d_cols - 1)) result = result + ' ';
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the table into a buffer

  void PrintTable::format (Buffer& buffer) const {
    rdlock ();
    try {
      // add the head if defined
      if (ishead () == true) {
	buffer.add (hformat ());
	buffer.add (eolc);
      }
      // add the data
      for (long i = 0; i < d_rows; i++) {
	buffer.add (format (i));
	buffer.add (eolc);
      }
      // add the foot if defined
      if (isfoot () == true) {
	buffer.add (fformat ());
	buffer.add (eolc);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the table for an output stream

  void PrintTable::format (OutputStream& os) const {
    rdlock ();
    try {
      // add the head if defined
      if (ishead () == true) os.writeln (hformat ());
      // add the data
      for (long i = 0; i < d_rows; i++)  os.writeln (format (i));
      // add the foot if defined
      if (isfoot () == true) os.writeln (fformat ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this print table

  void PrintTable::resize (const long size) {
    wrlock ();
    try {
      // check for valid size
      if (size <= d_size) {
	unlock ();
	return;
      }
      // create a new body table
      String** data = new String* [size];
      for (long i = 0;      i < d_rows; i++) data[i] = p_body[i];
      for (long i = d_rows; i < size;   i++) data[i] = nullptr;
      // update table and remove old one
      delete [] p_body;
      p_body = data;
      d_size = size;
      // eventually, update the tag table
      if (p_stag != nullptr) {
	String** stag = new String* [size];
	for (long i = 0;      i < d_rows; i++) stag[i] = p_stag[i];
	for (long i = d_rows; i < size;   i++) stag[i] = nullptr;
	delete [] p_stag;
	p_stag = stag;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // merge a print table into this one

  void PrintTable::merge (const PrintTable& ptbl) {
    wrlock ();
    try {
      merge (ptbl, false, false);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // merge a print table into this one

  void PrintTable::merge (const PrintTable& ptbl,
			  const bool hflg, const bool fflg) {
    wrlock ();
    try {
      // make sure e have enough columns
      long cols = ptbl.getcols ();
      if (d_cols < cols) {
	throw Exception ("merge-error", "print table is too large");
      }
      // merge the header
      if ((hflg == true) && (ishead () ==  true)) {
	long idx = add ();
	for (long col = 0; col < cols; col++) {
	  // get the table value
	  String value = ptbl.gethead (col);
	  // merge it locally
	  set (idx, col, value);
	}
      }
      // get the number of rows and iterate
      long rows = ptbl.getrows ();
      for (long row = 0; row < rows; row++) {
	long idx = add ();
	for (long col = 0; col < cols; col++) {
	  // get the table value
	  String value = ptbl.get (row, col);
	  // merge it locally
	  set (idx, col, value);
	  // eventually set the tag
	  if (ptbl.istag (row,col) == true) {
	    // get the tag
	    String tag = ptbl.gettag (row, col);
	    // merge it locally
	    settag (idx, col, tag);
	  }
	}
      }
      // merge the footer
      if ((fflg == true) && (isfoot () ==  true)) {
	long idx = add ();
	for (long col = 0; col < cols; col++) {
	  // get the table value
	  String value = ptbl.getfoot (col);
	  // merge it locally
	  set (idx, col, value);
	}
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // locate a row by string

  long PrintTable::locate (const String& sval) const {
    rdlock ();
    try {
      // loop in the rows
      for (long row = 0L; row < d_rows; row++) {
	String* line = p_body[row];
	// loop in the columns
	for (long col = 0L; col < d_cols; col++) {
	  if (line[col] == sval) {
	    long result = row;
	    unlock ();
	    return result;
	  }
	}
      }
      unlock ();
      return -1L;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // locate a row by string and column

  long PrintTable::locate (const String& sval, const long col) const {
    rdlock ();
    try {
      // check for valid column
      if ((col < 0L) || (col >= d_cols)) {
	throw Exception ("table-error", "invalid column index in locate");
      }
      // loop in the rows
      for (long row = 0L; row < d_rows; row++) {
	String* line = p_body[row];
	// check the column
	if (line[col] == sval) {
	  long result = row;
	  unlock ();
	  return result;
	}
      }
      unlock ();
      return -1L;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 24;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_ADDH    = zone.intern ("add-head");
  static const long QUARK_GETH    = zone.intern ("get-head");
  static const long QUARK_SETH    = zone.intern ("set-head");
  static const long QUARK_ADDF    = zone.intern ("add-foot");
  static const long QUARK_GETF    = zone.intern ("get-foot");
  static const long QUARK_SETF    = zone.intern ("set-foot");
  static const long QUARK_DUMP    = zone.intern ("dump");
  static const long QUARK_HEADP   = zone.intern ("head-p");
  static const long QUARK_FOOTP   = zone.intern ("head-p");
  static const long QUARK_MERGE   = zone.intern ("merge");
  static const long QUARK_TRUNC   = zone.intern ("truncate");
  static const long QUARK_ISTAGP  = zone.intern ("tag-p");
  static const long QUARK_SETTAG  = zone.intern ("set-tag");
  static const long QUARK_GETTAG  = zone.intern ("get-tag");
  static const long QUARK_FORMAT  = zone.intern ("format");
  static const long QUARK_LOCATE  = zone.intern ("locate");
  static const long QUARK_ADDCOLS = zone.intern ("add-columns");
  static const long QUARK_GETCOLS = zone.intern ("get-columns");
  static const long QUARK_GETROWS = zone.intern ("get-rows");
  static const long QUARK_SETSTYL = zone.intern ("set-style");
  static const long QUARK_GETSTYL = zone.intern ("get-style");

  // create a new object in a generic way

  Object* PrintTable::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new PrintTable;
    // check for 1 argument
    if (argc == 1) {
      long cols = argv->getlong (0);
      return new PrintTable (cols);
    }
    // check for 2 arguments
    if (argc == 2) {
      long cols = argv->getlong (0);
      long rows = argv->getlong (1);
      return new PrintTable (cols, rows);
    }
    throw Exception ("argument-error", "invavlid argument for print table");
  }

  // return true if the given quark is defined

  bool PrintTable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply an object method with a set of arguments and a quark
  
  Object* PrintTable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch generic quark
    if (quark == QUARK_ADDH) {
      wrlock ();
      try {
	if (argc != d_cols) {
	  throw Exception ("argument-error", 
			   "invalid number of arguments with add-header");
	}
	for (long i = 0; i < argc; i++) {
	  Object*  obj = argv->get (i);
	  String* sobj = dynamic_cast <String*> (obj);
	  if (sobj == nullptr) {
	    throw Exception ("type-error", "invalid object for add-header",
			     Object::repr (obj));
	  }
	  sethead (i, *sobj);
	}
	unlock ();
	return nullptr;
      } catch (...) {
	unlock ();
	throw;
      }
    }
    if (quark == QUARK_ADDF) {
      wrlock ();
      try {
	if (argc != d_cols) {
	  throw Exception ("argument-error", 
			   "invalid number of arguments with add-header");
	}
	for (long i = 0; i < argc; i++) {
	  Object*  obj = argv->get (i);
	  String* sobj = dynamic_cast <String*> (obj);
	  if (sobj == nullptr) {
	    throw Exception ("type-error", "invalid object for add-header",
			     Object::repr (obj));
	  }
	  setfoot (i, *sobj);
	}
	unlock ();
	return nullptr;
      } catch (...) {
	unlock ();
	throw;
      }
    }
    if (quark == QUARK_ADD) {
      if (argc == 0) return new Integer (add ());
      wrlock ();
      // get the new row
      long row = -1;
      try {
	row = add ();
      } catch (...) {
	unlock ();
	throw;
      }
      // loop in the arguments
      try {
	if (argc != d_cols) {
	  throw Exception ("argument-error", 
			   "invalid number of arguments with add");
	}
	for (long i = 0; i < argc; i++) {
	  Object*   obj = argv->get (i);
	  Literal* lobj = dynamic_cast <Literal*> (obj);
	  if (lobj == nullptr) {
	    throw Exception ("type-error", "invalid object for add",
			     Object::repr (obj));
	  }
	  set (row, i, lobj);
	}
	unlock ();
	return new Integer (row);
      } catch (...) {
	if (row >= 0) delete [] p_body[row];
	d_rows--;
	unlock ();
	throw;
      }
    }

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_HEADP)   return new Boolean (ishead   ());
      if (quark == QUARK_FOOTP)   return new Boolean (isfoot   ());
      if (quark == QUARK_GETROWS) return new Integer (getrows  ());
      if (quark == QUARK_GETCOLS) return new Integer (getcols  ());
      if (quark == QUARK_GETSTYL) return new Style   (getstyle ());
      if (quark == QUARK_DUMP) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) dump (*os);
	return nullptr;
      }
      if (quark == QUARK_FORMAT) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) format (*os);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETH) {
	long col = argv->getlong (0);
	return new String (gethead (col));
      }
      if (quark == QUARK_GETF) {
	long col = argv->getlong (0);
	return new String (getfoot (col));
      }
      if (quark == QUARK_ADDCOLS) {
	long cnum = argv->getlong (0);
	addcols (cnum);
	return nullptr;
      }
      if (quark == QUARK_MERGE) {
	Object* obj = argv->get (0);
	PrintTable* pobj = dynamic_cast <PrintTable*> (obj);
	if (pobj == nullptr) {
	  throw Exception ("type-error", "invalid object with merge",
			   Object::repr (obj));
	}
	merge (*pobj);
	return nullptr;
      }
      if (quark == QUARK_DUMP) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* ival = dynamic_cast <Integer*> (obj);
	if (ival != nullptr) {
	  return new String (dump (ival->tolong ()));
	}
	// check for buffer
	Buffer* buffer = dynamic_cast <Buffer*> (obj);
	if (buffer != nullptr) {
	  dump (*buffer);
	  return nullptr;
	}
	// check for output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  dump (*os);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with format",
			 Object::repr (obj));
      }
      if (quark == QUARK_FORMAT) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* ival = dynamic_cast <Integer*> (obj);
	if (ival != nullptr) {
	  return new String (format (ival->tolong ()));
	}
	// check for buffer
	Buffer* buffer = dynamic_cast <Buffer*> (obj);
	if (buffer != nullptr) {
	  format (*buffer);
	  return nullptr;
	}
	// check for output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  format (*os);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with format",
			 Object::repr (obj));
      }
      if (quark == QUARK_SETSTYL) {
	Object* obj = argv->get (0);
	Style* sobj = dynamic_cast <Style*> (obj);
	if (sobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set-style",
			   Object::repr (obj));
	}
	setstyle (*sobj);
	return nullptr;
      }
      if (quark == QUARK_GETSTYL) {
	long col = argv->getlong (0);
	return new Style (getstyle (col));
      }
      if (quark == QUARK_LOCATE) {
	String sval = argv->getstring (0);
	return new Integer (locate (sval));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_GET) {
	long row = argv->getlong (0);
	long col = argv->getlong (1);
	return new String (get (row, col));
      }
      if (quark == QUARK_TRUNC) {
	long row = argv->getlong (0);
	long col = argv->getlong (1);
	return new String (truncate (row, col));
      }
      if (quark == QUARK_SETH) {
	long   col = argv->getlong (0);
	String val = argv->getstring (1);
	sethead (col, val);
	return nullptr;
      }
      if (quark == QUARK_SETF) {
	long   col = argv->getlong (0);
	String val = argv->getstring (1);
	setfoot (col, val);
	return nullptr;
      }
      if (quark == QUARK_ISTAGP) {
	long row = argv->getlong (0);
	long col = argv->getlong (1);
	return new Boolean (istag (row, col));
      }
      if (quark == QUARK_GETTAG) {
	long row = argv->getlong (0);
	long col = argv->getlong (1);
	return new String (gettag (row, col));
      }
      if (quark == QUARK_SETSTYL) {
	long    col = argv->getlong (0);
	Object* obj = argv->get (1);
	Style* sobj = dynamic_cast <Style*> (obj);
	if (sobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set-style",
			   Object::repr (obj));
	}
	setstyle (col, *sobj);
	return nullptr;
      }
      if (quark == QUARK_LOCATE) {
	String sval = argv->getstring (0);
	long   col  = argv->getlong (1);
	return new Integer (locate (sval, col));
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
	long   row = argv->getlong (0);
	long   col = argv->getlong (1);
	String val = argv->getstring (2);
	set (row, col, val);
	return nullptr;
      }
      if (quark == QUARK_SETTAG) {
	long   row = argv->getlong (0);
	long   col = argv->getlong (1);
	String val = argv->getstring (2);
	settag (row, col, val);
	return nullptr;
      }
      if (quark == QUARK_MERGE) {
	Object* obj = argv->get (0);
	PrintTable* pobj = dynamic_cast <PrintTable*> (obj);
	if (pobj == nullptr) {
	  throw Exception ("type-error", "invalid object with merge",
			   Object::repr (obj));
	}
	bool hflg = argv->getbool (1);
	bool fflg = argv->getbool (2);
	merge (*pobj, hflg, fflg);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
