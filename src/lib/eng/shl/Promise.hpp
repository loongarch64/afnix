// ---------------------------------------------------------------------------
// - Promise.hpp                                                             -
// - afnix engine - promise class definition                                 -
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

#ifndef  AFNIX_PROMISE_HPP
#define  AFNIX_PROMISE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Promise class is the object used to performed delayed evaluation.
  /// When the promised object is created, the form is stored for that object
  /// until a call to force the evalutaion is made. When the evaluation has 
  /// been made, the evaluated object is returned.
  /// @author amaury darsch

  class Promise : public virtual Serial {
  private:
    /// the promise form
    Object* p_form;
    /// the evaluated object
    Object* p_eobj;
    /// the flag for the delay
    bool d_dlay;

  public:
    /// create a default promise
    Promise (void);

    /// create a new promise with a form
    /// @param form the promised form
    Promise (Object* form);

    /// destroy this promise
    ~Promise (void);

    /// @return the class name
    String repr (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;
    
    /// @return the serial sid
    t_word getsid (void) const override;

    /// serialize this promise to an output stream
    /// @param os the output stream to write
    void wrstream (OutputStream& os) const override;

    /// deserialize a promise from an input stream
    /// @param is the input steam to read in
    void rdstream (InputStream& is) override;

    /// force the evaluation of this promise
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    virtual Object* force (Evaluable* zobj, Nameset* nset);

  private:
    // make the copy constructor private
    Promise (const Promise&) =delete;
    // make the assignment operator private
    Promise& operator = (const Promise&) =delete;

  public:
    /// evaluate this promise according to the delay flag
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    Object* eval (Evaluable* zobj, Nameset* nset) override;
  };
}

#endif
