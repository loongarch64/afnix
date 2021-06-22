// ---------------------------------------------------------------------------
// - Librarian.cpp                                                           -
// - afnix engine - librarian class implementation                           -
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

#include "Byte.hpp"
#include "System.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "Librarian.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputFile.hpp"
#include "OutputFile.hpp"
#include "InputMapped.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // librarian constants
  static const long   AXL_HDR_MSIZE    = 4;
  static const t_byte AXL_HDR_MAGIC[4] = {'\177', 'A', 'X', 'L'};
  static const t_byte AXL_HDR_MAJOR    = AFNIX_VERSION_MAJOR;
  static const t_byte AXL_HDR_MINOR    = AFNIX_VERSION_MINOR;
  static const t_byte AXL_HDR_FLAGS    = nilc;

  // librarian flags marking
  static const t_byte AXL_DEF_MRK      = '-';
  static const long   AXL_FMT_SIZ      = 8;
  static const t_byte AXL_FMT_MRK[AXL_FMT_SIZ] = {
    's', 'c', 'u', 'u', 'u', 'u', 'u', 'u'
  };

  // the librarian start module bit
  static const t_byte AXL_STM_BIT    = 0x01; 

  // the librarian header
  struct s_lhead {
    t_byte d_magic[AXL_HDR_MSIZE];
    t_byte d_major;
    t_byte d_minor;
    t_byte d_flags;
    t_octa d_hsize;
    // create an empty header for reading
    s_lhead (void) {
      for (long i = 0; i < AXL_HDR_MSIZE; i++) d_magic[i] = nilc;
      d_major = 0;
      d_minor = 0;
      d_flags = 0;
      d_hsize = 0;
    }
    // create a new header with a size for writing
    s_lhead (const long size) {
      for (long i = 0; i < AXL_HDR_MSIZE; i++) d_magic[i] = AXL_HDR_MAGIC[i];
      d_major = AXL_HDR_MAJOR;
      d_minor = AXL_HDR_MINOR;
      d_flags = AXL_HDR_FLAGS;
      d_hsize = System::oswap (size);
    }
    // check this header
    bool check (void) {
      // check magic
      for (long i = 0; i < AXL_HDR_MSIZE; i++)
	if (d_magic[i] != AXL_HDR_MAGIC[i]) return false;
      // check major
      if (d_major != AXL_HDR_MAJOR) return false;
      // check minor
      if (d_minor > AXL_HDR_MINOR) return false;
      // look's ok
      return true;
    }
  };

  // the file descriptor
  struct s_desc {
    // file path
    String d_fpath;
    // file name
    String d_fname;
    // the file size
    t_long d_fsize;
    // the coding size
    t_long d_csize;
    // the librarian offset
    t_long d_lfoff;
    // file flags
    t_byte d_flags;
    // next file in list
    s_desc* p_next;
    // create an empty descriptor
    s_desc (void) {
      d_fsize = 0;
      d_lfoff = 0;
      d_flags = nilc;
      p_next  = nullptr;
    }
    // create a new descriptor by name, file size and computed size
    s_desc (const String& fpath, const t_long fsize, const t_long csize) {
      d_fpath = fpath;
      d_fname = System::xname (d_fpath);
      d_fsize = fsize;
      d_csize = csize;
      d_lfoff = 0;
      d_flags = nilc;
      p_next  = nullptr;
    }
    // delete a chain of descriptors
    ~s_desc (void) {
      delete p_next;
    }
    // add a descriptor at the end
    void add (s_desc* desc) {
      if (desc == nullptr) return;
      s_desc* last = this;
      while (last->p_next != nullptr) last = last->p_next;
      last->p_next = desc;
    }
    // return the serialized length
    long length (void) {
      long result = d_fname.length () + 1;
      result     += 16 + 1;
      return result;
    }
    // serialize this descriptor
    void wrstream (OutputStream& os) {
      Integer fsize = d_fsize;
      Integer csize = d_csize;
      Byte    flags = d_flags;
      d_fname.wrstream (os);
      fsize.wrstream   (os);
      csize.wrstream   (os);
      flags.wrstream   (os);
    }
    // deserialize this descriptor
    void rdstream (InputStream& is) {
      Integer fsize;
      Integer csize;
      Byte    flags;
      d_fname.rdstream (is);
      fsize.rdstream   (is);
      csize.rdstream   (is);
      flags.rdstream   (is);
      d_fpath = d_fname;
      d_fsize = fsize.tolong ();
      d_csize = csize.tolong ();
      d_flags = flags.tobyte    ();
    }
    // return true if a flag is set
    bool chkflg (const t_byte flag) const {
      return (d_flags & flag) == flag;
    }
    // format the flags into a string
    String fmtflg (void) const {
      String result = '[';
      // format the string
      for (long i = 0; i < AXL_FMT_SIZ; i++) {
	if (chkflg (0x01 << i) == true) {
	  result = result + (char) AXL_FMT_MRK[i];
	} else {
	  result = result + (char) AXL_DEF_MRK;
	}
      }
      result += ']';
      return result;
    }
    // format the file size into a string
    String fmtsiz (void) const {
      Integer ival (d_fsize);
      String result = ival.tostring ();
      return result.lfill (' ', 10);
    }
    // format a descriptor to an output stream
    void format (OutputStream& os) {
      os << fmtflg () << ' ' << fmtsiz () << ' ' << d_fname << eolc;
    }
  };

  // this procedure compute the size of descritpor chain
  static t_long get_chain_length (s_desc* desc) {
    t_long result = 0;
    while (desc != nullptr) {
      result += desc->length ();
      desc = desc->p_next;
    }
    return result;
  }

  // this procedure finds a descriptor by name
  static s_desc* get_named_desc (s_desc* desc, const String& name) {
    while (desc != nullptr) {
      if (desc->d_fname == name) return desc;
      desc = desc->p_next;
    }
    return nullptr;
  }

  // write the header on the output stream
  static void write_header (OutputStream& os, s_desc* desc) {
    // get the librarian header
    s_lhead lhead (get_chain_length (desc));
    // write the librarian header
    os.write ((char*) &lhead, sizeof (lhead));
    // serialize the chain
    while (desc != nullptr) {
      desc->wrstream (os);
      desc = desc->p_next;
    }
  }

  // read the header from an input stream
  static s_desc* read_header (const String& lname) {
    InputFile is (lname);
    // read the librarian header
    s_lhead lhead;
    Buffer* buf = is.InputStream::read (sizeof (lhead));
    if (buf->tomap (&lhead, sizeof (lhead)) != sizeof (lhead)) {
      delete buf;
      throw Exception ("librarian-error", "cannot read header");
    }
    delete buf;
    // check the header 
    if (lhead.check () == false)
      throw Exception ("librarian-error", "invalid librarian header");
    // check the input size
    t_long hsize = System::oswap (lhead.d_hsize);
    t_long lfoff = hsize + sizeof (s_lhead);
    if (hsize == 0) return nullptr;
    // prepare for reading
    s_desc* result = nullptr;
    s_desc* last   = nullptr;
    // read until the size is null
    while (hsize != 0) {
      // read in one descriptor
      s_desc* desc = new s_desc;
      desc->rdstream (is);
      // update the file offset
      desc->d_lfoff = lfoff;
      lfoff += desc->d_csize;
      // update result and size
      if (last == nullptr) {
	result = desc;
	last   = desc;
      } else {
	last->p_next = desc;
	last = desc;
      }
      hsize -= desc->length ();
      if (hsize < 0) {
	delete result;
	throw Exception ("librarian-error", "cannot read file descriptors");
      }
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return true if the path is a valid librarian file

  bool Librarian::valid (const String& path) {
    s_desc* desc = nullptr;
    try {
      // check for nil
      if (path.isnil () == true) return false;
      // attempt to read the header
      desc = read_header (path);
      delete desc;
      // success
      return true;
    } catch (...) {
      delete desc;
      return false;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty librarian

  Librarian::Librarian (void) {
    d_mode = false;
    p_desc = nullptr;
  }

  // create a librarian by name
  
  Librarian::Librarian (const String& lname) {
    d_mode = true;
    d_name = lname;
    p_desc = read_header (lname);
  }

  // destroy this librarian

  Librarian::~Librarian (void) {
    delete p_desc;
  }

  // return the class name

  String Librarian::repr (void) const {
    return "Librarian";
  }

  // return the librarian name

  String Librarian::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the expected file flag in the librarian 
  
  t_byte Librarian::mapflag (const t_byte flags) const {
    wrlock ();
    try {
      t_byte result = flags;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the expected file size in the librarian 
  
  t_long Librarian::mapsize (const t_long size) const {
    wrlock ();
    try {
      t_long result = size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an input file by name in the librarian
  InputStream* Librarian::mapfile (const String& name) const {
    rdlock ();
    try {
      InputStream* result = new InputFile (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new file descriptor to the chain
  
  void Librarian::add (const String& path) {
    wrlock ();
    try {
      // check for type - must be false for output mode
      if (d_mode == true) {
	throw Exception ("librarian-error", "cannot add file to librarian");
      }
      // check for a file first
      InputFile is (path);
      if (is.length () == 0) {
	unlock ();
	return;
      }
      // normalize the size if we have a cipher
      t_long fsize = is.length ();
      t_long csize = mapsize (fsize);
      // add the descriptor and update the flags
      s_desc* desc = new s_desc (path, fsize, csize);
      desc->d_flags = mapflag (desc->d_flags);
      if (p_desc == nullptr) {
	p_desc = desc;
      } else {
	p_desc->add (desc);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of files in the librarian

  long Librarian::length (void) const {
    rdlock ();
    try {
      long result   = 0;
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	result++;
	desc = desc->p_next;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the name exists in this librarian

  bool Librarian::exists (const String& name) const {
    rdlock ();
    try {
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	if (desc->d_fname == name) {
	  unlock ();
	  return true;
	}
	desc = desc->p_next;
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a list of file in this librarian

  Strvec Librarian::getlist (void) const {
    rdlock ();
    try {
      Strvec result;
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	result.add (desc->d_fname);
	desc = desc->p_next;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a vector of file in this librarian

  Vector* Librarian::getstr (void) const {
    rdlock ();
    try {
      Vector* result = new Vector;
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	result->add (new String (desc->d_fname));
	desc = desc->p_next;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // extract a file by name

  InputStream* Librarian::extract (const String& name) const {
    rdlock ();
    try {
      // check the mode - must be true for input mode
      if (d_mode == false) {
	throw Exception ("librarian-error", "cannot extract from librarian");
      }
      // get the descriptor by name
      s_desc* desc = get_named_desc (p_desc, name);
      if (desc == nullptr) {
	unlock ();
	throw Exception ("extract-error", "cannot extract file", name);
      }
      // get the mapped file
      t_long size = desc->d_csize;
      t_long foff = desc->d_lfoff;
      // map the result and return
      InputStream* result = new InputMapped (d_name, size, foff);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the librarian to an output file

  void Librarian::write (const String& lname) const {
    rdlock ();
    try {
      // create the output file
      OutputFile os (lname);
      // write the header
      write_header (os, p_desc);
      // write all file sequentialy
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	InputStream* is = mapfile (desc->d_fpath);
	if (is == nullptr) {
	  throw Exception ("librarian-error", "cannot map input file stream");
	}
	while (is->valid () == true) os.write (is->read ());
	delete is;
	desc = desc->p_next;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the librarian content to an output stream

  void Librarian::format (OutputStream& os) const {
    rdlock ();
    try {
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	desc->format (os);
	desc = desc->p_next;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the start module is set

  bool Librarian::isstm (void) const {
    rdlock ();
    try {
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	// clean the stm bit
	if ((desc->d_flags & AXL_STM_BIT) == AXL_STM_BIT) {
	  unlock ();
	  return true;
	}
	// next record
	desc = desc->p_next;
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the start module bit by name

  void Librarian::setstm (const String& name) {
    wrlock ();
    try {
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	// clean the stm bit
	desc->d_flags &= ~AXL_STM_BIT;
	// check for valid name
	if (desc->d_fname == name) desc->d_flags |= AXL_STM_BIT;
	// next record
	desc = desc->p_next;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the start module name

  String Librarian::getstm (void) const {
    rdlock ();
    try {
      s_desc* desc = p_desc;
      while (desc != nullptr) {
	// clean the stm bit
	if ((desc->d_flags & AXL_STM_BIT) == AXL_STM_BIT) {
	  String result = desc->d_fname;
	  unlock ();
	  return result;
	}
	// next record
	desc = desc->p_next;
      }
      throw Exception ("librarian-error", "cannot find start module");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the librarian supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETVEC  = zone.intern ("get-names");
  static const long QUARK_EXISTS  = zone.intern ("exists-p");
  static const long QUARK_EXTRACT = zone.intern ("extract");

  // create a new object in a generic way

  Object* Librarian::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Librarian;
    // check for 1 argument
    if (argc == 1) {
      String fname = argv->getstring (0);
      return new Librarian (fname);
    }
    throw Exception ("argument-error", 
		     "invalid number of argument with librarian");
  }

  // return true if the given quark is defined

  bool Librarian::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply a librarian method with a set of arguments and a quark
  
  Object* Librarian::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_GETVEC) return getstr ();
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTS) {
	String fname = argv->getstring (0);
	return new Boolean (exists (fname));
      }
      if (quark == QUARK_ADD) {
	String fname = argv->getstring (0);
	add (fname);
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	String fname = argv->getstring (0);
	write (fname);
	return nullptr;
      }
      if (quark == QUARK_EXTRACT) {
	String fname = argv->getstring (0);
	Object* result = extract (fname);
	zobj->post (result);
	return result;
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
