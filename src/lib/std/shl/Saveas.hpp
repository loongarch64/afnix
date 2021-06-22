// ---------------------------------------------------------------------------
// - Saveas.hpp                                                             -
// - afnix:sps module - saveas class definition                             -
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

#ifndef  AFNIX_SAVEAS_HPP
#define  AFNIX_SAVEAS_HPP

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Saveas class is a base class for a persistent object. The class
  /// defines the methods that are used to write a particular object. When
  /// saving, the object name is retreived with the getname method.
  /// The saveas method does the same as save but takes a name argument.
  /// @author amaury darsch

  class Saveas : public Taggable {
  public:
    /// create a default saveas
    Saveas (void) =default;
    
    /// create a saveas by name
    /// @param name the saveas name
    Saveas (const String& name);

    /// create a saveas by name and info
    /// @param name the saveas name
    /// @param info the saveas info
    Saveas (const String& name, const String& info);

    /// copy construct this saveas object
    /// @param that the saveas to copy
    Saveas (const Saveas& that);

    /// assign a saveas into this one
    /// @param that the saveas to assign
    Saveas& operator = (const Saveas& that);

    /// serialize this saveas
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this saveas
    /// @param is the input stream
    void rdstream (InputStream& os) override;
    
    /// save the object
    virtual void save (void) const;

    /// save the object by name
    virtual void saveas (const String& name) const;    

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
