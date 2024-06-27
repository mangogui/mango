# Find Java and set paths (assuming JAVA_HOME is set in your environment)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

# Specify the source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mangoproject/mango/android/MainActivity.java
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mangoproject/mango/android/WidgetView.java
)

# Specify the output JAR file
set(OUTPUT_JAR ${CMAKE_BINARY_DIR}/android/libs/MangoAndroid.jar)
set(COMPILED_CLASSES_DIR ${CMAKE_BINARY_DIR}/classes)

set(ANDROID_SDK_PATH ${USER_HOME_DIR}/Library/Android/sdk)
set(ANDROID_NDK ${USER_HOME_DIR}/Library/Android/sdk/ndk/26.1.10909125)
set(ANDROID_JAR "${ANDROID_SDK_PATH}/platforms/android-34/android.jar")

# Ensure the classes directory exists
add_custom_command(
    OUTPUT ${COMPILED_CLASSES_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COMPILED_CLASSES_DIR}
    COMMENT "Creating classes directory"
)
#
# Compile Java files
add_custom_command(
    OUTPUT ${COMPILED_CLASSES_DIR}/mangoproject/mango/android/MainActivity.class ${COMPILED_CLASSES_DIR}/mangoproject/mango/android/WidgetView.class
    COMMAND ${CMAKE_COMMAND} -E echo "Using javac: ${Java_JAVAC_EXECUTABLE}"
    COMMAND ${CMAKE_COMMAND} -E echo "Using classpath: ${ANDROID_JAR}"
    COMMAND ${Java_JAVAC_EXECUTABLE} -classpath ${ANDROID_JAR} -d ${COMPILED_CLASSES_DIR} ${SOURCES}
    DEPENDS ${SOURCES} ${COMPILED_CLASSES_DIR}
    COMMENT "Compiling Java sources"
)

# Package into JAR file
add_custom_command(
    OUTPUT ${OUTPUT_JAR}
    COMMAND ${Java_JAR_EXECUTABLE} cf ${OUTPUT_JAR} -C ${COMPILED_CLASSES_DIR} .
    DEPENDS ${COMPILED_CLASSES_DIR}/mangoproject/mango/android/MainActivity.class ${COMPILED_CLASSES_DIR}/mangoproject/mango/android/WidgetView.class
    COMMENT "Creating JAR file"
)

# Add a custom target that depends on the JAR file
add_custom_target(
    CreateJar ALL
    DEPENDS ${OUTPUT_JAR}
    COMMENT "Building the JAR file"
)

# Add the necessary include directories
include_directories(${JNI_INCLUDE_DIRS})

install(FILES
    ${OUTPUT_JAR}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/jar
)