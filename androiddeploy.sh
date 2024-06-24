#!/bin/bash

# Define the path to adb executable
ADB_EXECUTABLE="/Users/parhamoyan/Library/Android/sdk/platform-tools/adb"

# Define the path to your APK file
APK_FILE="/Users/parhamoyan/Desktop/MangoWidget/cmake-build-android/android/build/outputs/apk/debug/My Application-debug.apk"

PACKAGE_NAME="mangoproject.mango.android"
MAIN_ACTIVITY_NAME="mangoproject.mango.android.MainActivity"

# Check if any emulator is already running
EMULATOR_RUNNING=$(${ADB_EXECUTABLE} devices | grep -w "emulator-5554")

if [ -z "$EMULATOR_RUNNING" ]; then
  # Start the emulator if it's not running
  /Users/parhamoyan/Library/Android/sdk/emulator/emulator -avd Pixel_8_Pro_API_VanillaIceCream &

  # Wait for the emulator to boot up
  ${ADB_EXECUTABLE} wait-for-device
else
  echo "Emulator is already running."
fi

# Install the APK on the emulator
${ADB_EXECUTABLE} install "${APK_FILE}"

# Launch the main activity of the app
${ADB_EXECUTABLE} shell am start -n ${PACKAGE_NAME}/${MAIN_ACTIVITY_NAME}