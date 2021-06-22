// ---------------------------------------------------------------------------
// - Infoable.hpp                                                            -
// - standard object library - infoable abstract class definition            -
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

#ifndef  AFNIX_INFOABLE_HPP
#define  AFNIX_INFOABLE_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The Infoable class is an abstract class which represents an  object by
  /// name and info. It is an extension of the nameable class which does not
  /// have to be serialized.
  /// @author amaury darsch

  class Infoable : public Nameable {
  protected:
    /// the infoable name
    String  d_name;
    /// the infoable info
    String  d_info;
    
  public:
    /// create a default infoable
    Infoable (void) =default;

    /// create a infoable by name
    /// @param name the infoable name
    Infoable (const String& name);

    /// create a infoable by name and info
    /// @param name the infoable name
    /// @param info the infoable info
    Infoable (const String& name, const String& info);
    
    /// copy construct this infoable object
    /// @param that the infoable to copy
    Infoable (const Infoable& that);

    /// copy move this infoable object
    /// @param that the infoable to move
    Infoable (Infoable&& that) noexcept;

    /// assign a infoable into this one
    /// @param that the infoable to assign
    Infoable& operator = (const Infoable& that);

    /// move a infoable into this one
    /// @param that the infoable to move
    Infoable& operator = (Infoable&& that) noexcept;

    /// @return the object name
    String getname (void) const override;

    /// set the infoable name
    /// @param name the name to set
    virtual void setname (const String& name);
    
    /// set the infoable name and info at once
    /// @param name the name to set
    /// @param info the info to set
    virtual void setname (const String& name, const String& info);
    
    /// @return the infoable info
    virtual String getinfo (void) const;

    /// set the infoable info
    /// @param info the info to set
    virtual void setinfo (const String& info);

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
