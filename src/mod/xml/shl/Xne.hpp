// ---------------------------------------------------------------------------
// - Xne.hpp                                                                 -
// - afnix:xml module - xne class definition                                 -
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

#ifndef  AFNIX_XNE_HPP
#define  AFNIX_XNE_HPP

#ifndef  AFNIX_ITEM_HPP
#include "Item.hpp"
#endif

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The Xne class is the xml node engine (XNE) system class that
  /// holds the basic node definitions. Such engine is used for node
  /// manipulation as well as node location.
  /// @author amaury darsch

  class Xne {
  public:
    /// the node selector type
    enum t_xsel {
      XNE_ID,    // node id selector
      XNE_PI,    // pi node selector
      XNE_GE,    // ge node selector
      XNE_TAG,   // tag node selector
      XNE_ENT,   // entity node selector
      XNE_EREF,  // eref node selector
      XNE_CREF,  // cref node selector
      XNE_ELEM,  // element node selector
      XNE_TEXT,  // text node selector
      XNE_NAME,  // node name selector
      XNE_CDATA, // cdata node selector
      XNE_INDEX  // node index selector
    };

    /// @return a selector type by item
    static t_xsel totype (const Item& item);

    /// @return a selector type by object
    static t_xsel totype (Object* obj);

    /// @return an item by type
    static Item* toitem (const t_xsel xsel);

    /// @return true if the name is a valid nc name
    static bool isncnm (const String& name);

    /// get the prefix from a name
    /// @param name the name to parse
    static String getpnam (const String& name);

    /// get the local name from a name
    /// @param name the name to parse
    static String getlnam (const String& name);

    /// get an expanded name from a prefix and a local name
    /// @param pnam the prefix name
    /// @param lnam the local name
    static String toexnm (const String& pnam, const String& lnam);

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);
  };
}

#endif
