// ---------------------------------------------------------------------------
// - Libsio.cpp                                                              -
// - afnix:sio module - declaration & implementation                         -
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

#include "Meta.hpp"
#include "Libsio.hpp"
#include "Logtee.hpp"
#include "Predsio.hpp"
#include "Function.hpp"
#include "SioCalls.hpp"
#include "Pathname.hpp"
#include "Pathlist.hpp"
#include "FileInfo.hpp"
#include "Selector.hpp"
#include "Terminal.hpp"
#include "Intercom.hpp"
#include "NamedFifo.hpp"
#include "Directory.hpp"
#include "InputFile.hpp"
#include "OutputFile.hpp"
#include "InputMapped.hpp"
#include "InputString.hpp"
#include "InputOutput.hpp"
#include "OutputString.hpp"
#include "OutputBuffer.hpp"

namespace afnix {

  // initialize the afnix:sio module

  Object* init_afnix_sio (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:sio nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* nset = aset->mknset   ("sio");

    // bind all symbols in the afnix:sio nameset
    nset->symcst ("Logtee",              new Meta (Logtee::mknew));
    nset->symcst ("Pathname",            new Meta (Pathname::mknew));
    nset->symcst ("Pathlist",            new Meta (Pathlist::mknew));
    nset->symcst ("FileInfo",            new Meta (FileInfo::mknew));
    nset->symcst ("Selector",            new Meta (Selector::mknew));
    nset->symcst ("Terminal",            new Meta (Terminal::mknew));
    nset->symcst ("Intercom",            new Meta (Intercom::mknew));
    nset->symcst ("NamedFifo",           new Meta (NamedFifo::mknew));
    nset->symcst ("Directory",           new Meta (Directory::mknew));
    nset->symcst ("InputFile",           new Meta (InputFile::mknew));
    nset->symcst ("InputTerm",           new Meta (InputTerm::mknew));
    nset->symcst ("ErrorTerm",           new Meta (OutputTerm::mkerr));
    nset->symcst ("Transcoder",          new Meta (Transcoder::mknew));
    nset->symcst ("OutputFile",          new Meta (OutputFile::mknew));
    nset->symcst ("OutputTerm",          new Meta (OutputTerm::mkout));
    nset->symcst ("InputMapped",         new Meta (InputMapped::mknew));
    nset->symcst ("InputString",         new Meta (InputString::mknew));
    nset->symcst ("InputOutput",         new Meta (InputOutput::mknew));
    nset->symcst ("OutputString",        new Meta (OutputString::mknew));
    nset->symcst ("OutputBuffer",        new Meta (OutputBuffer::mknew));

    // bind the predicates
    nset->symcst ("dir-p",               new Function (sio_dirp));
    nset->symcst ("file-p",              new Function (sio_filep));
    nset->symcst ("stream-p",            new Function (sio_strmp));
    nset->symcst ("logtee-p",            new Function (sio_logtp));
    nset->symcst ("pathname-p",          new Function (sio_pathp));
    nset->symcst ("pathlist-p",          new Function (sio_plstp));
    nset->symcst ("selector-p",          new Function (sio_selectp));
    nset->symcst ("terminal-p",          new Function (sio_termp));
    nset->symcst ("intercom-p",          new Function (sio_icomp));
    nset->symcst ("directory-p",         new Function (sio_odirp));
    nset->symcst ("file-info-p",         new Function (sio_infop));
    nset->symcst ("named-fifo-p",        new Function (sio_fifop));
    nset->symcst ("input-file-p",        new Function (sio_ifilep));
    nset->symcst ("input-term-p",        new Function (sio_itermp));
    nset->symcst ("transcoder-p",        new Function (sio_trscp));
    nset->symcst ("output-file-p",       new Function (sio_ofilep));
    nset->symcst ("output-term-p",       new Function (sio_otermp));
    nset->symcst ("input-stream-p",      new Function (sio_inputp));
    nset->symcst ("input-mapped-p",      new Function (sio_inpmapp));
    nset->symcst ("input-string-p",      new Function (sio_inpstrp));
    nset->symcst ("input-output-p",      new Function (sio_inpoutp));
    nset->symcst ("output-stream-p",     new Function (sio_outputp));
    nset->symcst ("output-string-p",     new Function (sio_outstrp));
    nset->symcst ("output-buffer-p",     new Function (sio_outbufp));

    // bind other functions
    nset->symcst ("ident",               new Function (sio_ident));
    nset->symcst ("mkdir",               new Function (sio_mkdir));
    nset->symcst ("mhdir",               new Function (sio_mhdir));
    nset->symcst ("rmdir",               new Function (sio_rmdir));
    nset->symcst ("mvfile",              new Function (sio_mvfile));
    nset->symcst ("rmfile",              new Function (sio_rmfile));
    nset->symcst ("tmp-name",            new Function (sio_tmp_name));
    nset->symcst ("tmp-path",            new Function (sio_tmp_path));
    nset->symcst ("get-base-name",       new Function (sio_xname));
    nset->symcst ("get-base-path",       new Function (sio_xdir));
    nset->symcst ("get-extension",       new Function (sio_xext));
    nset->symcst ("absolute-path",       new Function (sio_abs_path));
    nset->symcst ("relative-path",       new Function (sio_rel_path));
    nset->symcst ("remove-extension",    new Function (sio_rmext));
    nset->symcst ("user-data-directory", new Function (sio_usr_ddir));
    
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_sio (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_sio (interp, argv);
  }
}
