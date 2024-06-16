#ifndef GLOBAL_H
#define GLOBAL_H

#include "excelReader.h"

#ifdef EXPORT_GLOBAL

excelReader* reader;

#else

extern excelReader* reader;

#endif

#endif /* GLOBAL_H */
