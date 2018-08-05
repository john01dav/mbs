set(MBS_MODULE_libmbs_FILES
    ./modules/libmbs/include/mbs/MbsProject.h
    ./modules/libmbs/include/mbs/LibraryModule.h
    ./modules/libmbs/include/mbs/Module.h
    ./modules/libmbs/include/mbs/MbsException.h
    ./modules/libmbs/include/mbs/ApplicationModule.h
    ./modules/libmbs/source/MbsProject.cpp
    ./modules/libmbs/source/ApplicationModule.cpp
    ./modules/libmbs/source/LibraryModule.cpp
    ./modules/libmbs/source/Module.cpp
)
add_library(module-libmbs-static STATIC ${MBS_MODULE_libmbs_FILES})
target_include_directories(module-libmbs-static PRIVATE modules/libmbs/include)
set_target_properties(module-libmbs-static PROPERTIES OUTPUT_NAME libmbs)

set(MBS_MODULE_mbs_FILES
    ./modules/mbs/source/main.cpp
)
add_executable(module-mbs ${MBS_MODULE_mbs_FILES})
target_include_directories(module-mbs PRIVATE modules/mbs/include)
target_include_directories(module-mbs PRIVATE modules/libmbs/include)
set_target_properties(module-mbs PROPERTIES OUTPUT_NAME mbs)

target_link_libraries(module-mbs module-libmbs-static)
