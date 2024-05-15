#pragma once

#include "miniutil.h"
#include "wing.h"

#include "FileIterator.h"

namespace FileIteratorOptions {

void endsWithPrepare();

class cFileIteratorCase_Data {
public:
	u4 filesTotal;
	u4 filesRenamed;
	u4 filesIgnored;
};

void FileIteratorCase_OnStart(FileIterator::cFileIterator* fi);
void FileIteratorCase_OnFile(FileIterator::cFileIterator* fi);
void FileIteratorCase_OnEnd(FileIterator::cFileIterator* fi);
void FileIteratorCase_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc);

class cFileIteratorCodepage_Data {
public:
	u4 filesTotal;
	u4 filesUtf8;
	u4 filesNotUtf8;
	u4 filesIgnored;
	u4 filesConvertSuccessful;
	u4 filesConvertFail;
};

void FileIteratorCodepage_OnStart(FileIterator::cFileIterator* fi);
void FileIteratorCodepage_OnFile(FileIterator::cFileIterator* fi);
void FileIteratorCodepage_OnFile_ToShiftJIS(FileIterator::cFileIterator* fi);
void FileIteratorCodepage_OnEnd(FileIterator::cFileIterator* fi);
void FileIteratorCodepage_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc);

class cFileIteratorLineend_Data {
public:
	u4 filesTotal;
	u4 filesNotUtf8Bom;
	u4 filesRN;
	u4 filesN;
	u4 filesMixed;
	u4 filesNone;
};

void FileIteratorLineend_OnStart(FileIterator::cFileIterator* fi);
void FileIteratorLineend_OnFile(FileIterator::cFileIterator* fi);
void FileIteratorLineend_OnEnd(FileIterator::cFileIterator* fi);
void FileIteratorLineend_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc);

} //namespace