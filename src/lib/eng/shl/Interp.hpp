// ---------------------------------------------------------------------------
// - Interp.hpp                                                              -
// - afnix engine - interpreter class definition                             -
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

#ifndef  AFNIX_INTERP_HPP
#define  AFNIX_INTERP_HPP

#ifndef  AFNIX_LOADER_HPP
#include "Loader.hpp"
#endif

#ifndef  AFNIX_OPTIONS_HPP
#include "Options.hpp"
#endif

#ifndef  AFNIX_TERMINAL_HPP
#include "Terminal.hpp"
#endif

#ifndef  AFNIX_SUPERSET_HPP
#include "Superset.hpp"
#endif

#ifndef  AFNIX_RESOLVER_HPP
#include "Resolver.hpp"
#endif

#ifndef  AFNIX_RUNNABLE_HPP
#include "Runnable.hpp"
#endif

namespace afnix {

  /// The Interp class is the main afnix interpreter engine. The interpreter
  /// operates on three streams, the standard input, the standard output and
  /// the standard error stream. By default, the interpreter install a 
  /// terminal object, unless the three streams are specified at construction.
  /// The interpreter holds a series of registers. For each basic types,
  /// like integer, real, character and boolean, the interpreter has 256 
  /// local thread registers. The operations associated with the registers
  /// are type dependant. For integer and real, these are mostly arithmetic 
  /// operations. For boolean, these are logical operations. The interpreter 
  /// has a top level (or global) nameset. The global nameset is shared 
  /// between all interpreters and is referenced as '...'. On top of this, 
  /// the interpreter provides a 'compile' method which takes a form and 
  /// generates a new one. The sole purpose of the compile method is to
  /// perform static checking and generate a new form ready for execution.
  /// @author amaury darsch

  class Interp : public virtual Runnable {
  public:
    /// @return an interpreter option object
    static Options* getopts (void);
    
  protected:
    /// the standard input stream
    mutable InputStream*  p_is;
    /// the standard output stream
    mutable OutputStream* p_os;
    /// the standard error stream
    mutable OutputStream* p_es;
    /// the cloned interpreter flag
    bool    d_clon;
    /// the posted object
    Object* p_post;
    /// the super global nameset
    Superset* p_gset;
    /// the vector arguments
    Vector* p_argv;
    /// the library loader
    Loader* p_shld;
    /// the evaluable form
    Object* p_rfrm;
    /// the default terminal
    Terminal* p_term;
    /// the path resolver
    Resolver* p_rslv;
    /// the encoding mode
    String d_emod;

  public:
    /// create a default interpreter
    Interp (void);

    /// create a new interpreter with or without a terminal
    /// @param tflg the terminal flag
    Interp (const bool tflg);

    /// create a new interpreter
    /// @param is the standard input stream
    /// @param os the standard output stream
    /// @param es the standard error stream
    Interp (InputStream* is, OutputStream* os, OutputStream* es);

    /// copy constructor for this interpreter
    /// @param that the interpreter to copy
    Interp (const Interp& that);

    /// destroy this interpreter
    ~Interp (void);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// evaluate the evaluable form
    Object* run (void) override;

    /// evaluate a form in a thread by cloning this interpreter
    /// @param form the form to evaluate
    Object* launch (Object* form) override;

    /// evaluate a form in a thread by cloning this interpreter
    /// @param tobj the thread object to run
    /// @param form the form to evaluate
    Object* launch (Object* tobj, Object* form) override;

    /// run the read-eval loop on the standard streams
    /// @return false if something bad happen
    bool loop (void) override;

    /// loop in the context of a nameset and an input stream
    /// @param nset the nameset context
    /// @param is   the input stream to use
    bool loop (Nameset* nset, InputStream* is) override;

    /// run the read-eval loop with a file
    /// @param fname the file name to read
    /// @return false if something bad happen
    bool loop (const String& fname) override;

    /// run the read-eval loop with a file
    /// @param fname the file name to read
    void load (const String& fname) override;

    /// @return the interpreter input stream
    InputStream* getis (void) const override;

    /// @return the interpreter output stream
    OutputStream* getos (void) const override;

    /// @return the interpreter error stream
    OutputStream* getes (void) const override;

    /// @return the interpreter global set
    Nameset* getgset (void) const override;

    /// post an object in this interpreter
    void post (Object* object) override;

    /// clone this interpreter and set the evaluable form
    /// @param form the evaluable form to set
    virtual Interp* dup (Object* form) const;

    /// duplicate this interpreter by seeting the terminal
    /// @param term the teminal stream to set
    virtual Interp* dup (Terminal* term) const;

    /// duplicate this interpreter by updating the streams
    /// @param is the input  stream to update
    /// @param os the output stream to update
    /// @param es the error  stream to update
    virtual Interp* dup (InputStream* is, OutputStream* os, 
			 OutputStream* es) const;

    /// create a new reserved name in the global nameset
    /// @param name the reserved name
    /// @param object the object to bind
    virtual void mkrsv (const String& name, Object* object);

    /// create a child nameset in the superset
    /// @param name the nameset name to create
    virtual Nameset* mknset (const String& name);
    
    /// set the primary prompt
    /// @param value the prompt to set
    virtual void setpp (const String& value);

    /// @return the primary prompt
    virtual String getpp (void) const;

    /// set the secondary prompt
    /// @param value the prompt to set
    virtual void setsp (const String& value);

    /// @return the secondary prompt
    virtual String getsp (void) const;

    /// read a line from the interpreter terminal
    /// @param pflg the primary prompt flag
    virtual String readline (const bool pflg);

    /// read a credential from the interpreter terminal
    virtual String readcred (void);

    /// read a credentialfrom the interpreter terminal
    /// @param prompt the initial prompt
    virtual String readcred (const String& prompt);

    /// set the interpreter encoding mode
    /// @param emod the encoding mode
    virtual void setemod (const String& emod);

    /// set the interpreter options
    /// @param opts the options object
    virtual String setopts (const Options& opts);

    /// set the interpreter arguments
    /// @param args the arguments to set
    virtual void setargs (const Strvec& args);

    /// @return the interpreter arguments
    virtual Strvec getargs (void) const;

    /// add a path to the resolver
    /// @param path the path to add
    virtual void addpath (const String& path);

    /// set the resolver with a list of path
    /// @param path the path to set
    virtual void setpath (const Strvec& path);

    /// @return the interpreter loader
    virtual Loader* getld (void) const;

    /// check if a library has been loaded
    /// @param name the library name to check
    virtual bool islib (const String& name) const;

    /// register a library by name and handle
    /// @param name the library name to register
    /// @param hand the library handle
    virtual void reglib (const String& name, void* hand);

    /// open a new dynamic library by name
    /// @param name the library name
    /// @param argv the vector arguments
    virtual Object* library (const String& name, Vector* argv);

    /// compile from an input stream to an output stream
    /// @param the file to compile
    /// @param os the output stream to write
    virtual void compile (const String& name, OutputStream& os);

  private:
    // make the assignment operator private
    Interp& operator = (const Interp&) =delete;
    // definitevely do not copy move
    Interp (Interp&&) noexcept =delete;
    // definitevely do not move
    Interp& operator = (Interp&&) noexcept =delete;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// evaluate an object in this interpreter
    /// @param object the object to evaluate
    Object* eval (Object* object) override;

    /// evaluate an object in this evaluable by quark
    /// @param object the object to evaluate
    /// @param quark  the quark to evaluate
    Object* eval (Object* object, const long quark) override;
    
    /// evaluate an object method by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark) override;

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
