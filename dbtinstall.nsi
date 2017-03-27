; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "�ຼ���ͬ"
!define PRODUCT_VERSION "V4.0 ��ʽ��"
!define PRODUCT_PUBLISHER "Lazypos"
!define PRODUCT_WEB_SITE "http://www.yhdbt.pw"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\�ຼ���ͬ.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\res\game.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ��װĿ¼ѡ��ҳ��
!insertmacro MUI_PAGE_DIRECTORY
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
!define MUI_FINISHPAGE_RUN "$INSTDIR\yhdbt\�ຼ���ͬ.exe"
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\yhdbt"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  CreateDirectory "$SMPROGRAMS\�ຼ���ͬ"
  CreateShortCut "$SMPROGRAMS\�ຼ���ͬ\�ຼ���ͬ.lnk" "$INSTDIR\yhdbt\�ຼ���ͬ.exe"
  CreateShortCut "$DESKTOP\�ຼ���ͬ.lnk" "$INSTDIR\yhdbt\�ຼ���ͬ.exe"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\vc_redist_x86-2012.exe"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\vc_redist_x86-2013.exe"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\vc_redist_x86-2015.exe"
  SetOutPath "$INSTDIR\yhdbt\config"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\config\strings.plist"
  SetOutPath "$INSTDIR\yhdbt\fonts"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\fonts\arial.ttf"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\fonts\Marker Felt.ttf"
  SetOutPath "$INSTDIR\yhdbt\sound"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\3.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\bj.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\at.wav"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\bg.wav"
	File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\boom.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\by.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\cp.wav"
	File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\jg.wav"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\lose.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\win.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\zb.mp3"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sound\cw.wav"
  SetOutPath "$INSTDIR\yhdbt"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\game.res"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\glew32.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\iconv.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libcocos2d.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libcurl.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libmpg123.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libogg.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libtiff.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libvorbis.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\libvorbisfile.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\noput.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\noput_press.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\num.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\OpenAL32.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\poker_flower.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\poker_num.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\poker_surface.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\put.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\put_press.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\ready.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\ready_press.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\start.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\hbj.jpg"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\bj.png"
  SetOutPath "$INSTDIR\yhdbt\res"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\res\.gitkeep"
  SetOutPath "$INSTDIR\yhdbt"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\return.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\return_press.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sqlite3.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\sucai.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\zhunbei.png"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\zlib1.dll"
  File "C:\Users\zcw\Desktop\�ຼ���ͬ\yhdbt\�ຼ���ͬ.exe"
SectionEnd

Function .onInit
	killer::IsProcessRunning "�ຼ���ͬ.exe"
	MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��װǰ��ǿ�ƹر� �ຼ���ͬ �������е�������Ƿ������" IDYES +2
  Abort
  KillProcDLL::KillProc "�ຼ���ͬ.exe"
FunctionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\�ຼ���ͬ\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\�ຼ���ͬ\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Install
  ExecWait "$INSTDIR\vc_redist_x86-2012.exe"
  ExecWait "$INSTDIR\vc_redist_x86-2013.exe"
  ExecWait "$INSTDIR\vc_redist_x86-2015.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\�ຼ���ͬ.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\�ຼ���ͬ.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\�ຼ���ͬ.exe"

  Delete "$SMPROGRAMS\�ຼ���ͬ\Uninstall.lnk"
  Delete "$SMPROGRAMS\�ຼ���ͬ\Website.lnk"
  Delete "$DESKTOP\�ຼ���ͬ.lnk"
  Delete "$SMPROGRAMS\�ຼ���ͬ\�ຼ���ͬ.lnk"

  RMDir "$SMPROGRAMS\�ຼ���ͬ"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd
