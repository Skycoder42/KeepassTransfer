@echo off
sc create server binPath= "%~dp0\kptransfer-server.exe --backend windows" start= demand displayname= "KeePass-Transfer Server Service" || exit /B 1
sc description server "The KeePass-Transfer Server Application" || exit /B 1
