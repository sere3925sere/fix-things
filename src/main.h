#pragma once

#include "util/miniutil.h"

class Menu {
public:
	u4 mode;
	i4 sel0;
	i4 sel1;
	i4 selprev;
	i4 selMax;
	u4 language;
	u4 fixWhat;
	u4 option0;
	bool convert;
	wchar* path;
	wchar* pathLog;
	wchar* fileExtensions;
	u4 fileExtensionsLen;
	u4 fileExtensionsCap;
};

extern Menu gMenu;