// ---------------------------------------------------------------------------
// - Loopable.hpp                                                            -
// - standard object library - loopable abstract class definition            -
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

#ifndef  AFNIX_LOOPABLE_HPP
#define  AFNIX_LOOPABLE_HPP

#ifndef  AFNIX_EVALUABLE_HPP
#include "Evaluable.hpp"
#endif

namespace afnix {

  /// The Loopable class is the virtual class used as an interface to the 
  /// interpreter loop execution subsystem. The class provides also an access
  /// the interpreter execution context like stream and global set.
  /// @author amaury darsch

  class Loopable : public Evaluable  {
  public:
    /// create a default loopable
    Loopable (void) =default;
    
    /// run the read-eval loop on the standard streams
    /// @return false if something bad happen
    virtual bool loop (void) =0;

    /// loop in the context of a nameset and an input stream
    /// @param nset the nameset to loop
    /// @param is   the input stream to use
    virtual bool loop (Nameset* nset, class InputStream* is) =0;

    /// run the read-eval loop with a file
    /// @param fname the file name to read
    /// @return false if something bad happen
    virtual bool loop (const String& fname) =0;

    /// run the read-eval loop with a file
    /// @param fname the file name to read
    virtual void load (const String& fname) =0;

    /// @return the evaluable input stream
    virtual class InputStream* getis (void) const =0;

    /// @return the evaluable output stream
    virtual class OutputStream* getos (void) const =0;

    /// @return the evaluable error stream
    virtual class OutputStream* getes (void) const =0;

  private:
    // definitively no copy move
    Loopable (Loopable&& that) noexcept =delete;
    // definitively no move operator
    Loopable& operator = (Loopable&& that) noexcept =delete;
  };
}

#endif
