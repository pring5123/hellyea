# 한미 폴더 백업 프로그램

![Build Status](https://github.com/pring5123/hellyea/workflows/Build%20Windows%20Backup%20Program/badge.svg)

C:\hanmi 폴더를 백업하는 간단한 GUI 백업 프로그램입니다.

## 다운로드

GitHub Actions에서 자동으로 빌드된 실행 파일을 다운로드할 수 있습니다:

1. [Actions 탭](../../actions) 방문
2. 최신 성공한 빌드 클릭
3. 하단의 "Artifacts" 섹션에서 `HanmiBackup-Windows-x64` 다운로드

또는 태그가 생성되면 [Releases 페이지](../../releases)에서 다운로드할 수 있습니다.

## 주요 기능

- **GUI 인터페이스**: 사용하기 쉬운 Windows GUI 프로그램
- **폴더 선택**: 백업할 폴더와 저장 위치를 자유롭게 선택 가능
- **자동 타임스탬프**: 백업할 때마다 타임스탬프가 포함된 폴더 생성
- **재귀 복사**: 하위 폴더와 파일을 모두 백업
- **진행 상황 표시**: 백업 진행 상황을 실시간으로 확인
- **자동 빌드 & 이메일 전송**: GitHub Actions로 자동 빌드 후 이메일 전송

## 시스템 요구사항

- Windows 7 이상
- C++ 컴파일러 (Visual Studio 또는 MinGW)
- CMake 3.15 이상

## 빌드 방법

### 방법 1: GitHub Actions 사용 (가장 쉬움)

코드를 푸시하면 GitHub Actions가 자동으로 Windows 환경에서 빌드합니다:

- Push나 PR 시 자동으로 빌드 실행
- 빌드된 `HanmiBackup.exe`는 Artifacts로 다운로드 가능
- v로 시작하는 태그 생성 시 자동으로 Release 생성

### 방법 2: 빌드 스크립트 사용 (로컬 빌드)

```batch
build.bat
```

### 방법 3: 수동 빌드

#### Visual Studio 사용

```batch
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

#### MinGW 사용

```batch
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

빌드가 완료되면 `build\bin\HanmiBackup.exe` 파일이 생성됩니다.

## 이메일 자동 전송 설정 (선택사항)

GitHub Actions 빌드가 성공하면 자동으로 `2024502@hanmisemi.com`으로 실행 파일을 이메일로 전송합니다.

**참고**: 이메일 설정은 선택사항입니다. Secrets를 설정하지 않아도 빌드는 정상적으로 완료되며, Artifacts에서 실행 파일을 다운로드할 수 있습니다.

### GitHub Secrets 설정 방법

이메일 자동 전송 기능을 활성화하려면 다음 설정이 필요합니다:

1. GitHub 리포지토리 페이지 → **Settings** 클릭
2. 왼쪽 메뉴에서 **Secrets and variables** → **Actions** 클릭
3. **New repository secret** 버튼 클릭하여 다음 시크릿 추가:

   - **`EMAIL_USERNAME`**: 발신자 Gmail 주소 (예: `your-email@gmail.com`)
   - **`EMAIL_PASSWORD`**: Gmail 앱 비밀번호

### Gmail 앱 비밀번호 생성 방법

1. [Google 계정 관리](https://myaccount.google.com/) 페이지 방문
2. **보안** 탭 클릭
3. **2단계 인증** 활성화 (아직 안 되어 있다면)
4. **앱 비밀번호** 검색 및 클릭
5. 앱 선택: **메일**, 기기 선택: **기타 (사용자 설정 이름)** → "GitHub Actions" 입력
6. **생성** 클릭 후 생성된 16자리 비밀번호를 `EMAIL_PASSWORD`로 사용

**참고**: Gmail 대신 다른 SMTP 서버를 사용하려면 `.github/workflows/build.yml` 파일의 `server_address`와 `server_port`를 수정하세요.

## 사용 방법

1. `HanmiBackup.exe` 실행
2. 백업할 폴더 경로 확인/변경 (기본값: `C:\hanmi`)
3. 백업 저장 위치 확인/변경 (기본값: `C:\hanmi_backup`)
4. "백업 시작" 버튼 클릭
5. 백업 완료 대화상자 확인

## 백업 구조

백업은 다음과 같은 구조로 저장됩니다:

```
C:\hanmi_backup\
├── 20260113_143025\  (타임스탬프 형식: YYYYMMDD_HHMMSS)
│   └── [백업된 파일들]
├── 20260113_150130\
│   └── [백업된 파일들]
└── ...
```

## 프로젝트 구조

```
hellyea/
├── .github/
│   └── workflows/
│       └── build.yml     # GitHub Actions 빌드 설정
├── src/
│   └── main.cpp          # 메인 프로그램 소스 코드
├── CMakeLists.txt        # CMake 빌드 설정
├── build.bat             # MinGW 빌드 스크립트
├── build_vs.bat          # Visual Studio 빌드 스크립트
└── README.md             # 이 파일
```

## 기술 스택

- **언어**: C++ (C++17)
- **GUI**: Windows API
- **빌드 시스템**: CMake
- **CI/CD**: GitHub Actions

## 특징

### 안전한 백업

- 원본 파일은 절대 수정하지 않음
- 타임스탬프로 여러 버전 관리 가능
- 백업 실패 시 오류 메시지 표시

### 사용자 친화적

- 직관적인 GUI 인터페이스
- 한글 지원
- 실시간 진행 상황 표시
- 폴더 찾아보기 기능

## 문제 해결

### 빌드 오류

- CMake가 설치되어 있는지 확인하세요
- C++ 컴파일러(Visual Studio 또는 MinGW)가 설치되어 있는지 확인하세요
- PATH 환경 변수가 올바르게 설정되어 있는지 확인하세요

### 실행 오류

- Windows 7 이상에서 실행하세요
- 백업할 폴더에 읽기 권한이 있는지 확인하세요
- 백업 저장 위치에 쓰기 권한이 있는지 확인하세요

## 라이선스

이 프로젝트는 자유롭게 사용, 수정, 배포할 수 있습니다.
