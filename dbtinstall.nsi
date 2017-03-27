; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

; 安装程序初始定义常量
!define PRODUCT_NAME "余杭大板同"
!define PRODUCT_VERSION "V4.0 正式版"
!define PRODUCT_PUBLISHER "Lazypos"
!define PRODUCT_WEB_SITE "http://www.yhdbt.pw"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\余杭大板同.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "C:\Users\zcw\Desktop\余杭大板同\yhdbt\res\game.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!define MUI_FINISHPAGE_RUN "$INSTDIR\yhdbt\余杭大板同.exe"
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\yhdbt"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  CreateDirectory "$SMPROGRAMS\余杭大板同"
  CreateShortCut "$SMPROGRAMS\余杭大板同\余杭大板同.lnk" "$INSTDIR\yhdbt\余杭大板同.exe"
  CreateShortCut "$DESKTOP\余杭大板同.lnk" "$INSTDIR\yhdbt\余杭大板同.exe"
  File "C:\Users\zcw\Desktop\余杭大板同\vc_redist_x86-2012.exe"
  File "C:\Users\zcw\Desktop\余杭大板同\vc_redist_x86-2013.exe"
  File "C:\Users\zcw\Desktop\余杭大板同\vc_redist_x86-2015.exe"
  SetOutPath "$INSTDIR\yhdbt\config"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\config\strings.plist"
  SetOutPath "$INSTDIR\yhdbt\fonts"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\fonts\arial.ttf"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\fonts\Marker Felt.ttf"
  SetOutPath "$INSTDIR\yhdbt\sound"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\3.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\bj.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\at.wav"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\bg.wav"
	File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\boom.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\by.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\cp.wav"
	File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\jg.wav"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\lose.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\win.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\zb.mp3"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sound\cw.wav"
  SetOutPath "$INSTDIR\yhdbt"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\game.res"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\glew32.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\iconv.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libcocos2d.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libcurl.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libmpg123.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libogg.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libtiff.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libvorbis.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\libvorbisfile.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\noput.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\noput_press.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\num.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\OpenAL32.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\poker_flower.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\poker_num.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\poker_surface.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\put.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\put_press.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\ready.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\ready_press.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\start.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\hbj.jpg"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\bj.png"
  SetOutPath "$INSTDIR\yhdbt\res"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\res\.gitkeep"
  SetOutPath "$INSTDIR\yhdbt"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\return.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\return_press.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sqlite3.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\sucai.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\zhunbei.png"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\zlib1.dll"
  File "C:\Users\zcw\Desktop\余杭大板同\yhdbt\余杭大板同.exe"
SectionEnd

Function .onInit
	killer::IsProcessRunning "余杭大板同.exe"
	MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "安装前将强制关闭 余杭大板同 及其所有的组件，是否继续？" IDYES +2
  Abort
  KillProcDLL::KillProc "余杭大板同.exe"
FunctionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\余杭大板同\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\余杭大板同\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Install
  ExecWait "$INSTDIR\vc_redist_x86-2012.exe"
  ExecWait "$INSTDIR\vc_redist_x86-2013.exe"
  ExecWait "$INSTDIR\vc_redist_x86-2015.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\余杭大板同.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\余杭大板同.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\余杭大板同.exe"

  Delete "$SMPROGRAMS\余杭大板同\Uninstall.lnk"
  Delete "$SMPROGRAMS\余杭大板同\Website.lnk"
  Delete "$DESKTOP\余杭大板同.lnk"
  Delete "$SMPROGRAMS\余杭大板同\余杭大板同.lnk"

  RMDir "$SMPROGRAMS\余杭大板同"

  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 $(^Name) ，及其所有的组件？" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。"
FunctionEnd
