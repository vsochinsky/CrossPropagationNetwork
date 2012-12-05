TARGET= main
#linux
CC_LINUX= gcc
LIB_LINUX= -lm

#win32
CC_WIN32= i686-pc-mingw32-gcc
LIB_WIN32= -I/usr/i686-pc-mingw32/include -L/usr/i686-pc-mingw32/lib -lmingw32 -luser32 -lgdi32 -lwinmm -ldxguid
WIN_PREFIX=win32

linux:

	$(CC_LINUX) -o $(TARGET)_linux $(TARGET).c $(LIB_LINUX)
	strip $(TARGET)_linux

win32:

	$(CC_WIN32) -o $(TARGET)_$(WIN_PREFIX).exe $(TARGET).c $(LIB_WIN32) $(LIBSDL_WIN32) $(CFLAGS_WIN32)
	i686-pc-mingw32-strip $(TARGET)_$(WIN_PREFIX).exe

clean:
	rm -f $(TARGET)_linux $(TARGET)_$(WIN_PREFIX).exe
