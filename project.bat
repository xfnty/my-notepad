@echo off
setlocal enableextensions enabledelayedexpansion
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do set esc=%%b

set PROJECT_NAME=MyNotepad
set SOURCE_DIR=%~dp0src\
set OUTPUT_DIR=%~dp0out\
set TEMP_DIR=%OUTPUT_DIR%temp\
set TEMP_MSVC_DIR=%OUTPUT_DIR%temp\msvc\
set TEMP_GCC_DIR=%OUTPUT_DIR%temp\gcc\
set SETTINGS_DIR=%OUTPUT_DIR%settings\
set SUPPORTED_COMPILERS=cl gcc

if "%1"=="" (
    call :Build || goto :ExitError
    call :Run || goto :ExitError
) else (
    for %%a in (%*) do (
        if "%%a"=="help" (
            call :PrintHelp
        ) else if "%%a"=="clean" (
            call :Clean || goto :ExitError
        ) else if "%%a"=="build" (
            call :Build || goto :ExitError
        ) else if "%%a"=="build-all" (
            call :BuildAll || goto :ExitError
        ) else if "%%a"=="build-msvc" (
            call :BuildMSVC || goto :ExitError
        ) else if "%%a"=="build-gcc" (
            call :BuildGCC || goto :ExitError
        ) else if "%%a"=="dist" (
            call :Dist || goto :ExitError
        ) else if "%%a"=="dist-msvc" (
            call :DistMSVC || goto :ExitError
        ) else if "%%a"=="dist-gcc" (
            call :DistGCC || goto :ExitError
        ) else if "%%a"=="run" (
            call :Run || goto :ExitError
        ) else if "%%a"=="run-msvc" (
            call :RunMSVC || goto :ExitError
        ) else if "%%a"=="run-gcc" (
            call :RunGCC || goto :ExitError
        ) else if "%%a"=="sum" (
            call :Sum || goto :ExitError
        ) else if "%%a"=="sum-msvc" (
            call :SumMSVC || goto :ExitError
        ) else if "%%a"=="sum-gcc" (
            call :SumGCC || goto :ExitError
        ) else (
            echo !esc![1;91merror!esc![0m: Unknown command !esc![1;91m%%a!esc![0m
            echo.
            call :PrintHelp
            goto :ExitError
        )
    )
)
goto :Exit


:PrintHelp
echo Usage: project [clean/build/run/dist/sum...]
echo.
echo By default executes "build" and "run" commands.
echo Run "clean" before "dist" if you changed compiler.
echo %%CC%% is used to find the compiler. If it's undefined the script tries to use either MSVC or GCC.
echo %%RUNARGS%% is passed to the executable.
echo "build", "run", "dist" and "sum" commands have "...-msvc" and "...-gcc" variations.
echo "build" has an additional "...-all" variation that builds the project using both compilers.
exit /b 0


:Clean
echo !esc![0mCleaning!esc![1;30m output files!esc![0m
rmdir /q /s "%OUTPUT_DIR%" 2> nul
exit /b 0


:Build
call :FindCompiler || goto :ExitError
if "!CC!"=="cl" (
    call :BuildMSVC || goto :ExitError
) else if "!CC!"=="gcc" (
    call :BuildGCC || goto :ExitError
) else (
    echo !esc![1;91merror!esc![0m: "!CC!" compiler is not supported!esc![0m
    goto :ExitError
)
exit /b 0


:BuildAll
set BUILD_ALL_OK=
where /q cl  && (set BUILD_ALL_OK=1 && call :BuildMSVC || exit /b 1)
where /q gcc && (set BUILD_ALL_OK=1 && call :BuildGCC  || exit /b 1)
if "%BUILD_ALL_OK%"=="" exit /b 1
exit /b 0


:BuildMSVC
echo !esc![0mBuilding!esc![1;30m using !esc![1;34mMSVC!esc![0m
mkdir "%SETTINGS_DIR%" "%TEMP_MSVC_DIR%" 2> nul
set links=
for /r "%SOURCE_DIR%def" %%a in (*.def) do (
    set links="%%~na.lib" !links!
    call :IsFileNewerThan "%%a" "%TEMP_MSVC_DIR%%%~na.lib" && (
        echo !esc![1;30mGenerating %%~na.lib!esc![0m
        lib /nologo /wx /def:"%%a" /out:"%TEMP_MSVC_DIR%%%~na.lib" /machine:x64 > "%TEMP_MSVC_DIR%%%~na.txt" || (
            type "%TEMP_MSVC_DIR%%%~na.txt"
            exit /b 1
        )
    )
)
call :IsFileNewerThan "%SOURCE_DIR%rsrc\resources.rc" "%TEMP_MSVC_DIR%resources.res" && (
    echo !esc![1;30mGenerating resources.res!esc![0m
    rc /nologo /i "%~dp0src" /fo "%TEMP_MSVC_DIR%resources.res" "%SOURCE_DIR%rsrc\resources.rc" || (echo RC error !errorlevel! && exit /b 1)
)
echo.>"%TEMP_MSVC_DIR%%PROJECT_NAME%.c"
for /r "%SOURCE_DIR%." %%a in (*.c) do set v=%%a && if "!v:.ignore=!"=="!v!" echo #include "%%a">>"%TEMP_MSVC_DIR%%PROJECT_NAME%.c"
:: C4711 - function selected for automatic inline expansion
:: C4820 - padding in structs
:: C5045 - Spectre mitigation
:: C5250 - intrinsic function (__va_start for example) not declared
cl /nologo /std:c11 /GS- /Gs1048576 /O2 /Z7 /Wall /WX /wd4711 /wd4820 /wd5045 /wd5250 "%TEMP_MSVC_DIR%%PROJECT_NAME%.c" "%TEMP_MSVC_DIR%resources.res" !links! /Fe:"%TEMP_MSVC_DIR%%PROJECT_NAME%.exe" /Fo:"%TEMP_MSVC_DIR%%PROJECT_NAME%.obj" /link /LIBPATH:"%TEMP_MSVC_DIR%." /entry:_start /subsystem:windows /INCREMENTAL:NO /NODEFAULTLIB > "%TEMP_MSVC_DIR%%PROJECT_NAME%.exe.txt" || (type "%TEMP_MSVC_DIR%%PROJECT_NAME%.exe.txt" && exit /b 1)
exit /b 0


:BuildGCC
echo !esc![0mBuilding!esc![1;30m using !esc![1;34mGCC!esc![0m
mkdir "%SETTINGS_DIR%" "%TEMP_GCC_DIR%" 2> nul
set links=
for /r "%SOURCE_DIR%def" %%a in (*.def) do (
    set links=-l%%~na !links!
    call :IsFileNewerThan "%%a" "%TEMP_GCC_DIR%%%~na.lib" && (
        echo !esc![1;30mGenerating %%~na.lib!esc![0m
        dlltool -m i386:x86-64 -d "%%a" -l "%TEMP_GCC_DIR%%%~na.lib" > "%TEMP_GCC_DIR%%%~na.txt" || (
            type "%TEMP_GCC_DIR%%%~na.txt"
            exit /b 1
        )
    )
)
call :IsFileNewerThan "%SOURCE_DIR%rsrc\resources.rc" "%TEMP_GCC_DIR%resources.res" && (
    echo !esc![1;30mGenerating resources.res!esc![0m
    windres "%SOURCE_DIR%rsrc\resources.rc" -o "%TEMP_GCC_DIR%resources.res" || exit /b 1
)
echo.>"%TEMP_GCC_DIR%%PROJECT_NAME%.c"
for /r "%SOURCE_DIR%." %%a in (*.c) do set v=%%a && if "!v:.ignore=!"=="!v!" echo #include "%%a">>"%TEMP_GCC_DIR%%PROJECT_NAME%.c"
gcc -Wl,-e_start -Wl,--subsystem,windows -O2 -nodefaultlibs -nostdlib -ansi -pedantic-errors -L"%TEMP_GCC_DIR%." !links! "%TEMP_GCC_DIR%%PROJECT_NAME%.c" "%TEMP_GCC_DIR%resources.res" -o "%TEMP_GCC_DIR%%PROJECT_NAME%.exe" || exit /b 1
exit /b 0


:Dist
call :FindCompiler || goto :ExitError
if "!CC!"=="cl" (
    call :DistMSVC || goto :ExitError
) else if "!CC!"=="gcc" (
    call :DistGCC || goto :ExitError
) else (
    echo !esc![1;91merror!esc![0m: "!CC!" compiler is not supported!esc![0m
    goto :ExitError
)
exit /b 0


:DistMSVC
echo !esc![0mPackaging!esc![1;30m stuff built using !esc![1;34mMSVC!esc![0m
if not exist "%TEMP_MSVC_DIR%MyNotepad.exe" (
    echo !esc![1;91merror!esc![0m: "MyNotepad.exe" does not exist!esc![0m
    goto :ExitError
)
tar -acf "%OUTPUT_DIR%%PROJECT_NAME%.zip" -C "%TEMP_MSVC_DIR%." MyNotepad.exe MyNotepad.pdb
exit /b 0


:DistGCC
echo !esc![0mPackaging!esc![1;30m stuff built using !esc![1;34mGCC!esc![0m
if not exist "%TEMP_GCC_DIR%MyNotepad.exe" (
    echo !esc![1;91merror!esc![0m: "MyNotepad.exe" does not exist!esc![0m
    goto :ExitError
)
tar -acf "%OUTPUT_DIR%%PROJECT_NAME%.zip" -C "%TEMP_GCC_DIR%." MyNotepad.exe
exit /b 0


:Run
call :FindCompiler || goto :ExitError
if "!CC!"=="cl" (
    call :RunMSVC || goto :ExitError
) else if "!CC!"=="gcc" (
    call :RunGCC || goto :ExitError
) else (
    echo !esc![1;91merror!esc![0m: "!CC!" compiler is not supported!esc![0m
    goto :ExitError
)
exit /b 0


:RunMSVC
echo !esc![0mRunning!esc![1;30m MyNotepad.exe built using !esc![1;34mMSVC!esc![0m
if not exist "%TEMP_MSVC_DIR%%PROJECT_NAME%.exe" (
    echo !esc![1;91merror!esc![0m: "%PROJECT_NAME%.exe" does not exist!esc![0m
    exit /b 1
)
start "" /d "%TEMP_MSVC_DIR%." /b /wait "%TEMP_MSVC_DIR%%PROJECT_NAME%.exe" %RUNARGS%
if !errorlevel! neq 0 (
    echo !esc![1;91merror!esc![0m: exited with code !esc![1;91m!errorlevel!!esc![0m
    exit /b 1
)
exit /b 0


:RunGCC
echo !esc![0mRunning!esc![1;30m MyNotepad.exe built using !esc![1;34mGCC!esc![0m
if not exist "%TEMP_GCC_DIR%%PROJECT_NAME%.exe" (
    echo !esc![1;91merror!esc![0m: "%PROJECT_NAME%.exe" does not exist!esc![0m
    exit /b 1
)
start "" /d "%TEMP_GCC_DIR%." /b /wait "%TEMP_GCC_DIR%%PROJECT_NAME%.exe" %RUNARGS%
if !errorlevel! neq 0 (
    echo !esc![1;91merror!esc![0m: exited with code !esc![1;91m!errorlevel!!esc![0m
    exit /b 1
)
exit /b 0


:Sum
call :FindCompiler || goto :ExitError
if "!CC!"=="cl" (
    call :SumMSVC || goto :ExitError
) else if "!CC!"=="gcc" (
    call :SumGCC || goto :ExitError
) else (
    echo !esc![1;91merror!esc![0m: "!CC!" compiler is not supported!esc![0m
    goto :ExitError
)
exit /b 0


:SumMSVC
echo !esc![0mSummary!esc![1;30m for MyNotepad.exe built using !esc![1;34mMSVC!esc![0m
if not exist "%TEMP_MSVC_DIR%%PROJECT_NAME%.exe" (
    echo !esc![1;91merror!esc![0m: "%PROJECT_NAME%.exe" does not exist!esc![0m
    exit /b 1
)
for %%I in ("%TEMP_MSVC_DIR%%PROJECT_NAME%.exe") do (
    echo !esc![1;30mEXE size: !esc![1;36m%%~zI!esc![1;30m bytes!esc![0m
)
exit /b 0


:SumGCC
echo !esc![0mSummary!esc![1;30m for MyNotepad.exe built using !esc![1;34mGCC!esc![0m
if not exist "%TEMP_GCC_DIR%%PROJECT_NAME%.exe" (
    echo !esc![1;91merror!esc![0m: "%PROJECT_NAME%.exe" does not exist!esc![0m
    exit /b 1
)
for %%I in ("%TEMP_GCC_DIR%%PROJECT_NAME%.exe") do (
    echo !esc![1;30mEXE size: !esc![1;36m%%~zI!esc![1;30m bytes!esc![0m
)
exit /b 0


:FindCompiler
if "%CC%" neq "" (
    for %%a in ("%CC%") do set CC=%%~na
    exit /b 0
)
if exist "%SETTINGS_DIR%CC" (
    set /p CC=<"%SETTINGS_DIR%CC"
    exit /b 0
)
for %%a in (%SUPPORTED_COMPILERS%) do (
    where /q %%a && (
        set CC=%%a
        mkdir "%SETTINGS_DIR%" 2> nul
        echo %%a>"%SETTINGS_DIR%CC"
        exit /b 0
    )
)
echo !esc![1;91merror!esc![0m: no C compiler available!esc![0m
exit /b 1


:IsFileNewerThan
if not exist "%~2" exit /b 0
xcopy /D /L /Y "%~1" "%~2" | FINDSTR /E /C:"%~nx1" >nul && exit /b 0
exit /b 1


:ExitError
cmd /c exit /b 1
:Exit
