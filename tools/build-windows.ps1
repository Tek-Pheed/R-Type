$ScriptPath = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent

cd $SCRIPTPATH/..
git submodule update --remote --recursive
cmake -B build .
cd build
cmake --build . -j $env:NUMBER_OF_PROCESSORS