cmake_minimum_required(VERSION 3.15)
project(HanmiBackup)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows 플랫폼 확인
if(NOT WIN32)
    message(WARNING "This program is designed for Windows only")
endif()

# 실행 파일 생성 (GUI 애플리케이션)
add_executable(HanmiBackup WIN32 src/main.cpp)

# Windows 라이브러리 링크
target_link_libraries(HanmiBackup
    comctl32
    shell32
    shlwapi
)

# 출력 디렉토리 설정
set_target_properties(HanmiBackup PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Visual Studio용 설정
if(MSVC)
    # 유니코드 사용
    add_definitions(-DUNICODE -D_UNICODE)

    # 경고 레벨 설정
    target_compile_options(HanmiBackup PRIVATE /W4)

    # 멀티스레드 빌드
    target_compile_options(HanmiBackup PRIVATE /MP)
endif()

# MinGW용 설정
if(MINGW)
    # 유니코드 사용
    add_definitions(-DUNICODE -D_UNICODE)

    # 정적 링크 (독립 실행형 실행 파일)
    target_link_options(HanmiBackup PRIVATE -static-libgcc -static-libstdc++ -static)
endif()
