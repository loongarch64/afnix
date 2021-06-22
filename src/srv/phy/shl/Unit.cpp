// ---------------------------------------------------------------------------
// - Unit.cpp                                                                -
// - afnix:phy service - si unit class implementation                        -
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

#include "Unit.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Boolean.hpp"
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // unit descriptor
  struct s_unit {
    /// the base units
    enum t_base
      {
       BASE_U, // unknown
       BASE_T, // time
       BASE_L, // length
       BASE_M, // mass
       BASE_I, // electric current
       BASE_K, // temperature
       BASE_N, // amount of substance
       BASE_J  // luminous intensity
      };

    /// unit prefix
    enum t_pfix 
      {
       PFIX_YOTA, // 10^24  yotta
       PFIX_ZETA, // 10^21  zetta
       PFIX_PEXA, // 10^18  exa
       PFIX_PETA, // 10^15  peta
       PFIX_TERA, // 10^12  tera
       PFIX_GIGA, // 10^9,  giga
       PFIX_MEGA, // 10^6,  mega
       PFIX_KILO, // 10^3,  kilo
       PFIX_ECTO, // 10^2,  hecto
       PFIX_DECA, // 10^1,  deca
       PFIX_PONE, // 1.0,     
       PFIX_DECI, // 10^-1  deci
       PFIX_CNTI, // 10^-2  centi
       PFIX_MILI, // 10^-3  milli
       PFIX_MICO, // 10^-6  micro
       PFIX_NANO, // 10^-9  nano
       PFIX_PICO, // 10^-12 pico
       PFIX_FMTO, // 10^-15 femto
       PFIX_ATTO, // 10^-18 atto
       PFIX_ZEPO, // 10^-21 zepto
       PFIX_YOTO, // 10^-24 yocto
      };
  
    /// the unit prefix
    s_unit::t_pfix d_pfix;
    /// the base unit
    s_unit::t_base d_base;
    // the unit exponent
    long d_expn;
  };

  // reset a descriptor
  static void si_rst_desc (s_unit& desc) {
    desc.d_pfix = s_unit::PFIX_PONE;
    desc.d_base = s_unit::BASE_U;
    desc.d_expn = 1L;
  }

  // copy a dscriptor array
  static s_unit* si_cpy_desc (const long size, const s_unit* desc) {
    // check for null
    if ((size == 0L) || (desc == nullptr)) return nullptr;
    // allocate and loop
    s_unit* result = new s_unit[size];
    for (long k = 0L; k < size; k++) {
      result[k].d_pfix = desc[k].d_pfix;
      result[k].d_base = desc[k].d_base;
      result[k].d_expn = desc[k].d_expn;
    }
    return result;
  }
  
  // map a prefix string length
  static long si_len_pfix (const s_unit::t_pfix pfix) {
    if (pfix == s_unit::PFIX_PONE) return 0L;
    if (pfix == s_unit::PFIX_DECA) return 2L;
    return 1L;
  }

  // map a base string length
  static long si_len_base (const s_unit::t_base base) {
    if (base == s_unit::BASE_J) return 2L;
    if (base == s_unit::BASE_N) return 3L;
    return 1L;
  }

  // map a prefix to a factor
  static long si_tofactor (const s_unit::t_pfix pfix) {
    long result = 0L;
    switch (pfix) {
    case s_unit::PFIX_YOTA:
      result = 24L;
      break;
    case s_unit::PFIX_ZETA:
      result = 21L;
      break;
    case s_unit::PFIX_PEXA:
      result = 18L;
      break;
    case s_unit::PFIX_PETA:
      result = 15L;
      break;
    case s_unit::PFIX_TERA:
      result = 12L;
      break;
    case s_unit::PFIX_GIGA:
      result = 9L;
      break;
    case s_unit::PFIX_MEGA:
      result = 6L;
      break;
    case s_unit::PFIX_KILO:
      result = 3L;
      break;
    case s_unit::PFIX_ECTO:
      result = 2L;
      break;
    case s_unit::PFIX_DECA:
      result = 1L;
      break;
    case s_unit::PFIX_PONE:
      result = 0L;
      break;
    case s_unit::PFIX_DECI:
      result = -1L;
      break;
    case s_unit::PFIX_CNTI:
      result = -2L;
      break;
    case s_unit::PFIX_MILI:
      result = -3L;
      break;
    case s_unit::PFIX_MICO:
      result = -6L;
      break;
    case s_unit::PFIX_NANO:
      result = -9L;
      break;
    case s_unit::PFIX_PICO:
      result = -12L;
      break;
    case s_unit::PFIX_FMTO:
      result = -15L;
      break;
    case s_unit::PFIX_ATTO:
      result = -18L;
      break;
    case s_unit::PFIX_ZEPO:
      result = -21L;
      break;
    case s_unit::PFIX_YOTO:
      result = -24L;
      break;
    };
    return result;
  }

  // map the prefix unit to a string
  static String si_tostring (const s_unit::t_pfix pfix) {
    String result;
    switch (pfix) {
    case s_unit::PFIX_YOTA:
      result = "Y";
      break;
    case s_unit::PFIX_ZETA:
      result = "Z";
      break;
    case s_unit::PFIX_PEXA:
      result = "E";
      break;
    case s_unit::PFIX_PETA:
      result = "P";
      break;
    case s_unit::PFIX_TERA:
      result = "T";
      break;
    case s_unit::PFIX_GIGA:
      result = "G";
      break;
    case s_unit::PFIX_MEGA:
      result = "M";
      break;
    case s_unit::PFIX_KILO:
      result = "k";
      break;
    case s_unit::PFIX_ECTO:
      result = "h";
      break;
    case s_unit::PFIX_DECA:
      result = "da";
      break;
    case s_unit::PFIX_PONE:
      break;
    case s_unit::PFIX_DECI:
      result = "d";
      break;
    case s_unit::PFIX_CNTI:
      result = "c";
      break;
    case s_unit::PFIX_MILI:
      result = "m";
      break;
    case s_unit::PFIX_MICO:
      result = reinterpret_cast<const t_quad*>(U"µ");
      break;
    case s_unit::PFIX_NANO:
      result = "n";
      break;
    case s_unit::PFIX_PICO:
      result = "p";
      break;
    case s_unit::PFIX_FMTO:
      result = "f";
      break;
    case s_unit::PFIX_ATTO:
      result = "a";
      break;
    case s_unit::PFIX_ZEPO:
      result = "z";
      break;
    case s_unit::PFIX_YOTO:
      result = "y";
      break;
    };
    return result;
  }
      
  // map the base unit to a string
  static String si_tostring (const s_unit::t_base base) {
    String result;
    switch (base) {
    case s_unit::BASE_U:
      break;
    case s_unit::BASE_T:
      result = "s";
      break;
    case s_unit::BASE_L:
      result = "m";
      break;
    case s_unit::BASE_M:
      result = "g";
      break;
    case s_unit::BASE_I:
      result = "A";
      break;
    case s_unit::BASE_K:
      result = "K";
      break;
    case s_unit::BASE_N:
      result = "mol";
      break;
    case s_unit::BASE_J:
      result = "cd";
      break;
    }
    return result;
  }
  
  // map a descriptor to a string
  static String si_tostring (const s_unit& desc) {
    // set the prefix
    String result = si_tostring (desc.d_pfix);
    // add the unit
    result += si_tostring (desc.d_base);
    // add the exponent
    if (desc.d_expn != 1) {
      result += '^';
      result += Utility::tostring (desc.d_expn);
    }
    return result;
  }

  // map a descriptor to a unit factor
  static long si_tofactor (const s_unit desc) {
    // get the descriptor factor
    long result = si_tofactor (desc.d_pfix);
    // adjust for mass
    if (desc.d_base == s_unit::BASE_M) result -= 3;
    // include exponent
    result *= desc.d_expn;
    return result;
  }

  // map a unit name to a prefix
  static s_unit::t_pfix to_si_pfix (const String& un) {
    // get the first character
    t_quad p = un[0];
    // check for special case deca (da)
    if (p == 'd') {
      if (un[1] == 'a') return s_unit::PFIX_DECA;
      return s_unit::PFIX_DECI;
    }
    // check for special case milli (m)
    if (p == 'm') {
      // no prefix
      if (un.length() == 1) return s_unit::PFIX_PONE;
      // check for mol
      if ((un.length() >= 3) && (un[1] == 'o') && (un[2] == 'l'))
	return s_unit::PFIX_PONE;
      // check for exponent
      if (un[1] == '^') return s_unit::PFIX_PONE;
      // default to milli
      return s_unit::PFIX_MILI;
    }
    // check for special case centi (c)
    if (p == 'c') {
      // check for cd
      if ((un.length() >= 2) && (un[1] == 'd')) return s_unit::PFIX_PONE;
      return s_unit::PFIX_CNTI;
    }
    // check valid prefix
    if (p == 'Y')  return s_unit::PFIX_YOTA;
    if (p == 'Z')  return s_unit::PFIX_ZETA;
    if (p == 'E')  return s_unit::PFIX_PEXA;
    if (p == 'P')  return s_unit::PFIX_PETA;
    if (p == 'T')  return s_unit::PFIX_TERA;
    if (p == 'G')  return s_unit::PFIX_GIGA;
    if (p == 'M')  return s_unit::PFIX_MEGA;
    if (p == 'k')  return s_unit::PFIX_KILO;
    if (p == 'm')  return s_unit::PFIX_MILI;
    if (p == 'c')  return s_unit::PFIX_CNTI;
    if (p == 'u')  return s_unit::PFIX_MICO;
    if (p == 'n')  return s_unit::PFIX_NANO;
    if (p == 'p')  return s_unit::PFIX_PICO;
    if (p == 'f')  return s_unit::PFIX_FMTO;
    if (p == 'a')  return s_unit::PFIX_ATTO;
    if (p == 'y')  return s_unit::PFIX_YOTO;
    // special prefix
    if (p == U'µ') return s_unit::PFIX_YOTA;
    // no prefix
    return s_unit::PFIX_PONE;
  }

  // map a unit name to its base
  static s_unit::t_base to_si_base (const String& un) {
    // split the string  with exponent
    Strvec uv = Strvec::split (un, "^");
    String us = uv.get (0);
    // map the string
    if (us == "s")   return s_unit::BASE_T;
    if (us == "m")   return s_unit::BASE_L;
    if (us == "g")   return s_unit::BASE_M;
    if (us == "A")   return s_unit::BASE_I;
    if (us == "K")   return s_unit::BASE_K;
    if (us == "mol") return s_unit::BASE_N;
    if (us == "cd")  return s_unit::BASE_J;
    // invalid base unit
    throw Exception ("unit-error", "invalid base unit", us);
  }

  // map a unit name to its exponent
  static long to_si_expn (const String& un) {
    // check for null
    if (un.isnil () == true) return 1L;
    // check for valid exponent
    if (un[0] != '^') {
      throw Exception ("unit-error", "invalid unit exponent", un);
    }
    String ue = un.rsubstr (1);
    if (ue.isnil () == true) {
      throw Exception ("unit-error", "invalid unit exponent", un);
    }
    long result = Utility::tolong (ue);
    if (result == 0L) {
      throw Exception ("unit-error", "invalid null unit exponent");
    }
    return result;
  }
  
  // parse a descriptor value into a descriptor array
  static s_unit* si_to_desc (long& size, const String& dval) {
    // check for null first
    if (dval.isnil () == true) {
      size = 0L;
      return nullptr;
    }
    // split the string
    Strvec sv = Strvec::split (dval, ".");
    long vlen = sv.length ();
    if (vlen == 0L) {
      throw Exception ("unit-error", "invalid unit descriptor", dval);
    }
    // allocate the descriptor array
    s_unit* desc = new s_unit[vlen];
    // loop in the descriptor
    for (long k = 0L; k < vlen; k++) {
      // reset descriptor
      si_rst_desc (desc[k]);
      // parse a single unit
      String su = sv.get (k);
      // extract the prefix
      desc[k].d_pfix = to_si_pfix (su);
      su = su.rsubstr (si_len_pfix(desc[k].d_pfix));
      // extract the base
      desc[k].d_base = to_si_base (su);
      su = su.rsubstr (si_len_base(desc[k].d_base));
      // extract the exponent
      desc[k].d_expn = to_si_expn (su);
    }
    size = vlen;
    return desc;
  }

  // common units including si units
  static const Unit SI_DEF_UNIT[] =
    {
     // base units
     {"s",    "second",            "s",                   1.0},
     {"m",    "metre",             "m",                   1.0},
     {"kg",   "kilogram",          "kg",                  1.0},
     {"A",    "ampere",            "A",                   1.0},
     {"mol",  "mole",              "mol",                 1.0},
     {"cd",   "candela",           "cd",                  1.0},
     // derived units
     {"rad",  "radian",             "m.m^-1",             1.0},
     {"sr",   "steradian",          "m^2.m^-2",           1.0},
     {"Hz",   "hertz",              "s^-1",               1.0},
     {"N",    "newton",             "kg.m.s^-2",          1.0},
     {"Pa",   "pascal",             "kg.m^-1.s^-2",       1.0},    
     {"J",    "joule",              "kg.m^2.s^-2",        1.0},    
     {"W",    "watt",               "kg.m^2.s^-3",        1.0},    
     {"C",    "coulomb",            "s.A",                1.0},    
     {"V",    "volt",               "kg.m^2.s^-3.A^-1",   1.0},    
     {"f",    "farad",              "kg^-1.m^-2.s^4.A^2", 1.0},  
     {"Ω",    "ohm",                "kg.m^2.s^-3.A^-2",   1.0},
     {"S",    "siemens",            "kg^-1.m^-2.s^3.A^2", 1.0},
     {"Wb",   "weber",              "kg.m^2.s^-2.A^-1",   1.0},    
     {"T",    "tesla",              "kg.s^-2.A^-1",       1.0},
     {"H",    "henry",              "kg.m^2.s^-2.A^-2",   1.0},
     {"lm",   "lumen",              "cd.m^2.m^-2",        1.0},
     {"lx",   "lux",                "m^2.cd^-1",          1.0},
     {"Bq",   "becquerel",          "s^-1",               1.0},
     {"Gy",   "gray",               "m^2.s^-2",           1.0},
     {"Sv",   "sievert",            "m^2.s^-2",           1.0},
     {"kat",  "katal",              "mol.s^-1",           1.0},
     // other units
     {"Å",    "ångström",           "m",                  1.0E-10},
     {"in",   "inch",               "m",                  0.0254},
     {"ft",   "foot",               "m",                  0.304800610},
     {"mi",   "mile",               "m",                  1609.344},
     {"nm",   "nautical mile",      "m",                  1852.0},
     {"nl",   "nautical league",    "m",                  5556.0},
     {"au",   "astronomical unit",  "m",                  149597870700.0},
     {"pc",   "parsec",             "m",                  30856775814913700.0},
     {"km/h", "kilometer per hour", "m.s^-1",             1000.0/3600.0},
     {"F/m",  "permittivity",       "kg^-1.m^-3.s^4.A^2", 1.0},
     {"H/m",  "permeability",       "kg.m.s^-2.A^-2",     1.0},
     {"Js",   "action",             "kg.m^2.s^-1",        1.0}
    };
  static const long SI_LEN_UNIT = sizeof(SI_DEF_UNIT) / sizeof (Unit);
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // create a unit by name

  Unit Unit:: create (const String& name) {
    for (long k = 0L; k < SI_LEN_UNIT; k++) {
      if ((SI_DEF_UNIT[k].d_name == name) ||
	  (SI_DEF_UNIT[k].d_info == name)) return SI_DEF_UNIT[k];
    }
    return Unit (name);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil unit

  Unit::Unit (void) {
    d_scal = 1.0;
    d_size = 0L;
    p_desc = nullptr;
  }

  // create a unit by name

  Unit::Unit (const String& name) : Infoable (name) {
    d_scal = 1.0;
    d_size = 0L;
    p_desc = nullptr;
    if (parse (name) == false) {
      throw Exception ("unit-error", "cannot parse unit", name);
    }
  }
    
  // create a unit by name and info

  Unit::Unit (const String& name, const String& info) : Infoable (name, info) {
    d_scal = 1.0;
    d_size = 0L;
    p_desc = nullptr;
    if (parse (name) == false) {
      throw Exception ("unit-error", "cannot parse unit", name);
    }
  }

  // create a unit by name, info and descriptor

  Unit::Unit (const String& name, const String& info, const String& dval) :
    Infoable (name, info) {
    d_scal = 1.0;
    d_size = 0L;
    p_desc = nullptr;
    if (parse (dval) == false) {
      throw Exception ("unit-error", "cannot parse unit", dval);
    }
  }

  // create a unit by name, info, descriptor and scaling factor

  Unit::Unit (const String& name, const String& info, const String& dval,
	      const t_real  scal) : Infoable (name, info) {
    d_scal = 1.0;
    d_size = 0L;
    p_desc = nullptr;
    if (parse (dval, scal) == false) {
      throw Exception ("unit-error", "cannot parse unit", dval);
    }
  }
  
  // copy construct this unit

  Unit::Unit (const Unit& that) {
    d_size = 0L;
    p_desc = nullptr;
    that.rdlock ();
    try {
      // copy base infoable
      Infoable::operator = (that);
      // copy locally
      d_scal = that.d_scal;
      d_size = that.d_size;
      p_desc = si_cpy_desc (that.d_size, that.p_desc);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // destroy this unit

  Unit::~Unit (void) {
    delete [] p_desc;
  }

  // assign a unit to this one

  Unit& Unit::operator = (const Unit& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // reset locally
      reset ();
      // copy base infoable
      Infoable::operator = (that);
      // copy locally
      d_scal = that.d_scal;
      d_size = that.d_size;
      p_desc = si_cpy_desc (that.d_size, that.p_desc);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Unit::repr (void) const {
    return "Unit";
  }

  // return a clone of this object

  Object* Unit::clone (void) const {
    return new Unit (*this);
  }

  // reset this unit

  void Unit::reset (void) {
    wrlock ();
    try {
      d_scal = 1.0;
      d_size = 0L;
      delete [] p_desc; p_desc = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the unit scaling factor

  t_real Unit::getscal (void) const {
    rdlock ();
    try {
      t_real result = d_scal;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the unit si representation

  String Unit::tosi (void) const {
    rdlock ();
    try {
      String result;
      for (long k = 0L; k < d_size; k++) {
	if (k > 0) result += '.';
	result += si_tostring (p_desc[k]);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a string representation of this unit
  
  String Unit::tostring (void) const {
    rdlock ();
    try {
      String result;
      if (d_scal != 1.0) {
	result += Utility::tostring (d_scal);
	result += '*';
      }
      result += tosi ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the unit factor

  t_real Unit::tofactor (void) const {
    rdlock ();
    try {
      long fact = 0L;
      for (long k = 0L; k < d_size; k++) fact += si_tofactor (p_desc[k]);
      t_real result = (fact == 0L) ? 1.0 : Math::pow (10, fact);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // parse a unit name

  bool Unit::parse (const String& dval) {
    wrlock ();
    try {
      // reset the unit
      reset ();
      // split the descriptor
      Strvec dvec = Strvec::split (dval, "*");
      if ((dvec.length () < 1L) || (dvec.length () > 2)) {
	throw Exception ("unit-error", "invalid unit descriptor", dval);
      }
      String uval = (dvec.length () == 1L) ? dvec.get (0) : dvec.get (1);
      if (dvec.length () == 2L) d_scal = Utility::toreal (dvec.get (0));
      // parse the unit
      p_desc = si_to_desc (d_size, uval);
      bool result = (p_desc != nullptr) && (d_size > 0L);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a unit name and value

  bool Unit::parse (const String& dval, const t_real scal) {
    wrlock ();
    try {
      // reset the unit
      reset ();
      bool result = parse (dval);
      if (result == true) d_scal *= scal;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compute a unit value

  t_real operator * (const t_real uval, const Unit& unit) {
    return uval * unit.getscal() * unit.tofactor();
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_MUL     = zone.intern ("mul");
  static const long QUARK_TOSI    = zone.intern ("to-si");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_TOSTNG  = zone.intern ("to-string");
  static const long QUARK_TOFACT  = zone.intern ("to-factor");
  static const long QUARK_GETSCAL = zone.intern ("get-scaling-factor");

  // create a new object in a generic way

  Object* Unit::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Unit;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Unit (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Unit (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      String dval = argv->getstring (2);
      return new Unit (name, info, dval);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with unit constructor");
  }

  // return true if the given quark is defined

  bool Unit::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Infoable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Unit::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOSI)    return new String (tosi     ());
      if (quark == QUARK_TOFACT)  return new Real   (tofactor ());
      if (quark == QUARK_TOSTNG)  return new String (tostring ());
      if (quark == QUARK_GETSCAL) return new Real   (getscal  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_MUL) {
	t_real uval = argv->getrint (0);
	return new Real (uval * *this);
      }
      if (quark == QUARK_PARSE) {
	String dval = argv->getstring (0);
	return new Boolean (parse (dval));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_PARSE) {
	String dval = argv->getstring (0);
	long   cnst = argv->getlong (1);
	return new Boolean (parse (dval, cnst));
      }
    }
    // call the infoable method
    return Infoable::apply (zobj, nset, quark, argv);
  }
}
