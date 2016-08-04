win32 {
#	DEFINES += USE_VLD VLD_FORCE_ENABLE
#	DEFINES += VLD_PER_PROJECT
#	INCLUDEPATH += "C:/Program Files (x86)/Visual Leak Detector/include"
#	LIBS += -L"C:/Program Files (x86)/Visual Leak Detector/lib/Win32"
}

#Required for TEST project in Debug mode
# DEFINES += QT_NO_DEBUG

#QMAKE_CXXFLAGS_RELEASE += -gdwarf-2
QMAKE_CXXFLAGS_RELEASE += -O2
#QMAKE_CFLAGS_RELEASE += -gdwarf-2
#QMAKE_LFLAGS_RELEASE =
