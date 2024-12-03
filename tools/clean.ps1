$ScriptPath = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

Set-Location (Join-Path $ScriptPath "..")

$ErrorActionPreference = "Stop"

if (Test-Path -Path "build") {
    Remove-Item -Recurse -Force "build\*"
}
if (Test-Path -Path "release") {
    Remove-Item -Recurse -Force "release\*"
}

Write-Output "Cleaned build directory"
