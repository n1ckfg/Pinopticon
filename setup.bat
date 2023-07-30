@echo off

git pull

git submodule init
git submodule update --init --recursive
git submodule sync
git submodule foreach git checkout master > nul 2> nul
git submodule foreach git checkout main > nul 2> nul
git submodule foreach git reset --hard
git submodule foreach git pull origin master > nul 2> nul
git submodule foreach git pull origin main > nul 2> nul

rem cd Assets\latkxGDrive
rem git checkout unity-2018-lts
rem cd ..\..

@pause