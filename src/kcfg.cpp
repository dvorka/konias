#include "kcfg.h"
#include "ktv.h"

KConfig::KConfig() {
	defaultConfigLocation="C:\konias\.konias";
	defaultLibraryName="default.kon";
	defaultLibraryDir="C:\konias\libraries";
	defaultBackupLocation="C:\konias\backup.lib";

	// now try to search for the configuration file and the library
	// TBD

	// if config not found, try to load default library
	autoLoadLibrary=TRUE;
}

//- eof ----------------------------------------------------------------------
