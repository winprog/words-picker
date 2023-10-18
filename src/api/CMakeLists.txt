set(TARGET_NAME api)

aux_source_directory(. DIR_LIB_SRCS)

# Find FFmpeg libraries
find_package(FFmpeg REQUIRED)
add_library (${TARGET_NAME} ${DIR_LIB_SRCS})
target_link_libraries(${TARGET_NAME} ${FFMPEG_LIBRARIES} swresample)
target_include_directories(${TARGET_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})


# 导入Qt包，需要放在add_executable之后
set(QT Core Network Widgets)
find_package(Qt5 REQUIRED ${QT})

qt5_use_modules(${TARGET_NAME} ${QT})
