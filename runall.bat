@echo off
setlocal

set "dirPath=CompTests"
set "exePath=main.exe"
set "outputFile=comp.txt"

for %%f in ("%dirPath%/*.cnf") do (
    echo Processing File: %%f >> "%outputFile%"
    "%exePath%" "%%f" >> "%outputFile%"
)

endlocal