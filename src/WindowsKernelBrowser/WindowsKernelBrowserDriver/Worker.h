#pragma once
#include"IDT.h"
#include "GDT.h"

VOID QueryCurrentIDTR(PIDTRegister pidtr);
VOID QueryCurrentGDTR(PGDTRegister pgdtr);

