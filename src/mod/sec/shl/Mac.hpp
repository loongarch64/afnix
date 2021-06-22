// ---------------------------------------------------------------------------
// - Mac.hpp                                                                 -
// - afnix:sec module - base mac class definition                            -
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

#ifndef  AFNIX_MAC_HPP
#define  AFNIX_MAC_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_HASHABLE_HPP
#include "Hashable.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The Mac class is a base class that is used to build a message
  /// authentication code. The class operates like a hasher object with
  /// the help of a secret key used to build the MAC result. The key
  /// is set at the object construction. The mac can be computed from
  /// an octet string or from an input stream.
  /// @author amaury darsch

  class Mac : public Hashable {
  protected:
    /// the mac key
    Key d_mkey;
    
  public:
    /// create a mac object by name and key
    /// @param name the mac name
    /// @param mkey the mac key
    Mac (const String& name, const Key& mkey);

    /// @return the mac key
    virtual Key getkey (void) const;

  private:
    // make the copy constructor private
    Mac (const Mac&);
    // make the assignment operator private
    Mac& operator = (const Mac&);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
