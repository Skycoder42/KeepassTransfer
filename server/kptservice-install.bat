@echo off
sc create server binPath= "C:\path\to\kptransfer-server.exe --backend windows" start= demand displayname= "Keepass Transfer Service" || exit /B 1
sc description server "The Keepass Transfer Service" || exit /B 1
