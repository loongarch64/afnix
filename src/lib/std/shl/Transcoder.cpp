// ---------------------------------------------------------------------------
// - Transcoder.cpp                                                          -
// - standard object library - codeset transcoder class implementation       -
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

#include "Item.hpp"
#include "Byte.hpp"
#include "Ascii.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Transcoder.hpp"
#include "cloc.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // get the static map
  static const t_quad* get_static_map (const Encoding::t_tmod tmod) {
    const t_quad* result = nullptr;
    switch (tmod) {
    case Encoding::TMOD_NONE:
      result = nullptr;
      break;
    case Encoding::TMOD_5901:
      result = c_gettmap (LOC_ISO_01);
      break;
    case Encoding::TMOD_5902:
      result = c_gettmap (LOC_ISO_02);
      break;
    case Encoding::TMOD_5903:
      result = c_gettmap (LOC_ISO_03);
      break;
    case Encoding::TMOD_5904:
      result = c_gettmap (LOC_ISO_04);
      break;
    case Encoding::TMOD_5905:
      result = c_gettmap (LOC_ISO_05);
      break;
    case Encoding::TMOD_5906:
      result = c_gettmap (LOC_ISO_06);
      break;
    case Encoding::TMOD_5907:
      result = c_gettmap (LOC_ISO_07);
      break;
    case Encoding::TMOD_5908:
      result = c_gettmap (LOC_ISO_08);
      break;
    case Encoding::TMOD_5909:
      result = c_gettmap (LOC_ISO_09);
      break;
    case Encoding::TMOD_5910:
      result = c_gettmap (LOC_ISO_10);
      break;
    case Encoding::TMOD_5911:
      result = c_gettmap (LOC_ISO_11);
      break;
    case Encoding::TMOD_5913:
      result = c_gettmap (LOC_ISO_13);
      break;
    case Encoding::TMOD_5914:
      result = c_gettmap (LOC_ISO_14);
      break;
    case Encoding::TMOD_5915:
      result = c_gettmap (LOC_ISO_15);
      break;
    case Encoding::TMOD_5916:
      result = c_gettmap (LOC_ISO_16);
      break;
    default:
      result = nullptr;
      break;
    }
    return result;
  }

  // get the encoding map
  static t_quad* get_encode_map (const Encoding::t_tmod tmod) {
    const t_quad* tmap = get_static_map (tmod);
    if (tmap == nullptr) return nullptr;
    // allocate table and copy
    t_quad* result = new t_quad[256];
    for (long i = 0; i < 256; i++) result[i] = tmap[i];
    return result;
  }

  // get the decoding map
  static Unimapper* get_decode_map (const Encoding::t_tmod tmod) {
    // get the encoding map and check result
    const t_quad* tmap = get_static_map (tmod);
    if (tmap == nullptr) return nullptr;
    // non default encoding
    Unimapper* umap = new Unimapper;
    for (long i = 0; i < 256; i++) {
      if (tmap[i] == 0xFFFFFFFFUL) continue;
      umap->add (tmap[i], i);
    }
    return umap;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new transcoder
  
  Transcoder::Transcoder (void) {
    d_tmod = Encoding::TMOD_NONE;
    p_tmap = nullptr;
    p_umap = nullptr;
  }
  
  // create a transcoder by mode
  
  Transcoder::Transcoder (const Encoding::t_tmod tmod) {
    p_tmap = nullptr;
    p_umap = nullptr;
    settmod (tmod);
  }
  
  // destroy this transcoder

  Transcoder::~Transcoder (void) {
    delete [] p_tmap;
    delete    p_umap;
  }

  // return the class name

  String Transcoder::repr (void) const {
    return "Transcoder";
  }

  // return the transcoding mode

  Encoding::t_tmod Transcoder::gettmod (void) const {
    rdlock ();
    try {
      Encoding::t_tmod result = d_tmod;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the transcoding mode

  void Transcoder::settmod (const Encoding::t_tmod tmod) {
    wrlock ();
    try {
      delete [] p_tmap;
      delete p_umap;
      d_tmod = tmod;
      p_tmap = get_encode_map (tmod);
      p_umap = get_decode_map (tmod);
      unlock ();
    } catch (...) {
      d_tmod = Encoding::TMOD_NONE;
      p_tmap = nullptr;
      p_umap = nullptr;      
      unlock ();
      throw;
    }
  }

  // return true if a character can be encoded

  bool Transcoder::valid (const char c)  const {
    rdlock ();
    try {
      // check for default mode
      if (d_tmod == Encoding::TMOD_NONE) {
	unlock ();
	return true;
      }
      if (p_tmap == nullptr) {
	throw Exception ("codeset-error", "cannot access encoding table");
      }
      // get result and check mapping
      t_byte index  = c;
      t_quad result = (p_tmap[index] == 0xFFFFFFFF) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a character can be decoded

  bool Transcoder::valid (const t_quad c)  const {
    rdlock ();
    try {
      // check for default mode
      if (d_tmod == Encoding::TMOD_NONE) {
	unlock ();
	return true;
      }
      if (p_umap == nullptr) {
	throw Exception ("codeset-error", "cannot access decoding table");
      }
      // get result and check mapping
      bool result = p_umap->exists (c);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode a character
  
  t_quad Transcoder::encode (const char c) const {
    rdlock ();
    try {
      // check for default mode
      if (d_tmod == Encoding::TMOD_NONE) {
	t_quad result = Unicode::toquad (c);
	unlock ();
	return result;
      }
      if (p_tmap == nullptr) {
	throw Exception ("codeset-error", "cannot access encoding table");
      }
      // get result and check mapping
      t_byte index  = c;
      t_quad result = p_tmap[index];
      if (result == 0xFFFFFFFF) {
	throw Exception ("codeset-error", "cannot encode character", c);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode a character buffer

  t_quad* Transcoder::encode (const char* s) const {
   rdlock ();
    try {
      long size = Ascii::strlen (s);
      t_quad* result = encode (s, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // encode a character buffer by size

  t_quad* Transcoder::encode (const char* s, const long size) const {
    // check for nil
    if (size <= 0) return nullptr;
    // lock and encode
    rdlock ();
    t_quad* result = new t_quad[size+1];
    try {
      // loop and encode
      for (long i = 0; i < size; i++) result[i] = encode (s[i]);
      // mark the end of string
      result[size] = nilq;
      unlock ();
      return result;
    } catch (...) {
      delete [] result;
      unlock ();
      throw;
    }
  }

  // decode a unicode character
  
  char Transcoder::decode (const t_quad c) const {
    rdlock ();
    try {
      // check for default mode
      if (d_tmod == Encoding::TMOD_NONE) {
	char result = Unicode::tochar (c);
	unlock ();
	return result;
      }
      if (p_umap == nullptr) {
	throw Exception ("codeset-error", "cannot access decoding table");
      }
      // get result and check mapping in the translation
      char result = p_umap->lookup (c);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an output buffer

  char* Transcoder::decode (const t_quad* s) const {
    rdlock ();
    try {
      long size = Unicode::strlen (s);
      char* result = decode (s, size);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an output buffer

  char* Transcoder::decode (const t_quad* s, const long size) const {
    // check the size
    if (size <= 0) return nullptr;
    // allocate the character buffer
    char* buf = new char[size+1];
    rdlock ();
    try {
      // loop in the buffer and encode
      for (long i = 0; i < size; i++) buf[i]= decode (s[i]);
      // mark the end of string
      buf[size] = nilc;
      unlock ();
      return buf;
    } catch (...) {
      delete [] buf;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_ZMOD_LENGTH = 16;
  static QuarkZone  zmod (QUARK_ZMOD_LENGTH);

  static const long QUARK_TMODNONE = zmod.intern ("DEFAULT");
  static const long QUARK_TMOD5901 = zmod.intern ("I8859-01");
  static const long QUARK_TMOD5902 = zmod.intern ("I8859-02");
  static const long QUARK_TMOD5903 = zmod.intern ("I8859-03");
  static const long QUARK_TMOD5904 = zmod.intern ("I8859-04");
  static const long QUARK_TMOD5905 = zmod.intern ("I8859-05");
  static const long QUARK_TMOD5906 = zmod.intern ("I8859-06");
  static const long QUARK_TMOD5907 = zmod.intern ("I8859-07");
  static const long QUARK_TMOD5908 = zmod.intern ("I8859-08");
  static const long QUARK_TMOD5909 = zmod.intern ("I8859-09");
  static const long QUARK_TMOD5910 = zmod.intern ("I8859-10");
  static const long QUARK_TMOD5911 = zmod.intern ("I8859-11");
  static const long QUARK_TMOD5913 = zmod.intern ("I8859-13");
  static const long QUARK_TMOD5914 = zmod.intern ("I8859-14");
  static const long QUARK_TMOD5915 = zmod.intern ("I8859-15");
  static const long QUARK_TMOD5916 = zmod.intern ("I8859-16");
  static const long QUARK_TSCODER  = String::intern ("Transcoder");

  // map a quark to a transcoding mode
  static inline Encoding::t_tmod quark_to_tmod (const long quark) {
    if (quark == QUARK_TMODNONE) return Encoding::TMOD_NONE;
    if (quark == QUARK_TMOD5901) return Encoding::TMOD_5901;
    if (quark == QUARK_TMOD5902) return Encoding::TMOD_5902;
    if (quark == QUARK_TMOD5903) return Encoding::TMOD_5903;
    if (quark == QUARK_TMOD5904) return Encoding::TMOD_5904;
    if (quark == QUARK_TMOD5905) return Encoding::TMOD_5905;
    if (quark == QUARK_TMOD5906) return Encoding::TMOD_5906;
    if (quark == QUARK_TMOD5907) return Encoding::TMOD_5907;
    if (quark == QUARK_TMOD5908) return Encoding::TMOD_5908;
    if (quark == QUARK_TMOD5909) return Encoding::TMOD_5909;
    if (quark == QUARK_TMOD5910) return Encoding::TMOD_5910;
    if (quark == QUARK_TMOD5911) return Encoding::TMOD_5911;
    if (quark == QUARK_TMOD5913) return Encoding::TMOD_5913;
    if (quark == QUARK_TMOD5914) return Encoding::TMOD_5914;
    if (quark == QUARK_TMOD5915) return Encoding::TMOD_5915;
    if (quark == QUARK_TMOD5916) return Encoding::TMOD_5916;
    throw Exception ("coding-error", "cannot map transcoder mode");
  }

  // map a transcoding mode to a quark
  static inline long tmod_to_quark (const Encoding::t_tmod tmod) {
    if (tmod == Encoding::TMOD_NONE) return QUARK_TMODNONE;
    if (tmod == Encoding::TMOD_5901) return QUARK_TMOD5901;
    if (tmod == Encoding::TMOD_5902) return QUARK_TMOD5902;
    if (tmod == Encoding::TMOD_5903) return QUARK_TMOD5903;
    if (tmod == Encoding::TMOD_5904) return QUARK_TMOD5904;
    if (tmod == Encoding::TMOD_5905) return QUARK_TMOD5905;
    if (tmod == Encoding::TMOD_5906) return QUARK_TMOD5906;
    if (tmod == Encoding::TMOD_5907) return QUARK_TMOD5907;
    if (tmod == Encoding::TMOD_5908) return QUARK_TMOD5908;
    if (tmod == Encoding::TMOD_5909) return QUARK_TMOD5909;
    if (tmod == Encoding::TMOD_5910) return QUARK_TMOD5910;
    if (tmod == Encoding::TMOD_5911) return QUARK_TMOD5911;
    if (tmod == Encoding::TMOD_5913) return QUARK_TMOD5913;
    if (tmod == Encoding::TMOD_5914) return QUARK_TMOD5914;
    if (tmod == Encoding::TMOD_5915) return QUARK_TMOD5915;
    if (tmod == Encoding::TMOD_5916) return QUARK_TMOD5916;
    throw Exception ("coding-error", "cannot map transcoder mode");
  }

  // map an enumeration item to a transcoding mode
  static inline Encoding::t_tmod item_to_tmod (const Item& item) {
    // check for a stream item 
    if (item.gettid () != QUARK_TSCODER)
      throw Exception ("item-error", "item is not a transcoder item");
    // map the item to the enumeration
    long quark = item.getquark ();
    // map the qaurk
    return quark_to_tmod (quark);
  }

  // map a transccoding mode to an item
  static inline Item* tmod_to_item (const Encoding::t_tmod tmod) {
    long quark = tmod_to_quark (tmod);
    return new Item (QUARK_TSCODER, quark);
  }

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  static const long QUARK_ENCODE  = zone.intern ("encode");
  static const long QUARK_DECODE  = zone.intern ("decode");
  static const long QUARK_GETTMOD = zone.intern ("get-transcoding-mode");
  static const long QUARK_SETTMOD = zone.intern ("set-transcoding-mode");
  
  // evaluate a quark statically
  
  Object* Transcoder::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (zmod.exists (quark) == true) {
      return new Item (QUARK_TSCODER, quark);
    }
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // create a new object in a generic way
  
  Object* Transcoder::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return new Transcoder;
    if (argc == 1) {
      Object* iobj = argv->get (0);
      Item*   item = dynamic_cast <Item*> (iobj);
      if (item == nullptr) {	  
	throw Exception ("argument-error", 
			 "invalid arguments with transcoder constructor");
      }
      Encoding::t_tmod tmod = item_to_tmod (*item);
      return new Transcoder (tmod);      
    }
    throw Exception ("argument-error", "too many argument for transcoder");
  }

  // return true if the given quark is defined
  
  bool Transcoder::isquark (const long quark, const bool hflg) const {
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
  
  Object* Transcoder::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETTMOD) return tmod_to_item (gettmod ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ENCODE) {
	char c = argv->getbyte (0);
	return new Character (encode (c));
      }
      if (quark == QUARK_DECODE) {
	t_quad c = argv->getchar (0);
	return new Byte (decode (c));
      }
      if (quark == QUARK_SETTMOD) {
	Object* iobj = argv->get (0);
	Item*   item = dynamic_cast <Item*> (iobj);
	if (item == nullptr) {	  
	  throw Exception ("argument-error", 
			   "invalid arguments with set-mode");
	}
	Encoding::t_tmod tmod = item_to_tmod (*item);
	settmod (tmod);
	return nullptr;
      }
      if (quark == QUARK_VALIDP) {
	Object* obj = argv->get (0);
	// check for a byte
	Byte* bobj = dynamic_cast<Byte*> (obj);
	if (bobj != nullptr) {
	  char c = bobj->tobyte ();
	  return new Boolean (valid (c));
	}
	// check for a character
	Character* cobj = dynamic_cast<Character*> (obj);
	if (cobj != nullptr) {
	  t_quad c = cobj->toquad ();
	  return new Boolean (valid (c));
	}
	throw Exception ("type-error", "invalid object with valid-p predicate",
			 Object::repr (obj));
      }      
    }
    // apply these arguments with the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
