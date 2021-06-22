// ---------------------------------------------------------------------------
// - Pixel.hxx                                                               -
// - afnix:dip service - private pixel definition                            -
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

#ifndef  AFNIX_PIXEL_HXX
#define  AFNIX_PIXEL_HXX

#ifndef  AFNIX_ITEM_HPP
#include "Item.hpp"
#endif

#ifndef  AFNIX_PIXEL_HPP
#include "Pixel.hpp"
#endif

#ifndef  AFNIX_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace afnix {
  // the pixel eval quarks
  static const long QUARK_PIXEL    = String::intern ("Pixel");
  static const long QUARK_PFMTNONE = String::intern ("PFMT-NONE");
  static const long QUARK_PFMTBYTE = String::intern ("PFMT-BYTE");
  static const long QUARK_PFMTWORD = String::intern ("PFMT-WORD");
  static const long QUARK_PFMTREAL = String::intern ("PFMT-REAL");
  static const long QUARK_PFMTFLOT = String::intern ("PFMT-FLOT");
  static const long QUARK_PFMTRGBA = String::intern ("PFMT-RGBA");
  static const long QUARK_PFMTBGRA = String::intern ("PFMT-BGRA");
  static const long QUARK_PFMTRGBO = String::intern ("PFMT-RGBO");
  static const long QUARK_PFMTBGRO = String::intern ("PFMT-BGRO");
  static const long QUARK_PFMTRGBR = String::intern ("PFMT-RGBR");
  static const long QUARK_PFMTRGBF = String::intern ("PFMT-RGBF");

  // map an item to a pixel format
  static inline Pixel::t_pfmt item_to_pfmt (const Item& item) {
    // check for a pixel item
    if (item.gettid () != QUARK_PIXEL)
      throw Exception ("item-error", "item is not a pixel item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_PFMTNONE) return Pixel::PFMT_NONE;
    if (quark == QUARK_PFMTBYTE) return Pixel::PFMT_BYTE;
    if (quark == QUARK_PFMTWORD) return Pixel::PFMT_WORD;
    if (quark == QUARK_PFMTREAL) return Pixel::PFMT_REAL;
    if (quark == QUARK_PFMTFLOT) return Pixel::PFMT_FLOT;
    if (quark == QUARK_PFMTRGBA) return Pixel::PFMT_RGBA;
    if (quark == QUARK_PFMTBGRA) return Pixel::PFMT_BGRA;
    if (quark == QUARK_PFMTRGBO) return Pixel::PFMT_RGBO;
    if (quark == QUARK_PFMTBGRO) return Pixel::PFMT_BGRO;
    if (quark == QUARK_PFMTRGBR) return Pixel::PFMT_RGBR;
    if (quark == QUARK_PFMTRGBF) return Pixel::PFMT_RGBF;
    throw Exception ("item-error", "cannot map item to pixel format");
  }

  // map a pixel format to an item
  static inline Item* pfmt_to_item (const Pixel::t_pfmt pfmt) {
    switch (pfmt) {
    case Pixel::PFMT_NONE:
      return new Item (QUARK_PIXEL, QUARK_PFMTNONE);
      break;
    case Pixel::PFMT_BYTE:
      return new Item (QUARK_PIXEL, QUARK_PFMTBYTE);
      break;
    case Pixel::PFMT_WORD:
      return new Item (QUARK_PIXEL, QUARK_PFMTWORD);
      break;
    case Pixel::PFMT_REAL:
      return new Item (QUARK_PIXEL, QUARK_PFMTREAL);
      break;
    case Pixel::PFMT_FLOT:
      return new Item (QUARK_PIXEL, QUARK_PFMTFLOT);
      break;
    case Pixel::PFMT_RGBA:
      return new Item (QUARK_PIXEL, QUARK_PFMTRGBA);
      break;
    case Pixel::PFMT_BGRA:
      return new Item (QUARK_PIXEL, QUARK_PFMTBGRA);
      break;
    case Pixel::PFMT_RGBO:
      return new Item (QUARK_PIXEL, QUARK_PFMTRGBO);
      break;
    case Pixel::PFMT_BGRO:
      return new Item (QUARK_PIXEL, QUARK_PFMTBGRO);
      break;
    case Pixel::PFMT_RGBR:
      return new Item (QUARK_PIXEL, QUARK_PFMTRGBR);
      break;
    case Pixel::PFMT_RGBF:
      return new Item (QUARK_PIXEL, QUARK_PFMTRGBF);
      break;
    }
    return nullptr;
  }
}

#endif
