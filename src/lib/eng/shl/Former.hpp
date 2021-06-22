// ---------------------------------------------------------------------------
// - Former.hpp                                                              -
// - afnix engine - form builder virtual class definition                    -
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

#ifndef  AFNIX_FORMER_HPP
#define  AFNIX_FORMER_HPP

#ifndef  AFNIX_FORM_HPP
#include "Form.hpp"
#endif

namespace afnix {

  /// The Former class is a virtual class used to derive form reader. The 
  /// class defines only a virtual method call "parse" which returns a form.
  /// The virtual method "getlnum" returns the reader line number
  /// @author amaury darsch

  class Former : public virtual Object {
  public:
    /// create a default former
    Former (void);
    
    /// copy move this former
    /// @param that the former to move
    Former (Former&& that) noexcept;

    /// move an former into this one
    /// @param that the former to move
    Former& operator = (Former&& that) noexcept;
    
    /// @return a new form
    virtual Form* parse (void) =0;

    /// @return the reader line number
    virtual long getlnum (void) const =0;

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
