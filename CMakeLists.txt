cmake_minimum_required( VERSION 2.8.11 )
project( lensshading )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g3 -std=c11 -D_GNU_SOURCE -D_BSD_SOURCE -Wfatal-errors" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -std=c++11 -fpermissive -Wno-pmf-conversions -Wfatal-errors" )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )

find_package( Qt5Widgets )
find_package( Qt5PrintSupport )

include_directories( ${CMAKE_BINARY_DIR} )

qt5_wrap_ui( UIS MainWindow.ui )

add_executable( lensshading
				main.cpp
				MainWindow.cpp
				${UIS}
				)

target_link_libraries( lensshading Qt5::Widgets Qt5::PrintSupport )

install( TARGETS lensshading RUNTIME DESTINATION bin )
