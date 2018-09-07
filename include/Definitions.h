//
// Created by cub3d on 06/09/18.
//

#ifndef LIBPAK_DEFINITIONS_H
#define LIBPAK_DEFINITIONS_H


#if COMPILING && WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __declspec(dllimport)
#endif

#ifdef UNIX
    #define EXPORT
#endif

#endif //LIBPAK_DEFINITIONS_H
