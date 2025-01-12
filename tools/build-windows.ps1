$ScriptPath = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent

cd $SCRIPTPATH/..
git submodule update --init --recursive
echo "Generating build files..."
cmake -B build .
cd build
echo "Building R-Type..."
cmake --build . -j $env:NUMBER_OF_PROCESSORS

$sourceDir = "dependencies/SFML/"
$destDir = "../release/"

echo "Copying binnaries..."

Get-ChildItem -Path $sourceDir -Include *.dll -Recurse | ForEach-Object {
    Write-Host "Copying file: $_"
    Copy-Item -Path $_.FullName -Destination $destDir
}

$sourceDir = "../dependencies/SFML/extlibs/bin/x64"

Get-ChildItem -Path $sourceDir -Include *.dll -Recurse | ForEach-Object {
    Write-Host "Copying file: $_"
    Copy-Item -Path $_.FullName -Destination $destDir
}

echo "Copying assets..."

mkdir ../release/assets/

$sourceDir = "../assets"
$destDir = "../release/assets/"
Get-ChildItem -Path $sourceDir | ForEach-Object {
    Write-Host "Copying file: $_"
    Copy-Item -Path $_.FullName -Destination $destDir  -Recurse
}

cd $SCRIPTPATH/..
echo "Done."