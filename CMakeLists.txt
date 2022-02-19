project (BakrNN)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE DEBUG)
FILE( GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )

# COPY kernals
FILE ( GLOB KERNELS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/kernels/*.cl") 
foreach (KERNEL ${KERNELS})
message("${KERNEL}")
get_filename_component(kernel_name ${KERNEL} NAME)
configure_file(${PROJECT_SOURCE_DIR}/src/kernels/${kernel_name} kernels/${kernel_name} COPYONLY)
endforeach()
# END

#

add_library(${PROJECT_NAME}  SHARED ${SOURCES}  )
