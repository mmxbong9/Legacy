@echo off
setlocal

:: 🔧 사용자 설정
set PROJECT_NAME=Legacy
set PROJECT_PATH=%~dp0%PROJECT_NAME%.uproject
set BUILD_PLATFORM=Win64
set CONFIG=Development

:: 🔨 엔진 경로 설정 (필요 시 수정)
set SORUCE_ENGINE="C:\UE\UnrealEngine-5.5.4-release\Engine"

:: 경로 간략화
set UBT=%SORUCE_ENGINE%\Build\BatchFiles\RunUAT.bat

echo ============================
echo 🚀 빌드 시작: %PROJECT_NAME%
echo ============================

:: 1. 클라이언트 빌드
echo ▶️ 클라이언트 빌드 중...
call %SORUCE_ENGINE%\Build\BatchFiles\Build.bat %PROJECT_NAME% %BUILD_PLATFORM% %CONFIG% -project="%PROJECT_PATH%" -waitmutex -Rebuild

:: 2. 서버 빌드
echo ▶️ 서버 빌드 중...
call %SORUCE_ENGINE%\Build\BatchFiles\Build.bat %PROJECT_NAME%Server %BUILD_PLATFORM% %CONFIG% -project="%PROJECT_PATH%" -waitmutex  -Rebuild

:: 3. 쿠킹 및 패키징
echo ▶️ 컨텐츠 쿠킹 및 패키징 중...
call %UBT% BuildCookRun ^
 -project="%PROJECT_PATH%" ^
 -noP4 ^
 -clientconfig=%CONFIG% ^
 -serverconfig=%CONFIG% ^
 -platform=%BUILD_PLATFORM% ^
 -targetplatform=%BUILD_PLATFORM% ^
 -cook ^
 -allmaps ^
 -build ^
 -stage ^
 -nopak ^
 -archive ^
 -archivedirectory="%~dp0BuildOutput" ^
 -server -compressed=false

echo ============================
echo ✅ 모든 작업 완료!
echo ============================
pause
