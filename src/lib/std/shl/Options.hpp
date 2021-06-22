// ---------------------------------------------------------------------------
// - Options.hpp                                                             -
// - standard object library - options class definition                      -
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

#ifndef  AFNIX_OPTIONS_HPP
#define  AFNIX_OPTIONS_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The Options class is a simple class used to define and retrieve user 
  /// options. The options object is constructed by specifying with option is
  /// valid and how it behaves. Then the arguments can be passed to the object
  /// for subsequent analysis.
  /// @author amaury darsch

  class Options : public Object {
  public:
    /// the option type
    enum t_type {
      UNIQ, // single option
      SOPT, // string option
      VOPT, // string vector
      LIST  // list option
    };

  private:
    /// the usage message
    String d_umsg;
    /// the option descriptors
    struct s_optd* p_optd;
    /// the program name
    String d_pgmn;
    /// the argument vector
    Strvec d_args;

  public:
    /// create a default option
    Options (void);

    /// create an option with a usage message
    /// @param umsg the usage message to use
    Options (const String& umsg);

    /// destroy this option class
    ~Options (void);

    /// @return the class name
    String repr (void) const;

    /// reset the option class
    void reset (void);

    /// set the usage message
    /// @param umsg the usage message to set
    void setumsg (const String& umsg);

    /// @return the usage message
    String getumsg (void) const;

    /// print a usage message to an output stream
    /// @param os the output stream to use
    void usage (OutputStream& os) const;

    /// add a uniq or string option
    /// @param type the option type
    /// @param opte the option element
    /// @param mesg the options messagge
    void add (t_type type, const char opte, const String& mesg);

    /// add a uniq or string option
    /// @param type the option type
    /// @param opte the option element
    /// @param mesg the options messagge
    void add (t_type type, const t_quad opte, const String& mesg);

    /// add a list option
    /// @param opte the option element
    /// @param opts the option string
    /// @param mesg the options messagge
    void add (const char opte, const String& opts, const String& mesg);

    /// add a list option
    /// @param opte the option element
    /// @param opts the option string
    /// @param mesg the options messagge
    void add (const t_quad opte, const String& opts, const String& mesg);

    /// @return an option flag by option element
    bool getoflg (const char opte) const;

    /// @return an option flag by option element
    bool getoflg (const t_quad opte) const;

    /// @return an option flag by option element and string
    bool getoflg (const char opte, const String& opts) const;

    /// @return an option flag by option element and string
    bool getoflg (const t_quad opte, const String& opts) const;

    /// set an option string by value
    /// @param opte the option element
    /// @param opts the option string
    void setopts (const char opte, const String& opts);

    /// set an option string by value
    /// @param opte the option element
    /// @param opts the option string
    void setopts (const t_quad opte, const String& opts);

    /// @return an option string by option element
    String getopts (const char opte) const;

    /// @return an option string by option element
    String getopts (const t_quad opte) const;

    /// @return an option vector by option element
    Strvec getoptv (const char opte) const;

    /// @return an option vector by option element
    Strvec getoptv (const t_quad opte) const;

    /// set the debug marking flag
    /// @param opte the option element
    void setdbug (const char opte);

    /// set the debug marking flag
    /// @param opte the option element
    void setdbug (const t_quad opte);

    /// set the debug marking flag
    /// @param opte the option element
    /// @param opts the option string
    void setdbug (const char opte, const String& opts);

    /// set the debug marking flag
    /// @param opte the option element
    /// @param opts the option string
    void setdbug (const t_quad opte, const String& opts);

    /// @return true if the argument vector is empty
    bool empty (void) const;

    /// @return true if the terminal flag is set
    bool istflg (void) const;

    /// @return the argument vector
    Strvec getargs (void) const;

    /// @return a vector option by option element
    Vector* getvopt (const t_quad opte) const;

    /// @return the vector argument
    Vector* getvarg (void) const;

    /// @return the program name
    String getpgmn (void) const;

    /// parse a vector argument
    /// @param args the vector arguments
    void parse (const Strvec& argv);

    /// parse a system vector argument
    /// @param argc the number of arguments
    /// @param argv the arguments vector
    void parse (const long argc, const char** argv);

    /// parse an object vector argument
    /// @param argv the object vector
    void parse (Vector* argv);

  private:
    // make the copy constructor private
    Options (const Options&);
    // make the assignment operator private
    Options& operator = (const Options&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
