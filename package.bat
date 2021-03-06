if not exist "Package" mkdir Package
xcopy .\Deploy\BrickGame9999.exe .\Package\ /Y
xcopy .\BrickGame9999\Resources .\Package\Resources\ /E /Y

rem copy dlls
xcopy .\BrickGame9999\*.dll .\Package\
copy readme.txt .\Package
copy saveData.ini .\Package

For /f "tokens=1-3 delims=/- " %%a in ('date /t') do (set packdate=%%a-%%b-%%c)
For /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set packtime=%%a%%b)

7z a -tzip BrickGame9999_%packdate%_%packtime%.zip .\Package\*

rmdir /S /Q .\Package