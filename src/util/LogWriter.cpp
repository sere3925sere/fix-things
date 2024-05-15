#include "LogWriter.h"
#include "FileIterator.h"
//#include "../main.h"
#include "uiid.h"
#include "MemorySimple.h"
#include "ufile.h"

namespace LogWriter {

void writerWriteError(cLogWriter* w) {
	if (w->fi->error) return;
	const wchar* what = uiid::uiStrings[uiid::uiidErrorLogfileMemory];
	u4 len = uiid::uiStringsLen[uiid::uiidErrorLogfileMemory];
	const wchar* curfrom = what;
	wchar* curto = w->cur;
	do {
		*curto++ = *curfrom++;
	} while (*curfrom != 0);
	*curto++ = L'\r';
	*curto++ = L'\n';
	w->cur = curto;
	w->fi->error = true;
}

void writerWrite(cLogWriter* w, const wchar* what, u4 len) {
	if (w->fi->error) return;
	//if (bytep(w->cur) + len*2 + 6 > bytep(w->fileContents) + w->allocated) THROW();
	if (bytep(w->cur) + 1024 > bytep(w->fileContents) + w->allocated) {
		if (mem::memgetChunk()) {
			writerWriteError(w);
			return;
		} else {
			w->allocated += gmem0.sizeGranularity;
		}
	}
	//need +MAX_PATH or max uiidString length, + "\r\n", + trailing zero.

	const wchar* curfrom = what;
	wchar* curto = w->cur;
	do {
		*curto++ = *curfrom++;
	} while (*curfrom != 0);
	*curto++ = L'\r';
	*curto++ = L'\n';
	w->cur = curto;
}

void writerWrite(cLogWriter* w, u4 uiid) {
	const wchar* what = uiid::uiStrings[uiid];
	u4 len = uiid::uiStringsLen[uiid];
	writerWrite(w, what, len);
}

void writerWriteNumber(cLogWriter* w, u4 number) {
	wchar tempbuf[9];
	u4 strlen;
	wchar_t* numstr;
	numstr = ustr::NumberuToWstringSafe(number, tempbuf, 9, &strlen);
	writerWrite(w, numstr, strlen);
}

void writerWriteFilename(cLogWriter* w, FileIterator::cFileIterator* fi) {
	wchar* what = fi->g_filepath;
	//u4 len = fi->cur - fi->g_filepath;
	//u4 len = wcslen(fi->g_filepath);
	u4 len = ustr::wcharLen(fi->g_filepath);
	writerWrite(w, what, len);
}

void writerInit(cLogWriter* w, FileIterator::cFileIterator* fi) {
	//w->allocated = gmem3.sizeGranularity;
	bytep used = gmem0.used;
	mem::memgetChunk();
	w->fileContents = (wchar_t*)used;
	gmem0.used = gmem0.allocated;
	w->allocated = gmem0.allocated - used;
	//w->fileContents = (wchar_t*)memget(w->allocated);
	w->cur = w->fileContents;
	w->fi = fi;

}

errorCode writerWriteFile(cLogWriter* w, const wchar* path) {
	errorCode e = ufile::FileWrite(path, bytep(w->fileContents), bytep(w->cur) - bytep(w->fileContents));
	return e;
}

} //namespace