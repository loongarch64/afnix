// ---------------------------------------------------------------------------
// - Message.hpp                                                             -
// - standard object library - message class definition                      -
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

#ifndef  AFNIX_MESSAGE_HPP
#define  AFNIX_MESSAGE_HPP

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Message class is container for an information message. The message
  /// can be either an informative message, a warning, an error or a fatal
  /// dignostic. A message can be optionnaly associated with a status code.
  /// The message information can be set in short form and associated with
  /// a long string description.
  /// @author amaury darsch

  class Message : public Taggable {
  public:
    /// the message type
    enum t_mesg : t_byte
      {
       MESG_NONE = 0x00U, // no message
       MESG_INFO = 0x01U, // information
       MESG_WARN = 0x02U, // warning message
       MESG_MERR = 0x03U, // error message
       MESG_FAIL = 0x04U  // fatal diagnostic
      };

  private:
    /// the message type
    t_mesg d_type;
    /// the message code
    long   d_code;

  public:
    /// create a default message
    Message (void);

    /// create message by name
    /// @param name the message name
    Message (const String& name);

    /// create message by name and info
    /// @param name the message name
    /// @param info the message info
    Message (const String& name, const String& info);

    /// create message by name, info and code
    /// @param name the message name
    /// @param info the message info
    /// @param code the message code
    Message (const String& name, const String& info, const long code);

    /// create message by type, name, info and code
    /// @param type the message type
    /// @param name the message name
    /// @param info the message info
    /// @param code the message code
    Message (const t_mesg type, const String& name, const String& info,
	     const long code);

    /// copy construct this message
    /// @param that the object to copy
    Message (const Message& that);

    /// copy move this message
    /// @param that the object to copy
    Message (Message&& that) noexcept;

    /// assign a message to this one
    /// @param that the obejct to assign
    Message& operator = (const Message& that);

    /// move a message to this one
    /// @param that the obejct to move
    Message& operator = (Message&& that) noexcept;

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial id
    t_word getdid (void) const override;

    /// @return the serial id
    t_word getsid (void) const override;

    /// serialize this object to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize an objectfrom an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// set the message content
    /// @param type the message type to set
    /// @param name the message name
    /// @param info the message info
    /// @param code the message code
    virtual void set (const t_mesg type, const String& name, const String& info,
		      const long   code);

    /// set the message type
    /// @param type the type to set
    virtual void settype (const t_mesg type);

    /// @return the message type
    virtual t_mesg gettype (void) const;

    /// set the message code
    /// @param code the message code to set
    virtual void setcode (const long code);

    /// @return the message code
    virtual long getcode (void) const;

    /// format the message as a string
    virtual String format (void) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
