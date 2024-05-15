
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
	L"«",
	L"‚ /A:",
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
	L"u",
	L"v",
	L"«",
	L"‚ /A:",
	L"ƒtƒHƒ‹ƒ_ƒpƒX:",
	L"Log Path:",
	L"•ÏŠ·‘ÎÛ:",
	L"Šg’£q",
	L"•¶šƒR[ƒh",
	L"‰üsƒR[ƒh",
	L"u•ÏŠ·‘ÎÛ‚Ìƒ`ƒFƒbƒNv",
	L"u•ÏŠ·‚ğÀsv",
	L"uƒwƒ‹ƒvv",
	L"u–ß‚év",

	//uiidMainHint0
	L"ƒ}ƒEƒX‘€ì‚à‚µ‚­‚Í–îˆóƒL[‚ÆEnterƒL[‚Å‘€ì‚Å‚«‚Ü‚·B",
	L"EscƒL[‚ÅI—¹‚µ‚Ü‚·",

	L"ƒNƒŠƒbƒN‚ÅƒNƒŠƒbƒvƒ{[ƒh‚©‚çƒy[ƒXƒg‚µ‚Ü‚·B",

	L"‰½‚à•ÏX‚µ‚Ü‚¹‚ñB",
	L"•ÏŠ·‘ÎÛ‚ğufix-things-out.txtvƒtƒ@ƒCƒ‹‚Éo—Í‚µ‚Ü‚·B",

	L"•ÏŠ·‚ğŠJn‚µ‚Ü‚·B",

	L"fix-things version " VERSION L".",
    L"‚±‚ÌƒvƒƒOƒ‰ƒ€‚Í‘ÎÛ‚Ìƒtƒ@ƒCƒ‹‚ÉˆÈ‰º‚ÌC³‚ğ‰Á‚¦‚Ü‚·BŠg’£q‚Ì•\‹L“ˆêA",
    L"•¶šƒR[ƒhAShift_JIS‚ÆUTF-8 BOM‚Ì“ˆêA",
    L"‰üsƒR[ƒhu\\r\\nv‚Æu\\nv‚Ì“ˆê",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":ƒtƒ@ƒCƒ‹ˆê——«:",
	L":ƒtƒ@ƒCƒ‹ˆê——ª:",
	L":ƒtƒ@ƒCƒ‹‘”:",
	L":ƒŠƒl[ƒ€‚³‚ê‚½ƒtƒ@ƒCƒ‹””:",
	L":ƒŠƒl[ƒ€‚³‚ê‚éƒtƒ@ƒCƒ‹”:",
	L":–³‹‚³‚ê‚½ƒtƒ@ƒCƒ‹”:",
	L":ƒŠƒl[ƒ€‚³‚ê‚½ƒtƒ@ƒCƒ‹:",
	L":ƒŠƒl[ƒ€‚ª•K—v‚Èƒtƒ@ƒCƒ‹:",

	L"Š®—¹Bufix-things-out.txtv‚Éo—Í‚µ‚Ü‚µ‚½B",
	L"ƒGƒ‰[B ufix-things-out.txtv‚Éo—Í‚µ‚Ü‚µ‚½B",
	L"Š®—¹Bufix-things-out.txtv‚Éo—Í‚Å‚«‚Ü‚¹‚ñ‚Å‚µ‚½B",
	L"ƒGƒ‰[Bufix-things-out.txtv‚Éo—Í‚Å‚«‚Ü‚¹‚ñ‚Å‚µ‚½B",
	L":ƒGƒ‰[:ƒtƒ@ƒCƒ‹‚ª0x00800000‚æ‚è‘å‚«‚¢‚½‚ßƒeƒXƒg‚Å‚«‚Ü‚¹‚ñB:",
	L":ƒGƒ‰[:fi->filebuf1size‚ª¬‚³‚·‚¬‚Ü‚·:",
	L":ƒGƒ‰[:ƒƒ‚ƒŠ•s‘«B:",
	L":ƒGƒ‰[:ƒtƒHƒ‹ƒ_‚ª‘¶İ‚µ‚Ü‚¹‚ñB:",
	L":ƒGƒ‰[:\\n‚ÌŒã‚É\\r‚ª‘±‚¢‚Ä‚¢‚és‚ª‚ ‚è‚Ü‚·:",

	L":–³‹‚³‚ê‚½ƒtƒ@ƒCƒ‹:",
	L":UTF-8 BOM‚Å‚È‚¢ƒtƒ@ƒCƒ‹:",
	L":^•ÏŠ·¬Œ÷:",
	L":^•ÏŠ·¸”s:",
	L":UTF8‚Ìƒtƒ@ƒCƒ‹”:",
	L":UTF8‚Å‚È‚¢ƒtƒ@ƒCƒ‹”:",
	L":•ÏŠ·¬Œ÷‚µ‚½ƒtƒ@ƒCƒ‹”:",
	L":•ÏŠ·¸”s‚µ‚½ƒtƒ@ƒCƒ‹”:",

	L":‰üsƒR[ƒh RN:",
	L":‰üsƒR[ƒh N:",
	L":‰üsƒR[ƒh ¬İ:",
	L":‰üsƒR[ƒh –³‚µ:",

	L"‰üsƒR[ƒh:",
	L"u\\r\\nv‚©‚çu\\nv‚É•ÏŠ·",
	L"u\\nv‚©‚çu\\r\\nv‚É•ÏŠ·",

    L"UTF-8 BOM‚É•ÏŠ·",
    L"Shift_JIS‚É•ÏŠ·",

    L"ƒtƒ@ƒCƒ‹Šg’£q:",
};

const wchar* uiStringsRU[uiidTotalLength] = {
	L" ",
	L">",
	L"[",
	L"]",
	L"«",
	L"‚ /A:",
	L"„P„p„„{„p:",
	L"„P„p„„{„p „t„|„‘ „|„€„s„p:",
	L"„X„„„€ „y„ƒ„„‚„p„r„y„„„:",
	L"„‚„p„ƒ„Š„y„‚„u„~„y„‘ „†„p„z„|„€„r",
	L"„{„€„t„y„‚„€„r„{„y „†„p„z„|„€„r",
	L"„„u„‚„u„~„€„ƒ „ƒ„„„‚„€„{",
	L"[„P„‚„€„r„u„‚„y„„„]",
	L"[„P„€„„‚„p„r„y„„„]",
	L"[„R„„‚„p„r„{„p]",
	L"[„N„p„x„p„t]",

	//uiidMainHint0
	L"„T„„‚„p„r„|„u„~„y„u: „}„„Š„, „ƒ„„„‚„u„|„{„y, Enter,",
	L"„r„„‡„€„t „~„p Escape.",

	L"„Z„v„|„{„~„y„„„u „‰„„„€„q„ „r„ƒ„„„p„r„y„„„ „„…„„„ „{ „„p„„{„u.",

	L"„N„y„‰„u„s„€ „~„u „}„u„~„‘„u„„,",
	L"„„„€„|„„{„€ „x„p„„y„ƒ„„r„p„u„„ „€„„„‰„v„„ „r fix-things-out.txt.",

	L"„I„x„}„u„~„‘„u„„ „†„p„z„|„.",

	L"fix-things „r„u„‚„ƒ„y„‘ " VERSION L".",
	L"„^„„„p „„‚„€„s„‚„p„}„}„p „y„ƒ„„‚„p„r„|„‘„u„„ „‚„u„s„y„ƒ„„„‚ „‚„p„ƒ„Š„y„‚„u„~„y„z „†„p„z„|„€„r,",
	L"„„‚„u„€„q„‚„p„x„…„u„„ „{„€„t„y„‚„€„r„{„… „†„p„z„|„€„r „}„u„w„t„… Shift_JIS „y UTF-8 BOM,",
	L"„y „„‚„u„€„q„‚„p„x„…„u„„ „{„€„t „{„€„~„ˆ„p „ƒ„„„‚„€„{„y „}„u„w„t„… \\r\\n „y \\n.",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":„ƒ„„y„ƒ„€„{ „†„p„z„|„€„r:",
	L":„{„€„~„u„ˆ „ƒ„„y„ƒ„{„p „†„p„z„|„€„r:",
	L":„†„p„z„|„€„r „y„„„€„s„€:",
	L":„†„p„z„|„€„r „„u„‚„u„y„}„u„~„€„r„p„~„€:",
	L":„†„p„z„|„€„r „~„p „„u„‚„u„y„}„u„~„€„r„p„~„y„u:",
	L":„†„p„z„|„€„r „„‚„€„y„s„~„€„‚„y„‚„€„r„p„~„€:",
	L":„†„p„z„| „„u„‚„u„y„}„u„~„€„r„p„~:",
	L":„~„p„t„€ „„u„‚„u„y„}„u„~„€„r„p„„„:",

	L"„C„€„„„€„r„€. fix-things-out.txt „x„p„„y„ƒ„p„~.",
	L"„O„Š„y„q„{„p. fix-things-out.txt „x„p„„y„ƒ„p„~.",
	L"„C„€„„„€„r„€. fix-things-out.txt „~„u „}„€„s „q„„„„ „x„p„„y„ƒ„p„~.",
	L"„O„Š„y„q„{„p. fix-things-out.txt „~„u „}„€„s „q„„„„ „x„p„„y„ƒ„p„~.",
	L":„O„Š„y„q„{„p:„N„u „„‚„€„r„u„‚„‘„|„ƒ„‘ „~„p „†„p„z„|„p„‡ „q„€„|„„Š„u „‰„u„} 0x00800000:",
	L":„O„Š„y„q„{„p:fi->filebuf1size „ƒ„|„y„Š„{„€„} „}„p„|„€:",
	L":„O„Š„y„q„{„p:„U„p„z„|„… „€„„„‰„v„„„p „~„u „‡„r„p„„„y„|„€ „„p„}„‘„„„y:",
	L":„O„Š„y„q„{„p:„P„p„„{„p „~„u „ƒ„…„‹„u„ƒ„„„r„…„u„„:",
	L":„O„Š„y„q„{„€:„H„p„}„u„‰„u„~ \\r „ƒ„|„u„t„…„„‹„y„z „x„p \\n:",

	L":„†„p„z„| „„‚„€„y„s„~„€„‚„y„‚„€„r„p„~:",
	L":„~„u utf-8-bom:",
	L":^„„u„‚„u„{„€„t„y„‚„€„r„p„~ „…„ƒ„„u„Š„~„€:",
	L":^„„u„‚„u„{„€„t„y„‚„€„r„{„p „~„u „…„t„p„|„p„ƒ„:",
	L":„†„p„z„|„ Utf8:",
	L":„†„p„z„|„ „~„u Utf8:",
	L":„†„p„z„|„€„r „„u„‚„u„{„€„t„y„‚„€„r„p„~„€ „…„ƒ„„u„Š„~„€:",
	L":„†„p„z„|„€„r „„u„‚„u„{„€„t„y„‚„€„r„p„~„€ „~„u„…„ƒ„„u„Š„~„€:",

	L":„{„€„~„u„ˆ „ƒ„„„‚„€„{„y RN:",
	L":„{„€„~„u„ˆ „ƒ„„„‚„€„{„y N:",
	L":„{„€„~„u„ˆ „ƒ„„„‚„€„{„y mixed:",
	L":„{„€„~„u„ˆ „ƒ„„„‚„€„{„y none:",

	L"„K„p„{ „y„ƒ„„‚„p„r„y„„„:",
	L"\\r\\n „~„p \\n",
	L"\\n „~„p \\r\\n",

	L"to UTF-8 BOM",
	L"to Shift_JIS",

	L"„Q„p„ƒ„Š„y„‚„u„~„y„‘:",
};

const wchar* * uiStrings = uiStringsEN;

u2 uiStringsLen[uiidTotalLength];
//u2 uiStringsLenEn[uiidTotalLength];
//u2* uiStringsLen = uiStringsLenEn;

const wchar* uiLangStrings[uiidTotalLength] = {
	L"English",
	L"“ú–{Œê",
	L"„Q„…„ƒ„ƒ„{„y„z",
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
