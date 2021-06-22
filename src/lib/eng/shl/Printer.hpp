// ---------------------------------------------------------------------------
// - Printer.hpp                                                             -
// - afnix engine - printer class definition                                 -
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

#ifndef  AFNIX_PRINTER_HPP
#define  AFNIX_PRINTER_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Printer class implements the functionality to print various objects
  /// on the evaluable stream. When a call to apply is made, the function
  /// prints the objects passed as arguments. 
  /// @author amaury darsch

  class Printer : public Object {
  public:
    /// the type of printer
    enum t_type {OUTPUT, OUTPUTLN, ERROR, ERRORLN};

  private:
    /// the printer type
    t_type d_type;

  public:
    /// create a default printer 
    Printer (void);

    /// create a new printer class
    /// @param type the printer type
    Printer (const t_type type);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    Printer (const Printer&);
    // make the assignment operator private
    Printer& operator = (const Printer&);

  public:
    /// apply this object with a set of arguments
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param args the arguments to apply with
    Object* apply (Evaluable* zobj, Nameset* nset, Cons* args);
  };
}

#endif
