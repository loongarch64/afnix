// ---------------------------------------------------------------------------
// - Taggable.hpp                                                            -
// - standard object library - taggable abstract class definition            -
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

#ifndef  AFNIX_TAGGABLE_HPP
#define  AFNIX_TAGGABLE_HPP

#ifndef  AFNIX_INFOABLE_HPP
#include "Infoable.hpp"
#endif

namespace afnix {

  /// The Taggable class is an abstract class which represents a serializable
  /// object by name and info.
  /// @author amaury darsch

  class Taggable : public virtual Serial, public Infoable {
  public:
    /// create a default taggable
    Taggable (void) =default;

    /// create a taggable by name
    /// @param name the taggable name
    Taggable (const String& name);

    /// create a taggable by name and info
    /// @param name the taggable name
    /// @param info the taggable info
    Taggable (const String& name, const String& info);
    
    /// copy construct this taggable object
    /// @param that the taggable to copy
    Taggable (const Taggable& that);

    /// copy move this taggable object
    /// @param that the taggable to move
    Taggable (Taggable&& that) noexcept;

    /// assign a taggable into this one
    /// @param that the taggable to assign
    Taggable& operator = (const Taggable& that);

    /// move a taggable into this one
    /// @param that the taggable to move
    Taggable& operator = (Taggable&& that) noexcept;

    /// serialize this taggable
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this taggable
    /// @param is the input stream
    void rdstream (InputStream& os) override;
    
    /// @return the object tag
    virtual String totag (void) const;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
