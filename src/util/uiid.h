#pragma once

#include "miniutil.h"

namespace uiid {

enum uiStringsId {
	uiidSpace,
	uiidSelect,
	uiidBracketsStart,
	uiidBracketsEnd,
	uiidArrowDown,
	uiidLanguageChoice,
	uiidPath,
	uiidPathLog,
	uiidFixWhat,
	uiidFixFileExtensionCase,
	uiidFixFileCodepage,
	uiidFixLineEnds,
	uiidInspect,
	uiidChange,
	uiidHelp,
	uiidBack,

	uiidMainHint0,
	uiidMainHint00,

	uiidMainHint1,

	uiidMainHint4,
	uiidMainHint40,

	uiidMainHint5,

	uiidHelp0,
	uiidHelp1,
	uiidHelp2,
	uiidHelp3,
	uiidHelp4,
	uiidHelpUrl,

	uiidLogFilelistStart,
	uiidLogFilelistEnd,
	uiidLogFilesTotal,
	uiidLogFilesRenamed,
	uiidLogFilesToRename,
	uiidLogFilesIgnored,
	uiidLogFileRenamed,
	uiidLogNeedsRenaming,

	uiidDone,
	uiidError,
	uiidDoneNoOutput,
	uiidErrorNoOutput,
	uiidErrorFileTooBig,
	uiidErrorMemTooSmall,
	uiidErrorLogfileMemory,
	uiidErrorFolderDoesntExist,
	uiidErrorRnotfollowedbyN,

	uiidLogFileIgnored,
	uiidLogNotUtf8Bom,
	uiidLogConvertedSuccessfully,
	uiidLogConversionFailed,
	uiidLogFilesUtf8,
	uiidLogFilesNotUtf8,
	uiidLogFilesConvertSuccessful,
	uiidLogFilesConvertFail,

	uiidLineendRN,
	uiidLineendN,
	uiidLineendMixed,
	uiidLineendNone,

	uiidFixHow,
	uiidOption0RNtoN,
	uiidOption0NtoRN,

	uiidCodepageToUTF8BOM,
	uiidCodepageToShiftJIS,

	uiidFileExtensions,

	uiidTotalLength,
};

extern const wchar* uiStringsEN[uiidTotalLength];
extern const wchar* uiStringsJP[uiidTotalLength];

extern const wchar* * uiStrings;

extern const wchar* * uiLangs[uiidTotalLength];

//extern u2 uiStringsLenEn[uiidTotalLength];

extern u2 uiStringsLen[];

enum uiLangStringsId {
	uiLangIdJp,
	uiLangIdEn,
	uiLangIdRu,

	uiLangIdTotalLength,
};

extern const wchar* uiLangStrings[uiidTotalLength];

extern u2 uiLangStringsLen[uiLangIdTotalLength];

void uiidInit();
void uiidDraw(u2 id);
void uiidDrawOption(bool active);

}

