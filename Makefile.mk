SOURCE = src/ae.c src/anet.c src/adlist.c src/Win32_Interop/*.c src/Win32_Interop/*.cpp
LIB_TARGET = libae.lib
LIBS = ws2_32.lib User32.lib libae.lib
LINK_FLAG = /DEBUG /subsystem:console

$(LIB_TARGET):
	@cl /EHsc /D_WIN32 /c $(SOURCE)
	@link -lib *.obj /out:$(LIB_TARGET)
	
timer:
	@cl /c example\timer.c
	@link $(LINK_FLAG) timer.obj $(LIBS) /OUT:timer.exe
	
echo:
	@cl /c example\echo.c
	@link $(LINK_FLAG) echo.obj $(LIBS) /OUT:echo.exe

clean:
	@del *.exe
	@del *.obj
	@del *.lib