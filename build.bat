if not exist build mkdir build
if not exist lib mkdir lib

gcc -std=c11 -O3 -Wall -Wextra -pedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wstrict-aliasing -D_FORTIFY_SOURCE=2 -Isrc -Iinclude -Ithird-party-includes -c src/TWAPI.c -o build/TWAPI.o
gcc -std=c11 -O3 -Wall -Wextra -pedantic -Wstrict-prototypes -Wold-style-definition -Wno-error=missing-prototypes -Wno-error=stringop-overflow -Isrc -Iinclude -Ithird-party-includes -c src/TW_audio.c -o build/TW_audio.o
gcc -std=c11 -O3 -Wall -Wextra -pedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wstrict-aliasing -D_FORTIFY_SOURCE=2 -Isrc -Iinclude -Ithird-party-includes -c src/TW_framebuffer.c -o build/TW_framebuffer.o
gcc -std=c11 -O3 -Wall -Wextra -pedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wstrict-aliasing -D_FORTIFY_SOURCE=2 -Isrc -Iinclude -Ithird-party-includes -c src/TW_platform.c -o build/TW_platform.o
gcc -std=c11 -O3 -Wall -Wextra -pedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wstrict-aliasing -D_FORTIFY_SOURCE=2 -Isrc -Iinclude -Ithird-party-includes -c src/TW_window.c -o build/TW_window.o
gcc -std=c11 -O3 -Wall -Wextra -pedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wstrict-aliasing -D_FORTIFY_SOURCE=2 -Isrc -Iinclude -Ithird-party-includes -c src/TW_win32.c -o build/TW_win32.o

ar rcs lib\libTimWorksAPI.a build\TWAPI.o build\TW_audio.o build\TW_framebuffer.o build\TW_platform.o build\TW_window.o build\TW_win32.o

ar t lib\libTimWorksAPI.a

:: gcc -std=c11 -O3 -Wall -Wextra -pedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wstrict-aliasing -D_FORTIFY_SOURCE=2 -Isrc -Iinclude -Ithird-party-includes -c src/TW_audio.c -o build/TW_audio.o