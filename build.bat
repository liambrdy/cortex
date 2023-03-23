@echo off
if not exist w:\cortex\build mkdir w:\cortex\build
pushd w:\cortex\build

set CommonCompilerFlags=-Od -nologo -FC -WX -W4 -wd4201 -wd4100 -wd4505 -wd4189 -wd4457 -wd4456 -wd4819 -MTd -Oi -GR- -Gm- -EHa- -Zi -DHANDMADE_SLOW=1 -DHANDMADE_INTERNAL=1 -DHANDMADE_WIN32=1
set CommonLinkerFlags=-incremental:no -opt:ref user32.lib gdi32.lib winmm.lib opengl32.lib

rem 32-bit build
rem cl %CommonCompilerFlags% w:\handmade\code\win32_handmade.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

rem 64-bit build
del *.pdb > NUL 2> NUL
REM Optimization switches: /O2 /Oi /fp:fast
cl %CommonCompilerFlags% -Fmwin32_cortex.map w:\cortex\src\win32_cortex.cpp /link %CommonLinkerFlags%
echo Build Lock > lock.tmp
cl %CommonCompilerFlags% -Fmcortex.map w:\cortex\src\cortex.cpp -LD /link opengl32.lib -EXPORT:GameUpdate -incremental:no -PDB:cortex_pdb_%random%.pdb
del lock.tmp
popd