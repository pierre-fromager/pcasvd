cmake_minimum_required (VERSION 2.8)
project (Pca)

add_definitions(
    -O2 -Werror -Wall -Wextra -Wpedantic -std=c++17
	-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable 
	-Wno-format-nonliteral -Wcast-qual -Wno-strict-aliasing 
	-Wformat=2 -Wformat-security 
	-Wno-null-dereference  -Wstack-protector -Wtrampolines -Wvla 
	-Warray-bounds=2 -Wno-maybe-uninitialized
	-Wshift-overflow=2 
	-Wlogical-op -Wduplicated-cond 
	-Wformat-signedness -Wshadow 
	-Wno-undef -Wswitch-enum 
	-Wstack-usage=250000 
	-D_FORTIFY_SOURCE=2
    )
# -Wconversion -Wstrict-overflow=4 -Wundef -Wnull-dereference 

file (GLOB_RECURSE Pca_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE Pca_HEADERS CONFIGURE_DEPENDS "include/*.h")

set (Pca_INCLUDE_DIRS "")
foreach (_headerFile ${Pca_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Pca_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES Pca_INCLUDE_DIRS)

add_executable(pca ${Pca_SOURCES})
target_include_directories(pca PRIVATE ${Pca_INCLUDE_DIRS})

include_directories(${Pca_INCLUDE_DIRS})
add_library (pca_lib ${Pca_SOURCES} ${Pca_INCLUDE_DIRS})
target_link_libraries(pca pca_lib)

#enable_testing ()
#add_subdirectory (test)
