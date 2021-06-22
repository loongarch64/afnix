// ---------------------------------------------------------------------------
// - OutputTerm.hpp                                                          -
// - standard object library - terminal output stream class definition       -
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

#ifndef  AFNIX_OUTPUTTERM_HPP
#define  AFNIX_OUTPUTTERM_HPP

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The OutputTerm class is the terminal output class for writing various
  /// formatted data. The class implements the Output stream interface. The
  /// output stream type is selected at construction and can be either standard
  /// output or error output
  /// @author amaury darsch

  class OutputTerm : public OutputStream {
  public:
    /// the terminal mode type
    enum t_mode {OUTPUT, ERROR};

  private:
    /// the stream id
    int d_sid;
    /// the terminal structure
    void*  p_term;
    /// the terminfo array
    char** p_tinfo;

  protected:
    /// the insert mode flag
    bool d_insert;

  public:
    /// create a new output stream.
    OutputTerm (void);

    /// create a new output stream. The output stream is created with the
    /// standard output mode (i.e. Output or error)
    /// @param mode the output mode
    OutputTerm (t_mode mode);

    /// destroy this output terminal
    ~OutputTerm (void);

    /// @return the class name
    String repr (void) const override;

    /// @return the stream descriptor
    int getsid (void) const override;

    /// @return true if the output stream is a tty
    bool istty (void) const override;

    /// @return the number of columns
    long getcols (void) const;

    /// set or reset the terminal error mode
    /// @param mode set in error mode if true
    void temode (const bool mode);

    /// @return true if the screen has been cleared
    bool clear (void);

    /// write one character on the output stream.
    /// @param value the character to write  
    long write (const char value) override;

    /// write a character string to the output stream
    /// @param data the data to write
    long write (const char* data) override;

    /// write an string with the terminal in error mode
    /// @param value the string to write
    void error (const String& value) override;
    
    /// insert a character - depending on the insert mode
    /// @param c the character to insert
    virtual void insert (const char c);

    /// insert a character buffer depending on the insert mode
    /// @param s the character buffer to insert
    virtual void insert (const char* s);

    /// insert a unicode character depending on the insert mode
    /// @param c the unicode character to insert
    virtual void insert (const t_quad c);

    /// insert a unicode buffer depending on the insert mode
    /// @param s the buffer to insert
    virtual void insert (const t_quad* s);

    /// insert a string depending on the character mode
    //// @param s the string to insert
    virtual void insert (const String& s);

    /// delete one character in position
    virtual bool chdel (void);

    /// move to the beginning of line
    virtual bool mvbol (void);

    /// move to the left by a certain amount
    /// @param num the number of move to do
    virtual bool movel (const long num);

    /// move to the right by a certain amount
    /// @param num the number of move to do
    virtual bool mover (const long num);

    /// move up by a certain amount
    /// @param num the number of move to do
    virtual bool moveu (const long num);

    /// move down by a certain amount
    /// @param num the number of move to do
    virtual bool moved (const long num);

    /// move to the bol from a position by a number
    /// @param pos the current position
    /// @param num the number of move
    void mvbol (const long pos, const long num);

    /// move to the eol from a position by a number
    /// @param pos the current position
    /// @param num the number of move
    void mveol (const long pos, const long num);

  private:
    // make the copy constructor private
    OutputTerm (const OutputTerm&) =delete;
    // make the assignment operator private
    OutputTerm& operator = (const OutputTerm&) =delete;

  public:
    /// create a new output terminal in a generic way
    /// @param argv the argument vector
    static Object* mkout (Vector* argv);
    
    /// create a new error terminal in a generic way
    /// @param argv the argument vector
    static Object* mkerr (Vector* argv);

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
