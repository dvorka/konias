//----------------------------------------------------------------------------
//                                 Kcfg.h
//
//                                 Dvorka
//				                    2002
//----------------------------------------------------------------------------

#ifndef __KCFG_H
 #define __KCFG_H

 #include "ktv.h"


 /*
  * This class takes care of Konias configuration. It
  * searches for c:\.konias file. If it is not found,
  * default values are used.
  */
class KConfig {
    
  public:

    char *defaultLibraryName; // e.g. marinka.lib - UP TO 8 characters
    char *defaultLibraryDir; // e.g. c:\konias\libs - MUST EXIST (check)
    char *defaultBackupLocation; // e.g. a:\mylibrary.lib
    char *defaultConfigLocation; // e.b. c:\konias\konias.cfg
    
    boolean autoLoadLibrary; // load default library on startup
    boolean autoSave; // save library after each modification

    KConfig();
};

#endif
