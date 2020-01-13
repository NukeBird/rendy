@echo off

set TOOLCHAIN=%ANDROID_NDK%\build\cmake\android.toolchain.cmake
set MAKE_PROG=%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe

set TARGET=%1

if %TARGET%==arm32 (
	mkdir build_android
	cd build_android
	mkdir arm32
	cd arm32
	call cmake -G"Unix Makefiles" -DCMAKE_MAKE_PROGRAM=%MAKE_PROG% -DANDROID_ABI=armeabi-v7a -DANDROID_ARM_NEON=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAIN% -DANDROID_NATIVE_API_LEVEL=19 -DCMAKE_C_FLAGS_RELEASE="-O3 -fsigned-char -std=c11 -g0" -DCMAKE_CXX_FLAGS_RELEASE="-Ofast -std=c++14 -frtti -fpermissive -fexceptions -Wall -fsigned-char -g0" -DANDROID_TOOLCHAIN=clang ../../
	call cmake --build . -- -j 4
	cd ../..
)
if %TARGET%==arm64 (
	mkdir build_android
	cd build_android
	mkdir arm64
	cd arm64
	call cmake -G"Unix Makefiles" -DCMAKE_MAKE_PROGRAM=%MAKE_PROG% -DANDROID_PLATFORM=android-21 -DANDROID_ABI=arm64-v8a -DANDROID_ARM_NEON=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAIN% -DANDROID_NATIVE_API_LEVEL=21 -DCMAKE_C_FLAGS_RELEASE="-O3 -fsigned-char -std=c11 -g0" -DCMAKE_CXX_FLAGS_RELEASE="-Ofast -std=c++14 -frtti -fpermissive -fexceptions -Wall -fsigned-char -g0" -DANDROID_TOOLCHAIN=clang ../../
	call cmake --build . -- -j 4
	cd ../..
)
