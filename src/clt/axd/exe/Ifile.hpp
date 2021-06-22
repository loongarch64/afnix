// ---------------------------------------------------------------------------
// - Ifile.hpp                                                               -
// - afnix cross debugger - instrumented file class definitions              -
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

#ifndef  AFNIX_IFILE_HPP
#define  AFNIX_IFILE_HPP

#ifndef  AFNIX_IFORM_HPP
#include "Iform.hpp"
#endif

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_MODULE_HPP
#include "Module.hpp"
#endif

#ifndef  AFNIX_QUARKTABLE_HPP
#include "QuarkTable.hpp"
#endif

namespace afnix {

  /// The Ifile class is an instrumented file class representation.
  /// The class is constructed by creating block cons cell and indexing
  /// in a quark table the form by their line number. An eval method
  /// is provided to evaluate the file at once.
  /// @author amaury darsch

  class Ifile : public Object {
  private:
    /// the file name
    String d_fname;
    /// the line->form mapping
    QuarkTable d_lmap;
    /// the form vector
    Vector d_fvec;

  public:
    /// create a new ifile with a module
    Ifile (Module& mp);

    /// destroy this ifile
    ~Ifile (void);

    /// return the class name
    String repr (void) const;

    /// @return the i-file file name
    String getfname (void) const;

    /// @return the form vector length
    long length (void) const;

    /// @return a form by index
    Iform* getform (const long idnex) const;

    /// @return a form by line number
    Iform* lookup (const long lnum) const;

    /// evaluate this i-file withing the calling nameset
    /// @param dbg  the current evaluable
    /// @param nset the current nameset 
    Object* eval (Evaluable* zobj, Nameset* nset);

  private:
    // make the copy constructor private
    Ifile (const Ifile&);
    // make the assignment operator private
    Ifile& operator = (const Ifile&);
  };
}

#endif
