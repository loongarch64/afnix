// ---------------------------------------------------------------------------
// - XmlBuffer.cpp                                                           -
// - afnix:xml module - xml character buffer class implementation            -
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

#include "Vector.hpp"
#include "Utility.hpp"
#include "Unicode.hpp"
#include "XmlBuffer.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default preserve space flag
  static const bool XML_BUFFER_PRSV = false;

  // the xml special characters
  static const t_quad XML_CHAR_NS = 0x00000023UL; // #
  static const t_quad XML_CHAR_DQ = 0x00000022UL; // "
  static const t_quad XML_CHAR_SQ = 0x00000027UL; // '
  static const t_quad XML_CHAR_PC = 0x00000025UL; // %
  static const t_quad XML_CHAR_SC = 0x0000003BUL; // ;
  static const t_quad XML_CHAR_OP = 0x00000028UL; // (
  static const t_quad XML_CHAR_CP = 0x00000029UL; // )
  static const t_quad XML_CHAR_VL = 0x0000007CUL; // |
  static const t_quad XML_CHAR_LB = 0x0000005BUL; // [
  static const t_quad XML_CHAR_RB = 0x0000005DUL; // ]
  
  // return true if the character is a valid XML 1.0 character
  static inline bool is_char_1_0 (const t_quad c) {
    if (c == 0x00000009) return true;
    if (c == 0x0000000A) return true;
    if (c == 0x0000000D) return true;
    if ((0x00000020<= c) && (c <= 0x0000D7FF)) return true;
    if ((0x0000E000<= c) && (c <= 0x0000FFFD)) return true;
    if ((0x00010000<= c) && (c <= 0x0010FFFF)) return true;
    return false;
  }

  // return true if the character is a valid XML 1.1 character
  static inline bool is_char_1_1 (const t_quad c) {
    if ((0x00000001<= c) && (c <= 0x0000D7FF)) return true;
    if ((0x0000E000<= c) && (c <= 0x0000FFFD)) return true;
    if ((0x00010000<= c) && (c <= 0x0010FFFF)) return true;
    return false;
  }

  // return true if the character is valid
  static inline bool is_char (const XmlSystem::t_xmlv xmlv, const t_quad c) {
    switch (xmlv) {
    case XmlSystem::XML_1_0:
      return is_char_1_0 (c);
    case XmlSystem::XML_1_1:
      return is_char_1_1 (c);
    }
    throw Exception ("internal-error", "illegal xml is-char access");
  }
  
  // return true if a buffer is valid
  static bool is_valid (const XmlSystem::t_xmlv xmlv, const t_quad* s) {
    t_quad c = nilq;
    while ((c = *s++) != nilq) {
      if (is_char (xmlv, c) == false) return false;
    }
    return true;
  }

  // return true if the character a valid XML 1.0 space
  static inline bool is_spcc_1_0 (const t_quad c) {
    if (c == 0x00000020) return true;
    if (c == 0x00000009) return true;
    if (c == 0x0000000D) return true;
    if (c == 0x0000000A) return true;
    return false;
  }

  // return true if the character a valid XML 1.1 space
  static inline bool is_spcc_1_1 (const t_quad c) {
    if (c == 0x00000020) return true;
    if (c == 0x00000009) return true;
    if (c == 0x0000000D) return true;
    if (c == 0x0000000A) return true;
    return false;
  }
	
  // return true if the character is a valid space
  static inline bool is_spcc (const XmlSystem::t_xmlv xmlv, const t_quad c) {
    switch (xmlv) {
    case XmlSystem::XML_1_0:
      return is_spcc_1_0 (c);
    case XmlSystem::XML_1_1:
      return is_spcc_1_1 (c);
    }
    throw Exception ("internal-error", "illegal xml is-spcc access");
  }
      
  // return true if the character is a valid XML 1.0 base character
  static inline bool is_base_1_0 (const t_quad c) {
    if ((0x00000041 <= c) && (c <= 0x0000005A)) return true; 
    if ((0x00000061 <= c) && (c <= 0x0000007A)) return true; 
    if ((0x000000C0 <= c) && (c <= 0x000000D6)) return true; 
    if ((0x000000D8 <= c) && (c <= 0x000000F6)) return true; 
    if ((0x000000F8 <= c) && (c <= 0x000000FF)) return true; 
    if ((0x00000100 <= c) && (c <= 0x00000131)) return true; 
    if ((0x00000134 <= c) && (c <= 0x0000013E)) return true; 
    if ((0x00000141 <= c) && (c <= 0x00000148)) return true; 
    if ((0x0000014A <= c) && (c <= 0x0000017E)) return true; 
    if ((0x00000180 <= c) && (c <= 0x000001C3)) return true; 
    if ((0x000001CD <= c) && (c <= 0x000001F0)) return true; 
    if ((0x000001F4 <= c) && (c <= 0x000001F5)) return true; 
    if ((0x000001FA <= c) && (c <= 0x00000217)) return true; 
    if ((0x00000250 <= c) && (c <= 0x000002A8)) return true; 
    if ((0x000002BB <= c) && (c <= 0x000002C1)) return true; 
    if (c == 0x00000386) return true; 
    if ((0x00000388 <= c) && (c <= 0x0000038A)) return true; 
    if (c == 0x0000038C) return true; 
    if ((0x0000038E <= c) && (c <= 0x000003A1)) return true; 
    if ((0x000003A3 <= c) && (c <= 0x000003CE)) return true; 
    if ((0x000003D0 <= c) && (c <= 0x000003D6)) return true; 
    if (c == 0x000003DA) return true; 
    if (c == 0x000003DC) return true; 
    if (c == 0x000003DE) return true; 
    if (c == 0x000003E0) return true; 
    if ((0x000003E2 <= c) && (c <= 0x000003F3)) return true; 
    if ((0x00000401 <= c) && (c <= 0x0000040C)) return true; 
    if ((0x0000040E <= c) && (c <= 0x0000044F)) return true; 
    if ((0x00000451 <= c) && (c <= 0x0000045C)) return true; 
    if ((0x0000045E <= c) && (c <= 0x00000481)) return true; 
    if ((0x00000490 <= c) && (c <= 0x000004C4)) return true; 
    if ((0x000004C7 <= c) && (c <= 0x000004C8)) return true; 
    if ((0x000004CB <= c) && (c <= 0x000004CC)) return true; 
    if ((0x000004D0 <= c) && (c <= 0x000004EB)) return true; 
    if ((0x000004EE <= c) && (c <= 0x000004F5)) return true; 
    if ((0x000004F8 <= c) && (c <= 0x000004F9)) return true; 
    if ((0x00000531 <= c) && (c <= 0x00000556)) return true; 
    if (c == 0x00000559) return true; 
    if ((0x00000561 <= c) && (c <= 0x00000586)) return true; 
    if ((0x000005D0 <= c) && (c <= 0x000005EA)) return true; 
    if ((0x000005F0 <= c) && (c <= 0x000005F2)) return true; 
    if ((0x00000621 <= c) && (c <= 0x0000063A)) return true; 
    if ((0x00000641 <= c) && (c <= 0x0000064A)) return true; 
    if ((0x00000671 <= c) && (c <= 0x000006B7)) return true; 
    if ((0x000006BA <= c) && (c <= 0x000006BE)) return true; 
    if ((0x000006C0 <= c) && (c <= 0x000006CE)) return true; 
    if ((0x000006D0 <= c) && (c <= 0x000006D3)) return true; 
    if (c == 0x000006D5) return true; 
    if ((0x000006E5 <= c) && (c <= 0x000006E6)) return true; 
    if ((0x00000905 <= c) && (c <= 0x00000939)) return true; 
    if (c == 0x0000093D) return true; 
    if ((0x00000958 <= c) && (c <= 0x00000961)) return true; 
    if ((0x00000985 <= c) && (c <= 0x0000098C)) return true; 
    if ((0x0000098F <= c) && (c <= 0x00000990)) return true; 
    if ((0x00000993 <= c) && (c <= 0x000009A8)) return true; 
    if ((0x000009AA <= c) && (c <= 0x000009B0)) return true; 
    if (c == 0x000009B2) return true; 
    if ((0x000009B6 <= c) && (c <= 0x000009B9)) return true; 
    if ((0x000009DC <= c) && (c <= 0x000009DD)) return true; 
    if ((0x000009DF <= c) && (c <= 0x000009E1)) return true; 
    if ((0x000009F0 <= c) && (c <= 0x000009F1)) return true; 
    if ((0x00000A05 <= c) && (c <= 0x00000A0A)) return true; 
    if ((0x00000A0F <= c) && (c <= 0x00000A10)) return true; 
    if ((0x00000A13 <= c) && (c <= 0x00000A28)) return true; 
    if ((0x00000A2A <= c) && (c <= 0x00000A30)) return true; 
    if ((0x00000A32 <= c) && (c <= 0x00000A33)) return true; 
    if ((0x00000A35 <= c) && (c <= 0x00000A36)) return true; 
    if ((0x00000A38 <= c) && (c <= 0x00000A39)) return true; 
    if ((0x00000A59 <= c) && (c <= 0x00000A5C)) return true; 
    if (c == 0x00000A5E) return true; 
    if ((0x00000A72 <= c) && (c <= 0x00000A74)) return true; 
    if ((0x00000A85 <= c) && (c <= 0x00000A8B)) return true; 
    if (c == 0x00000A8D) return true; 
    if ((0x00000A8F <= c) && (c <= 0x00000A91)) return true; 
    if ((0x00000A93 <= c) && (c <= 0x00000AA8)) return true; 
    if ((0x00000AAA <= c) && (c <= 0x00000AB0)) return true; 
    if ((0x00000AB2 <= c) && (c <= 0x00000AB3)) return true; 
    if ((0x00000AB5 <= c) && (c <= 0x00000AB9)) return true; 
    if (c == 0x00000ABD) return true; 
    if (c == 0x00000AE0) return true; 
    if ((0x00000B05 <= c) && (c <= 0x00000B0C)) return true; 
    if ((0x00000B0F <= c) && (c <= 0x00000B10)) return true; 
    if ((0x00000B13 <= c) && (c <= 0x00000B28)) return true; 
    if ((0x00000B2A <= c) && (c <= 0x00000B30)) return true; 
    if ((0x00000B32 <= c) && (c <= 0x00000B33)) return true; 
    if ((0x00000B36 <= c) && (c <= 0x00000B39)) return true; 
    if (c == 0x00000B3D) return true; 
    if ((0x00000B5C <= c) && (c <= 0x00000B5D)) return true; 
    if ((0x00000B5F <= c) && (c <= 0x00000B61)) return true; 
    if ((0x00000B85 <= c) && (c <= 0x00000B8A)) return true; 
    if ((0x00000B8E <= c) && (c <= 0x00000B90)) return true; 
    if ((0x00000B92 <= c) && (c <= 0x00000B95)) return true; 
    if ((0x00000B99 <= c) && (c <= 0x00000B9A)) return true; 
    if (c == 0x00000B9C) return true; 
    if ((0x00000B9E <= c) && (c <= 0x00000B9F)) return true; 
    if ((0x00000BA3 <= c) && (c <= 0x00000BA4)) return true; 
    if ((0x00000BA8 <= c) && (c <= 0x00000BAA)) return true; 
    if ((0x00000BAE <= c) && (c <= 0x00000BB5)) return true; 
    if ((0x00000BB7 <= c) && (c <= 0x00000BB9)) return true; 
    if ((0x00000C05 <= c) && (c <= 0x00000C0C)) return true; 
    if ((0x00000C0E <= c) && (c <= 0x00000C10)) return true; 
    if ((0x00000C12 <= c) && (c <= 0x00000C28)) return true; 
    if ((0x00000C2A <= c) && (c <= 0x00000C33)) return true; 
    if ((0x00000C35 <= c) && (c <= 0x00000C39)) return true; 
    if ((0x00000C60 <= c) && (c <= 0x00000C61)) return true; 
    if ((0x00000C85 <= c) && (c <= 0x00000C8C)) return true; 
    if ((0x00000C8E <= c) && (c <= 0x00000C90)) return true; 
    if ((0x00000C92 <= c) && (c <= 0x00000CA8)) return true; 
    if ((0x00000CAA <= c) && (c <= 0x00000CB3)) return true; 
    if ((0x00000CB5 <= c) && (c <= 0x00000CB9)) return true; 
    if (c == 0x00000CDE) return true; 
    if ((0x00000CE0 <= c) && (c <= 0x00000CE1)) return true; 
    if ((0x00000D05 <= c) && (c <= 0x00000D0C)) return true; 
    if ((0x00000D0E <= c) && (c <= 0x00000D10)) return true; 
    if ((0x00000D12 <= c) && (c <= 0x00000D28)) return true; 
    if ((0x00000D2A <= c) && (c <= 0x00000D39)) return true; 
    if ((0x00000D60 <= c) && (c <= 0x00000D61)) return true; 
    if ((0x00000E01 <= c) && (c <= 0x00000E2E)) return true; 
    if (c == 0x00000E30) return true; 
    if ((0x00000E32 <= c) && (c <= 0x00000E33)) return true; 
    if ((0x00000E40 <= c) && (c <= 0x00000E45)) return true; 
    if ((0x00000E81 <= c) && (c <= 0x00000E82)) return true; 
    if (c == 0x00000E84) return true; 
    if ((0x00000E87 <= c) && (c <= 0x00000E88)) return true; 
    if (c == 0x00000E8A) return true; 
    if (c == 0x00000E8D) return true; 
    if ((0x00000E94 <= c) && (c <= 0x00000E97)) return true; 
    if ((0x00000E99 <= c) && (c <= 0x00000E9F)) return true; 
    if ((0x00000EA1 <= c) && (c <= 0x00000EA3)) return true; 
    if (c == 0x00000EA5) return true; 
    if (c == 0x00000EA7) return true; 
    if ((0x00000EAA <= c) && (c <= 0x00000EAB)) return true; 
    if ((0x00000EAD <= c) && (c <= 0x00000EAE)) return true; 
    if (c == 0x00000EB0) return true; 
    if ((0x00000EB2 <= c) && (c <= 0x00000EB3)) return true; 
    if (c == 0x00000EBD) return true; 
    if ((0x00000EC0 <= c) && (c <= 0x00000EC4)) return true; 
    if ((0x00000F40 <= c) && (c <= 0x00000F47)) return true; 
    if ((0x00000F49 <= c) && (c <= 0x00000F69)) return true; 
    if ((0x000010A0 <= c) && (c <= 0x000010C5)) return true; 
    if ((0x000010D0 <= c) && (c <= 0x000010F6)) return true; 
    if (c == 0x00001100) return true; 
    if ((0x00001102 <= c) && (c <= 0x00001103)) return true; 
    if ((0x00001105 <= c) && (c <= 0x00001107)) return true; 
    if (c == 0x00001109) return true; 
    if ((0x0000110B <= c) && (c <= 0x0000110C)) return true; 
    if ((0x0000110E <= c) && (c <= 0x00001112)) return true; 
    if (c == 0x0000113C) return true; 
    if (c == 0x0000113E) return true; 
    if (c == 0x00001140) return true; 
    if (c == 0x0000114C) return true; 
    if (c == 0x0000114E) return true; 
    if (c == 0x00001150) return true; 
    if ((0x00001154 <= c) && (c <= 0x00001155)) return true; 
    if (c == 0x00001159) return true; 
    if ((0x0000115F <= c) && (c <= 0x00001161)) return true; 
    if (c == 0x00001163) return true; 
    if (c == 0x00001165) return true; 
    if (c == 0x00001167) return true; 
    if (c == 0x00001169) return true; 
    if ((0x0000116D <= c) && (c <= 0x0000116E)) return true; 
    if ((0x00001172 <= c) && (c <= 0x00001173)) return true; 
    if (c == 0x00001175) return true; 
    if (c == 0x0000119E) return true; 
    if (c == 0x000011A8) return true; 
    if (c == 0x000011AB) return true; 
    if ((0x000011AE <= c) && (c <= 0x000011AF)) return true; 
    if ((0x000011B7 <= c) && (c <= 0x000011B8)) return true; 
    if (c == 0x000011BA) return true; 
    if ((0x000011BC <= c) && (c <= 0x000011C2)) return true; 
    if (c == 0x000011EB) return true; 
    if (c == 0x000011F0) return true; 
    if (c == 0x000011F9) return true; 
    if ((0x00001E00 <= c) && (c <= 0x00001E9B)) return true; 
    if ((0x00001EA0 <= c) && (c <= 0x00001EF9)) return true; 
    if ((0x00001F00 <= c) && (c <= 0x00001F15)) return true; 
    if ((0x00001F18 <= c) && (c <= 0x00001F1D)) return true; 
    if ((0x00001F20 <= c) && (c <= 0x00001F45)) return true; 
    if ((0x00001F48 <= c) && (c <= 0x00001F4D)) return true; 
    if ((0x00001F50 <= c) && (c <= 0x00001F57)) return true; 
    if (c == 0x00001F59) return true; 
    if (c == 0x00001F5B) return true; 
    if (c == 0x00001F5D) return true; 
    if ((0x00001F5F <= c) && (c <= 0x00001F7D)) return true; 
    if ((0x00001F80 <= c) && (c <= 0x00001FB4)) return true; 
    if ((0x00001FB6 <= c) && (c <= 0x00001FBC)) return true; 
    if (c == 0x00001FBE) return true; 
    if ((0x00001FC2 <= c) && (c <= 0x00001FC4)) return true; 
    if ((0x00001FC6 <= c) && (c <= 0x00001FCC)) return true; 
    if ((0x00001FD0 <= c) && (c <= 0x00001FD3)) return true; 
    if ((0x00001FD6 <= c) && (c <= 0x00001FDB)) return true; 
    if ((0x00001FE0 <= c) && (c <= 0x00001FEC)) return true; 
    if ((0x00001FF2 <= c) && (c <= 0x00001FF4)) return true; 
    if ((0x00001FF6 <= c) && (c <= 0x00001FFC)) return true; 
    if (c == 0x00002126) return true; 
    if ((0x0000212A <= c) && (c <= 0x0000212B)) return true; 
    if (c == 0x0000212E) return true; 
    if ((0x00002180 <= c) && (c <= 0x00002182)) return true; 
    if ((0x00003041 <= c) && (c <= 0x00003094)) return true; 
    if ((0x000030A1 <= c) && (c <= 0x000030FA)) return true; 
    if ((0x00003105 <= c) && (c <= 0x0000312C)) return true; 
    if ((0x0000AC00 <= c) && (c <= 0x0000D7A3)) return true;
    return false;
  }

  // return true if the character is a valid XML 1.0 ideographic character
  static inline bool is_ideo_1_0 (const t_quad c) {
    if ((0x00004E00 <= c) && (c <= 0x00009FA5)) return true;
    if (c == 0x000030007) return true;
    if ((0x00003021 <= c) && (c <= 0x00003029)) return true;
    return false;
  }

  // return true if the character is a valid XML 1.0 combining character
  static inline bool is_comb_1_0 (const t_quad c) {
    if ((0x00000300 <= c) && (c <= 0x00000345)) return true; 
    if ((0x00000360 <= c) && (c <= 0x00000361)) return true; 
    if ((0x00000483 <= c) && (c <= 0x00000486)) return true; 
    if ((0x00000591 <= c) && (c <= 0x000005A1)) return true; 
    if ((0x000005A3 <= c) && (c <= 0x000005B9)) return true; 
    if ((0x000005BB <= c) && (c <= 0x000005BD)) return true; 
    if (c == 0x000005BF) return true; 
    if ((0x000005C1 <= c) && (c <= 0x000005C2)) return true; 
    if (c == 0x000005C4) return true; 
    if ((0x0000064B <= c) && (c <= 0x00000652)) return true; 
    if (c == 0x00000670) return true; 
    if ((0x000006D6 <= c) && (c <= 0x000006DC)) return true; 
    if ((0x000006DD <= c) && (c <= 0x000006DF)) return true; 
    if ((0x000006E0 <= c) && (c <= 0x000006E4)) return true; 
    if ((0x000006E7 <= c) && (c <= 0x000006E8)) return true; 
    if ((0x000006EA <= c) && (c <= 0x000006ED)) return true; 
    if ((0x00000901 <= c) && (c <= 0x00000903)) return true; 
    if (c == 0x0000093C) return true; 
    if ((0x0000093E <= c) && (c <= 0x0000094C)) return true; 
    if (c == 0x0000094D) return true; 
    if ((0x00000951 <= c) && (c <= 0x00000954)) return true; 
    if ((0x00000962 <= c) && (c <= 0x00000963)) return true; 
    if ((0x00000981 <= c) && (c <= 0x00000983)) return true; 
    if (c == 0x000009BC) return true; 
    if (c == 0x000009BE) return true; 
    if (c == 0x000009BF) return true; 
    if ((0x000009C0 <= c) && (c <= 0x000009C4)) return true; 
    if ((0x000009C7 <= c) && (c <= 0x000009C8)) return true; 
    if ((0x000009CB <= c) && (c <= 0x000009CD)) return true; 
    if (c == 0x000009D7) return true; 
    if ((0x000009E2 <= c) && (c <= 0x000009E3)) return true; 
    if (c == 0x00000A02) return true; 
    if (c == 0x00000A3C) return true; 
    if (c == 0x00000A3E) return true; 
    if (c == 0x00000A3F) return true; 
    if ((0x00000A40 <= c) && (c <= 0x00000A42)) return true; 
    if ((0x00000A47 <= c) && (c <= 0x00000A48)) return true; 
    if ((0x00000A4B <= c) && (c <= 0x00000A4D)) return true; 
    if ((0x00000A70 <= c) && (c <= 0x00000A71)) return true; 
    if ((0x00000A81 <= c) && (c <= 0x00000A83)) return true; 
    if (c == 0x00000ABC) return true; 
    if ((0x00000ABE <= c) && (c <= 0x00000AC5)) return true; 
    if ((0x00000AC7 <= c) && (c <= 0x00000AC9)) return true; 
    if ((0x00000ACB <= c) && (c <= 0x00000ACD)) return true; 
    if ((0x00000B01 <= c) && (c <= 0x00000B03)) return true; 
    if (c == 0x00000B3C) return true; 
    if ((0x00000B3E <= c) && (c <= 0x00000B43)) return true; 
    if ((0x00000B47 <= c) && (c <= 0x00000B48)) return true; 
    if ((0x00000B4B <= c) && (c <= 0x00000B4D)) return true; 
    if ((0x00000B56 <= c) && (c <= 0x00000B57)) return true; 
    if ((0x00000B82 <= c) && (c <= 0x00000B83)) return true; 
    if ((0x00000BBE <= c) && (c <= 0x00000BC2)) return true; 
    if ((0x00000BC6 <= c) && (c <= 0x00000BC8)) return true; 
    if ((0x00000BCA <= c) && (c <= 0x00000BCD)) return true; 
    if (c == 0x00000BD7) return true; 
    if ((0x00000C01 <= c) && (c <= 0x00000C03)) return true; 
    if ((0x00000C3E <= c) && (c <= 0x00000C44)) return true; 
    if ((0x00000C46 <= c) && (c <= 0x00000C48)) return true; 
    if ((0x00000C4A <= c) && (c <= 0x00000C4D)) return true; 
    if ((0x00000C55 <= c) && (c <= 0x00000C56)) return true; 
    if ((0x00000C82 <= c) && (c <= 0x00000C83)) return true; 
    if ((0x00000CBE <= c) && (c <= 0x00000CC4)) return true; 
    if ((0x00000CC6 <= c) && (c <= 0x00000CC8)) return true; 
    if ((0x00000CCA <= c) && (c <= 0x00000CCD)) return true; 
    if ((0x00000CD5 <= c) && (c <= 0x00000CD6)) return true; 
    if ((0x00000D02 <= c) && (c <= 0x00000D03)) return true; 
    if ((0x00000D3E <= c) && (c <= 0x00000D43)) return true; 
    if ((0x00000D46 <= c) && (c <= 0x00000D48)) return true; 
    if ((0x00000D4A <= c) && (c <= 0x00000D4D)) return true; 
    if (c == 0x00000D57) return true; 
    if (c == 0x00000E31) return true; 
    if ((0x00000E34 <= c) && (c <= 0x00000E3A)) return true; 
    if ((0x00000E47 <= c) && (c <= 0x00000E4E)) return true; 
    if (c == 0x00000EB1) return true; 
    if ((0x00000EB4 <= c) && (c <= 0x00000EB9)) return true; 
    if ((0x00000EBB <= c) && (c <= 0x00000EBC)) return true; 
    if ((0x00000EC8 <= c) && (c <= 0x00000ECD)) return true; 
    if ((0x00000F18 <= c) && (c <= 0x00000F19)) return true; 
    if (c == 0x00000F35) return true; 
    if (c == 0x00000F37) return true; 
    if (c == 0x00000F39) return true; 
    if (c == 0x00000F3E) return true; 
    if (c == 0x00000F3F) return true; 
    if ((0x00000F71 <= c) && (c <= 0x00000F84)) return true; 
    if ((0x00000F86 <= c) && (c <= 0x00000F8B)) return true; 
    if ((0x00000F90 <= c) && (c <= 0x00000F95)) return true; 
    if (c == 0x00000F97) return true; 
    if ((0x00000F99 <= c) && (c <= 0x00000FAD)) return true; 
    if ((0x00000FB1 <= c) && (c <= 0x00000FB7)) return true; 
    if (c == 0x00000FB9) return true; 
    if ((0x000020D0 <= c) && (c <= 0x000020DC)) return true; 
    if (c == 0x000020E1) return true; 
    if ((0x0000302A <= c) && (c <= 0x0000302F)) return true; 
    if (c == 0x00003099) return true; 
    if (c == 0x0000309A) return true;
    return false;
  }

  // return true if the character is a valid XML 1.0 digit character
  static inline bool is_digi_1_0 (const t_quad c) {
    if ((0x00000030 <= c) && (c <= 0x00000039)) return true; 
    if ((0x00000660 <= c) && (c <= 0x00000669)) return true; 
    if ((0x000006F0 <= c) && (c <= 0x000006F9)) return true; 
    if ((0x00000966 <= c) && (c <= 0x0000096F)) return true; 
    if ((0x000009E6 <= c) && (c <= 0x000009EF)) return true; 
    if ((0x00000A66 <= c) && (c <= 0x00000A6F)) return true; 
    if ((0x00000AE6 <= c) && (c <= 0x00000AEF)) return true; 
    if ((0x00000B66 <= c) && (c <= 0x00000B6F)) return true; 
    if ((0x00000BE7 <= c) && (c <= 0x00000BEF)) return true; 
    if ((0x00000C66 <= c) && (c <= 0x00000C6F)) return true; 
    if ((0x00000CE6 <= c) && (c <= 0x00000CEF)) return true; 
    if ((0x00000D66 <= c) && (c <= 0x00000D6F)) return true; 
    if ((0x00000E50 <= c) && (c <= 0x00000E59)) return true; 
    if ((0x00000ED0 <= c) && (c <= 0x00000ED9)) return true; 
    if ((0x00000F20 <= c) && (c <= 0x00000F29)) return true;
    return false;
  }

  // return true if the character is a valid XML 1.0 extender character
  static inline bool is_extd_1_0 (const t_quad c) {
    if (c == 0x000000B7) return true; 
    if (c == 0x000002D0) return true; 
    if (c == 0x000002D1) return true; 
    if (c == 0x00000387) return true; 
    if (c == 0x00000640) return true; 
    if (c == 0x00000E46) return true; 
    if (c == 0x00000EC6) return true; 
    if (c == 0x00003005) return true; 
    if ((0x00003031 <= c) && (c <= 0x00003035)) return true;
    if ((0x0000309D <= c) && (c <= 0x0000309E)) return true;
    if ((0x000030FC <= c) && (c <= 0x000030FE)) return true;
    return false;
  }

  // return true if the character is a valid XML 1.0 letter
  static inline bool is_letr_1_0 (const t_quad c) {
    if (is_base_1_0 (c) == true) return true;
    return is_ideo_1_0 (c);
  }

  // return true if the character is a valid XML 1.0 starting character
  static inline bool is_nams_1_0 (const t_quad c) {
    // letter
    if (is_letr_1_0 (c) == true) return true;
    // special characters _ :
    if (c == 0x0000005F) return true;
    if (c == 0x0000003A) return true;
    return false;
  }

  // return true if the character is a valid XML 1.0 name character
  static inline bool is_namc_1_0 (const t_quad c) {
    // letter
    if (is_letr_1_0 (c) == true) return true;
    // digit
    if (is_digi_1_0 (c) == true) return true;
    // special characters . - _ :
    if (c == 0x0000002E) return true; 
    if (c == 0x0000002D) return true;
    if (c == 0x0000005F) return true;
    if (c == 0x0000003A) return true;
    // combining
    if (is_comb_1_0 (c) == true) return true;
    // extenders
    if (is_extd_1_0 (c) == true) return true;
    return false;
  }

  // return true if a buffer is valid XML 1.0 name
  static bool is_name_1_0 (const t_quad* s) {
    // check for nil first
    if (s == nullptr) return false;
    // check for starting character
    t_quad c = *s++;
    if (is_nams_1_0 (c) == false) return false;
    // check the rest of the buffer
    while ((c = *s++) != nilq) {
      if (is_namc_1_0 (c) == false) return false;
    }
    return true;
  }

  // return true if the character is a valid XML 1.1 starting character
  static inline bool is_nams_1_1 (const t_quad c) {
    if (c == 0x0000003A) return true;
    if (c == 0x0000005F) return true;
    if ((0x00000041 <= c) && (c <= 0x0000005A)) return true;
    if ((0x00000061 <= c) && (c <= 0x0000007A)) return true;
    if ((0x000000C0 <= c) && (c <= 0x000000D6)) return true; 
    if ((0x000000D8 <= c) && (c <= 0x000000F6)) return true; 
    if ((0x000000F8 <= c) && (c <= 0x000002FF)) return true; 
    if ((0x00000370 <= c) && (c <= 0x0000037D)) return true; 
    if ((0x0000037F <= c) && (c <= 0x00001FFF)) return true; 
    if ((0x0000200C <= c) && (c <= 0x0000200D)) return true; 
    if ((0x00002070 <= c) && (c <= 0x0000218F)) return true; 
    if ((0x00002C00 <= c) && (c <= 0x00002FEF)) return true; 
    if ((0x00003001 <= c) && (c <= 0x0000D7FF)) return true; 
    if ((0x0000F900 <= c) && (c <= 0x0000FDCF)) return true; 
    if ((0x0000FDF0 <= c) && (c <= 0x0000FFFD)) return true; 
    if ((0x00010000 <= c) && (c <= 0x000EFFFF)) return true;
    return false;
  }

  // return true if the character is a valid XML 1.1 name character
  static inline bool is_namc_1_1 (const t_quad c) {
    if (is_nams_1_1 (c) == true) return true;
    if (c == 0x0000002D) return true;
    if (c == 0x0000002E) return true;
    if ((0x00000030 <= c) && (c <= 0x00000039)) return true;
    if (c == 0x000000B7) return true;
    if ((0x00000300 <= c) && (c <= 0x0000036F)) return true;
    if ((0x0000203F <= c) && (c <= 0x00002040)) return true;
    return false;
  }

  // return true if the character is a valid name character
  static inline bool is_namc (const XmlSystem::t_xmlv xmlv, const t_quad c) {
    switch (xmlv) {
    case XmlSystem::XML_1_0:
      return is_namc_1_0 (c);
    case XmlSystem::XML_1_1:
      return is_namc_1_1 (c);
    }
    throw Exception ("internal-error", "illegal xml is-namc access");
  }

  // return true if a buffer is valid XML 1.1 name
  static bool is_name_1_1 (const t_quad* s) {
    // check for nil first
    if (s == nullptr) return false;
    // check for starting character
    t_quad c = *s++;
    if (is_nams_1_1 (c) == false) return false;
    // check the rest of the buffer
    while ((c = *s++) != nilq) {
      if (is_namc_1_1 (c) == false) return false;
    }
    return true;
  }

  // return true if the character is a valid name string
  static inline bool is_name (const XmlSystem::t_xmlv xmlv, const t_quad* s) {
    switch (xmlv) {
    case XmlSystem::XML_1_0:
      return is_name_1_0 (s);
    case XmlSystem::XML_1_1:
      return is_name_1_1 (s);
    }
    throw Exception ("internal-error", "illegal xml is-name access");
  }

  // return true if the characters are a pe reference
  static inline bool is_pref (const XmlSystem::t_xmlv xmlv, const t_quad* s) {
    // get the string length and check
    long slen = Unicode::strlen (s);
    if (slen == 0) return false;
    // check first character for %
    if (s[0] != XML_CHAR_PC) return false;
    // check last character for ;
    long smax = slen - 1;
    if (s[smax] != XML_CHAR_SC) return false;
    // check the content for valid charcaters
    switch (xmlv) {
    case XmlSystem::XML_1_0:
      for (long i = 1; i < smax; i++) 
	if (is_namc_1_0 (s[i]) == false) return false;
    case XmlSystem::XML_1_1:
      for (long i = 1; i < smax; i++) 
	if (is_namc_1_1 (s[i]) == false) return false;
    }
    return true;
  }
  
  // return true if the characters are a name or a pe reference

  static inline bool is_pnam (const XmlSystem::t_xmlv xmlv, const t_quad* s) {
    // check for a pe ref
    if (is_pref (xmlv, s) == true) return true;
    // check for a name
    return is_name (xmlv, s);
  }

  // return true if the character is a valid enumeration character
  
  static inline bool is_enmc (const XmlSystem::t_xmlv xmlv, const t_quad c) {
    if (is_namc (xmlv, c) == true) return true;
    if (is_spcc (xmlv, c) == true) return true;
    if (c == XML_CHAR_VL) return true;
    return false;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new buffer class with a default size of 1024 characters

  XmlBuffer::XmlBuffer (void) {
    d_prsv = XML_BUFFER_PRSV;
    d_xmlv = XmlSystem::getxmlv ();
  }

  // create a new buffer with a xml version

  XmlBuffer::XmlBuffer (const XmlSystem::t_xmlv xmlv) {
    d_prsv = XML_BUFFER_PRSV;
    d_xmlv = xmlv;
  }

  // create a new buffer with a xml version and a string

  XmlBuffer::XmlBuffer (const XmlSystem::t_xmlv xmlv, 
			const String& xval) : XsoBuffer (xval) {
    d_prsv = XML_BUFFER_PRSV;
    d_xmlv = xmlv;
  }

  // copy construct this buffer

  XmlBuffer::XmlBuffer (const XmlBuffer& that) : XsoBuffer (that) {
    d_prsv = that.d_prsv;
    d_xmlv = that.d_xmlv;
  }

  // assign a buffer to this one

  XmlBuffer& XmlBuffer::operator = (const XmlBuffer& that){
    // check for asel-assignation
    if (this == &that) return *this;
    // copy the base class
    XsoBuffer::operator = (that);
    d_prsv = that.d_prsv;
    d_xmlv = that.d_xmlv;
    // here it is
    return *this;
  }

  // reset this buffer
  
  void XmlBuffer::reset (void) {
    XsoBuffer::reset ();
    d_prsv = XML_BUFFER_PRSV;
  }

  // get the xml buffer version id

  String XmlBuffer::getxvid (void) const {
    return XmlSystem::toxvid(d_xmlv);
  }

  // create a new buffer with the same properties

  XmlBuffer XmlBuffer::dup (void) const {
    // create a new bufer
    XmlBuffer result = *this;
    // reset the content only
    result.clear ();
    return result;
  }
  
  // return true if the buffer is not empty after striping

  bool XmlBuffer::isnext (void) {
    stripl ();
    return (d_blen > 0);
  }

  // return true if the buffer has a quoted string starter

  bool XmlBuffer::isqstr (void) {
    stripl ();
    if (d_blen == 0) return false;
    if (p_ubuf[0] == XML_CHAR_DQ) return true;
    if (p_ubuf[0] == XML_CHAR_SQ) return true;
    return false;
  }

  // return true if the buffer has a subset declaration starter

  bool XmlBuffer::issubs (void) {
    stripl ();
    if (d_blen == 0) return false;
    if (p_ubuf[0] == XML_CHAR_LB) return true;
    return false;
  }

  // set the preserve space flag

  void XmlBuffer::setprsv (const bool prsv) {
    d_prsv = prsv;
  }

  // get the preserve space flag

  bool XmlBuffer::getprsv (void) const {
    return d_prsv;
  }

  // set the xml version

  void XmlBuffer::setxmlv (const String& xvid) {
    d_xmlv = XmlSystem::toxmlv (xvid);
  }

  // return true if the character is a space

  bool XmlBuffer::isspc (const t_quad c) const {
    return is_spcc (d_xmlv, c);
  }

  // retun true if the character must be skipped
  bool XmlBuffer::isskip (const t_quad c) const {
    // chek for space
    if (isspc (c) == false) return false;
    // check for space preserve
    if (d_prsv == true) return false;
    // check for last space
    return isspc (last ());
  }

  // strip the buffer with leading space

  void XmlBuffer::stripl (void) {
    while (empty () == false) {
      t_quad c = getu ();
      if (is_spcc (d_xmlv, c) == true) continue;
      pushback (c);
      break;
    }
  }

  // strip the buffer with trailing blank

  void XmlBuffer::stripr (void) {
    while (d_blen > 0) {
      t_quad c = p_ubuf[d_blen-1];
      if (is_spcc (d_xmlv, c) == false) break;
      d_blen--;
    }
  }

  // strip the buffer with leading and trealing blanks

  void XmlBuffer::strip (void) {
    stripl ();
    stripr ();
  }

  // return the buffer content as a valid text

  String XmlBuffer::totext (void) const {
    // create a temporary buffer
    XmlBuffer xbuf = d_xmlv;
    // loop in the existing buffer and convert invalid characters
    // into a character entity repreentation
    for (long i = 0; i < d_blen; i++) {
      t_quad c = p_ubuf[i];
      if (is_char (d_xmlv, c) == true) {
	xbuf.add (c);
      } else {
	xbuf.add (XmlSystem::tocesc (c));
      }
    }
    // convert the buffer as a string
    return xbuf.tostring ();
  }

  // return the buffer content as a valid string
  
  String XmlBuffer::tostring (void) const {
    // mark the end of string
    t_quad* ubuf = Unicode::strdup (p_ubuf, d_blen);
    // check for valid characters
    if (is_valid (d_xmlv, ubuf) == false) {
      delete [] ubuf;
      throw Exception ("xml-error", "invalid character in buffer");
    }
    // convert the string
    String result = ubuf;
    delete [] ubuf;
    return result;
  }

  // return the buffer content as a valid name

  String XmlBuffer::toname (void) const {
    // mark the end of string
    t_quad* ubuf = Unicode::strdup (p_ubuf, d_blen);
    // check for valid characters
    if (is_name (d_xmlv, ubuf) == false) {
      delete [] ubuf;
      throw Exception ("xml-error", "invalid character in buffer");
    }
    // convert and clean
    String result = ubuf;
    delete [] ubuf;
    return result;
  }

  // return the buffer content as a valid name or pe reference

  String XmlBuffer::topnam (void) const {
    // mark the end of string
    t_quad* ubuf = Unicode::strdup (p_ubuf, d_blen);
    // check for valid characters
    if (is_pnam (d_xmlv, ubuf) == false) {
      delete [] ubuf;
      throw Exception ("xml-error", "invalid character in buffer");
    }
    // convert and clean
    String result = ubuf;
    delete [] ubuf;
    return result;
  }

  // return the buffer content as a character reference

  t_quad XmlBuffer::tocref (void) const {
    // get the character value
    t_quad cval = XsoBuffer::tocref ();
    // check the character value
    if (is_char (d_xmlv, cval) == false) {
      throw Exception ("xml-error", "invalid character reference");
    }
    return cval;
  }

  // get the next available general string in this buffer

  String XmlBuffer::getnstr (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    // read character from the buffer until it is empty
    while (empty () == false) {
      t_quad c = getu ();
      if (is_spcc (d_xmlv, c) == true) break;
      buf.add (c);
    }
    // get the string name
    return buf.tostring ();
  }

  // get the next available name in this buffer

  String XmlBuffer::getname (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    // read character from the buffer until it is empty
    while (empty () == false) {
      t_quad c = getu ();
      if (is_spcc (d_xmlv, c) == true) break;
      buf.add (c);
    }
    // get the string name
    return buf.toname ();
  }

  // get the next available name or pe reference in this buffer

  String XmlBuffer::getpnam (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    // check for empty
    if (empty () == true) return buf.tostring ();
    // check for pe reference
    if (get () == XML_CHAR_PC) {
      bool pend = false;
      while (empty () == false) {
	t_quad c = getu ();
	if (is_spcc (d_xmlv, c) == true) break;
	buf.add (c);
	if (c == XML_CHAR_SC) {
	  pend = true;
	  break;
	}
      }
      if (pend == false) {
	throw Exception ("xml-error", "unterminated pe reference", 
			 buf.tostring ());
      }
    } else {
      while (empty () == false) {
	t_quad c = getu ();
	if (is_spcc (d_xmlv, c) == true) break;
	buf.add (c);
      }
    }
    // get the string name
    return buf.topnam ();
  }

  // get the next available quoted string in this buffer

  String XmlBuffer::getqstr (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    if (empty () == true) {
      throw Exception ("xml-error", 
		       "empty buffer while reading quoted string");
    }
    // get the quote type
    t_quad c = getu ();
    // check for double quote
    if (c == XML_CHAR_DQ) {
      while (empty () == false) {
	c = getu ();
	if (c == XML_CHAR_DQ) return buf.tostring ();
	buf.add  (c);
      }
      throw Exception("xml-error", "unterminated quoted string");
    }
    // check fo single quote
    if (c == XML_CHAR_SQ) {
      while (empty () == false) {
	c = getu ();
	if (c == XML_CHAR_SQ) return buf.tostring ();
	buf.add  (c);
      }
      throw Exception("xml-error", "unterminated quoted string");
    }
    // illegal starter
    throw Exception("xml-error", 
		    "illegal character starter for quoted string");
  }

  // get the next available enumeration name in this buffer

  String XmlBuffer::getnenm (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    // read character from the buffer until it is empty
    while (empty () == false) {
      // check for end of enumeration
      if (get () == XML_CHAR_CP) break;
      // read next charcaters
      t_quad c = getu ();
      // check for space
      if (is_spcc (d_xmlv, c) == true) break;
      // check for separator
      if (c == XML_CHAR_VL) break;
      buf.add (c);
    }
    // get the string name
    return buf.toname ();
  }

  // get the next available enumeration

  String XmlBuffer::getenum (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    if (empty () == true) {
      throw Exception ("xml-error", "empty enumeration string");
    }
    // get the enumeration starter
    t_quad c = getu ();
    // check for open parenthesis
    if (c != XML_CHAR_OP) {
      throw Exception ("xml-error", 
		       "invalid starter character for parenthesis", c);
    }
    buf.add (c);
    while (empty () == false) {
      c = getu ();
      if (c == eosq) break;
      if (c == XML_CHAR_CP) {
	buf.add (c);
	return buf.tostring ();
      }
      if (is_enmc (d_xmlv, c) == false) {
	throw Exception("xml-error", "invalid enumeration character", c);
      }
      buf.add (c);
    }
    // unterminated enumeration
    throw Exception("xml-error", "unterminated enumeration string");
  }

  // get the next a available enumeration name

  String XmlBuffer::getenam (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // strip left the bufer
    stripl ();
    if (empty () == true) {
      throw Exception ("xml-error", 
		       "empty buffer while reading enumeration string");
    }
    // get the starter character
    t_quad c = get ();
    // check for open parenthesis
    if (c == XML_CHAR_OP) return getenum ();
    // this is a normal name
    return getname ();
  }
  
  // get the attribute default value

  String XmlBuffer::getxdef (void) {
    // check for predefined default declaration
    if (get() == XML_CHAR_NS) {
      String result = getu ();
      result += getname ();
      return result;
    }
    // here it is a quoted string
    return getqstr ();
  }

  // return a subset declaration string

  String XmlBuffer::getsubs (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // check for starter
    t_quad c = getu ();
    if (c != XML_CHAR_LB) {
      throw Exception ("xml-error", "invalid subset declaration starter", c);
    }
    // initialize counter and accumulate
    long bcnt = 1;
    while (empty () == false) {
      c = getu ();
      if (c == XML_CHAR_RB) {
	if (--bcnt == 0) break;
      }
      if (c == XML_CHAR_LB) bcnt++;
      buf.add (c);
    }
    if (bcnt != 0) {
      throw Exception ("xml-error", "unterminated subset declaration");
    }
    // here is the subset
    return buf.tostring ();
  }

  // get the next available attribute in this buffer

  Property XmlBuffer::getattr (void) {
    // create a working buffer
    XmlBuffer buf (d_xmlv);
    // initialize property name and value
    String name = "";
    String pval = "";
    // strip left the buffer
    stripl ();

    // get the name first
  s_name:
    t_quad c = getu ();
    if (c == nilq) goto s_serr;
    if (is_spcc (d_xmlv, c) == true) goto s_echk;
    if (c == Unicode::toquad ('=')) {
      pushback (c);
      goto s_echk;
    }
    buf.add (c);
    goto s_name;

    // check the equal (=) character
  s_echk:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (is_spcc (d_xmlv, c) == true) goto s_echk;
    if (c == Unicode::toquad ('=')) {
      name = buf.toname ();
      buf.reset ();
      stripl ();
      goto s_pval;
    }
    goto s_serr;

    // get the " or ' starting character
  s_pval:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (is_spcc (d_xmlv, c) == true) goto s_pval;
    if (c == Unicode::toquad ('\'')) goto s_sqav;
    if (c == Unicode::toquad ('"'))  goto s_dqav;
    goto s_serr;

    // single quoted attribute value
  s_sqav:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (c == Unicode::toquad ('\'')) goto s_done;
    buf.add (c);
    goto s_sqav;

    // double quoted attribute value
  s_dqav:
    c = getu ();
    if (c == nilq) goto s_serr;
    if (c == Unicode::toquad ('"')) goto s_done;
    buf.add (c);
    goto s_dqav;

    // syntax error
  s_serr:
    throw Exception ("xml-error", "syntax error while reading attribute");

    // get attribute value
  s_done:
    pval = buf.tostring ();
    return Property (name, pval);
  }

  // return true if the string is an enumeration
  
  bool XmlBuffer::isenum (const String& xval) {
    // create a working buffer
    XmlBuffer buf (d_xmlv, xval);
    // get the enumeration starter
    t_quad c = buf.getu ();
    // check for open parenthesis
    if (c != XML_CHAR_OP) return false;
    // loop in the buffer
    while (buf.empty () == false) {
      c = buf.getu ();
      if (c == XML_CHAR_CP) return true;
      if (is_enmc (d_xmlv, c) == false) break;
    }
    return false;
  }

  // return a string vector from an enumeration

  Strvec XmlBuffer::getxenm (const String& xval) {
    // create a working buffer
    XmlBuffer buf (d_xmlv, xval);
    // strip left the buffer
    buf.stripl ();
    if (buf.empty () == true) {
      throw Exception ("xml-error", "empty enumeration string");
    }
    // get the enumeration starter
    t_quad c = buf.getu ();
    // check for open parenthesis
    if (c != XML_CHAR_OP) {
      throw Exception ("xml-error", 
		       "invalid starter character for parenthesis", c);
    }
    // loop to get the enumeration name
    Strvec result;
    while (buf.empty () == false) {
      if (buf.get () == XML_CHAR_CP) break;
      String name = buf.getnenm ();
      result.add (name);
    }
    // check for proper termination
    if (buf.empty () == true) {
      throw Exception ("xml-error", "unterminated enumeration", xval);
    }
    if (buf.getu () != XML_CHAR_CP) {
      throw Exception ("xml-error", "unterminated enumeration", xval);
    }
    buf.strip ();
    if (buf.empty () == false) {
      throw Exception ("xml-error", "trailing enumeration characters", xval);
    }
    return result;
  }
}
