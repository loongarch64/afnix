// ---------------------------------------------------------------------------
// - Terminal.hpp                                                            -
// - afnix standard library - terminal class definition                      -
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

#ifndef  AFNIX_TERMINAL_HPP
#define  AFNIX_TERMINAL_HPP

#ifndef  AFNIX_CILO_HPP
#include "Cilo.hpp"
#endif

#ifndef  AFNIX_CURSOR_HPP
#include "Cursor.hpp"
#endif

#ifndef  AFNIX_INPUTTERM_HPP
#include "InputTerm.hpp"
#endif

#ifndef  AFNIX_OUTPUTTERM_HPP
#include "OutputTerm.hpp"
#endif

namespace afnix {

  /// The Terminal class implements the functionnality of a terminal. The class
  /// holds one input stream and two output streams. The associated streams can
  /// be retreived by using the appropriate get method. By default the class
  /// works on a line reading basis. A primary and secondary prompt can be
  /// associated with it. When a call to readline is made, the terminal set the
  /// input streams with that line. The eol character is not returned.
  /// @author amaury darsch

  class Terminal : public InputTerm, public OutputTerm {
  private:
    /// the readline buffer
    Cursor d_lbuf;
    /// the circular loop
    Cilo   d_cilo;
    /// the no prompt flag
    bool d_nprt;
    /// the primary prompt
    String d_prt1;
    /// the secondary prompt
    String d_prt2;

  public:
    /// create a new default terminal 
    Terminal (void);

    /// destroy this terminal 
    ~Terminal (void);

    /// @return the class name
    String repr (void) const override;

    /// insert a character in the terminal
    /// @param c the character to insert
    void insert (const t_quad c) override;

    /// delete a character in place
    void rmchr (void);

    /// delete a character in backspace mode
    void bkchr (void);

    /// move to the beginning of line
    bool mvbol (void) override;

    /// move to the end of line
    bool mveol (void);

    /// move the cursor terminal to the left
    bool movel (void);

    /// move the cursor terminal to the right
    bool mover (void);

    /// kill all characters from the current position
    void kbchr (void);

    /// kill all characters in the buffer
    void kachr (void);

    /// replace the current editing with another string
    /// @param line the line to replace
    void rline (const String& line);

    /// refresh the terminal with the current editing
    void rfrsh (void);

    /// toggle the terminal insert mode
    void setim (void);

    /// read a line from this terminal
    /// @param pflg use primary or secondary prompt
    /// @return a line read from this terminal
    String readline (bool pflg);

    /// read a credential from this terminal
    /// @return a credential from this terminal
    String readcred (void);

    /// set the no prompt flag
    /// @param value the flag to set
    void setnp (const bool value); 

    /// set the primary prompt
    /// @param value the prompt to set
    void setpp (const String& value); 

    /// set the secondary prompt
    /// @param value the prompt to set
    void setsp (const String& value); 

    /// @return the no prompt flag
    bool getnp (void) const;

    /// @return the primary prompt
    String getpp (void) const;

    /// @return the secondary prompt
    String getsp (void) const;

  private:
    // make the copy constructor private
    Terminal (const Terminal&) =delete;
    // make the assignment operator private
    Terminal& operator = (const Terminal&) =delete;

  public:
    /// cerate a new object in a generic way
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
