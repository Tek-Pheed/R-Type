$ScriptPath = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent

cd $SCRIPTPATH/..
git pull --recurse-submodules
cmake -B build .
cd build
cmake --build . -j %NUMBER_OF_PROCESSORS%