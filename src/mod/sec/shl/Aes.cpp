// ---------------------------------------------------------------------------
// - Aes.cpp                                                                 -
// - afnix:sec module - eas (rijndael) class implementation                  -
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

#include "Aes.hpp"
#include "Vector.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

 // aes state definitions
  static const long  AES_STATE_COL = 4;
  static const long  AES_STATE_ROW = 4;
  static const long  AES_STATE_LEN = AES_STATE_COL * AES_STATE_ROW;

  // aes block cipher constants
  static const char* AES_ALGO_NAME = "AES";
  static const long  AES_BLOK_SIZE = AES_STATE_LEN;

  // aes valid keys constants
  static const long  AES_K128_BITS = 128;
  static const long  AES_K192_BITS = 192;
  static const long  AES_K256_BITS = 256;

  // aes forward s-box
  const t_byte AES_FORWARD_SBOX [256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
  };

  // aes reverse s-box
  const t_byte AES_REVERSE_SBOX [256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38,
    0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87,
    0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D,
    0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2,
    0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA,
    0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A,
    0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02,
    0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA,
    0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89,
    0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20,
    0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31,
    0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D,
    0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0,
    0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26,
    0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
  };

  // multiplication in CF2(8) with 0x02
  const t_byte AES_MULT_02 [256] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
    0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E,
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E,
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E,
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E,
    0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E,
    0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E,
    0x70, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E,
    0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E,
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E,
    0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE,
    0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE,
    0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
    0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
    0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
    0xF0, 0xF2, 0xF4, 0xF6, 0xF8, 0xFA, 0xFC, 0xFE,
    0x1B, 0x19, 0x1F, 0x1D, 0x13, 0x11, 0x17, 0x15,
    0x0B, 0x09, 0x0F, 0x0D, 0x03, 0x01, 0x07, 0x05,
    0x3B, 0x39, 0x3F, 0x3D, 0x33, 0x31, 0x37, 0x35,
    0x2B, 0x29, 0x2F, 0x2D, 0x23, 0x21, 0x27, 0x25,
    0x5B, 0x59, 0x5F, 0x5D, 0x53, 0x51, 0x57, 0x55,
    0x4B, 0x49, 0x4F, 0x4D, 0x43, 0x41, 0x47, 0x45,
    0x7B, 0x79, 0x7F, 0x7D, 0x73, 0x71, 0x77, 0x75,
    0x6B, 0x69, 0x6F, 0x6D, 0x63, 0x61, 0x67, 0x65,
    0x9B, 0x99, 0x9F, 0x9D, 0x93, 0x91, 0x97, 0x95,
    0x8B, 0x89, 0x8F, 0x8D, 0x83, 0x81, 0x87, 0x85,
    0xBB, 0xB9, 0xBF, 0xBD, 0xB3, 0xB1, 0xB7, 0xB5,
    0xAB, 0xA9, 0xAF, 0xAD, 0xA3, 0xA1, 0xA7, 0xA5,
    0xDB, 0xD9, 0xDF, 0xDD, 0xD3, 0xD1, 0xD7, 0xD5,
    0xCB, 0xC9, 0xCF, 0xCD, 0xC3, 0xC1, 0xC7, 0xC5,
    0xFB, 0xF9, 0xFF, 0xFD, 0xF3, 0xF1, 0xF7, 0xF5,
    0xEB, 0xE9, 0xEF, 0xED, 0xE3, 0xE1, 0xE7, 0xE5  
  };
  
  // multiplication in CF2(8) with 0x03
  const t_byte AES_MULT_03 [256] = {
    0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09,
    0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11,
    0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39,
    0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21,
    0x60, 0x63, 0x66, 0x65, 0x6C, 0x6F, 0x6A, 0x69,
    0x78, 0x7B, 0x7E, 0x7D, 0x74, 0x77, 0x72, 0x71,
    0x50, 0x53, 0x56, 0x55, 0x5C, 0x5F, 0x5A, 0x59,
    0x48, 0x4B, 0x4E, 0x4D, 0x44, 0x47, 0x42, 0x41,
    0xC0, 0xC3, 0xC6, 0xC5, 0xCC, 0xCF, 0xCA, 0xC9,
    0xD8, 0xDB, 0xDE, 0xDD, 0xD4, 0xD7, 0xD2, 0xD1,
    0xF0, 0xF3, 0xF6, 0xF5, 0xFC, 0xFF, 0xFA, 0xF9,
    0xE8, 0xEB, 0xEE, 0xED, 0xE4, 0xE7, 0xE2, 0xE1,
    0xA0, 0xA3, 0xA6, 0xA5, 0xAC, 0xAF, 0xAA, 0xA9,
    0xB8, 0xBB, 0xBE, 0xBD, 0xB4, 0xB7, 0xB2, 0xB1,
    0x90, 0x93, 0x96, 0x95, 0x9C, 0x9F, 0x9A, 0x99,
    0x88, 0x8B, 0x8E, 0x8D, 0x84, 0x87, 0x82, 0x81,
    0x9B, 0x98, 0x9D, 0x9E, 0x97, 0x94, 0x91, 0x92,
    0x83, 0x80, 0x85, 0x86, 0x8F, 0x8C, 0x89, 0x8A,
    0xAB, 0xA8, 0xAD, 0xAE, 0xA7, 0xA4, 0xA1, 0xA2,
    0xB3, 0xB0, 0xB5, 0xB6, 0xBF, 0xBC, 0xB9, 0xBA,
    0xFB, 0xF8, 0xFD, 0xFE, 0xF7, 0xF4, 0xF1, 0xF2,
    0xE3, 0xE0, 0xE5, 0xE6, 0xEF, 0xEC, 0xE9, 0xEA,
    0xCB, 0xC8, 0xCD, 0xCE, 0xC7, 0xC4, 0xC1, 0xC2,
    0xD3, 0xD0, 0xD5, 0xD6, 0xDF, 0xDC, 0xD9, 0xDA,
    0x5B, 0x58, 0x5D, 0x5E, 0x57, 0x54, 0x51, 0x52,
    0x43, 0x40, 0x45, 0x46, 0x4F, 0x4C, 0x49, 0x4A,
    0x6B, 0x68, 0x6D, 0x6E, 0x67, 0x64, 0x61, 0x62,
    0x73, 0x70, 0x75, 0x76, 0x7F, 0x7C, 0x79, 0x7A,
    0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32,
    0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A,
    0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02,
    0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A
  };
  
  // multiplication in CF2(8) with 0x09
  const t_byte AES_MULT_09 [256] = {
    0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F,
    0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77,
    0x90, 0x99, 0x82, 0x8B, 0xB4, 0xBD, 0xA6, 0xAF,
    0xD8, 0xD1, 0xCA, 0xC3, 0xFC, 0xF5, 0xEE, 0xE7,
    0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04,
    0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C,
    0xAB, 0xA2, 0xB9, 0xB0, 0x8F, 0x86, 0x9D, 0x94,
    0xE3, 0xEA, 0xF1, 0xF8, 0xC7, 0xCE, 0xD5, 0xDC,
    0x76, 0x7F, 0x64, 0x6D, 0x52, 0x5B, 0x40, 0x49,
    0x3E, 0x37, 0x2C, 0x25, 0x1A, 0x13, 0x08, 0x01,
    0xE6, 0xEF, 0xF4, 0xFD, 0xC2, 0xCB, 0xD0, 0xD9,
    0xAE, 0xA7, 0xBC, 0xB5, 0x8A, 0x83, 0x98, 0x91,
    0x4D, 0x44, 0x5F, 0x56, 0x69, 0x60, 0x7B, 0x72,
    0x05, 0x0C, 0x17, 0x1E, 0x21, 0x28, 0x33, 0x3A,
    0xDD, 0xD4, 0xCF, 0xC6, 0xF9, 0xF0, 0xEB, 0xE2,
    0x95, 0x9C, 0x87, 0x8E, 0xB1, 0xB8, 0xA3, 0xAA,
    0xEC, 0xE5, 0xFE, 0xF7, 0xC8, 0xC1, 0xDA, 0xD3,
    0xA4, 0xAD, 0xB6, 0xBF, 0x80, 0x89, 0x92, 0x9B,
    0x7C, 0x75, 0x6E, 0x67, 0x58, 0x51, 0x4A, 0x43,
    0x34, 0x3D, 0x26, 0x2F, 0x10, 0x19, 0x02, 0x0B,
    0xD7, 0xDE, 0xC5, 0xCC, 0xF3, 0xFA, 0xE1, 0xE8,
    0x9F, 0x96, 0x8D, 0x84, 0xBB, 0xB2, 0xA9, 0xA0,
    0x47, 0x4E, 0x55, 0x5C, 0x63, 0x6A, 0x71, 0x78,
    0x0F, 0x06, 0x1D, 0x14, 0x2B, 0x22, 0x39, 0x30,
    0x9A, 0x93, 0x88, 0x81, 0xBE, 0xB7, 0xAC, 0xA5,
    0xD2, 0xDB, 0xC0, 0xC9, 0xF6, 0xFF, 0xE4, 0xED,
    0x0A, 0x03, 0x18, 0x11, 0x2E, 0x27, 0x3C, 0x35,
    0x42, 0x4B, 0x50, 0x59, 0x66, 0x6F, 0x74, 0x7D,
    0xA1, 0xA8, 0xB3, 0xBA, 0x85, 0x8C, 0x97, 0x9E,
    0xE9, 0xE0, 0xFB, 0xF2, 0xCD, 0xC4, 0xDF, 0xD6,
    0x31, 0x38, 0x23, 0x2A, 0x15, 0x1C, 0x07, 0x0E,
    0x79, 0x70, 0x6B, 0x62, 0x5D, 0x54, 0x4F, 0x46
  };
  
  // multiplication in CF2(8) with 0x0B
  const t_byte AES_MULT_0B [256] = {
    0x00, 0x0B, 0x16, 0x1D, 0x2C, 0x27, 0x3A, 0x31,
    0x58, 0x53, 0x4E, 0x45, 0x74, 0x7F, 0x62, 0x69,
    0xB0, 0xBB, 0xA6, 0xAD, 0x9C, 0x97, 0x8A, 0x81,
    0xE8, 0xE3, 0xFE, 0xF5, 0xC4, 0xCF, 0xD2, 0xD9,
    0x7B, 0x70, 0x6D, 0x66, 0x57, 0x5C, 0x41, 0x4A,
    0x23, 0x28, 0x35, 0x3E, 0x0F, 0x04, 0x19, 0x12,
    0xCB, 0xC0, 0xDD, 0xD6, 0xE7, 0xEC, 0xF1, 0xFA,
    0x93, 0x98, 0x85, 0x8E, 0xBF, 0xB4, 0xA9, 0xA2,
    0xF6, 0xFD, 0xE0, 0xEB, 0xDA, 0xD1, 0xCC, 0xC7,
    0xAE, 0xA5, 0xB8, 0xB3, 0x82, 0x89, 0x94, 0x9F,
    0x46, 0x4D, 0x50, 0x5B, 0x6A, 0x61, 0x7C, 0x77,
    0x1E, 0x15, 0x08, 0x03, 0x32, 0x39, 0x24, 0x2F,
    0x8D, 0x86, 0x9B, 0x90, 0xA1, 0xAA, 0xB7, 0xBC,
    0xD5, 0xDE, 0xC3, 0xC8, 0xF9, 0xF2, 0xEF, 0xE4,
    0x3D, 0x36, 0x2B, 0x20, 0x11, 0x1A, 0x07, 0x0C,
    0x65, 0x6E, 0x73, 0x78, 0x49, 0x42, 0x5F, 0x54,
    0xF7, 0xFC, 0xE1, 0xEA, 0xDB, 0xD0, 0xCD, 0xC6,
    0xAF, 0xA4, 0xB9, 0xB2, 0x83, 0x88, 0x95, 0x9E,
    0x47, 0x4C, 0x51, 0x5A, 0x6B, 0x60, 0x7D, 0x76,
    0x1F, 0x14, 0x09, 0x02, 0x33, 0x38, 0x25, 0x2E,
    0x8C, 0x87, 0x9A, 0x91, 0xA0, 0xAB, 0xB6, 0xBD,
    0xD4, 0xDF, 0xC2, 0xC9, 0xF8, 0xF3, 0xEE, 0xE5,
    0x3C, 0x37, 0x2A, 0x21, 0x10, 0x1B, 0x06, 0x0D,
    0x64, 0x6F, 0x72, 0x79, 0x48, 0x43, 0x5E, 0x55,
    0x01, 0x0A, 0x17, 0x1C, 0x2D, 0x26, 0x3B, 0x30,
    0x59, 0x52, 0x4F, 0x44, 0x75, 0x7E, 0x63, 0x68,
    0xB1, 0xBA, 0xA7, 0xAC, 0x9D, 0x96, 0x8B, 0x80,
    0xE9, 0xE2, 0xFF, 0xF4, 0xC5, 0xCE, 0xD3, 0xD8,
    0x7A, 0x71, 0x6C, 0x67, 0x56, 0x5D, 0x40, 0x4B,
    0x22, 0x29, 0x34, 0x3F, 0x0E, 0x05, 0x18, 0x13,
    0xCA, 0xC1, 0xDC, 0xD7, 0xE6, 0xED, 0xF0, 0xFB,
    0x92, 0x99, 0x84, 0x8F, 0xBE, 0xB5, 0xA8, 0xA3
  };
  
  // multiplication in CF2(8) with 0x0D
  const t_byte AES_MULT_0D [256] = {
    0x00, 0x0D, 0x1A, 0x17, 0x34, 0x39, 0x2E, 0x23,
    0x68, 0x65, 0x72, 0x7F, 0x5C, 0x51, 0x46, 0x4B,
    0xD0, 0xDD, 0xCA, 0xC7, 0xE4, 0xE9, 0xFE, 0xF3,
    0xB8, 0xB5, 0xA2, 0xAF, 0x8C, 0x81, 0x96, 0x9B,
    0xBB, 0xB6, 0xA1, 0xAC, 0x8F, 0x82, 0x95, 0x98,
    0xD3, 0xDE, 0xC9, 0xC4, 0xE7, 0xEA, 0xFD, 0xF0,
    0x6B, 0x66, 0x71, 0x7C, 0x5F, 0x52, 0x45, 0x48,
    0x03, 0x0E, 0x19, 0x14, 0x37, 0x3A, 0x2D, 0x20,
    0x6D, 0x60, 0x77, 0x7A, 0x59, 0x54, 0x43, 0x4E,
    0x05, 0x08, 0x1F, 0x12, 0x31, 0x3C, 0x2B, 0x26,
    0xBD, 0xB0, 0xA7, 0xAA, 0x89, 0x84, 0x93, 0x9E,
    0xD5, 0xD8, 0xCF, 0xC2, 0xE1, 0xEC, 0xFB, 0xF6,
    0xD6, 0xDB, 0xCC, 0xC1, 0xE2, 0xEF, 0xF8, 0xF5,
    0xBE, 0xB3, 0xA4, 0xA9, 0x8A, 0x87, 0x90, 0x9D,
    0x06, 0x0B, 0x1C, 0x11, 0x32, 0x3F, 0x28, 0x25,
    0x6E, 0x63, 0x74, 0x79, 0x5A, 0x57, 0x40, 0x4D,
    0xDA, 0xD7, 0xC0, 0xCD, 0xEE, 0xE3, 0xF4, 0xF9,
    0xB2, 0xBF, 0xA8, 0xA5, 0x86, 0x8B, 0x9C, 0x91,
    0x0A, 0x07, 0x10, 0x1D, 0x3E, 0x33, 0x24, 0x29,
    0x62, 0x6F, 0x78, 0x75, 0x56, 0x5B, 0x4C, 0x41,
    0x61, 0x6C, 0x7B, 0x76, 0x55, 0x58, 0x4F, 0x42,
    0x09, 0x04, 0x13, 0x1E, 0x3D, 0x30, 0x27, 0x2A,
    0xB1, 0xBC, 0xAB, 0xA6, 0x85, 0x88, 0x9F, 0x92,
    0xD9, 0xD4, 0xC3, 0xCE, 0xED, 0xE0, 0xF7, 0xFA,
    0xB7, 0xBA, 0xAD, 0xA0, 0x83, 0x8E, 0x99, 0x94,
    0xDF, 0xD2, 0xC5, 0xC8, 0xEB, 0xE6, 0xF1, 0xFC,
    0x67, 0x6A, 0x7D, 0x70, 0x53, 0x5E, 0x49, 0x44,
    0x0F, 0x02, 0x15, 0x18, 0x3B, 0x36, 0x21, 0x2C,
    0x0C, 0x01, 0x16, 0x1B, 0x38, 0x35, 0x22, 0x2F,
    0x64, 0x69, 0x7E, 0x73, 0x50, 0x5D, 0x4A, 0x47,
    0xDC, 0xD1, 0xC6, 0xCB, 0xE8, 0xE5, 0xF2, 0xFF,
    0xB4, 0xB9, 0xAE, 0xA3, 0x80, 0x8D, 0x9A, 0x97
  };
  
  // multiplication in CF2(8) with 0x0E
  const t_byte AES_MULT_0E [256] = {
    0x00, 0x0E, 0x1C, 0x12, 0x38, 0x36, 0x24, 0x2A,
    0x70, 0x7E, 0x6C, 0x62, 0x48, 0x46, 0x54, 0x5A,
    0xE0, 0xEE, 0xFC, 0xF2, 0xD8, 0xD6, 0xC4, 0xCA,
    0x90, 0x9E, 0x8C, 0x82, 0xA8, 0xA6, 0xB4, 0xBA,
    0xDB, 0xD5, 0xC7, 0xC9, 0xE3, 0xED, 0xFF, 0xF1,
    0xAB, 0xA5, 0xB7, 0xB9, 0x93, 0x9D, 0x8F, 0x81,
    0x3B, 0x35, 0x27, 0x29, 0x03, 0x0D, 0x1F, 0x11,
    0x4B, 0x45, 0x57, 0x59, 0x73, 0x7D, 0x6F, 0x61,
    0xAD, 0xA3, 0xB1, 0xBF, 0x95, 0x9B, 0x89, 0x87,
    0xDD, 0xD3, 0xC1, 0xCF, 0xE5, 0xEB, 0xF9, 0xF7,
    0x4D, 0x43, 0x51, 0x5F, 0x75, 0x7B, 0x69, 0x67,
    0x3D, 0x33, 0x21, 0x2F, 0x05, 0x0B, 0x19, 0x17,
    0x76, 0x78, 0x6A, 0x64, 0x4E, 0x40, 0x52, 0x5C,
    0x06, 0x08, 0x1A, 0x14, 0x3E, 0x30, 0x22, 0x2C,
    0x96, 0x98, 0x8A, 0x84, 0xAE, 0xA0, 0xB2, 0xBC,
    0xE6, 0xE8, 0xFA, 0xF4, 0xDE, 0xD0, 0xC2, 0xCC,
    0x41, 0x4F, 0x5D, 0x53, 0x79, 0x77, 0x65, 0x6B,
    0x31, 0x3F, 0x2D, 0x23, 0x09, 0x07, 0x15, 0x1B,
    0xA1, 0xAF, 0xBD, 0xB3, 0x99, 0x97, 0x85, 0x8B,
    0xD1, 0xDF, 0xCD, 0xC3, 0xE9, 0xE7, 0xF5, 0xFB,
    0x9A, 0x94, 0x86, 0x88, 0xA2, 0xAC, 0xBE, 0xB0,
    0xEA, 0xE4, 0xF6, 0xF8, 0xD2, 0xDC, 0xCE, 0xC0,
    0x7A, 0x74, 0x66, 0x68, 0x42, 0x4C, 0x5E, 0x50,
    0x0A, 0x04, 0x16, 0x18, 0x32, 0x3C, 0x2E, 0x20,
    0xEC, 0xE2, 0xF0, 0xFE, 0xD4, 0xDA, 0xC8, 0xC6,
    0x9C, 0x92, 0x80, 0x8E, 0xA4, 0xAA, 0xB8, 0xB6,
    0x0C, 0x02, 0x10, 0x1E, 0x34, 0x3A, 0x28, 0x26,
    0x7C, 0x72, 0x60, 0x6E, 0x44, 0x4A, 0x58, 0x56,
    0x37, 0x39, 0x2B, 0x25, 0x0F, 0x01, 0x13, 0x1D,
    0x47, 0x49, 0x5B, 0x55, 0x7F, 0x71, 0x63, 0x6D,
    0xD7, 0xD9, 0xCB, 0xC5, 0xEF, 0xE1, 0xF3, 0xFD,
    0xA7, 0xA9, 0xBB, 0xB5, 0x9F, 0x91, 0x83, 0x8D
  };
  
  // round constants
  const t_byte AES_RCON[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
    0x1B, 0x36
  };

  // map key type to cipher round
  static inline long aes_key_round (const Key& key) {
    if (key.gettype () == Key::CKEY_KSYM) {
      // 128 bits: 10 rounds
      if (key.getbits () == AES_K128_BITS) return 10;
      // 192 bits: 12 rounds
      if (key.getbits () == AES_K192_BITS) return 12;
      // 256 bits: 14 rounds
      if (key.getbits () == AES_K256_BITS) return 14;
    }
    throw Exception ("aes-error", "invalid key type or size with aes");
  }

  // rotate a key word
  static inline void aes_key_rotw (t_byte* word) {
    t_byte hb = word[0];
    word[0]   = word[1];
    word[1]   = word[2];
    word[2]   = word[3];
    word[3]   = hb;
  }

  // substitute a key word
  static inline void aes_key_subw (t_byte* word) {
    word[0] = AES_FORWARD_SBOX[word[0]];
    word[1] = AES_FORWARD_SBOX[word[1]];
    word[2] = AES_FORWARD_SBOX[word[2]];
    word[3] = AES_FORWARD_SBOX[word[3]];
  }
  
  // mix a key word with a round constant
  static inline void aes_key_mixw (t_byte* word, const long ri) {
    word[0] = word[0] ^ AES_RCON[ri];
    word[1] = word[1] ^ 0x00;
    word[2] = word[2] ^ 0x00;
    word[3] = word[3] ^ 0x00;
  }

  // expand the key
  static void aes_key_expand (t_byte* rkey, const long rksz, const Key& key) {
    // check arguments
    if ((rkey == nullptr) || (rksz == 0L)) return;
    // clear the whole round key
    for (long k = 0L; k < rksz; k++) rkey[k] = nilc;
    // key size in bytes and words
    long cksz = key.getsize ();
    long kwsz = cksz / 4;
    // copy the key at the beginning
    for (long i = 0; i < cksz; i++) rkey[i] = key.getbyte (i);
    // expand the key
    for (long i = cksz; i < rksz; i+= 4) {
      // word index
      long wi = i / 4;
      // fill-in the word
      t_byte word[4];
      word[0] = rkey[i-4];
      word[1] = rkey[i-3];
      word[2] = rkey[i-2];
      word[3] = rkey[i-1];
      // mix the word
      if ((wi % kwsz) == 0) {
	// rotate the word
	aes_key_rotw (word);
	// substitute the word
	aes_key_subw (word);
	// mix with the round constant
	aes_key_mixw (word, wi/kwsz-1);
      } else if ((kwsz > 6) && ((wi % kwsz) == 4)) {
	aes_key_subw (word);
      }
      // set new word
      rkey[i]   = rkey[i - cksz]     ^ word[0];
      rkey[i+1] = rkey[i + 1 - cksz] ^ word[1];
      rkey[i+2] = rkey[i + 2 - cksz] ^ word[2];
      rkey[i+3] = rkey[i + 3 - cksz] ^ word[3];
    }
  }

  // state byte substitute with s-box
  static void aes_state_sub_bytes (t_byte* state) {
    if (state == nullptr) return;
    for (long i = 0; i < AES_STATE_LEN; i++) {
      long idx = state[i];
      state[i] = AES_FORWARD_SBOX[idx];
    }
  }

  // shift the state rows
  static void aes_state_shift_rows (t_byte* state) {
    // check state and compute columns
    if (state == nullptr) return;
    // shift row 1
    t_byte s10 = state[1];
    state[1]   = state[5];
    state[5]   = state[9];
    state[9]   = state[13];
    state[13]  = s10;
    // shift row 2
    t_byte s20 = state[2];
    t_byte s21 = state[6];
    state[2]   = state[10];
    state[6]   = state[14];
    state[10]  = s20;
    state[14]  = s21;
    // shift row 3
    t_byte s30 = state[3];
    t_byte s31 = state[7];
    t_byte s32 = state[11];
    state[3]   = state[15];
    state[7]   = s30;
    state[11]  = s31;
    state[15]  = s32;
  }

  // mix the state column
  static void aes_state_mix_columns (t_byte* state) {
    if (state == nullptr) return;
    for (long i = 0; i < AES_STATE_COL; i++) {
      // get state column
      t_byte s0 = state[i*4];
      t_byte s1 = state[i*4+1];
      t_byte s2 = state[i*4+2];
      t_byte s3 = state[i*4+3];
      // mix column by multiplication
      t_byte r0 = AES_MULT_02[s0] ^ AES_MULT_03[s1] ^ s2 ^ s3;
      t_byte r1 = s0 ^ AES_MULT_02[s1] ^ AES_MULT_03[s2] ^ s3;
      t_byte r2 = s0 ^ s1 ^ AES_MULT_02[s2] ^ AES_MULT_03[s3];
      t_byte r3 = AES_MULT_03[s0] ^ s1 ^ s2 ^ AES_MULT_02[s3];
      // update the state
      state[i*4]   = r0;
      state[i*4+1] = r1;
      state[i*4+2] = r2;
      state[i*4+3] = r3;
    }
  }

  // add the round key to the state
  static void aes_state_add_round_key (t_byte* state, const t_byte* rkey,
				       const long round) {
    if ((state == nullptr) || (rkey == nullptr)) return;
    long ridx = round * AES_STATE_LEN;
    for (long i = 0; i < AES_STATE_LEN; i++) {
      state[i] = state[i] ^ rkey[ridx+i];
    }
  }

  // state byte substitute with reverse s-box
  static void aes_inv_state_sub_bytes (t_byte* state) {
    if (state == nullptr) return;
    for (long i = 0; i < AES_STATE_LEN; i++) {
      long idx = state[i];
      state[i] = AES_REVERSE_SBOX[idx];
    }
  }

  // shift the state rows
  static void aes_inv_state_shift_rows (t_byte* state) {
    // check state and compute columns
    if (state == nullptr) return;
    // shift row 1
    t_byte s13 = state[13];
    state[13]  = state[9];
    state[9]   = state[5];
    state[5]   = state[1];
    state[1]   = s13;
    // shift row 2
    t_byte s23 = state[14];
    t_byte s22 = state[10];
    state[14]  = state[6];
    state[10]  = state[2];
    state[6]   = s23;
    state[2]   = s22;
    // shift row 3
    t_byte s33 = state[15];
    t_byte s32 = state[11];
    t_byte s31 = state[7];
    state[15]  = state[3];
    state[11]  = s33;
    state[7]   = s32;
    state[3]   = s31;
  }

  // mix the state column
  static void aes_inv_state_mix_columns (t_byte* state) {
    if (state == nullptr) return;
    for (long i = 0; i < AES_STATE_COL; i++) {
      // get state column
      t_byte s0 = state[i*4];
      t_byte s1 = state[i*4+1];
      t_byte s2 = state[i*4+2];
      t_byte s3 = state[i*4+3];
      // mix column by multiplication
      t_byte r0 = AES_MULT_0E[s0] ^ AES_MULT_0B[s1] ^ 
	          AES_MULT_0D[s2] ^ AES_MULT_09[s3];
      t_byte r1 = AES_MULT_09[s0] ^ AES_MULT_0E[s1] ^ 
	          AES_MULT_0B[s2] ^ AES_MULT_0D[s3];
      t_byte r2 = AES_MULT_0D[s0] ^ AES_MULT_09[s1] ^ 
	          AES_MULT_0E[s2] ^ AES_MULT_0B[s3];
      t_byte r3 = AES_MULT_0B[s0] ^ AES_MULT_0D[s1] ^ 
	          AES_MULT_09[s2] ^ AES_MULT_0E[s3];

      // update the state
      state[i*4]   = r0;
      state[i*4+1] = r1;
      state[i*4+2] = r2;
      state[i*4+3] = r3;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an aes cipher by key

  Aes::Aes (const Key& key) : ModeCipher (AES_ALGO_NAME, AES_BLOK_SIZE) {
    // initialize the cipher
    d_rnds = 0L;
    d_rksz = 0L;
    p_rkey = nullptr;
    // set the key
    setkey (key);
  }

  // create an aes cipher by key and flag

  Aes::Aes (const Key& key, 
	    const bool rflg) : ModeCipher (AES_ALGO_NAME, AES_BLOK_SIZE) {
    // initialize the cipher
    d_rnds = 0L;
    d_rksz = 0L;
    p_rkey = nullptr;
    // set the key
    setkey (key);
    // set the reverse flag
    setrflg (rflg);
  }

  // destroy this cipher

  Aes::~Aes (void) {
    delete [] p_rkey;
  }

  // return the class name

  String Aes::repr (void) const {
    return "Aes";
  }

  // reset this cipher
  
  void Aes::reset (void) {
    wrlock ();
    try {
      // reset the mode cipher
      ModeCipher::reset ();
      // reset round key
      d_rnds = 0L;
      d_rksz = 0L;
      delete [] p_rkey; p_rkey = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // clear this cipher

  void Aes::clear (void) {
    wrlock ();
    try {
      // cleat the mode cipher
      ModeCipher::clear ();
      // expand the key
      aes_key_expand (p_rkey, d_rksz, d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // set the cipher key

  void Aes::setkey (const Key& key) {
    wrlock ();
    try {
      // set the base key
      ModeCipher::setkey (key);
      // reset key parameters
      delete [] p_rkey; p_rkey = nullptr;
      d_rnds = aes_key_round (key);
      d_rksz = AES_STATE_LEN * (d_rnds + 1);
      p_rkey = new t_byte[d_rksz];
      // expand the key
      aes_key_expand (p_rkey, d_rksz, d_ckey);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // encode a block buffer into another one

  void Aes::encode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // copy the input buffer to the state
      t_byte cbuf[d_cbsz];
      for (long i = 0; i < AES_STATE_LEN; i++) cbuf[i] = bi[i];
      // initial round key
      aes_state_add_round_key (cbuf, p_rkey, 0);
      // loop for all rounds
      for (long i = 1; i < d_rnds; i++) {
	aes_state_sub_bytes     (cbuf);
	aes_state_shift_rows    (cbuf);
	aes_state_mix_columns   (cbuf);
	aes_state_add_round_key (cbuf, p_rkey, i);
      }
      aes_state_sub_bytes     (cbuf);
      aes_state_shift_rows    (cbuf);
      aes_state_add_round_key (cbuf, p_rkey, d_rnds);
      // copy the state to the output buffer
      for (long i = 0; i < AES_STATE_LEN; i++) bo[i] = cbuf[i];
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // decode a block buffer into another one

  void Aes::decode (t_byte* bo, const t_byte* bi) {
    wrlock ();
    try {
      // copy the input buffer to the state
      t_byte cbuf[d_cbsz];
      for (long i = 0; i < AES_STATE_LEN; i++) cbuf[i] = bi[i];
      // initial round key
      aes_state_add_round_key (cbuf, p_rkey, d_rnds);
      // loop for all rounds
      for (long i = d_rnds-1; i > 0; i--) {
	aes_inv_state_shift_rows  (cbuf);
	aes_inv_state_sub_bytes   (cbuf);
	aes_state_add_round_key   (cbuf, p_rkey, i);
	aes_inv_state_mix_columns (cbuf);
      }
      aes_inv_state_shift_rows (cbuf);
      aes_inv_state_sub_bytes  (cbuf);
      aes_state_add_round_key  (cbuf, p_rkey, 0);
      // copy the state to the output buffer
      for (long i = 0; i < AES_STATE_LEN; i++) bo[i] = cbuf[i];
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object setion                                                         -
  // -------------------------------------------------------------------------

  // create a new object in a generic way
  
  Object* Aes::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key = dynamic_cast <Key*> (obj);
      if (key != nullptr) return new Aes (*key);
      throw Exception ("argument-error", "invalid arguments with aes");
    }
    // check for 2 arguments
    if (argc == 2) {
      // check for a key
      Object* obj = argv->get (0);
      Key*    key  = dynamic_cast <Key*> (obj);
      if (key == nullptr) {
	throw Exception ("argument-error", "invalid arguments with aes");
      }
      // get the reverse flag and object
      bool rflg = argv->getbool (1);
      return new Aes (*key, rflg);
    }
    throw Exception ("argument-error", "too many arguments with aes");
  }
}
