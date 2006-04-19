SOURCES	+= RlePack.cpp \
	main.cpp \
	ImageEditor.cpp \
	ImageStorage.cpp \
	OutlineDetector.cpp
HEADERS	+= RlePack.h \
	ImageEditor.h \
	ImageStorage.h \
	OutlineDetector.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= mortaleditor.ui
IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/searchfind \
	images/zoomin.png \
	images/zoomout.png \
	images/xrefresh.png
TEMPLATE	=app
CONFIG	+= qt warn_on thread debug
LANGUAGE	= C++
