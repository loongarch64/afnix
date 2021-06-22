// ---------------------------------------------------------------------------
// - InputBound.cpp                                                          -
// - standard object library - input bounded stream class implementation     -
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

#include "Vector.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputBound.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default input bounded stream

  InputBound::InputBound (void) {
    p_is = nullptr;
    reset ();
  }

  // create an input bounded stream by stream

  InputBound::InputBound (InputStream* is) {
    p_is = nullptr;
    bind (is);
  }

  // create an input bounded stream by stream and bound

  InputBound::InputBound (InputStream* is, const long isub) {
    p_is = nullptr;
    bind (is, isub);
  }

  // destroy this input bounded stream

  InputBound::~InputBound (void) {
    Object::dref (p_is);
  }

  // return the class name

  String InputBound::repr (void) const {
    return "InputBound";
  }

  // reset this input bound stream

  void InputBound::reset (void) {
    wrlock ();
    try {
      d_isub = 0;
      d_scnt = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind an input stream to this stream

  void InputBound::bind (InputStream* is) {
    // do not bind ourself
    if (this == is) return;
    wrlock ();
    try {
      Object::iref (is);
      Object::dref (p_is);
      p_is = is;
      reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind an input stream to this stream

  void InputBound::bind (InputStream* is, const long isub) {
    // do not bind ourself
    if (this == is) return;
    wrlock ();
    try {
      Object::iref (is);
      Object::dref (p_is);
      p_is = is;
      reset ();
      d_isub = (isub < 0) ? 0 : isub;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the transcoding mode

  Encoding::t_tmod InputBound::gettmod (void) const {
    rdlock ();
    try {
      // get the local transcoding mode
      Encoding::t_tmod result = Transcoder::gettmod ();
      // eventually overwrite it
      if (p_is != nullptr) result = p_is->gettmod ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the transcoding mode

  void InputBound::settmod (const Encoding::t_tmod tmod) {
    wrlock ();
    try {
      // set the local transcoding mode
      Transcoder::settmod (tmod);
      // eventually set the stream mode
      if (p_is != nullptr) p_is->settmod (tmod);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a character can be encoded
    
  bool InputBound::valid (const char c) const {
    rdlock ();
    try {
      bool result = false;
      if (p_is != nullptr) {
	result = p_is->Transcoder::valid (c);
      } else {
	result = Transcoder::valid (c);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a character can be decoded

  bool InputBound::valid (const t_quad c) const {
    rdlock ();
    try {
      bool result = false;
      if (p_is != nullptr) {
	result = p_is->Transcoder::valid (c);
      } else {
	result = Transcoder::valid (c);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an input character into a unicode

  t_quad InputBound::encode (const char c) const {
    rdlock ();
    try {
      t_quad result = nilq;
      if (p_is != nullptr) {
	result = p_is->encode (c);
      } else {
	result = Transcoder::encode (c);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an input character buffer 

  t_quad* InputBound::encode (const char* s) const {
    rdlock ();
    try {
      t_quad* result = nullptr;
      if (p_is != nullptr) {
	result = p_is->encode (s);
      } else {
	result = Transcoder::encode (s);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an input character buffer by size

  t_quad* InputBound::encode (const char* s, const long size) const {
    rdlock ();
    try {
      t_quad* result = nullptr;
      if (p_is != nullptr) {
	result = p_is->encode (s, size);
      } else {
	result = Transcoder::encode (s, size);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an output unicode quad into a character

  char InputBound::decode (const t_quad c) const {
    rdlock ();
    try {
      char result = nilc;
      if (p_is != nullptr) {
	result = p_is->decode (c);
      } else {
	result = Transcoder::decode (c);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an output unicode buffer into a character buffer

  char* InputBound::decode (const t_quad* s) const {
    rdlock ();
    try {
      char* result = nullptr;
      if (p_is != nullptr) {
	result = p_is->decode (s);
      } else {
	result = Transcoder::decode (s);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // transcode an output unicode buffer into a character buffer
  
  char* InputBound::decode (const t_quad* s, const long size) const {
    rdlock ();
    try {
      char* result = nullptr;
      if (p_is != nullptr) {
	result = p_is->decode (s, size);
      } else {
	result = Transcoder::decode (s, size);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the stream descriptor

  int InputBound::getsid (void) const {
    rdlock ();
    try {
      int result = (p_is == nullptr) ? Stream::getsid () : p_is->getsid ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the stream encoding mode

  Encoding::t_emod InputBound::getemod (void) const {
    rdlock ();
    try {
      // get the local encoding mode
      Encoding::t_emod result = Stream::getemod ();
      // evetually overwrite it
      if (p_is != nullptr) result = p_is->getemod ();
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream encoding mode
  
  void InputBound::setemod (const Encoding::t_emod emod) {
    wrlock ();
    try {
      // set the local encoding mode
      Stream::setemod (emod);
      // eventually set the stream mode
      if (p_is != nullptr) p_is->setemod (emod);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream encoding mode by name

  void InputBound::setemod (const String& mode) {
    wrlock ();
    try {
      // set the local encoding mode
      Stream::setemod (mode);
      // eventually set the stream mode
      if (p_is != nullptr) p_is->setemod (mode);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the stream timeout

  long InputBound::gettout (void) const {
    rdlock ();
    try {
      long result = (p_is == nullptr) ? 0 : p_is->gettout ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream timeout

  void InputBound::settout (const long tout) {
    wrlock ();
    try {
      if (p_is != nullptr) p_is->settout (tout);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // flush the input stream buffer

  void InputBound::flush (void) {
    wrlock ();
    try {
      d_scnt = 0;
      if (p_is != nullptr) p_is->flush ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we are at the eos

  bool InputBound::iseos (void) const {
    rdlock ();
    try {
      // check for content length
      if ((d_isub > 0) && (d_scnt >= d_isub)) {
	unlock ();
	return true;
      }
      // read the stream status
      bool result = (p_is == nullptr) ? true : p_is->iseos (); 
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if we can read a character

  bool InputBound::valid (void) const {
    rdlock ();
    try {
      // check for content length
      if ((d_isub > 0) && (d_scnt >= d_isub)) {
	unlock ();
	return false;
      }
      // read the stream status
      bool result = (p_is == nullptr) ? false : p_is->valid (); 
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the next available character

  char InputBound::read (void) {
    wrlock ();
    try {
      // check for content length
      if ((d_isub > 0) && (d_scnt >= d_isub)) {
	unlock ();
	return eosc;
      }
      // check for nil stream
      if (p_is == nullptr) {
	unlock ();
	return eosc;
      }
      // read the base stream
      char result = p_is->read ();
      d_scnt++;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a character in the input buffer.
  
  long InputBound::pushback (const char value) {
    wrlock ();
    try {
      long result = 0;
      if (p_is != nullptr) {
	result = p_is->pushback (value);
	d_scnt -= result;
	if (d_scnt < 0) d_scnt = 0;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a unicode character in the input buffer.
  
  long InputBound::pushback (const t_quad value) {
    wrlock ();
    try {
      long result = 0;
      if (p_is != nullptr) {
	result = p_is->pushback (value);
	d_scnt -= result;
	if (d_scnt < 0) d_scnt = 0;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a c-string in the input buffer
  
  long InputBound::pushback (const char* s) {
    wrlock ();
    try {
      long result = 0;
      if (p_is != nullptr) {
	result = p_is->pushback (s);
	d_scnt -= result;
	if (d_scnt < 0) d_scnt = 0;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // pushback a c-string by size in the input buffer

  long InputBound::pushback (const char* s, const long size) {
    wrlock ();
    try {
      long result = 0;
      if (p_is != nullptr) {
	result = p_is->pushback (s, size);
	d_scnt -= result;
	if (d_scnt < 0) d_scnt = 0;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // consume the stream into the input buffer

  long InputBound::consume (void) {
    wrlock ();
    try {
      long result = (p_is == nullptr) ? 0 : p_is->consume ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the size of the input buffer

  long InputBound::buflen (void) const {
    rdlock ();
    try {
      long result = (p_is == nullptr) ? 0 : p_is->buflen ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a copy of the input buffer

  Buffer InputBound::toibuf (void) const {
    rdlock ();
    try {
      Buffer result;
      if (p_is != nullptr) result = p_is->toibuf ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a string representation of the input buffer

  String InputBound::tostring (void) const {
    rdlock ();
    try {
      String result = (p_is == nullptr) ? "" : p_is->tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET = zone.intern ("reset");

  // return true if the given quark is defined

  bool InputBound::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? InputStream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* InputBound::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // call the input stream method
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
