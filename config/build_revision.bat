rem Savefile locally

set Vmajor=%1
set Vminor=%2
set BType=%3

for /f "delims=" %%x in (config/version.cfg) do set BuildStr=%%x

set Build=%BuildStr:~8%

set /A Build=Build+1

(
rem Autogenerated .cfg file in the building process
echo ;DO NOT CHANGE THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING
echo [Version]
echo Major = %Vmajor%
echo Minor = %Vminor%
echo Type = %BType%
echo Build = %Build%
) > config/version.cfg
pause
