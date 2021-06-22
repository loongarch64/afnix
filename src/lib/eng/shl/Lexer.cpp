// ---------------------------------------------------------------------------
// - Lexer.cpp                                                               -
// - afnix engine - lexical analyzer class implementation                    -
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

#include "Lexer.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Lexical.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  static const t_quad LEX_CHAR_DQ = 0x00000022UL; // "
  static const t_quad LEX_CHAR_DZ = 0x00000023UL; // #
  static const t_quad LEX_CHAR_SQ = 0x00000027UL; // '
  static const t_quad LEX_CHAR_LP = 0x00000028UL; // (
  static const t_quad LEX_CHAR_RP = 0x00000029UL; // )
  static const t_quad LEX_CHAR_PS = 0x0000002BUL; // +
  static const t_quad LEX_CHAR_MS = 0x0000002DUL; // -
  static const t_quad LEX_CHAR_SP = 0x0000002EUL; // .
  static const t_quad LEX_CHAR_D0 = 0x00000030UL; // 0
  static const t_quad LEX_CHAR_D1 = 0x00000031UL; // 0
  static const t_quad LEX_CHAR_DP = 0x0000003AUL; // :
  static const t_quad LEX_CHAR_LB = 0x0000005BUL; // [
  static const t_quad LEX_CHAR_AS = 0x0000005CUL; // ANTI-SLASH
  static const t_quad LEX_CHAR_RB = 0x0000005DUL; // ]
  static const t_quad LEX_CHAR_US = 0x0000005FUL; // _
  static const t_quad LEX_CHAR_LC = 0x0000007BUL; // {
  static const t_quad LEX_CHAR_RC = 0x0000007DUL; // }
  static const t_quad LEX_CHAR_LS = 0x000025C0UL; // ◀
  static const t_quad LEX_CHAR_LR = 0x000025B6UL; // ▶

  static const t_quad LEX_CHAR_BU = 0x00000042UL; // B
  static const t_quad LEX_CHAR_BL = 0x00000062UL; // b
  static const t_quad LEX_CHAR_EU = 0x00000045UL; // E
  static const t_quad LEX_CHAR_EL = 0x00000065UL; // e
  static const t_quad LEX_CHAR_NL = 0x0000006EUL; // n
  static const t_quad LEX_CHAR_RU = 0x00000052UL; // R
  static const t_quad LEX_CHAR_RL = 0x00000072UL; // r
  static const t_quad LEX_CHAR_XU = 0x00000058UL; // X
  static const t_quad LEX_CHAR_XL = 0x00000078UL; // x
  static const t_quad LEX_CHAR_TL = 0x00000074UL; // t

  // this procedure flush the input stream until the end comment zone
  static inline long lex_flush_cz (InputStream* is) {
    // check for nil
    if (is == nullptr) return 0;
    // initialize line count
    long result = 0;
    // flush until found
    while (is->valid () == true) {
      t_quad c = is->getu ();
      if (c == LEX_CHAR_LR) return result;
      if (c == eolq) result++;
    }
    throw Exception ("lexer-error", "unterminated comment zone");
  }

  // this procedure creates a lexical token by value but check for the
  // special boolean value true and false
  static Token lex_to_token (const String& lval, const long lnum) {
    // check for true
    if (lval == "true") return Token (Token::BOOLEAN, lval, lnum);
    // check for false
    if (lval == "false") return Token (Token::BOOLEAN, lval, lnum);
    // fallback to a lexical
    return Token (Token::LEXICAL, lval, lnum);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default lexer

  Lexer::Lexer (void) {
    d_lnum = 1;
    p_is   = nullptr;
    // reset the buffer encoding
    d_cbuf.setemod (Encoding::EMOD_UTF8);
  }

  // create a lexer with a string

  Lexer::Lexer (const String& sval) {
    d_lnum = 1;
    Object::iref (p_is = new InputString (sval));
    // reset the buffer encoding
    d_cbuf.setemod (Encoding::EMOD_UTF8);
    // reset the encoding mode if any
    if (p_is != nullptr) d_cbuf.setemod (p_is->getemod ());
  }

  // create a new lexer

  Lexer::Lexer (InputStream* is) {
    d_lnum = 1;
    Object::iref (p_is = is);
    // reset the buffer encoding
    d_cbuf.setemod (Encoding::EMOD_UTF8);
    // reset the encoding mode if any
    if (p_is != nullptr) d_cbuf.setemod (p_is->getemod ());
  }

  // destroy this lexer

  Lexer::~Lexer (void) {
    Object::dref (p_is);
  }

  // return the class name

  String Lexer::repr (void) const {
    return "Lexer";
  }

  // return the next available token

  Token Lexer::get (void) {
    // reset controls
    bool   eflag  = false;
    bool   esign  = false;
    bool   eonly  = false;
    long   rcount = 0;
    t_quad clast  = nilq;

    // check for nil stream and reset buffer
    if (p_is == nullptr) return Token (Token::ERROR,d_lnum);
    d_cbuf.reset ();

  s_begin:
    t_quad c = p_is->getu ();
    switch (c) {
    case blkq:
    case tabq:
    case crlq:
      goto s_begin;
    case eosq: 
      return Token (Token::EOS,d_lnum);
    case eolq: 
      return Token (Token::EOL,++d_lnum);
    case LEX_CHAR_LP:
      return Token (Token::RFB, d_lnum);
    case LEX_CHAR_RP:
      return Token (Token::RFE, d_lnum);
    case LEX_CHAR_LC:
      return Token (Token::BFB, d_lnum);
    case LEX_CHAR_RC:
      return Token (Token::BFE, d_lnum);
    case LEX_CHAR_LS:
      d_lnum += lex_flush_cz (p_is);
      goto s_begin;
    case LEX_CHAR_DZ:
      goto s_comment;
    case LEX_CHAR_SQ:
      goto s_character;
    case LEX_CHAR_DQ:
      goto s_string;
    case LEX_CHAR_LB:
      rcount++;
      d_cbuf.add (c);
      goto s_regex;
    case LEX_CHAR_PS:
    case LEX_CHAR_MS:
      d_cbuf.add (c);
      goto s_number;
    case LEX_CHAR_D0:
      d_cbuf.add (c);
      goto s_numfmt;
    default:
      d_cbuf.add (c);
      if (Unicode::isdigit (c) == true) goto s_integer;
      if (Lexical::valid   (c) == true) goto s_lexical;
      break;
    }
    goto s_error;

  s_comment:
    c = p_is->getu ();
    if (c == eolq) return Token (Token::EOL,++d_lnum);
    if (c == eosq) return Token (Token::EOS,++d_lnum);
    goto s_comment;

  s_number:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_number;
    }
    if (c == LEX_CHAR_D0) {
      d_cbuf.add (c);
      goto s_numfmt;
    }
    if (Unicode::isdigit (c) == true) {
      d_cbuf.add (c);
      goto s_integer;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return lex_to_token (d_cbuf.tostring (), d_lnum);

  s_lexical:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_lexical;
    }
    if (c == LEX_CHAR_DP) {
      d_cbuf.add (clast = c);
      goto s_qualified;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return lex_to_token (d_cbuf.tostring (), d_lnum);

  s_qualified:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_qualified;
    }
    if ((Lexical::valid (c) == true) || (c == LEX_CHAR_DP)) {
      if ((clast == LEX_CHAR_DP) && (c == LEX_CHAR_DP)) {
	d_cbuf.add (c);
	goto s_error;
      }
      d_cbuf.add (clast = c);
      goto s_qualified;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    if (clast == LEX_CHAR_DP) goto s_error;
    p_is->pushback (c);
    return Token (Token::QUALIFIED, d_cbuf.tostring (), d_lnum);

  s_numfmt:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_numfmt;
    }
    if (Unicode::isdigit (c) == true) {
      d_cbuf.add (c);
      goto s_integer;
    }
    if ((c == LEX_CHAR_RL) || (c == LEX_CHAR_RU)) {
      goto s_relatif;
    }
    if (c == LEX_CHAR_SP) {
      d_cbuf.add (c);
      goto s_real;
    }
    if ((c == LEX_CHAR_XL) || (c == LEX_CHAR_XU)) {
      d_cbuf.add (c);
      goto s_hexa;
    }
    if ((c == LEX_CHAR_BL) || (c == LEX_CHAR_BU)) {
      d_cbuf.add (c);
      goto s_binary;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return Token (Token::INTEGER, d_cbuf.tostring (), d_lnum);

  s_integer:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_integer;
    }
    if (Unicode::isdigit (c) == true) {
      d_cbuf.add (c);
      goto s_integer;
    }
    if ((c == LEX_CHAR_RL) || (c == LEX_CHAR_RU)) {
      goto s_relatif;
    }
    if (c == LEX_CHAR_SP) {
      d_cbuf.add (c);
      goto s_real;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return Token (Token::INTEGER, d_cbuf.tostring (), d_lnum);

  s_hexa:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_hexa;
    }
    if (c == LEX_CHAR_US) {
      goto s_hexa;
    }
    if ((c == LEX_CHAR_RL) || (c == LEX_CHAR_RU)) {
      goto s_relatif;
    }
    if (Unicode::ishexa (c) == true) {
      d_cbuf.add (c);
      goto s_hexa;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return Token (Token::INTEGER, d_cbuf.tostring (), d_lnum);

  s_binary:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_binary;
    }
    if (c == LEX_CHAR_US) {
      goto s_binary;
    }
    if ((c == LEX_CHAR_RL) || (c == LEX_CHAR_RU)) {
      goto s_relatif;
    }
    if ((c == LEX_CHAR_D0) || (c == LEX_CHAR_D1)) {
      d_cbuf.add (c);
      goto s_binary;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return Token (Token::INTEGER, d_cbuf.tostring (), d_lnum);

  s_relatif:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_relatif;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return Token (Token::RELATIF, d_cbuf.tostring (), d_lnum);

  s_real:
    c = p_is->getu ();
    if (c == LEX_CHAR_LS) {
      d_lnum += lex_flush_cz (p_is);
      goto s_real;
    }
    if (Unicode::isdigit (c) == true) {
      if ((eflag == true) && (eonly == true)) eonly = false;
      d_cbuf.add (c);
      goto s_real;
    }
    if (((c == LEX_CHAR_EL) || (c == LEX_CHAR_EU)) && (eflag == false)) {
      d_cbuf.add (LEX_CHAR_EL);
      eflag = true;
      eonly = true;
      goto s_real;
    }
    if (((c == LEX_CHAR_PS) || (c == LEX_CHAR_MS)) && 
	(eflag == true) && (esign == false) && (eonly == true)) {
      d_cbuf.add (c);
      esign = true;
      eonly = false;
      goto s_real;
    }
    if (Lexical::valid (c) == true) {
      d_cbuf.add (c);
      goto s_lexical;
    }
    if (Unicode::isncc (c) == false) {
      d_cbuf.add (c);
      goto s_error;
    }
    p_is->pushback (c);
    return Token (Token::REAL, d_cbuf.tostring (), d_lnum);

  s_character:
    c = p_is->getu ();
    if (c == LEX_CHAR_SQ) 
      return Token (Token::CHARACTER, d_cbuf.tostring (), d_lnum);
    if (c == LEX_CHAR_AS) goto s_charesc;
    d_cbuf.add (c);
    goto s_character;
    
  s_charesc:
    c = p_is->getu ();
    switch (c) {
    case LEX_CHAR_NL:
      d_cbuf.add (eolq);
      break;
    case LEX_CHAR_RL:
      d_cbuf.add (crlq);
      break;
    case LEX_CHAR_TL:
      d_cbuf.add (tabq);
      break;
    case LEX_CHAR_AS:
      d_cbuf.add (LEX_CHAR_AS);
      break;
    case LEX_CHAR_SQ:
      d_cbuf.add (LEX_CHAR_SQ);
      break;
    default:
      d_cbuf.add (LEX_CHAR_AS);
      p_is->pushback (c);
      break;
    }
    goto s_character;
    
  s_string:
    c = p_is->getu ();
    if (c == eosq) goto s_error;
    if (c == LEX_CHAR_DQ) {
      return Token (Token::STRING,d_cbuf.tostring (), d_lnum);
    }
    if (c == LEX_CHAR_AS) goto s_stresc;
    d_cbuf.add (c);
    goto s_string;
    
  s_stresc:
    c = p_is->getu ();
    switch (c) {
    case LEX_CHAR_NL:
      d_cbuf.add (eolq);
      break;
    case LEX_CHAR_RL:
      d_cbuf.add (crlq);
      break;
    case LEX_CHAR_TL:
      d_cbuf.add (tabq);
      break;
    case LEX_CHAR_AS:
      d_cbuf.add (LEX_CHAR_AS);
      break;
    case LEX_CHAR_DQ:
      d_cbuf.add (LEX_CHAR_DQ);
      break;
    default:
      d_cbuf.add (LEX_CHAR_AS);
      p_is->pushback (c);
      break;
    }
    goto s_string;

  s_regex:
    c = p_is->getu ();
    switch (c) {
    case eosq:
      goto s_error;
    case LEX_CHAR_LB:
      d_cbuf.add (c);
      rcount++;
      goto s_regex;
    case LEX_CHAR_RB:
      d_cbuf.add (c);
      if (--rcount == 0) 
	return Token (Token::REGEX, d_cbuf.tostring (), d_lnum);
      if (rcount < 0) goto s_error;
      goto s_regex;
    default:
      d_cbuf.add (c);
      goto s_regex;
    }

  s_error:
    p_is->flush (eolq);
    return Token (Token::ERROR, d_cbuf.tostring (), ++d_lnum);
  }

  // return the lexer line number
   
  long Lexer::getlnum (void) const {
    return d_lnum;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETTOK  = zone.intern ("get-token");
  static const long QUARK_GETOBJ  = zone.intern ("get-object");
  static const long QUARK_GETLNUM = zone.intern ("get-line-number");

  // create a new object in a generic way

  Object* Lexer::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new Lexer;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Lexer (*sobj);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new Lexer (is);
      // invalid object
      throw Exception ("type-error", "invalid object for lexer",
		       Object::repr (obj));
    }
    throw Exception ("argument-error", "too many argument for lexer");
  }

  // return true if the given quark is defined

  bool Lexer::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Lexer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETTOK) return new Token (get ());
      if (quark == QUARK_GETOBJ) {
	Token tok = get ();
	Object* result = tok.getobj ();
	zobj->post (result);
	return result;
      }
      if (quark == QUARK_GETLNUM) return new Integer (getlnum ());
    }
    // apply these arguments with the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
