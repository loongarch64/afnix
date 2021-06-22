// ---------------------------------------------------------------------------
// - InputTerm.cpp                                                           -
// - standard object library - terminal input stream class definition        -
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

#ifndef  AFNIX_INPUTTERM_HPP
#define  AFNIX_INPUTTERM_HPP

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

#ifndef  AFNIX_INPUTTIMEOUT_HPP
#include "InputTimeout.hpp"
#endif

namespace afnix {

  /// The InputTerm is the terminal input stream. The input terminal can be
  /// used in normal mode (with the read method) or in line editing mode
  /// (with the wread method). Special control characters are returned if they
  /// have a terminal capability definition. Thereset behaves like a normal
  /// input stream.
  /// @author amaury darsch

  class InputTerm : public InputBuffer, public InputTimeout {
  public:
    /// supported wide character code
    static const t_quad WIDE_STDBS = 0x00000008; // ^h
    static const t_quad WIDE_STDBL = 0x00000001; // ^a
    static const t_quad WIDE_STDEL = 0x00000005; // ^e
    static const t_quad WIDE_STDKL = 0x0000000b; // ^k
    static const t_quad WIDE_STDKB = 0x00000015; // ^u;
    static const t_quad WIDE_STDNL = 0x0000000a; // ^j;
    static const t_quad WIDE_STDIM = 0x00000009; // i;
    static const t_quad WIDE_STDEF = 0x00000004; // ^d
    static const t_quad WIDE_STDRF = 0x0000000c; // ^l
    static const t_quad WIDE_BS    = 0x80000000;
    static const t_quad WIDE_DEL   = 0x80000001;
    static const t_quad WIDE_UP    = 0x80000002;
    static const t_quad WIDE_DO    = 0x80000003;
    static const t_quad WIDE_LE    = 0x80000004;
    static const t_quad WIDE_RI    = 0x80000005;
    static const t_quad WIDE_INSCH = 0x80000006;
    static const t_quad WIDE_STDUP = 0x80000007;
    static const t_quad WIDE_STDDO = 0x80000008;
    static const t_quad WIDE_STDLE = 0x80000009;
    static const t_quad WIDE_STDRI = 0x8000000a;
    static const t_quad WIDE_STDDE = 0x8000000b;
    static const t_quad WIDE_STDIN = 0x8000000c;

    /// @return true if we have a wide character
    static bool iswide (const t_quad w);

    /// @return true if we have a backspace character
    static bool isbs (const t_quad w);

    /// @return true if we have a arrow up character
    static bool isup (const t_quad w);

    /// @return true if we have a arrow down character
    static bool isdo (const t_quad w);

    /// @return true if we have a arrow left character
    static bool isle (const t_quad w);

    /// @return true if we have a arrow right character
    static bool isri (const t_quad w);

    /// @return true if we have a printable character
    static bool ispr (const t_quad w);

    /// @return true if we have a delete character
    static bool isdel (const t_quad w);

    /// @return true if we have an insert character
    static bool isins (const t_quad w);

  private:
    /// the stream id
    int    d_sid;
    /// the terminal structure
    void* p_term;
    /// the terminal data structure
    void*  p_attr;
    /// the terminfo array
    char** p_tinfo;
    /// the maximum length in tinfo
    long   d_mtlen;
    /// ignore end of stream
    bool   d_ieos;
    /// the eos mapped char
    char   d_meos;

  protected:
    /// the eos flag
    bool d_eos;

  public:
    /// Create a new terminal stream mapped to the standard input.
   InputTerm (void);

    /// destroy this terminal
   ~InputTerm (void);

    /// @return the class name
    String repr (void) const override;

    /// @return the stream descriptor
    int getsid (void) const override;

    /// @return the next available character
    char read (void) override;

    /// @return the new available character or a special one
    t_quad wread (void);

    /// @return true if we are at the eos
    bool iseos (void) const override;

    /// @return true if we can read a character
    bool valid (void) const override;

    /// @return true if the input stream is a tty
    bool istty (void) const;

    /// save the terminal attributes
    void save (void);

    /// restore the terminal attribute
    void restore (void);

    /// turn the terminal in non canonical mode
    /// @return true if the operation succeeds
    bool nocanon (void);

    /// set the ignore eos flag
    void setieos (const bool flag);

    /// set the mapped eos
    void setmeos (const char c);
    
  private:
    // make copy constructor private
    InputTerm (const InputTerm&) =delete;
    // make the assignment operator private
    InputTerm& operator = (const InputTerm&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
