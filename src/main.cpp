
#include "main.h"

#ifndef _UNICODE
#error "Please define _UNICODE"
#endif

#include "util/miniutil.h"
#include "util/win.h"
#include "util/wing.h"
#include "util/ui.h"
#include "util/uiid.h"
#include "util/MemorySimple.h"
#include "util/FileIterator.h"
#include "util/FileIteratorOptions.h"

using namespace uiid;

//GLOBALS
Menu gMenu;

//void* operator new(u4 size) {
//	return mem::Get(size);
//}

u4 __stdcall edit_procedure(wing::HWND wnd, u4 msg, u4 wParam, u4 lParam);

class cEdit {
public:
	wing::HWND mHandle;
	bool mVisible;
	//bool mCtrlDown;
	//bool mShiftDown;
	u4 mLine;
	wing::WNDPROC m_edit_procedure_old;

	void Focus() {
		if (mVisible) wing::SetFocus(mHandle);
	}

	void Hide() {
		mVisible = false;
		wing::ShowWindow(mHandle, wing::SW_HIDE);
	}

	void Unhide() {
		mVisible = true;
		wing::ShowWindow(mHandle, wing::SW_SHOW);
		Focus();
	}

	void OnWmCreate(wing::HWND hWnd) {
		mHandle = wing::CreateWindowExW(0, L"EDIT", L"",
			//WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			wing::WS_CHILD | wing::WS_VISIBLE,
			0, 0, 0x400, gui.font_height, hWnd, 0, wing::GetModuleHandleW(null), null);
		if (mHandle == null) ErrorLazy();
		
		wing::SendMessageW(mHandle, wing::WM_SETFONT, gui.font, 0);
		m_edit_procedure_old = (wing::WNDPROC)wing::SetWindowLongW(mHandle, wing::GWL_WNDPROC, (u4)edit_procedure);

		mVisible = false;
		//mCtrlDown = false;
		//mShiftDown = false;
		mLine = 1;
		Hide();
	}

	void OnWmSize() {
		u4 lineHeight = mLine * gui.font_height;
		wing::SetWindowPos(mHandle, null, 0, lineHeight, gui.width, gui.font_height, wing::SWP_NOZORDER);
	}

	void OnWM_CTLCOLOREDIT(u4 wParam) {
		wing::HDC hdcEdit = (wing::HDC)wParam;
		wing::SetTextColor(hdcEdit, wing::RGB(0xB0, 0xB0, 0xB0));
		wing::SetBkColor(hdcEdit, wing::RGB(0, 0, 0));
	}

	void Set(const wchar* text, u4 textLen) {
		wing::SendMessageW(mHandle, wing::WM_SETTEXT, 0, (u4)text);
		wing::SendMessageW(mHandle, wing::EM_SETSEL, textLen, textLen);
	}

	void Get(wchar* text, u4* textLen, u4 textCap) {
		//ErrorLazy(); //Not Done Yet
		u4 len = wing::SendMessageW(mHandle, wing::WM_GETTEXTLENGTH, 0, 0);
		if (len > textCap) len = textCap;
		*textLen = len;
		wing::SendMessageW(mHandle, wing::WM_GETTEXT, textCap + 1, (u4)text);

		//program specific code
		u4 spaces = 0;
		wchar* cur = text;
		while (true) {
			if (*cur == 0) break;
			if (*cur == L' ') spaces++;
			cur++;
		}
		if (*textLen + spaces > textCap) {
			*textLen = textCap - spaces;
			*(text + *textLen) = 0;
		}
		//remove spaces at the end
		while (true) {
			if (*textLen == 0) goto labelEmpty;
			if (*(text + *textLen - 1) == L' ') {
				(*textLen)--;
				*(text + *textLen) = 0;
			} else break;
		}
		//remove spaces at the start
		while (true) {
			if (*textLen == 0) goto labelEmpty;
			if (*text == L' ') {
				wchar* cur = text + 1;
				while (true) {
					*(cur-1) = *cur;
					if (*cur == 0) break;
					cur++;
				}
				(*textLen)--;
			} else break;
		}
		//remove repeated spaces in the middle
		cur = text + *textLen - 2;
		while (true) {
			if (cur == text) break;
			if (*cur == L' ' && *(cur+1) == L' ') {
				(*textLen)--;
				wchar* cur2 = cur + 1;
				while (true) {
					*(cur2-1) = *cur2;
					if (*cur2 == 0) break;
					cur2++;
				}
			}
			cur--;
		}
		return;
labelEmpty:
		ustr::wcharCopy(L".ERB", text);
		*textLen = 4;
		return;
	}
};

cEdit gEdit;

namespace nmain {

//u4 edit_text_len;
//wing::HWND hEdit;
//
//u4 edit_command(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
//	i2 wNotifyCode = util::u2high(wParam); // notification code 
//	if (wNotifyCode == wing::EN_CHANGE) {
//		i2 wID = util::u2low(wParam);         // item, control, or accelerator identifier 
//		wing::HWND hwndCtl = (wing::HWND)lParam;      // handle of control 
//		//SendMessageW(hEdit, WM_SETTEXT, (WPARAM)0, (LPARAM)L"SUP, HOW ARE YOU"); //STACK OVERFLOW
//		edit_text_len = wing::SendMessageW(hEdit, wing::WM_GETTEXTLENGTH, 0, 0);
//		if (edit_text_len == 0) {
//			*edit_text = 0;
//			return 0;
//		}
//		SendMessageW(hEdit, wing::WM_GETTEXT, edit_text_len + 1, edit_text);
//		search(0);
//		return 0;
//	}
//	//else if (wNotifyCode != EN_SETFOCUS && wNotifyCode != EN_KILLFOCUS) {
//	//    do {} while (0);
//	//}
//	return 0;
//}

enum Mode {
	modeMain,
	modeLangSelect,
	modeFileExtensions,
	modeFixWhat,
	modeDoActualWork,
	modeHelp,
	modeFixWhatLineend,
	modeFixWhatCodepage,
};

void gMenuInit() {
	gMenu.mode = 0;
	gMenu.sel0 = 0;
	gMenu.selprev = 0;
	gMenu.language = 0;
	gMenu.fixWhat = 0;
	gMenu.selMax = 7;
	if (gMenu.fixWhat == 2) gMenu.selMax += 1;
	if (gMenu.fixWhat == 1) gMenu.selMax += 1;
	gMenu.option0 = 0;

	gMenu.path = (wchar_t*)mem::Get(win::file::MAX_PATH*2+2);
	gMenu.path[0] = L'.';
	gMenu.pathLog = (wchar_t*)mem::Get(win::file::MAX_PATH*2+2);
	gMenu.pathLog[0] = L'.';
	gMenu.fileExtensions = (wchar_t*)mem::Get(win::file::MAX_PATH*2+2);
	ustr::wcharCopy(L".ERB .ERH .csv .config", gMenu.fileExtensions);
	gMenu.fileExtensionsLen = ustr::wcharLen(gMenu.fileExtensions);
	gMenu.fileExtensionsCap = win::file::MAX_PATH - 2;
	//^ for terminating space and terminating zero, and u2 terminating zero.
	//TODO: check if this is really enough.

	//gMenu.fileExtensionsCap = 6;
	//ustr::wcharCopy(L"C:\\TW\\test", gMenu.path);
	//ustr::wcharCopy(L"C:\\TW\\test", gMenu.pathLog);
}


void menuBack() {
	gMenu.mode = modeMain;
	gMenu.sel0 = gMenu.sel1;
	gMenu.selprev = 0;
	gMenu.selMax = 7;
	if (gMenu.fixWhat == 2) gMenu.selMax += 1;
	if (gMenu.fixWhat == 1) gMenu.selMax += 1;
	gui.invalidate_all();
}



void menuClipboardGet(wchar* path) {
	if (!win::clipboard::IsClipboardFormatAvailable(win::clipboard::CF_UNICODETEXT)) return;
	if (!win::clipboard::OpenClipboard(null)) return;

	win::HANDLE hglb = win::clipboard::GetClipboardData(win::clipboard::CF_UNICODETEXT);
	if (hglb) {
		wchar* cur = (tchar*)win::clipboard::GlobalLock(hglb);
		if (cur) {
			wchar* curout = path;
			//tchar* curout = null;
			for (u4 i = win::file::MAX_PATH; i > 0; i--) {
				*curout++ = *cur++;
				if (*cur == 0) break;
			}
			*curout = 0;
			//if (*cur != 0 && *(cur+1) == t(':')) {

			//}
			//TODO
			win::clipboard::GlobalUnlock(hglb);
		}
	}
	win::clipboard::CloseClipboard();
}


void main_fiStart(FileIterator::cFileIterator* fi) {
	FileIteratorOptions::endsWithPrepare();

	fiInit(fi, gMenu.path);

	FileIterator::cFileIteratorOptions* fio = fi->fio;

	if (gMenu.fixWhat == 0) {
		fio->OnStart = FileIteratorOptions::FileIteratorCase_OnStart;
		fio->OnFile = FileIteratorOptions::FileIteratorCase_OnFile;
		fio->OnEnd = FileIteratorOptions::FileIteratorCase_OnEnd;
		fio->OnDraw = FileIteratorOptions::FileIteratorCase_OnDraw;
		fio->data = (FileIteratorOptions::cFileIteratorCase_Data*)mem::Get(sizeof(FileIteratorOptions::cFileIteratorCodepage_Data));
	} else if (gMenu.fixWhat == 1) {
		fio->OnStart = FileIteratorOptions::FileIteratorCodepage_OnStart;
		if (gMenu.option0 == 0) {
			fio->OnFile = FileIteratorOptions::FileIteratorCodepage_OnFile;
		} else {
			fio->OnFile = FileIteratorOptions::FileIteratorCodepage_OnFile_ToShiftJIS;
		}
		fio->OnEnd = FileIteratorOptions::FileIteratorCodepage_OnEnd;
		fio->OnDraw = FileIteratorOptions::FileIteratorCodepage_OnDraw;
		fio->data = (FileIteratorOptions::cFileIteratorCodepage_Data*)mem::Get(sizeof(FileIteratorOptions::cFileIteratorCodepage_Data));
	} else {
		fio->OnStart = FileIteratorOptions::FileIteratorLineend_OnStart;
		fio->OnFile = FileIteratorOptions::FileIteratorLineend_OnFile;
		fio->OnEnd = FileIteratorOptions::FileIteratorLineend_OnEnd;
		fio->OnDraw = FileIteratorOptions::FileIteratorLineend_OnDraw;
		fio->data = (FileIteratorOptions::cFileIteratorLineend_Data*)mem::Get(sizeof(FileIteratorOptions::cFileIteratorLineend_Data));
	}

	fiStart(fi);
}

void menuMainEnter() {
	if (gMenu.mode != modeMain) ErrorLazy();
	if (gMenu.sel0 == -1) return;
	if (gMenu.sel0 == 0) {
		gMenu.mode = modeLangSelect;
		gMenu.sel1 = gMenu.sel0;
		gMenu.sel0 = 0;
		gMenu.selprev = 0;
		gMenu.selMax = 2;
		gui.invalidate_all();
		return;
	}
	if (gMenu.sel0 == 1) {
		menuClipboardGet(gMenu.path);
		gui.invalidate_all();
		return;
	}
	if (gMenu.sel0 == 2) {
		menuClipboardGet(gMenu.pathLog);
		gui.invalidate_all();
		return;
	}
	if (gMenu.sel0 == 3) {
		gMenu.mode = modeFileExtensions;
		gMenu.sel1 = gMenu.sel0;
		gMenu.sel0 = 0;
		gMenu.selprev = 0;
		gMenu.selMax = 1;
		gEdit.Set(gMenu.fileExtensions, gMenu.fileExtensionsLen);
		gEdit.Unhide();
		gui.invalidate_all();
		return;
	}
	if (gMenu.sel0 == 4) {
		gMenu.mode = modeFixWhat;
		gMenu.sel1 = gMenu.sel0;
		gMenu.sel0 = 0;
		gMenu.selprev = 0;
		gMenu.selMax = 2;
		gui.invalidate_all();
		return;
	}
	u4 selCur = 5;
	if (gMenu.fixWhat == 2) {
		if (gMenu.sel0 == selCur++) {
			gMenu.mode = modeFixWhatLineend;
			gMenu.sel1 = gMenu.sel0;
			gMenu.sel0 = 0;
			gMenu.selprev = 0;
			gMenu.selMax = 1;
			gui.invalidate_all();
			return;
		}
	}
	if (gMenu.fixWhat == 1) {
		if (gMenu.sel0 == selCur++) {
			gMenu.mode = modeFixWhatCodepage;
			gMenu.sel1 = gMenu.sel0;
			gMenu.sel0 = 0;
			gMenu.selprev = 0;
			gMenu.selMax = 1;
			gui.invalidate_all();
			return;
		}
	}
	if (gMenu.sel0 == selCur++) {
		gMenu.mode = modeDoActualWork;
		gMenu.convert = false;
		gui.invalidate_all();
		main_fiStart(&gfi);
		return;
	}
	if (gMenu.sel0 == selCur++) {
		gMenu.mode = modeDoActualWork;
		gMenu.convert = true;
		gui.invalidate_all();
		main_fiStart(&gfi);
		return;
	}
	if (gMenu.sel0 == selCur++) {
		gMenu.mode = modeHelp;
		gMenu.sel1 = gMenu.sel0;
		gMenu.sel0 = 0;
		gMenu.selprev = 0;
		gMenu.selMax = 0;
		gui.invalidate_all();
		return;
	}
	ErrorLazy();
}

void menuHelp(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			uiidDrawOption(gMenu.sel0 == 0);
			uiidDraw(uiidBack);
			gui.finishLine();

			uiidDrawOption(false);
			uiidDraw(uiidHelp0);
			gui.finishLine();
			uiidDrawOption(false);
			uiidDraw(uiidHelp1);
			gui.finishLine();
			uiidDrawOption(false);
			uiidDraw(uiidHelp2);
			gui.finishLine();
			uiidDrawOption(false);
			uiidDraw(uiidHelp3);
			gui.finishLine();
			uiidDrawOption(false);
			uiidDraw(uiidHelp4);
			gui.finishLine();
			uiidDrawOption(false);
			uiidDraw(uiidHelpUrl);
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
					menuBack();
				}
			}
			if (wParam == wing::VK_ESCAPE) {
				menuBack();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
				menuBack();
			}
			break;
		}
	}
}

void LangChange(u4 lang) {
	//gMenu.language;
	uiid::uiStrings = uiid::uiLangs[lang];
	uiid::uiidInit();
}

void menuLangChange(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			uiidDrawOption(gMenu.sel0 == 0);
			gui.text(uiLangStrings[uiLangIdJp], uiLangStringsLen[uiLangIdJp]);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 1);
			gui.text(uiLangStrings[uiLangIdEn], uiLangStringsLen[uiLangIdEn]);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 2);
			gui.text(uiLangStrings[uiLangIdRu], uiLangStringsLen[uiLangIdRu]);
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
					gMenu.language = gMenu.sel0;
					LangChange(gMenu.language);
					menuBack();
				}
			}
			if (wParam == wing::VK_ESCAPE) {
				menuBack();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
				gMenu.language = gMenu.sel0;
				LangChange(gMenu.language);
				menuBack();
			}
			break;
		}
	}
}

void menuFixWhatLineend(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			uiidDrawOption(gMenu.sel0 == 0);
			uiidDraw(uiidOption0RNtoN);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 1);
			uiidDraw(uiidOption0NtoRN);
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
					gMenu.option0 = gMenu.sel0;
					menuBack();
				}
			}
			if (wParam == wing::VK_ESCAPE) {
				menuBack();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
				gMenu.option0 = gMenu.sel0;
				menuBack();
			}
			break;
		}
	}
}

void menuFixWhatCodepage(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			uiidDrawOption(gMenu.sel0 == 0);
			uiidDraw(uiidCodepageToUTF8BOM);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 1);
			uiidDraw(uiidCodepageToShiftJIS);
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
					gMenu.option0 = gMenu.sel0;
					menuBack();
				}
			}
			if (wParam == wing::VK_ESCAPE) {
				menuBack();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
				gMenu.option0 = gMenu.sel0;
				menuBack();
			}
			break;
		}
	}
}

void menuFixWhat(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			uiidDrawOption(gMenu.sel0 == 0);
			uiidDraw(uiidFixFileExtensionCase);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 1);
			uiidDraw(uiidFixFileCodepage);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 2);
			uiidDraw(uiidFixLineEnds);
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
					gMenu.fixWhat = gMenu.sel0;
					menuBack();
				}
			}
			if (wParam == wing::VK_ESCAPE) {
				menuBack();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
				gMenu.fixWhat = gMenu.sel0;
				menuBack();
			}
			break;
		}
	}
}

void menuFileExtensions(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			uiidDrawOption(gMenu.sel0 == 0);
			uiidDraw(uiidBack);
			gui.finishLine();

			gui.skipLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
					//gMenu.fixWhat = gMenu.sel0;
					gEdit.Hide();
					menuBack();
				}
			}
			if (wParam == wing::VK_ESCAPE) {
				gEdit.Hide();
				menuBack();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			if (gMenu.sel0 >= 0 && gMenu.sel0 <= gMenu.selMax) {
				gMenu.fixWhat = gMenu.sel0;
				gEdit.Hide();
				menuBack();
			}
			break;
		}
	}
}

void menuMain(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();
			
			uiidDrawOption(gMenu.sel0 == 0);
			uiidDraw(uiidLanguageChoice);
			uiidDraw(uiidBracketsStart);
			gui.text(uiLangStrings[gMenu.language], uiLangStringsLen[gMenu.language]);
			uiidDraw(uiidArrowDown);
			uiidDraw(uiidBracketsEnd);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 1);
			uiidDraw(uiidPath);
			uiidDraw(uiidBracketsStart);
			gui.text(gMenu.path);
			uiidDraw(uiidBracketsEnd);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 2);
			uiidDraw(uiidPathLog);
			uiidDraw(uiidBracketsStart);
			gui.text(gMenu.pathLog);
			uiidDraw(uiidBracketsEnd);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 3);
			uiidDraw(uiidFileExtensions);
			uiidDraw(uiidBracketsStart);
			gui.text(gMenu.fileExtensions);
			uiidDraw(uiidBracketsEnd);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == 4);
			uiidDraw(uiidFixWhat);
			uiidDraw(uiidBracketsStart);
			if (gMenu.fixWhat == 0) {
				uiidDraw(uiidFixFileExtensionCase);
			} else if (gMenu.fixWhat == 1) {
				uiidDraw(uiidFixFileCodepage);
			} else if (gMenu.fixWhat == 2) {
				uiidDraw(uiidFixLineEnds);
			} else ErrorLazy();
			uiidDraw(uiidArrowDown);
			uiidDraw(uiidBracketsEnd);
			gui.finishLine();

			u4 selCur = 5;
			u4 selWoodoo = 0;

			if (gMenu.fixWhat == 2) {
				uiidDrawOption(gMenu.sel0 == selCur++);
				uiidDraw(uiidFixHow);
				uiidDraw(uiidBracketsStart);
				if (gMenu.option0 == 0) {
					uiidDraw(uiidOption0RNtoN);
				} else {
					uiidDraw(uiidOption0NtoRN);
				}
				uiidDraw(uiidArrowDown);
				uiidDraw(uiidBracketsEnd);
				gui.finishLine();
			}

			if (gMenu.fixWhat == 1) {
				uiidDrawOption(gMenu.sel0 == selCur++);
				uiidDraw(uiidFixHow);
				uiidDraw(uiidBracketsStart);
				if (gMenu.option0 == 0) {
					gui.text(L"to UTF-8 BOM");
				} else {
					gui.text(L"to Shift_JIS");
				}
				uiidDraw(uiidArrowDown);
				uiidDraw(uiidBracketsEnd);
				gui.finishLine();
			}

			if (gMenu.sel0 == selCur) selWoodoo = 5;
			uiidDrawOption(gMenu.sel0 == selCur++);
			uiidDraw(uiidInspect);
			gui.finishLine();

			if (gMenu.sel0 == selCur) selWoodoo = 6;
			uiidDrawOption(gMenu.sel0 == selCur++);
			uiidDraw(uiidChange);
			gui.finishLine();

			uiidDrawOption(gMenu.sel0 == selCur++);
			uiidDraw(uiidHelp);
			gui.finishLine();

			if (gMenu.sel0 == 0) {
				uiidDrawOption(false);
				uiidDraw(uiidMainHint0);
				gui.finishLine();
				uiidDrawOption(false);
				uiidDraw(uiidMainHint00);
				gui.finishLine();
			} else if (gMenu.sel0 == 1) {
				uiidDrawOption(false);
				uiidDraw(uiidMainHint1);
				gui.finishLine();
			} else if (selWoodoo == 5) {
				uiidDrawOption(false);
				uiidDraw(uiidMainHint4);
				gui.finishLine();
				uiidDrawOption(false);
				uiidDraw(uiidMainHint40);
				gui.finishLine();
			} else if (selWoodoo == 6) {
				uiidDrawOption(false);
				uiidDraw(uiidMainHint5);
				gui.finishLine();
			}

			gui.finishWindow();
			//fiDrawProgress(&gfi, gui.hdc);
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.sel0++;
				if (gMenu.sel0 > gMenu.selMax) gMenu.sel0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.sel0--;
				if (gMenu.sel0 < 0) gMenu.sel0 = gMenu.selMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				menuMainEnter();
			}
			if (wParam == wing::VK_ESCAPE) {
				wincore::ExitProcess(0);
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.selMax) linePos = -1;
			gMenu.sel0 = linePos;
			//to not redraw too often
			if (gMenu.selprev != gMenu.sel0) {
				gMenu.selprev = gMenu.sel0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			menuMainEnter();
			break;
		}
	}
}

void menuDoActualWork(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();
			gfi.fio->OnDraw(&gfi, gui.hdc);
			//fiDrawProgress(&gfi, gui.hdc);
			gui.drawend_wm_paint();
			break;
		}
	}
}

void menu(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	if (gMenu.mode == modeMain) return menuMain(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeLangSelect) return menuLangChange(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeFileExtensions) return menuFileExtensions(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeFixWhat) return menuFixWhat(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeDoActualWork) return menuDoActualWork(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeHelp) return menuHelp(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeFixWhatLineend) return menuFixWhatLineend(hWnd, message, wParam, lParam);
	else if (gMenu.mode == modeFixWhatCodepage) return menuFixWhatCodepage(hWnd, message, wParam, lParam);
}

} //namespace main

u4 __stdcall edit_procedure(wing::HWND wnd, u4 msg, u4 wParam, u4 lParam)
{
	//return 0; if you don't want to pass it further to def proc
	switch (msg)
	{
	//case wing::WM_KEYUP:
	//	switch (wParam)
	//	{
	//	//case VK_SHIFT:
	//	//	edit_shift_down = false;
	//	//	break;
	//	//case VK_CONTROL:
	//	//	edit_control_down = false;
	//	//	break;
	//	}
	//	break;
	case wing::WM_KEYDOWN:
		switch (wParam)
		{
	//	//case VK_SHIFT:
	//	//	edit_shift_down = true;
	//	//	break;
	//	//case VK_CONTROL:
	//	//	edit_control_down = true;
	//	//	break;
		case wing::VK_RETURN:
			gEdit.Get(gMenu.fileExtensions, &gMenu.fileExtensionsLen, gMenu.fileExtensionsCap);
			gEdit.Hide();
			nmain::menuBack();
			break;
	//	//case VK_DOWN:
	//	//	if (displayed_line != displayed_line_max) {
	//	//		displayed_line++;
	//	//		need_redraw = true;
	//	//	}
	//	//	return 0;
	//	//case VK_UP:
	//	//	if (displayed_line != 0) {
	//	//		displayed_line--;
	//	//		need_redraw = true;
	//	//	}
	//	//	return 0;
	//	//case VK_NEXT: //page down
	//	//	if (displayed_line != displayed_line_max) {
	//	//		displayed_line += 10;
	//	//		if (displayed_line > displayed_line_max) displayed_line = displayed_line_max;
	//	//		need_redraw = true;
	//	//	}
	//	//	return 0;
	//	//case VK_PRIOR: //page up
	//	//	if (displayed_line != 0) {
	//	//		displayed_line-=10;
	//	//		if (displayed_line < 0) displayed_line = 0;
	//	//		need_redraw = true;
	//	//	}
	//	//	return 0;
	//	//case VK_HOME:
	//	//	if (displayed_line != 0) {
	//	//		displayed_line = 0;
	//	//		need_redraw = true;
	//	//	}
	//	//	return 0;
	//	//case VK_END:
	//	//	if (displayed_line != displayed_line_max) {
	//	//		displayed_line = displayed_line_max;
	//	//		need_redraw = true;
	//	//	}
 // //      case VK_BACK: //ctrl backspace will remove the whole thing
 // //          //LATER: make it remove only the last word
 // //          if (edit_control_down) {
 // //              //delete last word
	//	//		if (edit_text_len == 0) return 0;
	//	//		TCHAR* cur = edit_text + edit_text_len - 1;
	//	//		//in case there is more than one space
	//	//		do {
	//	//			if (*cur != TEXT(' ')) {
	//	//				break;
	//	//			}
	//	//		} while (--cur != edit_text);
	//	//		if (cur != edit_text) do {
	//	//			if (*cur == TEXT(' ')) {
	//	//				break;
	//	//			}
	//	//		} while (--cur != edit_text);
	//	//		*cur = 0;
	//	//		edit_text_len = cur - edit_text;
	//	//		SendMessage(hEdit, WM_SETTEXT, (WPARAM)0, (LPARAM)edit_text);
	//	//		if (edit_text_len != 0) {
	//	//			SendMessage(hEdit, EM_SETSEL, (WPARAM)edit_text_len, (LPARAM)edit_text_len);
	//	//		}
	//	//		search(0);
 // //          }
	//	//	break;
		case wing::VK_ESCAPE:
			//wing::SendMessageW(wnd, wing::WM_SETTEXT, 0, (u4)L"");
			gEdit.Hide();
			nmain::menuBack();
			//search(0);
			break;
		}
		break;
	case wing::WM_CHAR:
		//stopping the beeping sound
		if (wParam == wing::VK_RETURN) return 0;
		if (wParam == wing::VK_ESCAPE) return 0;
		//I get this (0x7f) if I press ctrl-backspace
		if (wParam == wing::VK_F16) return 0;
		break;
	}
	return wing::CallWindowProcW(gEdit.m_edit_procedure_old, wnd, msg, wParam, lParam);
}

u4 __stdcall WndProc(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam)
{
	nmain::menu(hWnd, message, wParam, lParam);
    switch (message)
    {
	case wing::WM_CTLCOLOREDIT:
    {
		gEdit.OnWM_CTLCOLOREDIT(wParam);
        //return (LRESULT)hbrushBlack;
        return null;
    }
	case wing::WM_SETFOCUS:
    {
		if (gEdit.mVisible) gEdit.Focus();
        return 0;
    }
	case wing::WM_CREATE:
		{
			gEdit.OnWmCreate(hWnd);
			break;
		}
	//case wing::WM_PAINT:
	//	{
	//		gui.drawstart_wm_paint();
	//		gui.text(t("hi"));
	//		gui.finishLine();
	//		gui.finishWindow();
	//		gui.drawend_wm_paint();
	//		break;
	//	}
	case wing::WM_SIZE:
		{
			gui.on_wm_size(lParam);

			//hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
			gEdit.OnWmSize();

			break;
		}
    case wing::WM_DESTROY:
        wincore::ExitProcess(0);
        //PostQuitMessage(0);
        break;
    case wing::WM_ERASEBKGND:
		{
			return 1; //to avoid filling the whole window with black on every WM_PAINT and reduce flickering
			break;
		}
    default:
        return wing::DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

int __stdcall wwWinMain()
{
    __asm {
        finit
    }

	uiid::uiidInit();

	//mem::MemInitGranular(&gmem0, 0xC0000, 0x02000000, 0x00020000);
	mem::MemInitGranular(&gmem0, 0, 0x02000000, 0x00020000);

	//test::cTest1* t = new test::cTest1;

	nmain::gMenuInit();

	gui.fontInit();

	gui.init(t("fix-things"), 0x200, 0x200, WndProc);

	while (gui.messageWaitFor())
    {
		gui.messageINeedWmChar();
		gui.messageProcess();
    }

	wincore::ExitProcess(0);
	
    return 0;
}

//int main() {
//	wwWinMain();
//}

int __stdcall wWinMain(
	u4 hInstance,
	u4 hPrevInstance,
	u4 lpCmdLine,
	u4 nShowCmd
)
{

	wwWinMain();

	return 0;
}


#ifdef __DMC__ //if compiled with digital mars c++ compiler
#pragma startaddress(wwWinMain)
#endif