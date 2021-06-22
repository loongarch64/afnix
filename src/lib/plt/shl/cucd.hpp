// ---------------------------------------------------------------------------
// - cucd.hpp                                                                -
// - standard platform library - unicode database definitions                -
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

#ifndef  AFNIX_CUCD_HPP
#define  AFNIX_CUCD_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the definitions of the standard unicode database
  /// code points. For each code point, a structure with attributes is defined
  /// and used by the various conversion functions. The code points are
  /// structured in planes which are loaded on demand automatically by the
  /// library.
  /// @author amaury darsch

  // -------------------------------------------------------------------------
  // - ucd section                                                           -
  // -------------------------------------------------------------------------

  /// the ucd version
  const t_byte UCD_MAJOR   = 13;
  const t_byte UCD_MINOR   = 0;
  const t_byte UCD_PATCH   = 0;

  /// general category value
  const t_byte UCD_GCV_LU  = 0x00; // letter, uppercase           
  const t_byte UCD_GCV_LL  = 0x01; // letter, lowercase           
  const t_byte UCD_GCV_LT  = 0x02; // letter, titlecase           
  const t_byte UCD_GCV_LM  = 0x03; // letter, modifier            
  const t_byte UCD_GCV_LO  = 0x04; // letter, other               
  
  const t_byte UCD_GCV_MN  = 0x10; // mark, nonspacing            
  const t_byte UCD_GCV_MC  = 0x11; // mark, spacing combining     
  const t_byte UCD_GCV_ME  = 0x12; // mark, enclosing             
  
  const t_byte UCD_GCV_ND  = 0x20; // number, decimal digit       
  const t_byte UCD_GCV_NL  = 0x21; // number, letter              
  const t_byte UCD_GCV_NO  = 0x22; // number, other               
  
  const t_byte UCD_GCV_PC  = 0x30; // punctuation, connector      
  const t_byte UCD_GCV_PD  = 0x31; // punctuation, dash           
  const t_byte UCD_GCV_PS  = 0x32; // punctuation, open           
  const t_byte UCD_GCV_PE  = 0x33; // punctuation, close          
  const t_byte UCD_GCV_PI  = 0x34; // punctuation, initial quote   
  const t_byte UCD_GCV_PF  = 0x35; // punctuation, final quote    
  const t_byte UCD_GCV_PO  = 0x36; // punctuation, other          
  
  const t_byte UCD_GCV_SM  = 0x40; // symbol, math                
  const t_byte UCD_GCV_SC  = 0x41; // symbol, currency            
  const t_byte UCD_GCV_SK  = 0x42; // symbol, modifier            
  const t_byte UCD_GCV_SO  = 0x43; // symbol, other               
  
  const t_byte UCD_GCV_ZS  = 0x50; // separator, space            
  const t_byte UCD_GCV_ZL  = 0x51; // separator, line             
  const t_byte UCD_GCV_ZP  = 0x52; // separator, paragraph        
  
  const t_byte UCD_GCV_CC  = 0x60; // other, control              
  const t_byte UCD_GCV_CF  = 0x61; // other, format               
  const t_byte UCD_GCV_CS  = 0x62; // other, surrogate            
  const t_byte UCD_GCV_CO  = 0x63; // other, private use          
  const t_byte UCD_GCV_CN  = 0x64; // other, not assigned         
  
  /// bidirectional class value (BCV) 
  const t_byte UCD_BCV_L   = 0x00; // left-to-right
  const t_byte UCD_BCV_R   = 0x01; // right-to-left
  const t_byte UCD_BCV_AL  = 0x02; // right-to-left arabic
  const t_byte UCD_BCV_EN  = 0x03; // european number
  const t_byte UCD_BCV_ES  = 0x04; // european number separator
  const t_byte UCD_BCV_ET  = 0x05; // european number terminator
  const t_byte UCD_BCV_AN  = 0x06; // arabic number
  const t_byte UCD_BCV_CS  = 0x07; // common number separator
  const t_byte UCD_BCV_NSM = 0x08; // non-spacing mark
  const t_byte UCD_BCV_BN  = 0x09; // boundary neutral
  const t_byte UCD_BCV_B   = 0x0A; // paragraph separator
  const t_byte UCD_BCV_S   = 0x0B; // segment separator
  const t_byte UCD_BCV_WS  = 0x0C; // whitespace
  const t_byte UCD_BCV_ON  = 0x0D; // other neutrals
  const t_byte UCD_BCV_LRE = 0x0E; // left to right embedding
  const t_byte UCD_BCV_LRO = 0x0F; // left to right override
  const t_byte UCD_BCV_RLE = 0x10; // right to left embedding
  const t_byte UCD_BCV_RLO = 0x11; // right to left override
  const t_byte UCD_BCV_PDF = 0x12; // pop directional format
  const t_byte UCD_BCV_LRI = 0x13; // left to right isolate
  const t_byte UCD_BCV_RLI = 0x14; // right to left isolate
  const t_byte UCD_BCV_FSI = 0x15; // first strong isolate
  const t_byte UCD_BCV_PDI = 0x16; // pop directional isolate
    
  /// character decomposition mapping value 
  const t_byte UCD_DMV_NIL = 0x00; // no decompostion
  const t_byte UCD_DMV_FNT = 0x01; // font variant
  const t_byte UCD_DMV_NOB = 0x02; // no-break of space or hyphen
  const t_byte UCD_DMV_INI = 0x03; // initial presentation form (arabic)
  const t_byte UCD_DMV_MED = 0x04; // medial presentation form (arabic)
  const t_byte UCD_DMV_FIN = 0x05; // final presentation form (arabic)
  const t_byte UCD_DMV_ISO = 0x06; // isolated presentation form (arabic)
  const t_byte UCD_DMV_CIR = 0x07; // encircled form
  const t_byte UCD_DMV_SUP = 0x08; // superscript form
  const t_byte UCD_DMV_SUB = 0x09; // subscript form
  const t_byte UCD_DMV_VER = 0x0A; // vertical layout presentation form
  const t_byte UCD_DMV_WID = 0x0B; // wide compatibility character
  const t_byte UCD_DMV_NRW = 0x0C; // narrow compatibility character
  const t_byte UCD_DMV_SML = 0x0D; // small variant form
  const t_byte UCD_DMV_SQR = 0x0E; // CJK squared font variant
  const t_byte UCD_DMV_FRA = 0x0F; // vulgar fraction form
  const t_byte UCD_DMV_CPT = 0x10; // unspecified compatible character
  
  /// the canonical decomposition field
  const long UCD_CDV_MAX = 18;
  /// the upper case mapping max fields
  const long UCD_UCM_MAX = 3;
  /// the lower case mapping max fields
  const long UCD_LCM_MAX = 3;
  /// the title case mapping max fields
  const long UCD_TCM_MAX = 3;

  /// the ucd structure 
  struct s_cucd {
    // code point value
    t_quad      d_code;
    // code point name
    const char* p_name;
    // general category value
    t_byte      d_pgcv;
    // canonical combining class
    long        d_pccc;
    // bidirectional class value
    t_byte      d_pbcv;
    // composition exclusion flag
    t_byte      d_cexc;
    // decomposition mapping value 
    t_byte      d_pdmv;
    // canonical decomposition    
    t_quad      d_cdmv[UCD_CDV_MAX];
    // upper case mapping
    t_quad      d_umap[UCD_UCM_MAX];
    // lower case mapping         
    t_quad      d_lmap[UCD_LCM_MAX];      
    // title case mapping
    t_quad      d_tmap[UCD_TCM_MAX];
  };

  /// @return a plane size by plane index
  const long c_ucdpsize (const long index);

  /// @return a plane array by plane index
  const s_cucd* c_ucdplane (const long index);

  /// @return a ucd structure by code point
  const s_cucd* c_getcucd (const t_quad code);

  /// perform a normal form decomposition (canonical)
  /// @param dst  the destination buffer
  /// @param code the code to decompose
  bool c_ucdnfd (t_quad dst[UCD_CDV_MAX], const t_quad code);

  /// perform a normal form decomposition (canonical)
  /// @param dst the destination buffer
  /// @param src the source buffer to decompose
  bool c_ucdnfd (t_quad dst[UCD_CDV_MAX], const t_quad src[UCD_CDV_MAX]);

  /// @return a nil allocated string
  t_quad* c_ucdnil (void);

  /// normalize character buffer into a canonical form
  /// @param s    the string to convert
  /// @param size the string size
  t_quad* c_ucdnrm (const char* s, const long size);

  /// normalize a string into a canonical form
  /// @param s    the string to convert
  /// @param size the string size
  t_quad* c_ucdnrm (const t_quad* s, const long size);

  /// put an array in a canonical order form
  /// @param buf  the character buffer to sort
  /// @param size the buffer size
  void c_ucdcof (t_quad* buf, const long size);

  /// convert a unicode character to lower case
  /// @param dst  the destination buffer
  /// @param code the code to decompose
  /// @return the destination size
  long c_ucdtol (t_quad dst[UCD_LCM_MAX], const t_quad code);

  /// convert a unicode character to upper case
  /// @param dst  the destination buffer
  /// @param code the code to decompose
  /// @return the destination size
  long c_ucdtou (t_quad dst[UCD_UCM_MAX], const t_quad code);

  /// @return true if the code point is not combining
  bool c_ucdncc (const t_quad code);
};

#endif
