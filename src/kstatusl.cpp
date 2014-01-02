#include "kstatusl.h"

const char* KStatusLine::hint(ushort aHelpCtx) {
	switch(aHelpCtx) {
				case hcLibraryRestore:
					return "Obnoven� knihovny ze z�lo�n� diskety...";
				case hcLibraryLoad:
					return "Otev�en� knihovny...";
				case hcLibraryStore:
					return "Ulo�en� knihovny...";
				case hcLibraryLoadAs:
					return "Otev�en� knihovny podle jm�na...";
				case hcLibraryStoreAs:
					return "Ulo�en� knihovny pod jm�nem...";
				case hcLibraryClose:
					return "Zav�en� aktu�ln� knihovny...";
				case hcLibraryInfo:
					return "Profil aktu�ln� knihovny...";
				case hcLibraryBackup:
					return "Z�lohov�n� knihovny na disketu...";
				case hcAboutKonias:
				case hcMenuKonias:
					return "O Koni��ovi...";
				case hcMenuLibrary:
					return "Spr�va knihoven - otev�en�, zav�en�, ulo�en�, ...";
				case hcChangeVideoMode:
					return "Zm�na rozli�en� - 25 nebo 43/50 ��dek)";
				case hcMenuBook:
					return "Spr�va kn��ek";
				case hcNewBook:
					return "Registrace nov� kn��ky...";
				case hcSearchBook:
					return "Vyp�j�en�, hled�n�, editace, smaz�n� kn��ky...";
				case hcMenuReaders:
					return "Spr�va �ten���";
				case hcNewReader:
					return "Registrace nov�ho �ten��e...";
				case hcSearchReader:
					return "Hled�n�, editace, smaz�n� �ten��e...";
				case hcMenuLoans:
					return "Spr�va v�p�j�ek";
				case hcNewLoanToReader:
					return "Nov� v�p�j�ka �ten��i...";
				case hcNewLoanOfBook:
					return "Zaznamen�n� v�p�j�ky kn��ky...";
				case hcSearchLoans:
					return "Hled�n�, editace, smaz�n� v�p�j�ky...";
				case hcMenuCd:
					return "Spr�va CD�ek";
				case hcNewCd:
					return "Registrace nov�ho CD�ka...";
				case hcSearchCd:
					return "Vyp�j�en�, hled�n�, editace, smaz�n� CD�ka...";
				case hcExitProgram:
					return "Ukon�en� Koni��e...";
				default:
					return "Koni��";
	}
}

//- eof ----------------------------------------------------------------------
