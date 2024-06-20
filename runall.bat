@echo off
setlocal

set "dirPath=DIMACS"
set "exePath=main.exe"
set "outputFile=results.txt"

for %%f in ("%dirPath%/*.cnf") do (
    echo Processing File: %%f >> "%outputFile%"
    "%exePath%" "%%f" >> "%outputFile%"
)

endlocal