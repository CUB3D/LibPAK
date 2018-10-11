//
// Created by cub3d on 06/09/18.
//

#ifndef LIBPAK_DEFINITIONS_H
#define LIBPAK_DEFINITIONS_H


// Handle declspec for cross platform
#if COMPILING && _WIN32
    #define EXPORT __declspec(dllexport)
#elif _WIN32
    #define EXPORT __declspec(dllimport)
#endif

#ifdef linux
    #define EXPORT
#endif


#define max(a, b) ((a) < (b)) ? b : a

#endif //LIBPAK_DEFINITIONS_H
