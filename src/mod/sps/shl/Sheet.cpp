// ---------------------------------------------------------------------------
// - Sheet.cpp                                                               -
// - afnix:sps module - sheet class implementation                           -
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

#include "Sheet.hpp"
#include "Spssid.hxx"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  class SheetSorter {
  private:
    // the column index
    long d_col;
    // the sorting mode
    bool d_mode;

    // compare two objects for ascending order
    bool cmplth (Object* ref, Object* slv) const {
      // check that we have a object records
      Record* rref = dynamic_cast <Record*> (ref);
      Record* rslv = dynamic_cast <Record*> (slv);
      // map the cell to their object
      Object* zobj = nullptr;
      try {
	zobj = (rref == nullptr) ? nullptr : rref->map (d_col);
      } catch (...) {
	zobj = nullptr;
      }
      Object* sobj = nullptr;
      try {
	sobj = (rslv == nullptr) ? nullptr : rslv->map (d_col);
      } catch (...) {
	sobj = nullptr;
      }
      // compare with operator
      if (zobj == nullptr) return true;
      Object*   obj = zobj->oper (Object::OPER_LTH, sobj);
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      bool result   = (bobj == nullptr) ? false : bobj->tobool ();
      Object::cref (obj);
      return result;
    }

    // compare two objects for descending order
    bool cmpgth (Object* ref, Object* slv) const {
      // check that we have a object records
      Record* rref = dynamic_cast <Record*> (ref);
      Record* rslv = dynamic_cast <Record*> (slv);
      // map the cell to their object
      Object* zobj = nullptr;
      try {
	zobj = (rref == nullptr) ? nullptr : rref->map (d_col);
      } catch (...) {
	zobj = nullptr;
      }
      Object* sobj = nullptr;
      try {
	sobj = (rslv == nullptr) ? nullptr : rslv->map (d_col);
      } catch (...) {
	sobj = nullptr;
      }
      // compare with operator
      if ((zobj == nullptr) && (sobj == nullptr)) return true;
      if ((zobj == nullptr) && (sobj != nullptr)) return false;
      Object*   obj = zobj->oper (Object::OPER_GTH, sobj);
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      bool result   = (bobj == nullptr) ? false : bobj->tobool ();
      Object::cref (obj);
      return result;
    }

    // partition the vector and returns the pivot index

    long partition (Vector& argv, const long first, const long last) const {
      // initialize the pivot index, last index and next (unknown) index
      long pidx = first;
      long lidx = first;
      long uidx = lidx + 1;
      // get the pivot object - and protect it
      Object* pvt = Object::iref (argv.get (pidx));
      // swap until we have reached the last element
      while (uidx <= last) {
	Object* ref = Object::iref (argv.get (uidx));
	bool   cflg = d_mode ? cmplth (ref, pvt) : cmpgth (ref, pvt);
	if (cflg == true) {
	  lidx++;
	  argv.set (uidx, argv.get (lidx));
	  argv.set (lidx, ref);
	}
	Object::tref (ref);
	// the intel is in the other region
	uidx++;
      }
      // place the pivot in proper position
      argv.set (first, argv.get (lidx));
      argv.set (lidx,  pvt);
      Object::tref (pvt);
      // the pivot index is now the last index
      return lidx;
    }
    
    // this function performs a recursive quick sort

    void quicksort (Vector& argv, const long first, const long last) const {
      if (first >= last) return;
      long pidx = partition (argv, first, last);
      quicksort (argv, first, pidx - 1);
      quicksort (argv, pidx + 1, last);
    }

  public:
    // create a default sheet sorter
    SheetSorter (void) {
      d_col  = 0;
      d_mode = false;
    }

    // create a sorting object by index
    SheetSorter (const long col, const bool mode) {
      d_col  = col;
      d_mode = mode;
    }

    // sort a vector

    void qsort (Vector& argv) const {
      // sort the vector in place
      argv.wrlock ();
      try {
	long argc = argv.length ();
	quicksort (argv, 0, argc - 1);
	argv.unlock ();
      } catch (...) {
	argv.unlock ();
	throw;
      }
    }
  };

  // this function computes the maximum between two numbers
  static inline long max (const long x, const long y) {
    return (x < y) ? y : x;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil sheet

  Sheet::Sheet (void) {
    reset ();
  }

  // create a new sheet by name

  Sheet::Sheet (const String& name) : Saveas (name) {
    reset ();
  }

  // create a new sheet by name and info

  Sheet::Sheet (const String& name, const String& info) : Saveas (name, info) {
    reset ();
  }

  // copy construct this sheet

  Sheet::Sheet (const Sheet& that) {
    that.rdlock ();
    try {
      // copy the base taggable
      Saveas::operator = (that);
      // copy locally
      d_tags = that.d_tags;
      d_sign = that.d_sign;
      d_mark = that.d_mark;
      d_head = that.d_head;
      d_foot = that.d_foot;
      long hlen = that.d_hstl.length ();
      for (long k = 0L; k < hlen; k++) {
	Style* obj = dynamic_cast <Style*> (that.d_hstl.get (k));
	Style* stl = (obj == nullptr) ? nullptr : new Style (*obj);
	d_hstl.add (stl);
      }
      long slen = that.length ();
      for (long i = 0; i < slen; i++) {
	Record* rcd = that.get (i);
	add (new Record (*rcd));
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the object name

  String Sheet::repr (void) const {
    return "Sheet";
  }

  // return a clone of this object

  Object* Sheet::clone (void) const {
    return new Sheet (*this);
  }
  
  // return the serial did

  t_word Sheet::getdid (void) const {
    return SRL_DEOD_SPS;
  }

  // return the serial sid

  t_word Sheet::getsid (void) const {
    return SRL_SHTT_SID;
  }
  
  // serialize a sheet

  void Sheet::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // save the base object
      Saveas::wrstream (os);
      // save the tags
      d_tags.wrstream (os);
      // save the signature
      d_sign.wrstream (os);
      // save the header style
      d_hstl.wrstream (os);
      // save the marker
      d_mark.wrstream (os);
      // save the header
      d_head.wrstream (os);
      // save the footer
      d_foot.wrstream (os);
      // save the vector
      d_body.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this sheet

  void Sheet::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the base object
      Saveas::rdstream (is);
      // get the tags
      d_tags.rdstream (is);
      // get the signature
      d_sign.rdstream (is);
      // get the header style
      d_hstl.rdstream (is);
      // get the marker
      d_mark.rdstream (is);
      // get the header
      d_head.rdstream (is);
      // get the footer
      d_foot.rdstream (is);
      // get the vector
      d_body.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a sheet to this one

  Sheet& Sheet::operator = (const Sheet& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      reset ();
      // assign base object
      Saveas::operator = (that);
      // assign locally
      d_tags = that.d_tags;
      d_sign = that.d_sign;
      d_mark = that.d_mark;
      d_head = that.d_head;
      d_foot = that.d_foot;
      long hlen = that.d_hstl.length ();
      for (long k = 0L; k < hlen; k++) {
	Style* obj = dynamic_cast <Style*> (that.d_hstl.get (k));
	Style* stl = (obj == nullptr) ? nullptr : new Style (*obj);
	d_hstl.add (stl);
      }
      long slen = that.length ();
      for (long i = 0; i < slen; i++) {
	Record* rcd = that.get (i);
	add (new Record (*rcd));
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset the sheet

  void Sheet::reset (void) {
    wrlock ();
    try {
      d_sign = "";
      d_tags.reset ();
      d_mark.clear ();
      d_head.reset ();
      d_foot.reset ();
      d_body.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the sheet

  void Sheet::clear (void) {
    wrlock ();
    try {
      d_sign = "";
      d_mark.clear ();
      long blen = d_body.length ();
      for (long k = 0L; k < blen; k++) {
	Record* rcd = get (k);
	if (rcd != nullptr) rcd->clear ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a sheet tag

  void Sheet::addtag (const String& tag) {
    wrlock ();
    try {
      if (d_tags.exists (tag) == false) d_tags.add (tag);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector of literals in the tags descriptor

  void Sheet::addtag (const Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return;
    wrlock ();
    try {
      for (long i = 0; i < argc; i++) {
	Object*   obj = argv->get (i);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	  throw Exception ("type-error", 
			   "non literal object with sheet add-tag",
			   Object::repr (obj));
	}
	d_tags.add (lobj->tostring ());
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the tag descriptor length

  long Sheet::tagslen (void) const {
    rdlock ();
    try {
      long result = d_tags.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a tag exists

  bool Sheet::istag (const String& tag) const {
    rdlock ();
    try {
      bool result = d_tags.exists (tag);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a tag by index
  
  String Sheet::gettag (const long index) const {
    wrlock ();
    try {
      String result = d_tags.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the sheet tag by index
  
  void Sheet::settag (const long index, Literal* lobj) {
    wrlock ();
    try {
      String value = (lobj == nullptr) ? "" : lobj->tostring ();
      d_tags.set (index, value);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a tag index by tag

  long Sheet::findtag (const String& tag) const {
    rdlock ();
    try {
      long result = d_tags.lookup (tag);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a sheet signature
  
  void Sheet::setsign (const String& sign) {
    wrlock ();
    try {
      d_sign = sign;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sheet signature
  
  String Sheet::getsign (void) const {
    wrlock ();
    try {
      String result = d_sign;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a header style by index

  void Sheet::sethstl (const long index, const Style& hstl) {
    wrlock ();
    try {
      d_hstl.add (index, new Style (hstl));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header style

  Style* Sheet::gethstl (const long index) const {
    rdlock ();
    try {
      long hlen = d_hstl.length ();
      Style* result = (index < hlen) 
	? dynamic_cast <Style*> (d_hstl.get (index))
	: nullptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the marker length

  long Sheet::marklen (void) const {
    rdlock ();
    try {
      long result = d_mark.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a boolean marker

  void Sheet::addmark (const bool bval) {
    wrlock ();
    try {
      d_mark.add (bval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an integer marker

  void Sheet::addmark (const long ival) {
    wrlock ();
    try {
      d_mark.add (ival);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a real marker

  void Sheet::addmark (const t_real rval) {
    wrlock ();
    try {
      d_mark.add (rval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a literal marker

  void Sheet::addmark (Literal* mark) {
    wrlock ();
    try {
      d_mark.add (mark);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the sheet marker

  Literal* Sheet::getmark (void) const {
    rdlock ();
    try {
      Literal* result = d_mark.get (d_mark.getlidx ());
      unlock ();
      return result;
     } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the sheet marker by index

  Literal* Sheet::getmark (const long midx) const {
    rdlock ();
    try {
      Literal* result = d_mark.get (midx);
      unlock ();
      return result;
     } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add an object in the header record
  
  void Sheet::addhead (Object* object) {
    wrlock ();
    try {
      d_head.add (object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector of objects in the header record

  void Sheet::addhead (const Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return;
    wrlock ();
    try {
      for (long i = 0; i < argc; i++) d_head.add (argv->get (i));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header cell by index

  Cell* Sheet::gethead (const long index) const {
    rdlock ();
    try {
      Cell* result = d_head.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate a header cell by index

  Literal* Sheet::maphead (const long index) const {
    rdlock ();
    try {
      Cell* cell = gethead (index);
      Literal* result = (cell == nullptr) ? nullptr : cell->get ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a record header by index and literal
  
  Cell* Sheet::sethead (const long index, Literal* lobj) {
    wrlock ();
    try {
      Cell* cell = d_head.set (index, lobj);
      unlock ();
      return cell;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an object in the footer record
  
  void Sheet::addfoot (Object* object) {
    wrlock ();
    try {
      d_foot.add (object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector of objects in the footer record

  void Sheet::addfoot (const Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return;
    wrlock ();
    try {
      for (long i = 0; i < argc; i++) d_foot.add (argv->get (i));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a footer cell by index

  Cell* Sheet::getfoot (const long index) const {
    rdlock ();
    try {
      Cell* result = dynamic_cast <Cell*> (d_foot.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate a footer cell by index

  Literal* Sheet::mapfoot (const long index) const {
    rdlock ();
    try {
      Cell* cell = getfoot (index);
      Literal* result = (cell == nullptr) ? nullptr : cell->get ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a record footer by index and literal
  
  Cell* Sheet::setfoot (const long index, Literal* lobj) {
    wrlock ();
    try {
      Cell* cell = dynamic_cast <Cell*> (d_foot.get (index)); 
      if (cell != nullptr) cell->set (lobj);
      unlock ();
      return cell;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the length of the sheet list

  long Sheet::length (void) const {
    rdlock ();
    try {
      long result = d_body.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of columns
  
  long Sheet::getcols (void) const {
    rdlock ();
    try {
      // get the sheet length
      long tlen = length ();
      // compute the maximum columns
      long result = 0;
      for (long i = 0; i < tlen; i++) {
	Record* rcd = get (i);
	if (rcd == nullptr) continue;
	result = max (result, rcd->length ());
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a record in this sheet

  void Sheet::add (Record* rcd) {
    if (rcd == nullptr) return;
    wrlock ();
    try {
      d_body.add (rcd);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a record by index

  Record* Sheet::get (const long index) const {
    rdlock ();
    try {
      Record* result = dynamic_cast <Record*> (d_body.get (index));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cell by row and column

  Cell* Sheet::get (const long row, const long col) const {
    rdlock ();
    try {
      // get the record
      Record* rcd = get (row);
      if (rcd == nullptr) {
	unlock ();
	return nullptr;
      }
      // get the cell
      Cell* cell = rcd->get (col);
      unlock ();
      return cell;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a cell literal by row and column

  Literal* Sheet::map (const long row, const long col) const {
    rdlock ();
    try {
      Cell* cell = get (row, col);
      Literal* lobj = (cell == nullptr) ? nullptr : cell->get ();
      unlock ();
      return lobj;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a record in this sheet by index

  void Sheet::set (const long index, Record* rcd) {
    wrlock ();
    try {
      d_body.set (index, rcd);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an object in this sheet by row and column

  Cell* Sheet::set (const long row, const long col, Object* object) {
    wrlock ();
    try {
      // check if we have enough record
      long slen = length ();
      if (row >= slen) {
	for (long k = slen; k <= row; k++) add (new Record);
      }
      // get the record
      Record* rcd = get (row);
      // set the cell
      Cell* cell = rcd->set (col, object);
      unlock ();
      return cell;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector of literal as a record

  void Sheet::adddata (const Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return;
    wrlock ();
    Record* rcd = new Record;
    try {
      for (long i = 0; i < argc; i++) rcd->add (argv->get (i));
      d_body.add (rcd);
      unlock ();
    } catch (...) {
      Object::cref (rcd);
      unlock ();
      throw;
    }
  }

  // remove a record by index

  void Sheet::remove (const long index) {
    wrlock ();
    try {
      d_body.remove (index);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert this sheet into a print sheet

  PrintTable* Sheet::convert (long max, long start, bool flag) const {
    rdlock ();
    PrintTable* result = nullptr;
    try {
      // get the number of rows
      long rows = length ();
      // check for start index
      if ((rows != 0) && ((start < 0) || (start >= rows))) {
	throw Exception ("sheet-error", "start index out of range for convert");
      }
      // check for max index
      long tlen = start + ((max == 0) ? rows : max);
      if (tlen > rows) {
	throw Exception ("sheet-error", 
			 "max index is out of range for convert");
      }
      // create the print table
      long cols = getcols ();
      result = new PrintTable (cols);
      // set the column style if any
      for (long k = 0L; k < cols; k++) {
	Style* hstl = gethstl (k);
	if (hstl != nullptr) result->setstyle (k, *hstl);
      }
      // set the header if any
      for (long k = 0L; k < cols; k++) {
	Literal* lobj = maphead (k);
	if (lobj == nullptr) {
	  if (flag == false) result->sethead (k, "nil");
	} else {
	  String data = flag ? lobj->tostring () : lobj->toliteral ();
	  result->sethead (k, data);
	}
      }
      // set the footer if any
      for (long k = 0L; k < cols; k++) {
	Literal* lobj = mapfoot (k);
	if (lobj == nullptr) {
	  if (flag == false) result->sethead (k, "nil");
	} else {
	  String data = flag ? lobj->tostring () : lobj->toliteral ();
	  result->setfoot (k, data);
	}
      }
      // iterate through the record
      for (long i = start; i < tlen; i++) {
	Record* rcd = get (i);
	if (rcd == nullptr) continue;
	long row = result->add ();
	// get the record length
	long rlen = rcd->length ();
	for (long j = 0; j < rlen; j++) {
	  // get the literal
	  Literal* lobj = rcd->map (j);
	  // get the associated style
	  Style* cstl = rcd->getstyle (j);
	  if (cstl == nullptr) cstl = gethstl (j);
	  // translate the literal
	  if (lobj == nullptr) {
	    String data = flag ? "" : "nil";
	    result->set (row, j, data);
	  } else {
	    if (cstl == nullptr) {
	      String data = flag ? lobj->tostring () : lobj->toliteral ();
	      result->set (row, j, data);
	    } else {
	      String data = flag ? lobj->format (*cstl) : lobj->toliteral ();
	      result->set (row, j, data);
	    }
	  }
	}
	// fill the rest of the sheet
	String text = flag ? "" : "nil";
	for (long j = rlen; j < cols; j++) result->set (row, j, text);
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // sort this sheet by column index and mode

  void Sheet::sort (const long col, const bool mode) {
    wrlock ();
    try {
      // build the sorter object
      SheetSorter sorter (col, mode);
      // sort the vector
      sorter.qsort (d_body);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // link a sheet column into this sheet

  void Sheet::lkcol (const Sheet* sheet, const long col) {
    // check for a sheet and lock
    if ((sheet == nullptr) || (sheet == this)) return;
    sheet->rdlock ();
    wrlock ();
    try {
      // get the insert column
      long cidx = getcols ();
      // get the column size
      long size = sheet->length ();
      // copy the column
      for (long i = 0; i < size; i++) {
	Cell* cell = sheet->get (i, col);
	set (i, cidx, cell);
      }
    } catch (...) {
      unlock ();
      sheet->unlock ();
      throw;
    }
  }

  // check if a row exists by column index and literal

  bool Sheet::isrow (const long col, const Literal& lobj) const {
    rdlock ();
    try {
      // get the number of rows
      long rows = length ();
      // initialize result
      bool result = false;
      // loop in the sheet and check
      for (long row = 0; row < rows; row++) {
	Record* rcd = get (row);
	if (rcd == nullptr) continue;
	if (rcd->isequal (col, lobj) == true) {
	  result = true;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a row by column index and literal

  long Sheet::rfind (const long col, const Literal& lobj) const {
    rdlock ();
    try {
      // get the number of rows
      long rows = length ();
      // initialize result
      long result = -1;
      // loop in the sheet and check
      for (long row = 0; row < rows; row++) {
	Record* rcd = get (row);
	if (rcd == nullptr) continue;
	if (rcd->isequal (col, lobj) == true) {
	  result = row;
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

 // find a row by column index and literal or throw an exception

  long Sheet::rlookup (const long col, const Literal& lobj) const {
    rdlock ();
    try {
      long result = rfind (col, lobj);
      if (result == -1) {
	throw Exception ("lookup-error", "cannot find row by literal",
			 lobj.tostring ());
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 34;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the sheet supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_MAP     = zone.intern ("map");
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_SORT    = zone.intern ("sort");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_RFIND   = zone.intern ("find-row");
  static const long QUARK_REMOVE  = zone.intern ("remove");
  static const long QUARK_ISROWP  = zone.intern ("row-p");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_COLLEN  = zone.intern ("column-length");
  static const long QUARK_ADDTAG  = zone.intern ("add-tag");
  static const long QUARK_TAGLEN  = zone.intern ("tag-length");
  static const long QUARK_ISTAGP  = zone.intern ("tag-p");
  static const long QUARK_GETTAG  = zone.intern ("get-tag");
  static const long QUARK_SETTAG  = zone.intern ("set-tag");
  static const long QUARK_FINDTAG = zone.intern ("find-tag");
  static const long QUARK_CONVERT = zone.intern ("convert");
  static const long QUARK_GETSIGN = zone.intern ("get-signature");
  static const long QUARK_SETSIGN = zone.intern ("set-signature");
  static const long QUARK_ADDMARK = zone.intern ("add-marker");
  static const long QUARK_GETMARK = zone.intern ("get-marker");
  static const long QUARK_ADDHEAD = zone.intern ("add-header");
  static const long QUARK_GETHEAD = zone.intern ("get-header");
  static const long QUARK_MAPHEAD = zone.intern ("map-header");
  static const long QUARK_SETHEAD = zone.intern ("set-header");
  static const long QUARK_ADDFOOT = zone.intern ("add-footer");
  static const long QUARK_GETFOOT = zone.intern ("get-footer");
  static const long QUARK_MAPFOOT = zone.intern ("map-footer");
  static const long QUARK_SETFOOT = zone.intern ("set-footer");
  static const long QUARK_ADDDATA = zone.intern ("add-data");
  static const long QUARK_LINKCOL = zone.intern ("link-column");
  static const long QUARK_RLOOKUP = zone.intern ("lookup-row");

  // create a new object in a generic way

  Object* Sheet::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sheet;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Sheet (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Sheet (name, info);
    }
    throw Exception ("argument-error", "too many argument with sheet");
  }

  // return true if the given quark is defined

  bool Sheet::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Saveas::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Sheet::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // check for generic quark
    if (quark == QUARK_ADDDATA) {
      adddata (argv);
      return nullptr;
    }
    if (quark == QUARK_ADDTAG) {
      addtag (argv);
      return nullptr;
    }
    if (quark == QUARK_ADDHEAD) {
      addhead (argv);
      return nullptr;
    }
    if (quark == QUARK_ADDFOOT) {
      addfoot (argv);
      return nullptr;
    }

    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH ) return new Integer (length  ());
      if (quark == QUARK_COLLEN ) return new Integer (getcols ());
      if (quark == QUARK_TAGLEN ) return new Integer (tagslen ());
      if (quark == QUARK_GETSIGN) return new String  (getsign ());
      if (quark == QUARK_GETMARK) return getmark ();
      if (quark == QUARK_CONVERT) return convert (0, 0, true);
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_SORT) {
	sort (0, true);
	return nullptr;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADDMARK) {
	Object*   obj = argv->get (0);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) {
	  throw Exception ("type-error", "non literal object with add-marker",
			   Object::repr (obj));
	}	  
	addmark (lobj);
	return nullptr;
      }
      if (quark == QUARK_SETSIGN) {
	String sign = argv->getstring (0);
	setsign (sign);
	return nullptr;
      }
      if (quark == QUARK_ISTAGP) {
	Object*   obj = argv->get (0);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	  throw Exception ("type-error", "non literal object with tag-p",
			   Object::repr (obj));
	}	  
	String tag = lobj->tostring ();
	return new Boolean (istag (tag));
      }
      if (quark == QUARK_GETTAG) {
	long index = argv->getlong (0);
	return new String (gettag (index));
      }
      if (quark == QUARK_FINDTAG) {
	Object*   obj = argv->get (0);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	  throw Exception ("type-error", "non literal object with find-tag",
			   Object::repr (obj));
	}	  
	String tag = lobj->tostring ();
	return new Integer (findtag (tag));
      }
      if (quark == QUARK_GET) {
	long idx = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = get (idx);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_ADD) {
	Object* obj = argv->get (0);
	Record* rcd = dynamic_cast <Record*> (obj);
	if (rcd != nullptr) {
	  add (rcd);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object to add in sheet",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETHEAD) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = gethead (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MAPHEAD) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = maphead (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETFOOT) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = getfoot (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MAPFOOT) {
	long index = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = mapfoot (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_REMOVE) {
	long index= argv->getlong (0);
	remove (index);
	return nullptr;
      }
      if (quark == QUARK_CONVERT) {
	long max = argv->getlong (0);
	return convert (max, 0, true);
      }
      if (quark == QUARK_SORT) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* col = dynamic_cast <Integer*> (obj);
	if (col != nullptr) {
	  sort (col->tolong (), true);
	  return nullptr;
	}
	// check for boolean
	Boolean* mode = dynamic_cast <Boolean*> (obj);
	if (mode != nullptr) {
	  sort (0, mode->tobool ());
	  return nullptr;
	}
	// invalid arguments
	throw Exception ("type-error", "invalid object for sorting ",
			 Object::repr (obj));
      }
    }

    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_SETTAG) {
	long     idx  = argv->getlong (0);
	Object*  obj  = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in tags",
			   Object::repr (obj));
	settag (idx, lobj);
	return nullptr;
      }
      if (quark == QUARK_SETHEAD) {
	long    idx = argv->getlong (0);
	Object*  obj  = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in header",
			   Object::repr (obj));
	wrlock ();
	try {
	  Object* result = sethead (idx, lobj);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SETFOOT) {
	long    idx = argv->getlong (0);
	Object*  obj  = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in footer",
			   Object::repr (obj));
	wrlock ();
	try {
	  Object* result = setfoot (idx, lobj);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GET) {
	long row = argv->getlong (0);
	long col = argv->getlong (1);
	rdlock ();
	try {
	  Object* result = get (row, col);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MAP) {
	long row = argv->getlong (0);
	long col = argv->getlong (1);
	rdlock ();
	try {
	  Object* result = map (row, col);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SET) {
	long    idx = argv->getlong (0);
	Object* obj = argv->get (1);
	Record* rcd = dynamic_cast <Record*> (obj);
	if ((obj != nullptr) && (rcd == nullptr)) 
	  throw Exception ("type-error", "invalid object to set in sheet",
			   Object::repr (obj));
	set (idx, rcd);
	return nullptr;
      }
      if (quark == QUARK_CONVERT) {
	long max   = argv->getlong (0);
	long start = argv->getlong (1);
	return convert (max, start, true);
      }
      if (quark == QUARK_SORT) {
	long col  = argv->getlong (0);
	bool mode = argv->getbool (1);
	sort (col, mode);
	return nullptr;
      }
      if (quark == QUARK_LINKCOL) {
	Object* obj = argv->get (0);
	Sheet*  sht = dynamic_cast <Sheet*> (obj);
	if (sht == nullptr) {
	  throw Exception ("type-error", "invalid object with link-column",
			   Object::repr (obj));
	}
	long col = argv->getlong (1);
	lkcol (sht, col);
	return nullptr;
      }
      if (quark == QUARK_ISROWP) {
	long      col = argv->getlong (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) 
	  throw Exception ("type-error", "invalid object for row check",
			   Object::repr (obj));
	bool result = isrow (col, *lobj);
	return new Boolean (result);
      }
      if (quark == QUARK_RFIND) {
	long      col = argv->getlong (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) 
	  throw Exception ("type-error", "invalid object for row find",
			   Object::repr (obj));
        long result = rfind (col, *lobj);
	return new Integer (result);
      }
      if (quark == QUARK_RLOOKUP) {
	long      col = argv->getlong (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if ((obj != nullptr) && (lobj == nullptr)) 
	  throw Exception ("type-error", "invalid object for row lookup",
			   Object::repr (obj));
        long result = rlookup (col, *lobj);
	return new Integer (result);
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_CONVERT) {
	long max   = argv->getlong (0);
	long start = argv->getlong (1);
	bool flag  = argv->getbool (2);
	return convert (max, start, flag);
      }
      if (quark == QUARK_SET) {
	long    row = argv->getlong (0);
	long    col = argv->getlong (1);
	Object* obj = argv->get    (2);
	wrlock ();
	try {
	  Object* result = set (row, col, obj);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the saveas method
    return Saveas::apply (zobj, nset, quark, argv);
  }
}
