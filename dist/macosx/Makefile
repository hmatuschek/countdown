SYSTEM_LIBRARY_DIR = /opt/local/lib
SYSTEM_BINARY_DIR = /opt/local/bin
QTCORE_FRAMEWORK = /opt/local/Library/Frameworks/QtCore.framework/Versions/5/QtCore
QTGUI_FRAMEWORK = /opt/local/Library/Frameworks/QtGui.framework/Versions/5/QtGui
QTWIDGETS_FRAMEWORK = /opt/local/Library/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
QTXML_FRAMEWORK = /opt/local/Library/Frameworks/QtXml.framework/Versions/5/QtXml
QTSVG_FRAMEWORK = /opt/local/Library/Frameworks/QtSvg.framework/Versions/5/QtSvg
QTPRINT_FRAMEWORK = /opt/local/Library/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport
QT_PLUGINS = /opt/local/share/qt5/plugins
QT_PLUGINS_PLATFORM =${QT_PLUGINS}/platforms
QT_PLUGINS_PRINT =${QT_PLUGINS}/printsupport
QT_PLUGINS_ACCESSIBLE = ${QT_PLUGINS}/accessible
QT_PLUGINS_IMAGE =${QT_PLUGINS}/imageformats
QT_PLUGINS_AUDIO =${QT_PLUGINS}/audio
APP_LIBRARY_DIR = Countdown.app/Contents/Frameworks
APP_BINARY_DIR  = Countdown.app/Contents/MacOS
APP_PLUGIN_DIR  = Countdown.app/Contents/PlugIns
APP_RESOURCE_DIR  = Countdown.app/Contents/Resources
VERSION = 1.2.2

app: dmg

clean:
	rm -f ${APP_BINARY_DIR}/countdown
	rm -f ${APP_LIBRARY_DIR}/*
	rm -f Countdown-${VERSION}.dmg

dmg: countdown-app
	cp countdown.icns ${APP_RESOURCE_DIR}
	hdiutil create -srcfolder Countdown.app countdown-${VERSION}.dmg

countdown-app:
	# Install plugins
	cp $(SYSTEM_BINARY_DIR)/countdown ${APP_BINARY_DIR}
	mkdir -p ${APP_PLUGIN_DIR}/platforms
	cp ${QT_PLUGINS_PLATFORM}/libqcocoa.dylib ${APP_PLUGIN_DIR}/platforms
	mkdir -p ${APP_PLUGIN_DIR}/printsupport
	cp ${QT_PLUGINS_PRINT}/libcocoaprintersupport.dylib ${APP_PLUGIN_DIR}/printsupport
	mkdir -p ${APP_PLUGIN_DIR}/accessible
	cp ${QT_PLUGINS_ACCESSIBLE}/libqtaccessiblewidgets.dylib ${APP_PLUGIN_DIR}/accessible
	mkdir -p ${APP_PLUGIN_DIR}/audio
	cp ${QT_PLUGINS_AUDIO}/libqtaudio_coreaudio.dylib  ${APP_PLUGIN_DIR}/audio
	macdeployqt Countdown.app -verbose=2 -always-overwrite
