## Process this file with automake to produce Makefile.in

bin_PROGRAMS = openmortal
openmortal_SOURCES = Audio.cpp Backend.cpp common.cpp \
	Demo.cpp FighterStats.cpp FlyingChars.cpp Game.cpp gfx.cpp main.cpp \
	menu.cpp PlayerSelect.cpp RlePack.cpp sge_bm_text.cpp \
	sge_primitives.cpp sge_surface.cpp sge_tt_text.cpp State.cpp \
	GameOver.cpp

EXTRA_DIST = Audio.h Backend.h common.h Demo.h FlyingChars.h gfx.h \
	menu.h RlePack.h sge_bm_text.h sge_config.h sge_internal.h \
	sge_primitives.h sge_surface.h sge_tt_text.h State.h \
	Game.h \
	Audio.cpp Backend.cpp common.cpp \
	Demo.cpp FighterStats.cpp FlyingChars.cpp Game.cpp gfx.cpp main.cpp \
	menu.cpp PlayerSelect.cpp RlePack.cpp sge_bm_text.cpp \
	sge_primitives.cpp sge_surface.cpp sge_tt_text.cpp State.cpp \
	GameOver.cpp

CXXFLAGS= @CXXFLAGS@ -DDATADIR=\"${pkgdatadir}\" -Wall

# set the include path found by configure
#INCLUDES= $(all_includes)
 
# the library search path.
#msz_LDFLAGS = $(all_libraries) 
                                                                                
