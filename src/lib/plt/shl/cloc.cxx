// ---------------------------------------------------------------------------
// - cloc.cxx                                                                -
// - standard platform library - c localization function implementation      -
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

#include "cstr.hpp"
#include "csys.hpp"
#include "cloc.hpp"
#include "cloc.hxx"

namespace afnix {

  // system wide localization initialization

  void c_iloc (void) {
    // set the system locale
    setlocale (LC_ALL, NULL);
  }

  // get the system codeset

  t_codeset c_getcset (void) {
    // map with the global locale
    const char* locale = c_getenv ("LC_ALL");
    if (c_strlen (locale) > 0) return c_mapcset (locale);
    // map with the character type
    locale = c_getenv ("LC_CTYPE");
    if (c_strlen (locale) > 0) return c_mapcset (locale);
    // map with the language type
    locale = c_getenv ("LANG");
    return c_mapcset (locale);    
  }

  // map the codeset by string

  t_codeset c_mapcset (const char* locale) {
    // check for nil
    if (locale == nullptr) return LOC_ASC_00;
    // start by a fresh copy
    char* lptr = c_strdup (locale);
    char* cptr = lptr;
    // try to find a dot
    while (*cptr++ != nilc) {
      if (*cptr == '.') break;
    }
    // if we are at the end return in the ascii locale
    if (*cptr == nilc) return LOC_ASC_00;
    // here we have a dot - double check and advance
    if (*cptr++ != '.') return LOC_ASC_00;
    // eventually find a modifier
    char* cend = cptr;
    while (*cend++ != nilc) {
      if (*cend == '@') {
	*cend = nilc;
	break;
      }
    }
    // map the resulting string in upper case
    char* codeset = c_toupper (cptr);
    delete [] lptr;
    // map the ascii codeset
    if ((c_strcmp (codeset, "C")        == true)    ||
	(c_strcmp (codeset, "POSIX")    == true)    ||
	(c_strcmp (codeset, "US-ASCII") == true)) {
      delete [] codeset;
      return LOC_ASC_00;
    }
    // map the iso/iec 8859-1 codeset
    if ((c_strcmp (codeset, "ISO8859-1")   == true) ||
	(c_strcmp (codeset, "ISO-8859-1")  == true)) {
      delete [] codeset;
      return LOC_ISO_01;
    }
    // map the iso/iec 8859-2 codeset
    if ((c_strcmp (codeset, "ISO8859-2")   == true) ||
	(c_strcmp (codeset, "ISO-8859-2")  == true)) {
      delete [] codeset;
      return LOC_ISO_02;
    }
    // map the iso/iec 8859-3 codeset
    if ((c_strcmp (codeset, "ISO8859-3")   == true) ||
	(c_strcmp (codeset, "ISO-8859-3")  == true)) {
      delete [] codeset;
      return LOC_ISO_03;
    }
    // map the iso/iec 8859-4 codeset
    if ((c_strcmp (codeset, "ISO8859-4")   == true) ||
	(c_strcmp (codeset, "ISO-8859-4")  == true)) {
      delete [] codeset;
      return LOC_ISO_04;
    }
    // map the iso/iec 8859-5 codeset
    if ((c_strcmp (codeset, "ISO8859-5")   == true) ||
	(c_strcmp (codeset, "ISO-8859-5")  == true)) {
      delete [] codeset;
      return LOC_ISO_05;
    }    
    // map the iso/iec 8859-6 codeset
    if ((c_strcmp (codeset, "ISO8859-6")   == true) ||
	(c_strcmp (codeset, "ISO-8859-6")  == true)) {
      delete [] codeset;
      return LOC_ISO_06;
    }
    // map the iso/iec 8859-7 codeset
    if ((c_strcmp (codeset, "ISO8859-7")   == true) ||
	(c_strcmp (codeset, "ISO-8859-7")  == true)) {
      delete [] codeset;
      return LOC_ISO_07;
    }
    // map the iso/iec 8859-8 codeset
    if ((c_strcmp (codeset, "ISO8859-8")   == true) ||
	(c_strcmp (codeset, "ISO-8859-8")  == true)) {
      delete [] codeset;
      return LOC_ISO_08;
    }
    // map the iso/iec 8859-9 codeset
    if ((c_strcmp (codeset, "ISO8859-9")   == true) ||
	(c_strcmp (codeset, "ISO-8859-9")  == true)) {
      delete [] codeset;
      return LOC_ISO_09;
    }
    // map the iso/iec 8859-10 codeset
    if ((c_strcmp (codeset, "ISO8859-10")   == true) ||
	(c_strcmp (codeset, "ISO-8859-10")  == true)) {
      delete [] codeset;
      return LOC_ISO_10;
    }
    // map the iso/iec 8859-11 codeset
    if ((c_strcmp (codeset, "ISO8859-11")   == true) ||
	(c_strcmp (codeset, "ISO-8859-11")  == true)) {
      delete [] codeset;
      return LOC_ISO_11;
    }
    // map the iso/iec 8859-13 codeset
    if ((c_strcmp (codeset, "ISO8859-13")   == true) ||
	(c_strcmp (codeset, "ISO-8859-13")  == true)) {
      delete [] codeset;
      return LOC_ISO_13;
    }
    // map the iso/iec 8859-14 codeset
    if ((c_strcmp (codeset, "ISO8859-14")   == true) ||
	(c_strcmp (codeset, "ISO-8859-14")  == true)) {
      delete [] codeset;
      return LOC_ISO_14;
    }
    // map the iso/iec 8859-15 codeset
    if ((c_strcmp (codeset, "ISO8859-15")  == true) ||
	(c_strcmp (codeset, "ISO-8859-15") == true)) {
      delete [] codeset;
      return LOC_ISO_15;
    }
    // map the iso/iec 8859-16 codeset
    if ((c_strcmp (codeset, "ISO8859-16")  == true) ||
	(c_strcmp (codeset, "ISO-8859-16") == true)) {
      delete [] codeset;
      return LOC_ISO_16;
    }
    // map the unicode codeset
    if (c_strcmp (codeset, "UTF8")  ||
	c_strcmp (codeset, "UTF-8")) {
      delete [] codeset;
      return LOC_UTF_08;
    }
    // by default map to ascii
    delete [] codeset;
    return LOC_ASC_00;
  }

  // return the transcoding table by codeset

  const t_quad* c_gettmap (const t_codeset cset) {
    switch (cset) {
    case LOC_ISO_01:
      return ISO_8859_01_MAP;
      break;
    case LOC_ISO_02:
      return ISO_8859_02_MAP;
      break;
    case LOC_ISO_03:
      return ISO_8859_03_MAP;
      break;
    case LOC_ISO_04:
      return ISO_8859_04_MAP;
      break;
    case LOC_ISO_05:
      return ISO_8859_05_MAP;
      break;
    case LOC_ISO_06:
      return ISO_8859_06_MAP;
      break;
    case LOC_ISO_07:
      return ISO_8859_07_MAP;
      break;
    case LOC_ISO_08:
      return ISO_8859_08_MAP;
      break;
    case LOC_ISO_09:
      return ISO_8859_09_MAP;
      break;
    case LOC_ISO_10:
      return ISO_8859_10_MAP;
      break;
    case LOC_ISO_11:
      return ISO_8859_11_MAP;
      break;
    case LOC_ISO_13:
      return ISO_8859_13_MAP;
      break;
    case LOC_ISO_14:
      return ISO_8859_14_MAP;
      break;
    case LOC_ISO_15:
      return ISO_8859_15_MAP;
      break;
    case LOC_ISO_16:
      return ISO_8859_16_MAP;
      break;
    default:
      break;
    }
    return nullptr;
  }
}
