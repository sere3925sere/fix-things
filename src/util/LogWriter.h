#pragma once

#include "miniutil.h"
#include "FileIterator.h"

//forward declarations
namespace FileIterator {
class cFileIterator;
}
//forward declarations end

namespace LogWriter {

class cLogWriter {
public:
	wchar_t* cur;
	wchar_t* fileContents;
	u4 allocated;
	FileIterator::cFileIterator* fi;
};

void writerWrite(cLogWriter* w, u4 uiid);

void writerWriteNumber(cLogWriter* w, u4 number);

void writerWriteFilename(cLogWriter* w, FileIterator::cFileIterator* fi);

void writerWrite(cLogWriter* w, const wchar* what, u4 len);

void writerInit(cLogWriter* w, FileIterator::cFileIterator* fi);

errorCode writerWriteFile(cLogWriter* w, const wchar* path);

} //namespace