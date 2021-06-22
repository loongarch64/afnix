// ---------------------------------------------------------------------------
// - t_logger.cpp                                                            -
// - standard object library - message logger class tester                   -
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

#include "Logger.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a message logger
  #ifdef AFNIX_DEBUG
  static const long AFNIX_LOGR_SIZE = 5;
  #else
  static const long AFNIX_LOGR_SIZE = 4;
  #endif
  Logger* log = new Logger (AFNIX_LOGR_SIZE);
  
  // check default
  if (log->length  () != 0) return 1;
  // add a message and check
  if (log->add (Logger::MLVL_FATL, "fatal") == -1L) return 1;
  if (log->length () != 1) return 1;
  if (log->add (Logger::MLVL_XERR, "error") == -1L) return 1;
  if (log->length () != 2) return 1;
  if (log->add (Logger::MLVL_WRNG, "warning") == -1L) return 1;
  if (log->length () != 3) return 1;
  if (log->add (Logger::MLVL_INFO, "info") == -1L) return 1;
  if (log->length () != 4) return 1;
  #ifdef AFNIX_DEBUG
  if (log->add (Logger::MLVL_DBUG, "debug") == -1L) return 1;
  if (log->length () != 5) return 1;
  #else
  if (log->add (Logger::MLVL_DBUG, "debug") != -1L) return 1;
  #endif
  // check the log level
  if (log->getmlvl (0) != Logger::MLVL_FATL) return 1;
  if (log->getmlvl (1) != Logger::MLVL_XERR) return 1;
  if (log->getmlvl (2) != Logger::MLVL_WRNG) return 1;
  if (log->getmlvl (3) != Logger::MLVL_INFO) return 1;
  #ifdef AFNIX_DEBUG
  if (log->getmlvl (4) != Logger::MLVL_DBUG) return 1;
  #endif
  // check the messages
  if (log->getmesg (0) != "fatal")   return 1;
  if (log->getmesg (1) != "error")   return 1;
  if (log->getmesg (2) != "warning") return 1;
  if (log->getmesg (3) != "info")    return 1;
  #ifdef AFNIX_DEBUG  
  if (log->getmesg (4) != "debug")   return 1;
  #endif

  // add new message for circulation
  log->add (Logger::MLVL_INFO, "afnix");
  if (log->length () != AFNIX_LOGR_SIZE) return 1;
  // check at index 0
  if (log->getmlvl (0) != Logger::MLVL_XERR)         return 1;
  if (log->getmesg (0) != "error") return 1;
    
  // delete everything
  delete log;
  return 0;
}
