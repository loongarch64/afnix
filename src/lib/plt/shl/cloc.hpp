// ---------------------------------------------------------------------------
// - cloc.hpp                                                                -
// - standard platform library - c localization function definition          -
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

#ifndef  AFNIX_CLOC_HPP
#define  AFNIX_CLOC_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the generic codeset mapping table as defined by
  /// ISO 8859. The codeset cqn be mapped directly or from the locale
  /// definition available from the running system
  /// @author amaury darsch

  /// the supported codeset
  enum t_codeset {
    LOC_ASC_00, // generic 7 bits ascii
    LOC_ISO_01, // 8 bits iso 8859-1
    LOC_ISO_02, // 8 bits iso 8859-2
    LOC_ISO_03, // 8 bits iso 8859-3
    LOC_ISO_04, // 8 bits iso 8859-4
    LOC_ISO_05, // 8 bits iso 8859-5
    LOC_ISO_06, // 8 bits iso 8859-6
    LOC_ISO_07, // 8 bits iso 8859-7
    LOC_ISO_08, // 8 bits iso 8859-8
    LOC_ISO_09, // 8 bits iso 8859-9
    LOC_ISO_10, // 8 bits iso 8859-10
    LOC_ISO_11, // 8 bits iso 8859-11
    LOC_ISO_13, // 8 bits iso 8859-13
    LOC_ISO_14, // 8 bits iso 8859-14
    LOC_ISO_15, // 8 bits iso 8859-15
    LOC_ISO_16, // 8 bits iso 8859-16
    LOC_UTF_08  // utf-8 encoding
  };

  /// localization initialization
  void c_iloc (void);

  /// @return the system codeset
  t_codeset c_getcset (void);

  /// get the codeset from a locale name
  /// @param locale the locale name to parse
  t_codeset c_mapcset (const char* locale);

  /// get a trancoding table by codeset - but do not delete it
  /// @param cset the codeset to map
  const t_quad* c_gettmap (const t_codeset cset);

}

#endif
