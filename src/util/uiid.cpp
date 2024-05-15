
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
	L"��",
	L"��/A:",
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
	L"�u",
	L"�v",
	L"��",
	L"��/A:",
	L"�t�H���_�p�X:",
	L"Log Path:",
	L"�ϊ��Ώ�:",
	L"�g���q",
	L"�����R�[�h",
	L"���s�R�[�h",
	L"�u�ϊ��Ώۂ̃`�F�b�N�v",
	L"�u�ϊ������s�v",
	L"�u�w���v�v",
	L"�u�߂�v",

	//uiidMainHint0
	L"�}�E�X����������͖��L�[��Enter�L�[�ő���ł��܂��B",
	L"Esc�L�[�ŏI�����܂�",

	L"�N���b�N�ŃN���b�v�{�[�h����y�[�X�g���܂��B",

	L"�����ύX���܂���B",
	L"�ϊ��Ώۂ��ufix-things-out.txt�v�t�@�C���ɏo�͂��܂��B",

	L"�ϊ����J�n���܂��B",

	L"fix-things version " VERSION L".",
    L"���̃v���O�����͑Ώۂ̃t�@�C���Ɉȉ��̏C���������܂��B�g���q�̕\�L����A",
    L"�����R�[�h�AShift_JIS��UTF-8 BOM�̓���A",
    L"���s�R�[�h�u\\r\\n�v�Ɓu\\n�v�̓���",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":�t�@�C���ꗗ��:",
	L":�t�@�C���ꗗ��:",
	L":�t�@�C������:",
	L":���l�[�����ꂽ�t�@�C������:",
	L":���l�[�������t�@�C����:",
	L":�������ꂽ�t�@�C����:",
	L":���l�[�����ꂽ�t�@�C��:",
	L":���l�[�����K�v�ȃt�@�C��:",

	L"�����B�ufix-things-out.txt�v�ɏo�͂��܂����B",
	L"�G���[�B �ufix-things-out.txt�v�ɏo�͂��܂����B",
	L"�����B�ufix-things-out.txt�v�ɏo�͂ł��܂���ł����B",
	L"�G���[�B�ufix-things-out.txt�v�ɏo�͂ł��܂���ł����B",
	L":�G���[:�t�@�C����0x00800000���傫�����߃e�X�g�ł��܂���B:",
	L":�G���[:fi->filebuf1size�����������܂�:",
	L":�G���[:�������s���B:",
	L":�G���[:�t�H���_�����݂��܂���B:",
	L":�G���[:\\n�̌��\\r�������Ă���s������܂�:",

	L":�������ꂽ�t�@�C��:",
	L":UTF-8 BOM�łȂ��t�@�C��:",
	L":^�ϊ�����:",
	L":^�ϊ����s:",
	L":UTF8�̃t�@�C����:",
	L":UTF8�łȂ��t�@�C����:",
	L":�ϊ����������t�@�C����:",
	L":�ϊ����s�����t�@�C����:",

	L":���s�R�[�h RN:",
	L":���s�R�[�h N:",
	L":���s�R�[�h ����:",
	L":���s�R�[�h ����:",

	L"���s�R�[�h:",
	L"�u\\r\\n�v����u\\n�v�ɕϊ�",
	L"�u\\n�v����u\\r\\n�v�ɕϊ�",

    L"UTF-8 BOM�ɕϊ�",
    L"Shift_JIS�ɕϊ�",

    L"�t�@�C���g���q:",
};

const wchar* uiStringsRU[uiidTotalLength] = {
	L" ",
	L">",
	L"[",
	L"]",
	L"��",
	L"��/A:",
	L"�P�p���{�p:",
	L"�P�p���{�p �t�|�� �|���s�p:",
	L"�X���� �y�������p�r�y����:",
	L"���p�����y���u�~�y�� ���p�z�|���r",
	L"�{���t�y�����r�{�y ���p�z�|���r",
	L"���u���u�~���� ���������{",
	L"[�P�����r�u���y����]",
	L"[�P�������p�r�y����]",
	L"[�R�����p�r�{�p]",
	L"[�N�p�x�p�t]",

	//uiidMainHint0
	L"�T�����p�r�|�u�~�y�u: �}������, �������u�|�{�y, Enter,",
	L"�r�������t �~�p Escape.",

	L"�Z�v�|�{�~�y���u �������q�� �r�����p�r�y���� �������� �{ ���p���{�u.",

	L"�N�y���u�s�� �~�u �}�u�~���u��,",
	L"�����|���{�� �x�p���y�����r�p�u�� �������v�� �r fix-things-out.txt.",

	L"�I�x�}�u�~���u�� ���p�z�|��.",

	L"fix-things �r�u�����y�� " VERSION L".",
	L"�^���p �������s���p�}�}�p �y�������p�r�|���u�� ���u�s�y������ ���p�����y���u�~�y�z ���p�z�|���r,",
	L"�����u���q���p�x���u�� �{���t�y�����r�{�� ���p�z�|���r �}�u�w�t�� Shift_JIS �y UTF-8 BOM,",
	L"�y �����u���q���p�x���u�� �{���t �{���~���p ���������{�y �}�u�w�t�� \\r\\n �y \\n.",
	L"",
	L"https://gitlab.com/aimino777/fix-things",

	L":�����y�����{ ���p�z�|���r:",
	L":�{���~�u�� �����y���{�p ���p�z�|���r:",
	L":���p�z�|���r �y�����s��:",
	L":���p�z�|���r ���u���u�y�}�u�~���r�p�~��:",
	L":���p�z�|���r �~�p ���u���u�y�}�u�~���r�p�~�y�u:",
	L":���p�z�|���r �������y�s�~�����y�����r�p�~��:",
	L":���p�z�| ���u���u�y�}�u�~���r�p�~:",
	L":�~�p�t�� ���u���u�y�}�u�~���r�p����:",

	L"�C�������r��. fix-things-out.txt �x�p���y���p�~.",
	L"�O���y�q�{�p. fix-things-out.txt �x�p���y���p�~.",
	L"�C�������r��. fix-things-out.txt �~�u �}���s �q������ �x�p���y���p�~.",
	L"�O���y�q�{�p. fix-things-out.txt �~�u �}���s �q������ �x�p���y���p�~.",
	L":�O���y�q�{�p:�N�u �������r�u�����|���� �~�p ���p�z�|�p�� �q���|�����u ���u�} 0x00800000:",
	L":�O���y�q�{�p:fi->filebuf1size ���|�y���{���} �}�p�|��:",
	L":�O���y�q�{�p:�U�p�z�|�� �������v���p �~�u ���r�p���y�|�� ���p�}�����y:",
	L":�O���y�q�{�p:�P�p���{�p �~�u �������u�����r���u��:",
	L":�O���y�q�{��:�H�p�}�u���u�~ \\r ���|�u�t�������y�z �x�p \\n:",

	L":���p�z�| �������y�s�~�����y�����r�p�~:",
	L":�~�u utf-8-bom:",
	L":^���u���u�{���t�y�����r�p�~ �������u���~��:",
	L":^���u���u�{���t�y�����r�{�p �~�u ���t�p�|�p����:",
	L":���p�z�|�� Utf8:",
	L":���p�z�|�� �~�u Utf8:",
	L":���p�z�|���r ���u���u�{���t�y�����r�p�~�� �������u���~��:",
	L":���p�z�|���r ���u���u�{���t�y�����r�p�~�� �~�u�������u���~��:",

	L":�{���~�u�� ���������{�y RN:",
	L":�{���~�u�� ���������{�y N:",
	L":�{���~�u�� ���������{�y mixed:",
	L":�{���~�u�� ���������{�y none:",

	L"�K�p�{ �y�������p�r�y����:",
	L"\\r\\n �~�p \\n",
	L"\\n �~�p \\r\\n",

	L"to UTF-8 BOM",
	L"to Shift_JIS",

	L"�Q�p�����y���u�~�y��:",
};

const wchar* * uiStrings = uiStringsEN;

u2 uiStringsLen[uiidTotalLength];
//u2 uiStringsLenEn[uiidTotalLength];
//u2* uiStringsLen = uiStringsLenEn;

const wchar* uiLangStrings[uiidTotalLength] = {
	L"English",
	L"���{��",
	L"�Q�������{�y�z",
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
