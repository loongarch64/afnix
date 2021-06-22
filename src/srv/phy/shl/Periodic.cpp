// ---------------------------------------------------------------------------
// - Periodic.cpp                                                            -
// - afnix:phy service - periodic element class implementation               -
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

#include "Math.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Periodic.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the internal atom structure
  struct s_atom {
    // atom symbol name
    String d_name;
    // atom name
    String d_info;
    // atomic number
    long   d_anum;
    // atomic weight
    t_real d_wght;
    // ionisation energy
    t_real d_eion;
  };
  
  // the periodic table atomic properties - data from nist
  static const long   PT_STD_SIZE = 118;
  static const s_atom PT_STD_ATOM[PT_STD_SIZE] =
    {
     {"H",  "hydrogen",        1,   1.0079400, 13.5984},
     {"He", "helium",          2,   4.0026020, 24.5874},
     {"Li", "lithium",         3,   6.9410000,  5.3917},
     {"Be", "beryllium",       4,   9.0121820,  9.3227},
     {"B",  "boron",           5,  10.8110000,  8.2980},
     {"C",  "carbon",          6,  12.0107000, 11.2603},
     {"N",  "nitrogen",        7,  14.0067000, 14.5341},
     {"O",  "oxygen",          8,  15.9994000, 13.6181},
     {"F",  "fluorine",        9,  18.9984032, 17.4228},
     {"Ne", "neon",           10,  20.1797000, 21.5645},
     {"Na", "sodium",         11,  22.9897700,  5.1391},
     {"Mg", "magnesium",      12,  24.3050000,  7.6462},
     {"Al", "aluminium",      13,  26.9815380,  5.9858},
     {"Si", "silicon",        14,  28.0855000,  8.1517},
     {"P",  "phosphorus",     15,  30.9737610, 10.4867},
     {"S",  "sulfur",         16,  32.0650000, 10.3600},
     {"Cl", "chlorine",       17,  35.4530000, 12.9676},
     {"Ar", "argon",          18,  39.9480000, 15.7596},
     {"K",  "potassium",      19,  39.0983000,  4.3407},
     {"Ca", "calcium",        20,  40.0780000,  6.1132},
     {"Sc", "scandium",       21,  44.9559100,  6.5615},
     {"Ti", "titanium",       22,  47.8670000,  6.8281},
     {"V",  "vanadium",       23,  50.9415000,  6.7462},
     {"Cr", "chromium",       24,  51.9961000,  6.7665},
     {"Mn", "manganese",      25,  54.9380490,  7.4340},
     {"Fe", "iron",           26,  55.8450000,  7.9024},
     {"Co", "cobalt",         27,  58.9332000,  7.8810},
     {"Ni", "nickel",         28,  58.6934000,  7.6398},
     {"Cu", "copper",         29,  63.5460000,  7.7264},
     {"Zn", "zinc",           30,  65.4090000,  9.3942},
     {"Ga", "gallium",        30,  69.7230000,  5.9993},
     {"Ge", "germanium",      32,  72.6400000,  7.8994},
     {"As", "arsenic",        33,  74.9216000,  9.7886},
     {"Se", "selenium",       34,  78.9600000,  9.7524},
     {"Br", "bromine",        35,  79.9040000, 11.8138},
     {"Kr", "krypton",        36,  83.7980000, 13.9996},
     {"Rb", "rubidium",       37,  85.4678000,  4.1771},
     {"Sr", "strontium",      38,  87.6200000,  5.6949},
     {"Y",  "yttrium",        39,  88.9058500,  6.2173},
     {"Zr", "zirconium",      40,  91.2240000,  6.6339},
     {"Nb", "niobium",        41,  92.9063800,  6.7589},
     {"Mo", "molybdenum",     42,  95.9400000,  7.0924},
     {"Tc", "technetium",     43,  98.0000000,  7.2800},
     {"Ru", "ruthenium",      44, 101.0700000,  7.3506},
     {"Rh", "rhodium",        45, 102.9055000,  7.4589},
     {"Pd", "palladium",      46, 106.4200000,  8.3369},
     {"Ag", "silver",         47, 107.8682000,  7.5762},
     {"Cd", "cadmium",        48, 112.4110000,  8.9938},
     {"In", "indium",         49, 114.8180000,  5.7864},
     {"Sn", "tin",            50, 118.7100000,  7.3439},
     {"Sb", "antimony",       51, 121.7600000,  8.6084},
     {"Te", "tellurium",      52, 127.6000000,  9.0096},
     {"I",  "iodine",         53, 126.9044700, 10.4513},
     {"Xe", "xenon",          54, 131.2930000, 12.1298},
     {"Cs", "cesium",         55, 132.9054500,  3.8939},
     {"Ba", "barium",         56, 137.3270000,  5.2117},
     {"La", "lanthanum",      57, 138.9055000,  5.5769},
     {"Ce", "cerium",         58, 140.1160000,  5.5387},
     {"Pr", "praseodymium",   59, 140.9076500,  5.4730},
     {"Nd", "neodymium",      60, 144.2400000,  5.5250},
     {"Pm", "promethium",     61, 145.0000000,  5.5820},
     {"Sm", "samarium",       62, 150.3600000,  5.6437},
     {"Eu", "europium",       63, 151.9640000,  5.6704},
     {"Gd", "gadolinium",     64, 157.2500000,  6.1498},
     {"Tb", "terbium",        65, 158.9253400,  5.8638},
     {"Dy", "dysprosium",     66, 162.5000000,  5.9389},
     {"Ho", "holmium",        67, 164.9303200,  6.0215},
     {"Er", "erbium",         68, 167.2590000,  6.1077},
     {"Tm", "thulium",        69, 168.9342100,  6.1843},
     {"Yb", "ytterbium",      70, 173.0400000,  6.2542},
     {"Lu", "lutetium",       71, 174.9670000,  5.4259},
     {"Hf", "hafnium",        72, 178.4900000,  6.8251},
     {"Ta", "tantalum",       73, 180.9479000,  7.5496},
     {"W",  "tungsten",       74, 183.8400000,  7.8640},
     {"Re", "rhenium",        75, 186.2070000,  7.8335},
     {"Os", "osmium",         76, 190.2300000,  8.4382},
     {"Ir", "iridium",        77, 192.2170000,  8.9670},
     {"Pt", "platinum",       78, 195.0780000,  8.9588},
     {"Au", "gold",           79, 196.9665500,  9.2255},
     {"Hg", "mercury",        80, 200.5900000, 10.4375},
     {"Tl", "thallium",       81, 204.3833000,  6.1082},
     {"Pb", "lead",           82, 207.2000000,  7.4167},
     {"Bi", "bismuth",        83, 208.9803800,  7.2855},
     {"Po", "polonium",       84, 209.0000000,  8.4140},
     {"At", "astatine",       85, 210.0000000,  9.3175},
     {"Rn", "radon",          86, 222.0000000, 10.7485},
     {"Fr", "francium",       87, 223.0000000,  4.0727},
     {"Ra", "radium",         88, 226.0000000,  5.2784},
     {"Ac", "actinium",       89, 227.0000000,  5.1700},
     {"Th", "thorium",        90, 232.0381000,  6.3067},
     {"Pa", "protactinium",   91, 231.0358800,  5.8900},
     {"U",  "uranium",        92, 238.0289100,  6.1941},
     {"Np", "neptunium",      93, 237.0000000,  6.2657},
     {"Pu", "plutonium",      94, 244.0000000,  6.0260},
     {"Am", "americium",      95, 243.0000000,  5.9738},
     {"Cm", "curium",         96, 247.0000000,  5.9914},
     {"Bk", "berkelium",      97, 247.0000000,  6.1979},
     {"Cf", "californium",    98, 251.0000000,  6.2817},
     {"Es", "einsteinium",    99, 252.0000000,  6.4200},
     {"Fm", "fermium",       100, 257.0000000,  6.5000},
     {"Md", "mendelevium",   101, 258.0000000,  6.5800},
     {"No", "nobelium",      102, 259.0000000,  6.6500},
     {"Lr", "lawrencium",    103, 262.0000000,  4.9000},
     {"Rf", "rutherfordium", 104, 261.0000000,  6.0000},
     {"Db", "dubnium",       105, 262.0000000,  6.8000},
     {"Sg", "seaborgium",    106, 266.0000000,  7.8000},
     {"Bh", "bohrium",       107, 264.0000000,  7.7000},
     {"Hs", "hassium",       108, 277.0000000,  7.6000},
     {"Mt", "meitnerium",    109, 268.0000000, Math::CV_NAN},
     {"Ds", "darmstadtium",  110, 281.0000000, Math::CV_NAN},
     {"Rg", "roentgenium",   111, 272.0000000, Math::CV_NAN},
     {"Cn", "copernicium",   112, 285.0000000, Math::CV_NAN},
     {"Nh", "nihonium",      113, 286.0000000, Math::CV_NAN},
     {"Fl", "flerovium",     114, 289.0000000, Math::CV_NAN},
     {"Mc", "moscovium",     115, 290.0000000, Math::CV_NAN},
     {"Lv", "livermorium",   116, 293.0000000, Math::CV_NAN},
     {"Ts", "tennessine",    117, 294.0000000, Math::CV_NAN},
     {"Og", "oganesson",     118, 294.0000000, Math::CV_NAN}
    };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // return the class name

  String Periodic::repr (void) const {
    return "Periodic";
  }

  // return true if an element exists by name

  bool Periodic::exists (const String& name) const {
    rdlock ();
    try {
      for (long k = 0L; k < PT_STD_SIZE; k++) {
	s_atom atom = PT_STD_ATOM[k];
	if (atom.d_name == name) return true; 
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an element by atomic number

  Atom Periodic::getatom (const long anum) const {
    rdlock ();
    try {
      Atom result;
      for (long k = 0L; k < PT_STD_SIZE; k++) {
	s_atom atom = PT_STD_ATOM[k];
	if (atom.d_anum == anum) {
	  t_real prop[3] = {(t_real) atom.d_anum, atom.d_wght, atom.d_eion};
	  result = Atom (atom.d_name, atom.d_info, prop);
	  break;
	}
      }
      // return the null atom
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find an element by name

  Atom Periodic::getatom (const String& name) const {
    rdlock ();
    try {
      Atom result;
      for (long k = 0L; k < PT_STD_SIZE; k++) {
	s_atom atom = PT_STD_ATOM[k];
	if (atom.d_name == name) {
	  t_real prop[3] = {(t_real) atom.d_anum, atom.d_wght, atom.d_eion};
	  result = Atom (atom.d_name, atom.d_info, prop);
	}
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EXISTP  = zone.intern ("exists-p");
  static const long QUARK_GETATOM = zone.intern ("get-atom");

  // create a new object in a generic way

  Object* Periodic::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Periodic;
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with periodic constructor");
  }

  // return true if the given quark is defined

  bool Periodic::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Periodic::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTP) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_GETATOM) {
	Object* obj = argv->get (0);
	// check for an integer
	auto iobj = dynamic_cast<Integer*>(obj);
	if (iobj != nullptr) return new Atom (getatom (iobj->tolong()));
	// check for a string
	auto sobj = dynamic_cast<String*>(obj);
	if (sobj != nullptr) return new Atom (getatom (*sobj));
	// invalid object
	throw Exception ("type-error", "invalid object for get-atom",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
