cmake_minimum_required(VERSION 3.5)

if($ENV{TRAVIS_BUILD_NUMBER})
	project(poolAllocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
	project(poolAllocator VERSION 0.0.1)
endif()

set(CMAKE_CXX_STANDARD 14)

set(SOURCES src/main.cpp
)

# Main target
add_executable(poolAllocator ${SOURCES})

set_target_properties(poolAllocator PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)

target_include_directories(
	poolAllocator
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

if (MSVC)
	# @see https://github.com/google/googletest/blob/master/googletest/README.md
	# Visual Studio Dynamic vs Static Runtimes
	#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	target_compile_options(
		poolAllocator PRIVATE
		/W4
	)

else ()
	target_compile_options(
		poolAllocator PRIVATE
		-Wall -Wextra -pedantic #-Werror
	)
endif()

install(TARGETS poolAllocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT cnickol@gmail.com)

include(CPack)