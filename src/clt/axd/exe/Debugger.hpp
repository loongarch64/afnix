// ---------------------------------------------------------------------------
// - Debugger.hpp                                                            -
// - afnix cross debugger - debugger class definitions                       -
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

#ifndef AFNIX_DEBUGGER_HPP
#define AFNIX_DEBUGGER_HPP

#ifndef  AFNIX_INTERP_HPP
#include "Interp.hpp"
#endif

#ifndef  AFNIX_READER_HPP
#include "Reader.hpp"
#endif

#ifndef  AFNIX_IFILE_HPP
#include "Ifile.hpp"
#endif

#ifndef  AFNIX_IFORM_HPP
#include "Iform.hpp"
#endif

#ifndef  AFNIX_BREAKABLE_HPP
#include "Breakable.hpp"
#endif

namespace afnix {

  /// The Debugger class is a derived class from the afnix interpreter that
  /// provide facility to debug an afnix program. The class works with the
  /// standard i/o streams and provides a special nameset called axd. Such
  /// nameset incorporates all commands needed to perform successfuly a
  /// debug or program tracing.
  /// @author amaury darsch

  class Debugger : public Interp, public Breakable {
  private:
    /// emacs mode flag
    bool d_emcs;
    /// the start flag
    bool d_strt;
    /// the next flag
    bool d_next;
    /// th finish flag
    bool d_fini;
    /// the exit flag
    bool d_exit;
    /// the verbose flag
    bool d_vflg;
    /// the max number of lines
    long d_lmax;
    /// the initial file name
    String  d_fnam;
    /// the breakpoint vector
    Vector  d_brkp;
    /// the debugger reader
    Reader* p_dbrd;
    /// the current form
    Iform* p_form;
    /// the axd nameset
    Nameset* p_aset;
    /// the i-files nameset
    Nameset* p_iset;

  public:
    /// create a new debugger
    Debugger (void);

    /// copy construct this debugger
    /// @param that the object to copy
    Debugger (const Debugger& that);

    /// destroy this debugger
    ~Debugger (void);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// the debugger top level loop
    bool loop (void) override;

    /// loop on the standard input in a nameset context
    /// @param nset the nameset context
    virtual bool loop (Nameset* nset) ;

    /// load a file by doing a read-eval loop
    void load (const String& fname) override;

    /// break the evaluable in a nameset with an object
    /// @param nset   the nameset to loop
    /// @param object the object to break on
    bool bpoint (Nameset* nset, Object* object) override;

    /// @return the next flag for this breakable
    bool getnext (void) const override;

    /// set the next flag
    /// @param flag the flag to set
    virtual void setnext (const bool flag) override;
    
    /// set the emacs info mode
    /// @param mode the mode to set
    virtual void setemacs (const bool mode);

    /// @return the start flag
    virtual bool getstart (void) const;

    /// set the exit flag
    virtual void setexit (const bool flag);

    /// set the initial file name
    /// @param fname the file name to set
    virtual void setinitial (const String& fname);

    /// run the initial file name
    virtual void runinitial (void);

    /// instrument a file
    /// @param fname the file name to instrument
    virtual Ifile* instrument (const String& fname);

    /// install a breakpoint at the current position
    virtual void setbpoint (void);

    /// install a breakpoint by line number
    /// @param lnum  the line number for the form
    virtual void setbpoint (const long lnum);

    /// install a breakpoint by file name and line number
    /// @param fname the file name to use
    /// @param lnum  the line number for the form
    virtual void setbpoint (const String& fname, const long lnum);

    /// print a listing at the current form
    virtual void flist (void) const;

    /// print a listing at current form  line number
    /// @param lnum  the line number to start
    virtual void flist (const long lnum) const;

    /// print a listing upto to max lines
    /// @param fname the file name
    /// @param lnum  the line number to start
    virtual void flist (const String& fname, const long lnum) const;

    /// write an info message to the output stream
    /// @param name the file name to info
    /// @param lnum the line number in the file
    virtual void info (const String& name, const long lnum) const;

    /// print a message if the verbose flag is on
    /// @param msg   the message to print
    /// @param data  the info data
    /// @param fname the optional file name
    /// @param lnum  the line number
    virtual void pmsg (const String& msg,   const long data,
		       const String& fname, const long lnum) const;

    /// write some debugger information
    virtual void dbginfo (void) const;

    /// write some breakpoint information
    virtual void brkinfo (void) const;

  private:
    // make the assignment operator private
    Debugger& operator = (const Debugger&) =delete;
  };
}

#endif
