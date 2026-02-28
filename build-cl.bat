:: 它会在常见的 Visual Studio 安装位置中查找 Developer Command Prompt for Visual Studio，
:: 如果有多个，它应该会让你选一个，然后自动开始编译。
:: Works on my machine.

@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo Shell Environment Detector
echo ==========================================

:: 寻找 VsDevCmd.bat
:: Visual Studio 2019 或者更旧版本会在 Program Files (x86) 里面, VS 2022+ 会在 Program Files 里面。
set "idx=0"
echo [Info] Scanning for VsDevCmd.bat in standard Visual Studio installation paths...

for %%D in ("%ProgramFiles(x86)%\Microsoft Visual Studio" "%ProgramFiles%\Microsoft Visual Studio") do (
    if exist "%%~D" (
        for /f "delims=" %%F in ('dir /s /b "%%~D\VsDevCmd.bat" 2^>nul') do (
            set /a idx+=1
            set "vs_path_!idx!=%%F"
        )
    )
)

if !idx! equ 0 (
    echo [Error] Could not find VsDevCmd.bat in standard Microsoft Visual Studio directories.
    pause
    goto :EOF
)

if !idx! equ 1 (
    set "SELECTED_VS=!vs_path_1!"
    echo [Info] Automatically selected: !SELECTED_VS!
) else (
    echo.
    echo Multiple VsDevCmd.bat installations found:
    for /l %%I in (1, 1, !idx!) do (
        echo   [%%I] !vs_path_%%I!
    )
    echo.
    :ask_vs
    set "choice="
    set /p "choice=Select the number of the version you want to use: "
    :: Validate input is a positive integer
    echo !choice!| findstr /r "^[1-9][0-9]*$" >nul
    if errorlevel 1 goto ask_vs
    if !choice! gtr !idx! goto ask_vs
    
    :: Retrieve the chosen path dynamically
    for %%C in (!choice!) do set "SELECTED_VS=!vs_path_%%C!"
)
echo.

:: To detect if we were spawned by PowerShell/Bash vs running natively in an open CMD,
:: we check the %CMDCMDLINE% variable. If another shell or Explorer launched this script, 
:: it almost always passes the "/c" parameter to cmd.exe (which tells it to run and terminate).
echo !CMDCMDLINE! | findstr /i /c:" /c" >nul
if !errorlevel! neq 0 (
    echo [Status] You are already running natively in an interactive CMD shell.
    echo Executing VsDevCmd and changing directory...
    call "!SELECTED_VS!"
    cd /d "%userprofile%\rescale"
    
    echo [Status] Starting compilation...
    if not exist .build mkdir .build
    if not exist publish mkdir publish
    cl.exe /EHsc /O2 rescale.cpp change-dpi.cpp help.cpp /Fe:publish\Rescale.exe /Fo:.build\ /link /SUBSYSTEM:CONSOLE
    
    echo [Status] Done.
    goto :EOF
)

:: If we reach here, we know the script was executed from a non-native or ephemeral environment.
:: Let's do some WMI research to pinpoint exactly what shell invoked us.
set "INVOKER=Unknown"

:: We use a PowerShell one-liner to query the process tree:
:: $PID = the temporary PowerShell instance evaluating this query
:: Parent = the cmd.exe running this batch script
:: Grandparent = the actual shell/program that launched the batch script (e.g., pwsh.exe, explorer.exe)
set "PS_CMD=$p=Get-CimInstance Win32_Process -Filter ('ProcessId='+$PID); $parent=Get-CimInstance Win32_Process -Filter ('ProcessId='+$p.ParentProcessId); $gp=Get-CimInstance Win32_Process -Filter ('ProcessId='+$parent.ParentProcessId); if($gp){$gp.Name}else{'Unknown'}"

for /f "usebackq tokens=*" %%i in (`powershell -NoProfile -Command "%PS_CMD%" 2^>nul`) do (
    set "INVOKER=%%i"
)

echo [Status] Detected launch from outside interactive CMD.
echo [Source] Invoking process: !INVOKER!
echo.
echo Switching to an interactive CMD session and executing target commands...

:: Launch an interactive command prompt, run the batch file, change directory, compile, and keep the session open
:: (The outer quotes are required for cmd.exe to correctly parse inner quotes and the & operator)
cmd /k ""!SELECTED_VS!" & cd /d "%userprofile%\rescale" & echo [Status] Starting compilation... & if not exist .build mkdir .build & if not exist publish mkdir publish & cl.exe /EHsc /O2 rescale.cpp change-dpi.cpp help.cpp /Fe:publish\Rescale.exe /Fo:.build\ /link /SUBSYSTEM:CONSOLE"