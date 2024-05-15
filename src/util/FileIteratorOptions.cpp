
#include "FileIteratorOptions.h"
#include "FileIterator.h"
#include "LogWriter.h"
#include "MemorySimple.h"
#include "../main.h"
#include "ufile.h"
#include "ui.h"
#include "miniutil.h"
#include "uiid.h"

using namespace LogWriter;

namespace FileIteratorOptions {

void endsWithPrepare() {
	wchar* cur = gMenu.fileExtensions;
	wchar* out = cur + gMenu.fileExtensionsLen;
	*out++ = L' ';
	*out++ = 0;
	gMenu.fileExtensionsLen++;
	
	while (true) {
		u2 len = 0;
		while (true) {
			if (*cur == L' ') {
				*out++ = len;
				cur++;
				break;
			}
			cur++;
			len++;
		}
		if (*cur == 0) {
			*out = 0;
			break;
		}
	}
}

//TODO: I probably can get filenameLen from fi->cur or something.
bool endsWithSpaceSeparatedList(const wchar* filename, u4 filenameLen) {
	u2* len = (u2*)gMenu.fileExtensions + gMenu.fileExtensionsLen + 1;
	wchar* extStart = gMenu.fileExtensions;
	while (true) {
		const wchar* ext = filename + filenameLen - *len;
		wchar* ext2 = extStart;
		while(true) {
			if (*ext != *ext2) break;
			ext++;
			ext2++;
			if (*ext == 0) return true;
		}
		extStart += *len + 1;
		len++;
		if (*len == 0) return false;
	}
}

//returns true if needs renaming
bool endsWithListCheckCase(const wchar* filename, u4 filenameLen, wchar** outExtension, u4* outExtensionLen) {
	if (endsWithSpaceSeparatedList(filename, filenameLen)) return false;
	u2* len = (u2*)gMenu.fileExtensions + gMenu.fileExtensionsLen + 1;
	wchar* extStart = gMenu.fileExtensions;
	while (true) {
		const wchar* ext = filename + filenameLen - *len;
		wchar* ext2 = extStart;
		while(true) {
			if ((*ext | 32) != (*ext2 | 32)) break;
			ext++;
			ext2++;
			if (*ext == 0) {
				*outExtension = extStart;
				*outExtensionLen = *len;
				return true;
			}
		}
		extStart += *len + 1;
		len++;
		if (*len == 0) return false;
	}
}

// Returns false if successful.
bool FileConvertToUTF8BOM(FileIterator::cFileIterator* fi) {
	const wchar* filename = fi->g_filepath;
	u4 filesize = fi->findFileData.nFileSizeLow;

	u4 space_needed = fi->filebuf1size;

	bytep fileread = fi->filebuf1;
	bytep filewrite = fi->filebuf2;
	//This function converts fileread into filewrite,
	//and filewrite back into fileread.

	ufile::cFile file;
	errorCode err;
	err = file.OpenForReadWrite(filename);
	if (err) ErrorLazy();
	
	if (filesize == 0) {
		//not sure if a good idea, but just for consistency
		bytep cur = (bytep)fileread;
		*cur++ = 0xEF;
		*cur++ = 0xBB;
		*cur++ = 0xBF;

		file.Write(fileread, 3, &err);
		if (err) ErrorLazy();
		file.Trim();
		file.Close();
		return false;
	}

	u4 fileread_size = file.Read(fileread, 4, &err);
	if (err) ErrorLazy();

    if (fileread_size < 3) goto not_utf8_bom;
	{
		bytep cur = fileread;
		if (*cur++ != 0xEF) goto not_utf8_bom;
		if (*cur++ != 0xBB) goto not_utf8_bom;
		if (*cur++ != 0xBF) goto not_utf8_bom;
	}

	file.Close();
	return false;

not_utf8_bom:

	//read the rest of the file
	{
		i4 filesize_left = filesize - 4;
		if (filesize_left < 0) filesize_left = 0;
		if (filesize_left > 0) {
			u4 fileread_size2 = file.Read(fileread+4, filesize-4, &err);
			if (err) ErrorLazy();
			fileread_size += fileread_size2;
		}
	}

	int res_mbtwc = win::MultiByteToWideChar(932, win::MB_ERR_INVALID_CHARS, (char*)fileread, fileread_size, (wchar*)filewrite, space_needed / 2);
	if (res_mbtwc == 0) {
		res_mbtwc = win::MultiByteToWideChar(win::CP_UTF8, win::MB_ERR_INVALID_CHARS, (char*)fileread, fileread_size, (wchar*)filewrite, space_needed / 2);
		if (res_mbtwc == 0) {
			file.Close();
			return true;
		}
	}

	bytep cur = (bytep)fileread;
	*cur++ = 0xEF;
	*cur++ = 0xBB;
	*cur++ = 0xBF;

	// win::WC_ERR_INVALID_CHARS isn't as necessary here, since it's UTF8.
	int res_wctmb = win::WideCharToMultiByte(win::CP_UTF8, win::WC_ERR_INVALID_CHARS, (wchar*)filewrite, res_mbtwc, (char*)cur, space_needed, 0, 0);
	//not space_needed / 2, here it is a byte length, above it was wchar_t length
	if (res_wctmb == 0) {
		file.Close();
		return true;
	}

	{
		file.GoToStart();
		u4 filesize = res_wctmb + 3;
		file.Write(fileread, filesize, &err);
		if (err) ErrorLazy();
		file.Trim();
		file.SetTime(&fi->findFileData.ftCreationTime, &fi->findFileData.ftLastAccessTime, &fi->findFileData.ftLastWriteTime);
		file.Close();
	}

	return false;
}

//only called for UTF-8 BOM files.
bool FileConvertToShift_JIS(FileIterator::cFileIterator* fi) {
	const wchar* filename = fi->g_filepath;
	u4 filesize = fi->findFileData.nFileSizeLow;

	u4 space_needed = fi->filebuf1size;

	bytep fileread = fi->filebuf1;
	bytep filewrite = fi->filebuf2;

	ufile::cFile file;
	errorCode err;
	err = file.OpenForReadWrite(filename);
	if (err) ErrorLazy();
	
	if (filesize < 3) {
		//paranoia
		Error(0x6E);
	}

	u4 fileread_size = file.Read(fileread, 4, &err);
	if (err) ErrorLazy();

    if (fileread_size < 3) goto not_utf8_bom;
	{
		bytep cur = fileread;
		if (*cur++ != 0xEF) goto not_utf8_bom;
		if (*cur++ != 0xBB) goto not_utf8_bom;
		if (*cur++ != 0xBF) goto not_utf8_bom;
	}

	if (false) {
not_utf8_bom:
		//paranoia
		Error(0x6F);
	}

	if (filesize == 3) {
		file.GoToStart();
		goto labelTrimAndEnd;
	}

	//read the rest of the file
	{
		i4 filesize_left = filesize - 4;
		if (filesize_left < 0) filesize_left = 0;
		if (filesize_left > 0) {
			u4 fileread_size2 = file.Read(fileread+4, filesize-4, &err);
			if (err) ErrorLazy();
			fileread_size += fileread_size2;
		}
	}

	int res_mbtwc = win::MultiByteToWideChar(win::CP_UTF8, win::MB_ERR_INVALID_CHARS, ((char*)fileread) + 3, fileread_size - 3, (wchar*)filewrite, space_needed / 2);
	if (res_mbtwc == 0) {
		file.Close();
		return true;
	}

	u4 usedDefaultChar;
	//just win::WC_NO_BEST_FIT_CHARS doesn't workfor some reason
	//, gotta use usedDefaultChar
	int res_wctmb = win::WideCharToMultiByte(932, win::WC_NO_BEST_FIT_CHARS, (wchar*)filewrite, res_mbtwc, (char*)fileread, space_needed, 0, &usedDefaultChar);
	//not space_needed / 2, here it is a byte length, above it was wchar_t length
	if (res_wctmb == 0 || usedDefaultChar) {
		file.Close();
		return true;
	}

	{
		file.GoToStart();
		u4 filesize = res_wctmb;
		file.Write(fileread, filesize, &err);
		if (err) ErrorLazy();
labelTrimAndEnd:
		file.Trim();
		file.SetTime(&fi->findFileData.ftCreationTime, &fi->findFileData.ftLastAccessTime, &fi->findFileData.ftLastWriteTime);
		file.Close();
	}

	return false;
}

bool FileIsUTF8BOM(const wchar* filepath, u4 filesize) {
	if (filesize < 3) return false;
	ufile::cFile file;
	errorCode e;
	bool res = file.OpenForRead(filepath);
	if (res) ErrorLazy();

	u4 fileContents[1];

	file.Read((bytep)fileContents, 3, &e);
	if (e) ErrorLazy();

	file.Close();

	return (fileContents[0] & 0x00FFFFFF) == 0x00BFBBEF;
}

void FileIteratorCase_OnStart(FileIterator::cFileIterator* fi) {
	cFileIteratorCase_Data* data = (cFileIteratorCase_Data*)fi->fio->data;

	//probably not needed
	data->filesTotal = 0;
	data->filesRenamed = 0;
	data->filesIgnored = 0;

	fi->w = (cLogWriter*)mem::Get(sizeof(cLogWriter));
	writerInit(fi->w, fi);
	*fi->w->cur++ = 0xFEFF; //BOM
	writerWrite(fi->w, uiid::uiidLogFilelistStart);
}

void ChangeExtension(FileIterator::cFileIterator* fi, const wchar* newExt, u4 newExtLen) {
	if (gMenu.convert == false) {
		writerWrite(fi->w, uiid::uiidLogNeedsRenaming);
		writerWriteFilename(fi->w, fi);
		return;
	}
	wchar_t pathNew[win::file::MAX_PATH];
	ustr::wcharCopy(fi->g_filepath, pathNew);
	wchar* cur = pathNew;
	do {cur++;} while (*cur);
	cur -= newExtLen;
	const wchar* curIn = newExt;
	do {
		*cur++ = *curIn++;
	} while(*cur);
	win::file::MoveFileW(fi->g_filepath, pathNew);
	writerWrite(fi->w, uiid::uiidLogFileRenamed);
	writerWriteFilename(fi->w, fi);
}

void FileIteratorCase_OnFile(FileIterator::cFileIterator* fi) {
	cFileIteratorCase_Data* data = (cFileIteratorCase_Data*)fi->fio->data;

	data->filesTotal++;
	
	wchar* filename = fi->findFileData.cFileName;
	u4 filenameLen = ustr::wcharLen(filename);
	wchar* ext;
	u4 extLen;
	bool needRenaming = endsWithListCheckCase(filename, filenameLen, &ext, &extLen);
	if (!needRenaming) {
		data->filesIgnored++;
	} else {
		data->filesRenamed++;
		ChangeExtension(fi, ext, extLen);
	}
}

void FileIteratorCase_OnEnd(FileIterator::cFileIterator* fi) {
	cFileIteratorCase_Data* data = (cFileIteratorCase_Data*)fi->fio->data;
	writerWrite(fi->w, uiid::uiidLogFilelistEnd);

	if (gMenu.convert) {
		writerWrite(fi->w, uiid::uiidLogFilesRenamed);
	} else {
		writerWrite(fi->w, uiid::uiidLogFilesToRename);
	}
	writerWriteNumber(fi->w, data->filesRenamed);

	writerWrite(fi->w, uiid::uiidLogFilesIgnored);
	writerWriteNumber(fi->w, data->filesIgnored);

	writerWrite(fi->w, uiid::uiidLogFilesTotal);
	writerWriteNumber(fi->w, data->filesTotal);

	u4 len = ustr::wcharLen(gMenu.pathLog);
	wchar* cur = gMenu.pathLog + len - 1;
	errorCode err = 0;
	if (*cur != L'/' && *cur != L'\\')
	{
		err = ustr::wcharAttachSafe(L"/", gMenu.pathLog, win::file::MAX_PATH);
	}
	if (!err) err = ustr::wcharAttachSafe(L"fix-things-out.txt", gMenu.pathLog, win::file::MAX_PATH);
	if (!err) err = writerWriteFile(fi->w, gMenu.pathLog);
	fi->done = true;
	fi->logFileWritten = !err;
	wing::InvalidateRect(gui.hWnd, null, null);
}

void FileIteratorCase_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc) {
	cFileIteratorCase_Data* data = (cFileIteratorCase_Data*)fi->fio->data;
	if (!ahdc) {
		u4 ticknext = win::GetTickCount();
		if (i4(ticknext - fi->tick) < 0) return;
		fi->tick = ticknext + fi->tickdelay;
	}

	wing::HDC hdc;
	if (!ahdc) {
		gui.drawstart();
		gui.fontSet();
		hdc = gui.hdc;
	} else {
		hdc = ahdc;
	}

	if (!ahdc) {
		gui.skipLine();
		gui.numberu(data->filesRenamed);
		gui.skipLine();
		gui.skipLine();
		gui.numberu(data->filesIgnored);
		gui.skipLine();
		gui.skipLine();
		gui.numberu(data->filesTotal);
		gui.skipLine();
		if (fi->logFileWritten) {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidError);
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDone);
			}
		} else {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidErrorNoOutput);
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDoneNoOutput);
			}
		}
	} else {

		if (gMenu.convert) {
			uiid::uiidDraw(uiid::uiidLogFilesRenamed);
		} else {
			uiid::uiidDraw(uiid::uiidLogFilesToRename);
		}
		gui.finishLine();
		gui.numberu(data->filesRenamed);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLogFilesIgnored);
		gui.finishLine();
		gui.numberu(data->filesIgnored);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLogFilesTotal);
		gui.finishLine();
		gui.numberu(data->filesTotal);
		gui.finishLine();

		if (fi->logFileWritten) {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidError);
				gui.finishLine();
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDone);
				gui.finishLine();
			}
		} else {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidErrorNoOutput);
				gui.finishLine();
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDoneNoOutput);
				gui.finishLine();
			}
		}
		gui.finishWindow();
	}
	
	if (!ahdc) {
		gui.drawend();
	}
}

void FileIteratorCodepage_OnStart(FileIterator::cFileIterator* fi) {
	cFileIteratorCodepage_Data* data = (cFileIteratorCodepage_Data*)fi->fio->data;

	data->filesTotal = 0;
	data->filesUtf8 = 0;
	data->filesNotUtf8 = 0;
	data->filesIgnored = 0;

	fi->filebuf1size = 0x00800000;
	if (gMenu.convert) {
#if _DEBUG
		if (fi->filebuf1size * 2 != 0x01000000) ErrorLazy();
#endif
		bytep res = mem::memgetBig(0x01000000);

		fi->filebuf1 = res;
		fi->filebuf2 = res + fi->filebuf1size;

	}

	fi->w = (cLogWriter*)mem::Get(sizeof(cLogWriter));
	writerInit(fi->w, fi);
#if _DEBUG
	mem::memgetLock();
#endif
	*fi->w->cur++ = 0xFEFF; //BOM
	writerWrite(fi->w, uiid::uiidLogFilelistStart);
}

void FileIteratorCodepage_OnFile(FileIterator::cFileIterator* fi) {
	cFileIteratorCodepage_Data* data = (cFileIteratorCodepage_Data*)fi->fio->data;

	wchar* filename = fi->findFileData.cFileName;
	u4 filenameLen = ustr::wcharLen(filename);

	bool toProcess = endsWithSpaceSeparatedList(filename, filenameLen);

	if (toProcess) {
		bool isUtf8 = FileIsUTF8BOM(fi->g_filepath, fi->findFileData.nFileSizeLow);
		if (isUtf8) {
			data->filesUtf8++;
		} else {
			data->filesNotUtf8++;
			writerWrite(fi->w, uiid::uiidLogNotUtf8Bom);
			writerWriteFilename(fi->w, fi);

			if (fi->findFileData.nFileSizeLow > fi->filebuf1size) {
				writerWrite(fi->w, uiid::uiidErrorFileTooBig);
				fi->error = true;
			} else if (gMenu.convert) {
				bool falseIfConverted = FileConvertToUTF8BOM(fi);
				if (falseIfConverted == false) {
					writerWrite(fi->w, uiid::uiidLogConvertedSuccessfully);
					data->filesConvertSuccessful++;
					
				} else {
					writerWrite(fi->w, uiid::uiidLogConversionFailed);
					data->filesConvertFail++;
				}
			}
		}
	} else {
		data->filesIgnored++;
	}
	data->filesTotal++;
}

void FileIteratorCodepage_OnFile_ToShiftJIS(FileIterator::cFileIterator* fi) {
	cFileIteratorCodepage_Data* data = (cFileIteratorCodepage_Data*)fi->fio->data;

	wchar* filename = fi->findFileData.cFileName;
	u4 filenameLen = ustr::wcharLen(filename);

	bool toProcess = endsWithSpaceSeparatedList(filename, filenameLen);

	if (toProcess) {
		bool isUtf8 = FileIsUTF8BOM(fi->g_filepath, fi->findFileData.nFileSizeLow);
		if (isUtf8) {
			data->filesUtf8++;

			writerWriteFilename(fi->w, fi);

			if (fi->findFileData.nFileSizeLow > fi->filebuf1size) {
				writerWrite(fi->w, uiid::uiidErrorFileTooBig);
				fi->error = true;
			} else if (gMenu.convert) {
				bool falseIfConverted = FileConvertToShift_JIS(fi);
				if (falseIfConverted == false) {
					writerWrite(fi->w, uiid::uiidLogConvertedSuccessfully);
					data->filesConvertSuccessful++;
					
				} else {
					writerWrite(fi->w, uiid::uiidLogConversionFailed);
					data->filesConvertFail++;
				}
			}
		} else {
			data->filesNotUtf8++;
			writerWrite(fi->w, uiid::uiidLogNotUtf8Bom);
			writerWriteFilename(fi->w, fi);

		}
	} else {
		data->filesIgnored++;
	}
	data->filesTotal++;
}

void FileIteratorCodepage_OnEnd(FileIterator::cFileIterator* fi) {
	cFileIteratorCodepage_Data* data = (cFileIteratorCodepage_Data*)fi->fio->data;
	
	writerWrite(fi->w, uiid::uiidLogFilelistEnd);

	writerWrite(fi->w, uiid::uiidLogFilesUtf8);
	writerWriteNumber(fi->w, data->filesUtf8);

	writerWrite(fi->w, uiid::uiidLogFilesNotUtf8);
	writerWriteNumber(fi->w, data->filesNotUtf8);

	writerWrite(fi->w, uiid::uiidLogFilesIgnored);
	writerWriteNumber(fi->w, data->filesIgnored);

	writerWrite(fi->w, uiid::uiidLogFilesTotal);
	writerWriteNumber(fi->w, data->filesTotal);

	if (gMenu.convert) {
		writerWrite(fi->w, uiid::uiidLogFilesConvertSuccessful);
		writerWriteNumber(fi->w, data->filesConvertSuccessful);

		writerWrite(fi->w, uiid::uiidLogFilesConvertFail);
		writerWriteNumber(fi->w, data->filesConvertFail);
	}

	u4 len = ustr::wcharLen(gMenu.pathLog);
	wchar* cur = gMenu.pathLog + len - 1;
	errorCode err = 0;
	if (*cur != L'/' && *cur != L'\\')
	{
		err = ustr::wcharAttachSafe(L"/", gMenu.pathLog, win::file::MAX_PATH);
	}
	if (!err) err = ustr::wcharAttachSafe(L"fix-things-out.txt", gMenu.pathLog, win::file::MAX_PATH);
	if (!err) err = writerWriteFile(fi->w, gMenu.pathLog);
	fi->done = true;
	fi->logFileWritten = !err;
	wing::InvalidateRect(gui.hWnd, null, null);

}

void FileIteratorCodepage_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc) {
	cFileIteratorCodepage_Data* data = (cFileIteratorCodepage_Data*)fi->fio->data;
	if (!ahdc) {
		u4 ticknext = win::GetTickCount();
		if (i4(ticknext - fi->tick) < 0) return;
		fi->tick = ticknext + fi->tickdelay;
	}

	wing::HDC hdc;
	if (!ahdc) {
		gui.drawstart();
		gui.fontSet();
		hdc = gui.hdc;
	} else {
		hdc = ahdc;
	}

	if (!ahdc) {
		gui.skipLine();
		gui.numberu(data->filesUtf8);
		gui.skipLine();
		gui.skipLine();
		gui.numberu(data->filesNotUtf8);
		gui.skipLine();
		gui.skipLine();
		gui.numberu(data->filesIgnored);
		gui.skipLine();
		gui.skipLine();
		gui.numberu(data->filesTotal);
		gui.skipLine();
		if (gMenu.convert) {
			gui.skipLine();
			gui.numberu(data->filesConvertSuccessful);
			gui.skipLine();
			gui.skipLine();
			gui.numberu(data->filesConvertFail);
			gui.skipLine();
		}
		
		if (fi->logFileWritten) {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidError);
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDone);
			}
		} else {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidErrorNoOutput);
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDoneNoOutput);
			}
		}
	} else {
		uiid::uiidDraw(uiid::uiidLogFilesUtf8);
		gui.finishLine();
		gui.numberu(data->filesUtf8);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLogFilesNotUtf8);
		gui.finishLine();
		gui.numberu(data->filesNotUtf8);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLogFilesIgnored);
		gui.finishLine();
		gui.numberu(data->filesIgnored);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLogFilesTotal);
		gui.finishLine();
		gui.numberu(data->filesTotal);
		gui.finishLine();

		if (gMenu.convert) {
			uiid::uiidDraw(uiid::uiidLogFilesConvertSuccessful);
			gui.finishLine();
			gui.numberu(data->filesConvertSuccessful);
			gui.finishLine();

			uiid::uiidDraw(uiid::uiidLogFilesConvertFail);
			gui.finishLine();
			gui.numberu(data->filesConvertFail);
			gui.finishLine();
		}

		if (fi->logFileWritten) {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidError);
				gui.finishLine();
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDone);
				gui.finishLine();
			}
		} else {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidErrorNoOutput);
				gui.finishLine();
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDoneNoOutput);
				gui.finishLine();
			}
		}
		gui.finishWindow();
	}
	
	if (!ahdc) {
		gui.drawend();
	}
}

void FileIteratorLineend_OnStart(FileIterator::cFileIterator* fi) {
	cFileIteratorLineend_Data* data = (cFileIteratorLineend_Data*)fi->fio->data;

	fi->filebuf1size = 0x00800000;

#if _DEBUG
	if (fi->filebuf1size * 2 != 0x01000000) ErrorLazy();
#endif
	bytep res = mem::memgetBig(0x01000000);

	fi->filebuf1 = res;
	fi->filebuf2 = res + fi->filebuf1size;

	fi->w = (cLogWriter*)mem::Get(sizeof(cLogWriter));
	writerInit(fi->w, fi);
#if _DEBUG
	mem::memgetLock();
#endif
	*fi->w->cur++ = 0xFEFF; //BOM
	writerWrite(fi->w, uiid::uiidLogFilelistStart);
}

enum eLineend {
	eLineendNotUTF8BOM,
	eLineendRN,
	eLineendN,
	eLineendBoth,
	eLineendNone,
};

u4 LineendCheck(FileIterator::cFileIterator* fi) {
	const wchar* filename = fi->g_filepath;
	u4 filesize = fi->findFileData.nFileSizeLow;
	if (filesize < 3) return eLineendNotUTF8BOM;

	bytep fileread = fi->filebuf1;
	bytep filewrite = fi->filebuf2;

	ufile::cFile file;
	errorCode err;

	err = file.OpenForReadWrite(filename);
	if (err) ErrorLazy();

	u4 fileread_size = file.Read(fileread, 4, &err);
	if (err) ErrorLazy();

	if (fileread_size < 3) goto not_utf8_bom;
	bytep in = fileread;
	if (*in++ != 0xEF) goto not_utf8_bom;
	if (*in++ != 0xBB) goto not_utf8_bom;
	if (*in++ != 0xBF) goto not_utf8_bom;

	if (false) {
not_utf8_bom:
		file.Close();
		return eLineendNotUTF8BOM;
	}

	//read the rest of the file
	{
		i4 filesize_left = filesize - 4;
		if (filesize_left < 0) filesize_left = 0;
		if (filesize_left > 0) {
			u4 fileread_size2 = file.Read(fileread+4, filesize-4, &err);
			if (err) ErrorLazy();
			fileread_size += fileread_size2;
		}
	}
	file.Close();
	*(fileread+fileread_size)=0; //file ends with zero byte.

	bool hasLineendN = false;
	bool hasLineendRN = false;

	bytep instart = in;
	if (*in) do {
		if (*in == '\n') {
			hasLineendN = true;
		}
		else if (*in == '\r') {
			in++;
			if (*in != '\n') Error(0xa0);
			hasLineendRN = true;
		}
		in++;
	} while (*in);

	if (hasLineendN && hasLineendRN) return eLineendBoth;
	if (hasLineendRN) return eLineendRN;
	if (hasLineendN) return eLineendN;
	return eLineendNone;
}

u4 LineendConvertRNtoN(FileIterator::cFileIterator* fi) {
	const wchar* filename = fi->g_filepath;
	u4 filesize = fi->findFileData.nFileSizeLow;
	if (filesize < 3) return eLineendNotUTF8BOM;

	bytep fileread = fi->filebuf1;
	bytep filewrite = fi->filebuf2;

	ufile::cFile file;
	errorCode err;

	err = file.OpenForReadWrite(filename);
	if (err) ErrorLazy();

	u4 fileread_size = file.Read(fileread, 4, &err);
	if (err) ErrorLazy();

	if (fileread_size < 3) goto not_utf8_bom;
	bytep in = fileread;
	if (*in++ != 0xEF) goto not_utf8_bom;
	if (*in++ != 0xBB) goto not_utf8_bom;
	if (*in++ != 0xBF) goto not_utf8_bom;

	if (false) {
not_utf8_bom:
		file.Close();
		return eLineendNotUTF8BOM;
	}

	//read the rest of the file
	{
		i4 filesize_left = filesize - 4;
		if (filesize_left < 0) filesize_left = 0;
		if (filesize_left > 0) {
			u4 fileread_size2 = file.Read(fileread+4, filesize-4, &err);
			if (err) ErrorLazy();
			fileread_size += fileread_size2;
		}
	}
	*(fileread+fileread_size)=0; //file ends with zero byte.
	file.GoToStart();

	bytep out = (bytep)filewrite;
	*out++ = 0xEF;
	*out++ = 0xBB;
	*out++ = 0xBF;
	bool hasLineendN = false;
	bool hasLineendRN = false;

	bytep instart = in;
	if (*in) do {
		if (*in == '\n') {
			hasLineendN = true;
		}
		else if (*in == '\r') {
			in++;
			if (*in != '\n') goto error_RnotfollowedbyN;
			hasLineendRN = true;
		}
		*out++ = *in++;
	} while (*in);

	if (hasLineendRN) {
		file.Write(filewrite, out - filewrite, &err);
		file.Trim();
		file.SetTime(&fi->findFileData.ftCreationTime, &fi->findFileData.ftLastAccessTime, &fi->findFileData.ftLastWriteTime);
	}
	file.Close();

	if (false) {
error_RnotfollowedbyN:
		file.Close();
		writerWrite(fi->w, uiid::uiidErrorRnotfollowedbyN);
		writerWriteFilename(fi->w, fi);
		fi->error = true;
	}

	if (hasLineendN && hasLineendRN) return eLineendBoth;
	if (hasLineendRN) return eLineendRN;
	if (hasLineendN) return eLineendN;
	return eLineendN;
}

u4 LineendConvertNtoRN(FileIterator::cFileIterator* fi) {
	const wchar* filename = fi->g_filepath;
	u4 filesize = fi->findFileData.nFileSizeLow;
	if (filesize < 3) return eLineendNotUTF8BOM;

	bytep fileread = fi->filebuf1;
	bytep filewrite = fi->filebuf2;

	ufile::cFile file;
	errorCode err;

	err = file.OpenForReadWrite(filename);
	if (err) ErrorLazy();

	u4 fileread_size = file.Read(fileread, 4, &err);
	if (err) ErrorLazy();

	if (fileread_size < 3) goto not_utf8_bom;
	bytep in = fileread;
	if (*in++ != 0xEF) goto not_utf8_bom;
	if (*in++ != 0xBB) goto not_utf8_bom;
	if (*in++ != 0xBF) goto not_utf8_bom;

	if (false) {
not_utf8_bom:
		file.Close();
		return eLineendNotUTF8BOM;
	}

	//read the rest of the file
	{
		i4 filesize_left = filesize - 4;
		if (filesize_left < 0) filesize_left = 0;
		if (filesize_left > 0) {
			u4 fileread_size2 = file.Read(fileread+4, filesize-4, &err);
			if (err) ErrorLazy();
			fileread_size += fileread_size2;
		}
	}
	*(fileread+fileread_size)=0; //file ends with zero byte.
	file.GoToStart();

	bytep out = (bytep)filewrite;
	*out++ = 0xEF;
	*out++ = 0xBB;
	*out++ = 0xBF;

	u4 filewrite_size = fileread_size;

	bool hasLineendN = false;
	bool hasLineendRN = false;

	bytep instart = in;
	if (*in) do {
		if (*in == '\n') {
			hasLineendN = true;
			*out++ = '\r';
			filewrite_size++;
			if (filewrite_size >= fi->filebuf1size) {
				goto error_filebuf1size;
			}
		}
		else if (*in == '\r') {
			*out++ = *in++;
			if (*in != '\n') goto error_RnotfollowedbyN;
			hasLineendRN = true;
		}
		*out++ = *in++;
	} while (*in);

	if (hasLineendN) {
		file.Write(filewrite, out - filewrite, &err);
		file.Trim();
		file.SetTime(&fi->findFileData.ftCreationTime, &fi->findFileData.ftLastAccessTime, &fi->findFileData.ftLastWriteTime);
	}
	file.Close();

	if (false) {
error_filebuf1size:
		file.Close();
		writerWrite(fi->w, uiid::uiidErrorMemTooSmall);
		writerWriteFilename(fi->w, fi);
		fi->error = true;
	}
	if (false) {
error_RnotfollowedbyN:
		file.Close();
		writerWrite(fi->w, uiid::uiidErrorRnotfollowedbyN);
		writerWriteFilename(fi->w, fi);
		fi->error = true;
	}

	if (hasLineendN && hasLineendRN) return eLineendBoth;
	if (hasLineendRN) return eLineendRN;
	if (hasLineendN) return eLineendN;
	return eLineendN;
}

void FileIteratorLineend_OnFile(FileIterator::cFileIterator* fi) {
	cFileIteratorLineend_Data* data = (cFileIteratorLineend_Data*)fi->fio->data;

	wchar* filename = fi->findFileData.cFileName;
	u4 filenameLen = ustr::wcharLen(filename);

	bool toProcess = endsWithSpaceSeparatedList(filename, filenameLen);

	if (toProcess) {
		u4 res;
		if (gMenu.convert) {
			if (gMenu.option0 == 0) {
				res = LineendConvertRNtoN(fi);
			} else {
				res = LineendConvertNtoRN(fi);
			}
		} else {
			res = LineendCheck(fi);
		}
		switch(res) {
			case eLineendBoth:
				data->filesMixed++;
				writerWrite(fi->w, uiid::uiidLineendMixed);
				writerWriteFilename(fi->w, fi);
				break;
			case eLineendRN:
				data->filesRN++;
				break;
			case eLineendN:
				data->filesN++;
				writerWrite(fi->w, uiid::uiidLineendN);
				writerWriteFilename(fi->w, fi);
				break;
			case eLineendNone:
				data->filesNone++;
				writerWrite(fi->w, uiid::uiidLineendNone);
				writerWriteFilename(fi->w, fi);
				break;
			case eLineendNotUTF8BOM:
				data->filesNotUtf8Bom++;
				writerWrite(fi->w, uiid::uiidLogNotUtf8Bom);
				writerWriteFilename(fi->w, fi);
				break;
			default:
				ErrorLazy();
		}
	}
	data->filesTotal++;
}


void FileIteratorLineend_OnEnd(FileIterator::cFileIterator* fi) {
	cFileIteratorLineend_Data* data = (cFileIteratorLineend_Data*)fi->fio->data;
	
	writerWrite(fi->w, uiid::uiidLogFilelistEnd);

	writerWrite(fi->w, uiid::uiidLogFilesTotal);
	writerWriteNumber(fi->w, data->filesTotal);

	writerWrite(fi->w, uiid::uiidLogFilesNotUtf8);
	writerWriteNumber(fi->w, data->filesNotUtf8Bom);

	writerWrite(fi->w, uiid::uiidLineendRN);
	writerWriteNumber(fi->w, data->filesRN);
	
	writerWrite(fi->w, uiid::uiidLineendN);
	writerWriteNumber(fi->w, data->filesN);
	
	writerWrite(fi->w, uiid::uiidLineendMixed);
	writerWriteNumber(fi->w, data->filesMixed);
	
	writerWrite(fi->w, uiid::uiidLineendNone);
	writerWriteNumber(fi->w, data->filesNone);

	u4 len = ustr::wcharLen(gMenu.pathLog);
	wchar* cur = gMenu.pathLog + len - 1;
	errorCode err = 0;
	if (*cur != L'/' && *cur != L'\\')
	{
		err = ustr::wcharAttachSafe(L"/", gMenu.pathLog, win::file::MAX_PATH);
	}
	if (!err) err = ustr::wcharAttachSafe(L"fix-things-out.txt", gMenu.pathLog, win::file::MAX_PATH);
	if (!err) err = writerWriteFile(fi->w, gMenu.pathLog);
	fi->done = true;
	fi->logFileWritten = !err;
	wing::InvalidateRect(gui.hWnd, null, null);

}


void FileIteratorLineend_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc) {
	cFileIteratorLineend_Data* data = (cFileIteratorLineend_Data*)fi->fio->data;
	if (!ahdc) {
		u4 ticknext = win::GetTickCount();
		if (i4(ticknext - fi->tick) < 0) return;
		fi->tick = ticknext + fi->tickdelay;
	}

	wing::HDC hdc;
	if (!ahdc) {
		gui.drawstart();
		gui.fontSet();
		hdc = gui.hdc;
	} else {
		hdc = ahdc;
	}

	if (!ahdc) {

		gui.skipLine();
		gui.numberu(data->filesTotal);
		gui.skipLine();

		gui.skipLine();
		gui.numberu(data->filesNotUtf8Bom);
		gui.skipLine();

		gui.skipLine();
		gui.numberu(data->filesRN);
		gui.skipLine();

		gui.skipLine();
		gui.numberu(data->filesN);
		gui.skipLine();

		gui.skipLine();
		gui.numberu(data->filesMixed);
		gui.skipLine();

		gui.skipLine();
		gui.numberu(data->filesNone);
		gui.skipLine();
		
		if (fi->logFileWritten) {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidError);
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDone);
			}
		} else {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidErrorNoOutput);
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDoneNoOutput);
			}
		}
	} else {

		uiid::uiidDraw(uiid::uiidLogFilesTotal);
		gui.finishLine();
		gui.numberu(data->filesTotal);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLogFilesNotUtf8);
		gui.finishLine();
		gui.numberu(data->filesNotUtf8Bom);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLineendRN);
		gui.finishLine();
		gui.numberu(data->filesRN);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLineendN);
		gui.finishLine();
		gui.numberu(data->filesN);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLineendMixed);
		gui.finishLine();
		gui.numberu(data->filesMixed);
		gui.finishLine();

		uiid::uiidDraw(uiid::uiidLineendNone);
		gui.finishLine();
		gui.numberu(data->filesNone);
		gui.finishLine();

		if (fi->logFileWritten) {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidError);
				gui.finishLine();
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDone);
				gui.finishLine();
			}
		} else {
			if (fi->error) {
				uiid::uiidDraw(uiid::uiidErrorNoOutput);
				gui.finishLine();
			} else if (fi->done) {
				uiid::uiidDraw(uiid::uiidDoneNoOutput);
				gui.finishLine();
			}
		}
		gui.finishWindow();
	}
	
	if (!ahdc) {
		gui.drawend();
	}
}

} //namespace