@echo off
move /y "..\rorfile.h" "rorfile.h.bak"
move /y "..\scenario.ror" "scenario.ror.bak"
copy "rorfile.h" "..\rorfile.h"
copy "scenario.ror" "..\scenario.ror"