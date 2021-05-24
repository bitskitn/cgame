VERSION = 1.0

PREFIX = /usr/local

EXENAME = game

CFLAGS = -std=c99 -Wall -DVERSION=\"${VERSION}\"

ifeq ("${BUILD}","release")
CFLAGS += -O2
LDFLAGS += -s
else
CFLAGS += -g -O0 -DDEBUG
endif

CC = gcc

ifeq ("${TARGET}","linux")
STARTUP = startup-linux.c
GRAPHICS = graphics-gl.c
LDFLAGS += -ldl -lX11 -lGLX
endif

ifeq ("${TARGET}","windows")
STARTUP = startup-windows.c
GRAPHICS = graphics-gdi.c graphics-gl.c
EXEEXT = .exe
CFLAGS += -D_WIN32
LDFLAGS += -lgdi32
endif

ifeq ("${TARGET}","psp")
EXTRA = ${PSP_EBOOT}
EXTRA_CLEAN = ${EXTRA} ${PSP_EBOOT_SFO}
STARTUP = startup-psp.c
GRAPHICS = graphics-scegu.c
EXEEXT = .elf
PSP_FW_VERSION = 150
PSP_EBOOT = EBOOT.PBP
PSP_EBOOT_TITLE = ${EXENAME}
PSP_EBOOT_SFO = PARAM.SFO
PSP_EBOOT_ICON = NULL
PSP_EBOOT_ICON1 = NULL
PSP_EBOOT_UNKPNG = NULL
PSP_EBOOT_PIC1 = NULL
PSP_EBOOT_SND0 = NULL
PSP_EBOOT_PSAR = NULL
PSPSDK = ${HOME}/../../pspsdk/psp/sdk
PACK_PBP = pack-pbp
MKSFO = mksfo
FIXUP = psp-fixup-imports
STRIP = psp-strip
CC = psp-gcc
CFLAGS += -G0 -D_PSP_FW_VERSION=$(PSP_FW_VERSION) \
         -I. -I${PSPSDK}/include
LDFLAGS += -L. -L${PSPSDK}/lib \
           -lpspgum -lpspgu -lm \
		   -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc \
		   -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver \
		   -lpsputility -lpspuser -lpspkernel
${PSP_EBOOT}: ${EXENAME}${EXEEXT} ${PSP_EBOOT_SFO}
	${FIXUP} ${EXENAME}${EXEEXT}
	${STRIP} ${EXENAME}${EXEEXT}
	${PACK_PBP} $@ ${PSP_EBOOT_SFO} ${PSP_EBOOT_ICON} \
		${PSP_EBOOT_ICON1} ${PSP_EBOOT_UNKPNG} ${PSP_EBOOT_PIC1} \
		${PSP_EBOOT_SND0} ${EXENAME}${EXEEXT} ${PSP_EBOOT_PSAR}
${PSP_EBOOT_SFO}:
	${MKSFO} '${PSP_EBOOT_TITLE}' $@
logo.o: logo.raw
	bin2o -i logo.raw logo.o logo
endif
