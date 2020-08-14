#include "Worker.h"

extern VOID QueryCurrentIDTRAsm(PIDTRegister pidtr);
extern VOID QueryCurrentGDTRAsm(PGDTRegister pgdtr);

VOID QueryCurrentIDTR(PIDTRegister pidtr)
{
	QueryCurrentIDTRAsm(pidtr);
}
VOID QueryCurrentGDTR(PGDTRegister pgdtr)
{
	QueryCurrentGDTRAsm(pgdtr);
}