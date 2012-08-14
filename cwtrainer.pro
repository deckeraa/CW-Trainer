CONFIG += qt thread warn_on release debug
HEADERS	= cwsound.h cwtest.h layout.h lcdrange.h 
SOURCES	= main.cpp cwsound.cpp layout.cpp lcdrange.cpp 
unix:LIBS += -lasound
