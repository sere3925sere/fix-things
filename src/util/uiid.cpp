
#include "uiid.h"

#include "miniutil.h"
#include "ui.h"
//#include "../main.h" //just for gui variable

namespace uiid {

//const wchar* version = L"3";
#define VERSION L"4.1"

//GLOBALS
const wchar* uiStringsEN[uiidTotalLength] = {
	L" ",
	L">",
	L"[",
	L"]",
	L"↓",
	L"あ/A:",
	L"Path:",
	L"Log Path:",
	L"Fix:",
	L"file extension case",
	L"file codepage",
	L"line ends",
	L"[Inspect]",
	L"[Change]",
	L"[Help]",
	L"[Back]",

	//uiidMainHint0
	L"Use mouse or arrow keys + enter to select,",
	L"escape to exit.",

	L"Click to paste path.",

	L"Does not change anything,",
	L"only writes fix-things-out.txt file.",

	L"Changes files.",

	L"fix-things version " VERSION L".",
	L"This program fixes file extensions case,",
	L"converts between Shift_JIS and UTF-8 BOM,",
	L"and changes newline codes between \\r\\n and \\n.",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":filelist start:",
	L":filelist end:",
	L":filesTotal:",
	L":filesRenamed:",
	L":filesToRename:",
	L":filesIgnored:",
	L":file renamed:",
	L":needs renaming:",

	L"Done. fix-things-out.txt written.",
	L"Error. fix-things-out.txt written.",
	L"Done. fix-things-out.txt couldn't be written.",
	L"Error. fix-things-out.txt couldn't be written.",
	L":Error:Not tested on files bigger than 0x00800000:",
	L":Error:fi->filebuf1size is too small:",
	L":Error:Logfile out of memory:",
	L":Error:Folder doesn't exist:",
	L":Error:Spotted \\r followed by \\n:",

	L":file ignored:",
	L":not UTF-8 BOM:",
	L":^converted successfully:",
	L":^conversion failed:",
	L":filesUtf8:",
	L":filesNotUtf8:",
	L":filesConvertSuccessful:",
	L":filesConvertFail:",

	L":lineend RN:",
	L":lineend N:",
	L":lineend mixed:",
	L":lineend none:",

	L"Fix how:",
	L"\\r\\n to \\n",
	L"\\n to \\r\\n",

	L"to UTF-8 BOM",
	L"to Shift_JIS",

	L"File extensions:",

};

const wchar* uiStringsJP[uiidTotalLength] = {
	L" ",
	L">",
	L"「",
	L"」",
	L"↓",
	L"あ/A:",
	L"フォルダパス:",
	L"Log Path:",
	L"変換対象:",
	L"拡張子",
	L"文字コード",
	L"改行コード",
	L"「変換対象のチェック」",
	L"「変換を実行」",
	L"「ヘルプ」",
	L"「戻る」",

	//uiidMainHint0
	L"マウス操作もしくは矢印キーとEnterキーで操作できます。",
	L"Escキーで終了します",

	L"クリックでクリップボードからペーストします。",

	L"何も変更しません。",
	L"変換対象を「fix-things-out.txt」ファイルに出力します。",

	L"変換を開始します。",

	L"fix-things version " VERSION L".",
    L"このプログラムは対象のファイルに以下の修正を加えます。拡張子の表記統一、",
    L"文字コード、Shift_JISとUTF-8 BOMの統一、",
    L"改行コード「\\r\\n」と「\\n」の統一",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":ファイル一覧↓:",
	L":ファイル一覧↑:",
	L":ファイル総数:",
	L":リネームされたファイル数数:",
	L":リネームされるファイル数:",
	L":無視されたファイル数:",
	L":リネームされたファイル:",
	L":リネームが必要なファイル:",

	L"完了。「fix-things-out.txt」に出力しました。",
	L"エラー。 「fix-things-out.txt」に出力しました。",
	L"完了。「fix-things-out.txt」に出力できませんでした。",
	L"エラー。「fix-things-out.txt」に出力できませんでした。",
	L":エラー:ファイルが0x00800000より大きいためテストできません。:",
	L":エラー:fi->filebuf1sizeが小さすぎます:",
	L":エラー:メモリ不足。:",
	L":エラー:フォルダが存在しません。:",
	L":エラー:\\nの後に\\rが続いている行があります:",

	L":無視されたファイル:",
	L":UTF-8 BOMでないファイル:",
	L":^変換成功:",
	L":^変換失敗:",
	L":UTF8のファイル数:",
	L":UTF8でないファイル数:",
	L":変換成功したファイル数:",
	L":変換失敗したファイル数:",

	L":改行コード RN:",
	L":改行コード N:",
	L":改行コード 混在:",
	L":改行コード 無し:",

	L"改行コード:",
	L"「\\r\\n」から「\\n」に変換",
	L"「\\n」から「\\r\\n」に変換",

    L"UTF-8 BOMに変換",
    L"Shift_JISに変換",

    L"ファイル拡張子:",
};

const wchar* uiStringsRU[uiidTotalLength] = {
	L" ",
	L">",
	L"[",
	L"]",
	L"↓",
	L"あ/A:",
	L"Папка:",
	L"Папка для лога:",
	L"Что исправить:",
	L"расширения файлов",
	L"кодировки файлов",
	L"перенос строк",
	L"[Проверить]",
	L"[Поправить]",
	L"[Справка]",
	L"[Назад]",

	//uiidMainHint0
	L"Управление: мышь, стрелки, Enter,",
	L"выход на Escape.",

	L"Щёлкните чтобы вставить путь к папке.",

	L"Ничего не меняет,",
	L"только записывает отчёт в fix-things-out.txt.",

	L"Изменяет файлы.",

	L"fix-things версия " VERSION L".",
	L"Эта программа исправляет регистр расширений файлов,",
	L"преобразует кодировку файлов между Shift_JIS и UTF-8 BOM,",
	L"и преобразует код конца строки между \\r\\n и \\n.",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":список файлов:",
	L":конец списка файлов:",
	L":файлов итого:",
	L":файлов переименовано:",
	L":файлов на переименование:",
	L":файлов проигнорировано:",
	L":файл переименован:",
	L":надо переименовать:",

	L"Готово. fix-things-out.txt записан.",
	L"Ошибка. fix-things-out.txt записан.",
	L"Готово. fix-things-out.txt не мог быть записан.",
	L"Ошибка. fix-things-out.txt не мог быть записан.",
	L":Ошибка:Не проверялся на файлах больше чем 0x00800000:",
	L":Ошибка:fi->filebuf1size слишком мало:",
	L":Ошибка:Файлу отчёта не хватило памяти:",
	L":Ошибка:Папка не существует:",
	L":Ошибко:Замечен \\r следующий за \\n:",

	L":файл проигнорирован:",
	L":не utf-8-bom:",
	L":^перекодирован успешно:",
	L":^перекодировка не удалась:",
	L":файлы Utf8:",
	L":файлы не Utf8:",
	L":файлов перекодировано успешно:",
	L":файлов перекодировано неуспешно:",

	L":конец строки RN:",
	L":конец строки N:",
	L":конец строки mixed:",
	L":конец строки none:",

	L"Как исправить:",
	L"\\r\\n на \\n",
	L"\\n на \\r\\n",

	L"to UTF-8 BOM",
	L"to Shift_JIS",

	L"Расширения:",
};

const wchar* * uiStrings = uiStringsEN;

u2 uiStringsLen[uiidTotalLength];
//u2 uiStringsLenEn[uiidTotalLength];
//u2* uiStringsLen = uiStringsLenEn;

const wchar* uiLangStrings[uiidTotalLength] = {
	L"English",
	L"日本語",
	L"Русский",
};

const wchar* * uiLangs[uiidTotalLength] = {
	uiStringsEN,
	uiStringsJP,
	uiStringsRU,
};

u2 uiLangStringsLen[uiLangIdTotalLength];

void uiidInit() {
	for (u4 i = 0; i < uiidTotalLength; i++) {
		uiStringsLen[i] = ustr::wcharLen(uiStrings[i]);
	}
	for (u4 i = 0; i < uiLangIdTotalLength; i++) {
		uiLangStringsLen[i] = ustr::wcharLen(uiLangStrings[i]);
	}
}

void uiidDraw(u2 id) {
	gui.text(uiStrings[id], uiStringsLen[id]);
}

void uiidDrawOption(bool active) {
	uiidDraw(uiidSpace);
	if (active) uiidDraw(uiidSelect);
	else uiidDraw(uiidSpace);
}

}
