all: so-cpp.exe

so-cpp.exe: main.c hashmap.c helpers.c
	cl main.c hashmap.c helpers.c -o so-cpp.exe /MD
	
clean:
	del /Q /F *.obj so-cpp.exe  