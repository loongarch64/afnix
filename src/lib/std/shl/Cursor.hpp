// ---------------------------------------------------------------------------
// - Cursor.hpp                                                              -
// - standard object library - character buffer class definition             -
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

#ifndef  AFNIX_CURSOR_HPP
#define  AFNIX_CURSOR_HPP

#ifndef  AFNIX_STRBUF_HPP
#include "Strbuf.hpp"
#endif

namespace afnix {
  /// The Cursor class is a character buffer which can be used to accumulate
  /// character. The class maintains a set of indexes which can be used for 
  /// editing purpose. The Cursor class is used by the terminal class to
  /// support editing operation. Note that the class is self-resizable.
  /// @author amaury darsch

  class Cursor : public Strbuf {
  protected:
    /// the insert flag
    bool d_insert;
    /// the cursor position
    long d_cursor;
    /// the prompt length
    long d_prtlen;
    /// the current prompt
    String d_prompt;
  
  public:
    /// create a new buffer class.
    Cursor (void);

    /// create a new buffer class with a default size.
    /// @param size the buffer default size
    Cursor (const long size);
    
    /// create a new buffer and initialize it with a c string
    /// @param value the c string to initialize
    Cursor (const char* value);

    /// create a new buffer and initialize it with a string
    /// @param value the string to initialize
    Cursor (const String& value);

    /// @return the class name
    String repr (void) const override;

    /// reset this cursor
    void reset (void) override;

    /// add a character to this buffer
    /// @param value the character to add to this buffer
    void add (const t_quad value) override;

    /// clear this cursor but do not change the prompt
    virtual void clear (void);

    /// @return the relative cursor position
    virtual long getrel (void) const;

    /// @return the absolute cursor position
    virtual long getabs (void) const;

    /// @return the remaining length
    virtual long getrem (void) const;

    /// @return true if the cursor is at the bol
    virtual bool isbol (void) const;

    /// @return true if the cursor is at the eol
    virtual bool iseol (void) const;

    /// @return true if the cursor is at the last position
    virtual bool islst (void) const;

    /// @return true if the character can be deleted
    virtual bool isdel (void) const;

    /// set the cursor prompt
    /// @param prt the cursor prompt to set
    virtual void setprt (const String& ptr);

    /// @return the cursor prompt
    virtual String getprt (void) const;

    /// @return the prompt length
    virtual long prtlen (void) const;

    /// remove a character at the cursor (delete mode)
    /// @return true if the delete succeded
    virtual bool chdel (void);

    /// @return true if the cursor was moved one character left
    virtual bool movel (void);

    /// @return true if the cursor was moved one character right
    virtual bool mover (void);

    /// move cursor to the begining of the buffer and return the amount
    /// of move done by the cursor
    /// @return the number of cursor move
    virtual long setcb (void);

    /// move cursor to the end of the buffer and return the amount
    /// of move done by the cursor
    /// @return the number of cursor move
    virtual long setce (void);

    /// set the buffer insert flag
    /// @param mode the insert mode
    virtual void setim (const bool mode);

    /// @return the full line with its prompt
    virtual String toline (void) const;

    /// @return the substring from the cursor to the end
    virtual String substr (void) const;

    /// @return the substring from next cursor to the end
    virtual String delstr (void) const;

  private:
    // make the copy constructor private
    Cursor (const Cursor&) =delete;
    // make the assignment operator private
    Cursor& operator =  (const Cursor&) =delete;
  };
}

#endif
