$ScriptPath = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent

cd $SCRIPTPATH/..
git submodule update --init --recursive
cmake -B build .
cd build
cmake --build . -j $env:NUMBER_OF_PROCESSORS

Write-Host "Listing contents of build directory:"
ls

$sourceDir = "dependencies/SFML/"
$destDir = "../release/"

Get-ChildItem -Path $sourceDir -Include *.dll -Recurse | ForEach-Object {
    Write-Host "Copying file: $_.FullName"
    Copy-Item -Path $_.FullName -Destination $destDir
}
