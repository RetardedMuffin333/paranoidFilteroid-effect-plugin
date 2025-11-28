@echo off
cd /d "%~dp0"
git add -A
git commit -m "VS2026 build configuration"
git log --oneline -5
