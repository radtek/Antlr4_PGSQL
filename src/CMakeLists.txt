aux_source_directory(${PROJECT_SOURCE_DIR}/src/cpp CPP_LIST)
SET(SRC_LIST main.cpp ${CPP_LIST})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(/usr/include/pcap)

INCLUDE_DIRECTORIES(/usr/local/antlr4-4.7.0/include/antlr4-runtime)
link_directories(/usr/local/antlr4-4.7.0/lib)
link_libraries(antlr4-runtime)

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(nids)

ADD_EXECUTABLE(MYHELLO ${SRC_LIST})

TARGET_LINK_LIBRARIES(MYHELLO pcap)
TARGET_LINK_LIBRARIES(MYHELLO net)
TARGET_LINK_LIBRARIES(MYHELLO gthread-2.0)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
