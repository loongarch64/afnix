// ---------------------------------------------------------------------------
// - Transcoder.hpp                                                          -
// - standard object library - unicode codeset transcoder class definition   -
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

#ifndef  AFNIX_TRANSCODER_HPP
#define  AFNIX_TRANSCODER_HPP

#ifndef  AFNIX_ENCODING_HPP
#include "Encoding.hpp"
#endif

#ifndef  AFNIX_UNIMAPPER_HPP
#include "Unimapper.hpp"
#endif

namespace afnix {

  /// The Transcoder class is a codeset transcoder class. The class is
  /// responsible to map a byte character in a given codeset into its
  /// associated unicode character. Care should be taken that not all
  /// characteres can be transcoded.
  /// @author amaury darsch

  class Transcoder : public virtual Object {
  private:
    /// the transcoding mode
    Encoding::t_tmod d_tmod;
    /// the encoding table (if any)
    t_quad* p_tmap;
    /// the decoding mapper
    Unimapper* p_umap;

  public:
    /// create a default transcoder
    Transcoder (void);
  
    /// create a transcoder by type
    /// @param tmod the tanscoding mod to use
    Transcoder (const Encoding::t_tmod tmod);

    /// destroy this transcoder
    ~Transcoder (void);

    /// return the class name
    String repr (void) const override;

    /// @return the transcoding mode
    virtual Encoding::t_tmod gettmod (void) const;

    /// set the transcoding mode
    /// @param tmod the transcoding mode to set
    virtual void settmod (const Encoding::t_tmod tmod);

    /// @return true if a character can be encoded
    virtual bool valid (const char c) const;

    /// @return true if a character can be decoded
    virtual bool valid (const t_quad c) const;

    /// transcode an input character into a unicode
    /// @param c the character to transcode
    virtual t_quad encode (const char c) const;

    /// transcode an input character buffer 
    /// @param s the character buffer to transcode
    virtual t_quad* encode (const char* s) const;

    /// transcode an input character buffer by size
    /// @param s    the character buffer to transcode
    /// @param size the size to transcode
    virtual t_quad* encode (const char* s, const long size) const;

    /// transcode an output unicode quad into a character
    /// @param c the character to transcode
    virtual char decode (const t_quad c) const;

    /// transcode an output unicode buffer into a character buffer
    /// @param s the character buffer to transcode
    virtual char* decode (const t_quad* s) const;

    /// transcode an output unicode buffer into a character buffer
    /// @param s    the character buffer to transcode
    /// @param size the size to transcode
    virtual char* decode (const t_quad* s, const long size) const;

  private:
    // make the copy constructor private
    Transcoder (const Transcoder&);
    // make the assignment operator private
    Transcoder& operator = (const Transcoder&);    

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
