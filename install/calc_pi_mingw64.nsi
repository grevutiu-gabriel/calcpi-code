; calc_pi.nsi
;
; This is an installer script for the Nullsoft Installer System (NSIS).
; See http://nsis.sourceforge.net/
;
;--------------------------------

; Use Modern UI 2
!include MUI2.nsh

!include "x64.nsh"

;Get installation folder from registry if available
InstallDirRegKey HKCU "Software\hiesti.ch\CalcPi64" ""

!searchparse /file ..\version.h "#define CALC_PI_VERSION_MAJOR " VER_MAJOR
!searchparse /file ..\version.h `#define CALC_PI_VERSION_MINOR ` VER_MINOR
!searchparse /file ..\version.h `#define CALC_PI_VERSION_BUILD ` VER_BUILD
!searchparse /file ..\version.h '#define CALC_PI_NAME "' APP_NAME '"'
!searchparse /file ..\version.h '#define CALC_PI_COPYRIGHT_NAME "' COMPANY_NAME '"'
!searchparse /file ..\version.h '#define CALC_PI_COPYRIGHT_YEAR "' COPYRIGHT_YEAR '"'

; The name of the installer
Name "CalcPi ${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"

; The file to write
OutFile "CalcPi_${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}_MinGW64_Setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES64\CalcPi

; Request application privileges for Windows Vista
RequestExecutionLevel admin

XPStyle on

; Set best compression
SetCompressor lzma

; Set file and version information in the installer itself
VIAddVersionKey "ProductName" "CalcPi_Setup"
VIAddVersionKey "Comments" "Installer for ${APP_NAME} x64"
VIAddVersionKey "CompanyName" "${COMPANY_NAME}"
VIAddVersionKey "LegalCopyright" "Copyright (C) ${COPYRIGHT_YEAR} ${COMPANY_NAME}"
VIAddVersionKey "FileDescription" "Installer for ${APP_NAME}"
VIAddVersionKey "FileVersion" "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
VIProductVersion "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.0"

; Do not automatically jump to the finish page, to allow the user to check the install log.
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\nsis.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\nsis.bmp"

Var StartMenuFolder

;--------------------------------

; Pages

!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\hiesti.ch\CalcPi64" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "CalcPi"
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES


!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Esperanto"
!insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------

; The section for the main program
Section "Main program" MainProgram

	SectionIn 1

	; Set output path to the installation directory.
	SetOutPath $INSTDIR

	; Store install path in registry
	WriteRegStr HKCU "Software\hiesti.ch\CalcPi64" "" $INSTDIR

	; Put files there
	File ..\..\build_mingw64tdm\calc_pi.exe
	File ..\..\build_mingw64tdm\libgomp_64-1.dll
	File ..\..\build_mingw64tdm\pthreadGC2_64.dll
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	; Insert links in start menu
	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
		CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\CalcPi.lnk" "$INSTDIR\calc_pi.exe"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  	!insertmacro MUI_STARTMENU_WRITE_END

	; Write uninstall routine and some additional info into registry
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "DisplayName" "CalcPi x64"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "QuietUninstallString" "$\"$INSTDIR\Uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "DisplayIcon" "$\"$INSTDIR\calc_pi.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "Publisher" "${COMPANY_NAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "DisplayVersion" "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
	WriteRegDword HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "NoModify" "1"
	WriteRegDword HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64" "NoRepair" "1"

SectionEnd

Function .onInit
	${IfNot} ${RunningX64}
		MessageBox MB_OK "This installer only works on a 64 bit version of Windows."
		Abort
	${EndIf}
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Uninstall" Uninstall
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\pthreadGC2_64.dll"
	Delete "$INSTDIR\libgomp_64-1.dll"
	Delete "$INSTDIR\calc_pi.exe"
	RMDir "$INSTDIR"

	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
	Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
	Delete "$SMPROGRAMS\$StartMenuFolder\CalcPi.lnk"
	RMDir "$SMPROGRAMS\$StartMenuFolder"
	DeleteRegKey /ifempty HKCU "Software\hiesti.ch\CalcPi64"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CalcPi64"

SectionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
