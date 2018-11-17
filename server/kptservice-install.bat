@echo off
sc create server binPath= "C:\path\to\server.exe --backend windows" start= demand displayname= "server Service" || exit /B 1
sc description server "The server Service" || exit /B 1
