cmake_minimum_required(VERSION 3.5)
project(SCHEMATIC)

set(CMAKE_CXX_FLAGS "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
set(CMAKE_INSTALL_PREFIX "../install")

find_package(LibXml2 REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include_directories(${SCHEMATIC_SOURCE_DIR}
                    ${LIBXML2_INCLUDE_DIR}
                    ${Qt5_INCLUDE_DIRS})

set(includes Indentation.h
             XmlUtil.h 
             Point.h
             Box.h
             Term.h
             Net.h
             Instance.h
             Node.h
             Line.h
             Shape.h
             Symbol.h
             Cell.h)

set(mocIncludes SaveCellDialog.h
                OpenCellDialog.h
                CellWidget.h
                CellViewer.h
                InstancesWidget.h
                InstancesModel.h
                CellsLib.h
                CellsModel.h
                OpenAbout.h)

set(cpps Indentation.cpp
          XmlUtil.cpp 
          Point.cpp
          Box.cpp
          Term.cpp
          Net.cpp
          Instance.cpp
          Node.cpp
          Line.cpp
          Shape.cpp
          Symbol.cpp
          Cell.cpp
          SaveCellDialog.cpp
          OpenCellDialog.cpp
          CellWidget.cpp
          CellViewer.cpp
          InstancesWidget.cpp
          InstancesModel.cpp
          CellsLib.cpp
          CellsModel.cpp
          Main.cpp
          OpenAbout.cpp)

qt5_wrap_cpp(mocCpps ${mocIncludes})

add_executable(tme810 ${cpps} ${mocCpps})
target_link_libraries(tme810 Qt5::Core Qt5::Widgets ${LIBXML2_LIBRARIES})
install(TARGETS tme810 DESTINATION bin)
install(FILES ${includes} ${mocIncludes} DESTINATION include)
