#include <TWAPI/TW_platform.h>
#include <TW_platform_detect.h>

const char* TWAPI_GetPlatform( void )
{
#if defined(TWAPI_PLATFORM_PRIVATE)
    return TWAPI_PLATFORM_PRIVATE_NAME;
#elif defined(TWAPI_PLATFORM_ANDROID)
    return "Android";
#elif defined(TWAPI_PLATFORM_OPENHARMONY)
    return "OpenHarmony";
#elif defined(TWAPI_PLATFORM_WASI)
    return "WASI";
#elif defined(TWAPI_PLATFORM_EMSCRIPTEN)
    return "Emscripten";
#elif defined(TWAPI_PLATFORM_FUCHSIA)
    return "Fuchsia";
#elif defined(TWAPI_PLATFORM_REDOX)
    return "Redox";
#elif defined(TWAPI_PLATFORM_SERENITY)
    return "SerenityOS";
#elif defined(TWAPI_PLATFORM_MINIX)
    return "Minix";
#elif defined(TWAPI_PLATFORM_MORPHOS)
    return "MorphOS";
#elif defined(TWAPI_PLATFORM_AROS)
    return "AROS";
#elif defined(TWAPI_PLATFORM_AMIGAOS4)
    return "AmigaOS 4";
#elif defined(TWAPI_PLATFORM_MINT)
    return "Atari MiNT";
#elif defined(TWAPI_PLATFORM_HAIKU)
    return "Haiku";
#elif defined(TWAPI_PLATFORM_FREEBSD)
    #if defined(__DragonFly__)
        return "DragonFly BSD";
    #else
        return "FreeBSD";
    #endif
#elif defined(TWAPI_PLATFORM_NETBSD)
    return "NetBSD";
#elif defined(TWAPI_PLATFORM_OPENBSD)
    return "OpenBSD";
#elif defined(TWAPI_PLATFORM_HURD)
    return "GNU/Hurd";
#elif defined(TWAPI_PLATFORM_LINUX)
    return "Linux";
#elif defined(TWAPI_PLATFORM_MACOS)
    return "macOS";
#elif defined(TWAPI_PLATFORM_IOS)
    return "iOS";
#elif defined(TWAPI_PLATFORM_TVOS)
    return "tvOS";
#elif defined(TWAPI_PLATFORM_VISIONOS)
    return "visionOS";
#elif defined(TWAPI_PLATFORM_CYGWIN)
    return "Cygwin";
#elif defined(TWAPI_PLATFORM_WINRT)
    return "Windows RT/UWP";
#elif defined(TWAPI_PLATFORM_WIN32)
    return "Windows";
#elif defined(TWAPI_PLATFORM_DOS)
    return "MS-DOS";
#else
    return "Unknown";
#endif
}