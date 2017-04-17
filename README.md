# openglbug
shows a bug in windows 7/10/? opengl driver used in safe mode and such

cmake help-

in windows,
SDL2_SEARCH_PATHS
in findsdl.cmake
must include
SDL2-2.0.4/i686-w64-mingw32
.......
but that does set... so how you do this??


#visual studio - right click - choose set startup project



Project -> Properties -> Configuration Properties -> Linker -> System
changing SubSystem
