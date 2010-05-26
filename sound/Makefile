#Generalized, self-updating Makefile
# Author: Harrison McKenzie Chapter (HMC)

CXX=g++
CXXFLAGS=-O3 -Wall
LDFLAGS=-Llib -lGL -lGLU -lglut -lSDL -lSDL_mixer -lm
OBJS= $(shell for i in *.cpp; do echo `basename $$i .cpp`.o; done)
PROG=program

all: depend $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) lib/libSDL_mixer.a

depend:
	makedepend *.cpp &> /dev/null

clean:
	rm *.o $(PROG) Makefile.bak
# DO NOT DELETE

sound.o: /usr/include/stdio.h /usr/include/features.h
sound.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
sound.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
sound.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
sound.o: /usr/include/bits/typesizes.h /usr/include/libio.h
sound.o: /usr/include/_G_config.h /usr/include/wchar.h
sound.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sound.o: /usr/include/signal.h /usr/include/bits/sigset.h
sound.o: /usr/include/bits/signum.h /usr/include/time.h
sound.o: /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h
sound.o: /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h
sound.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
sound.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h
sound.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
sound.o: /usr/include/sys/select.h /usr/include/bits/select.h
sound.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
sound.o: /usr/include/alloca.h /usr/include/SDL/SDL.h
sound.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
sound.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
sound.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/strings.h
sound.o: /usr/include/inttypes.h /usr/include/stdint.h
sound.o: /usr/include/bits/wchar.h /usr/include/ctype.h /usr/include/iconv.h
sound.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
sound.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
sound.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
sound.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
sound.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
sound.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
sound.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
sound.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
sound.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
sound.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
sound.o: /usr/include/SDL/SDL_version.h SDL_mixer.h
sound.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_rwops.h
sound.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_endian.h
sound.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/begin_code.h
sound.o: /usr/include/SDL/close_code.h
