#!/bin/bash


SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH/

printf "\n### Making a release build of R-Type...\n\n"

./clean.sh
./build-linux.sh
set -e

mkdir -p appimageTool
cd appimageTool

if [ ! -f "appimagetool-x86_64.AppImage" ]; then
    printf "\n### Downloading AppImageTool...\n"
    curl -L -o appimagetool-x86_64.AppImage https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
    chmod +x appimagetool-x86_64.AppImage
fi

cd $SCRIPTPATH/..


printf "\n### Packaging the game into a tar.gz....\n"
cd release
tar czf ../R-Type.tar.gz .
cd ..

mkdir -p release/packages/
mv R-Type.tar.gz ./release/packages/


printf "\n### Done\n"


printf "\n### Packaging the game server into a standalone AppImage....\n"

mkdir -p release/packages/server/appdir

cd release/packages/server/appdir

cat > AppRun << 'EOF'
#!/bin/sh
cd "$(dirname "$0")"
exec ./server "$@"
EOF

chmod +x AppRun

cat > R-Type-Server.desktop << 'EOF'
[Desktop Entry]
Type=Application
Name=R-Type-Server
Comment=The server of the R-Type project.
Icon=server
Categories=Game
EOF

cp $SCRIPTPATH/../release/server ./server
find $SCRIPTPATH/../release/ -type f \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $SCRIPTPATH/../release/packages/server/appdir \;
find $SCRIPTPATH/../release/ -type l \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $SCRIPTPATH/../release/packages/server/appdir \;
mkdir -p $SCRIPTPATH/../release/packages/server/appdir/assets/
cp -R $SCRIPTPATH/../release/assets/levels $SCRIPTPATH/../release/packages/server/appdir/assets/
cp $SCRIPTPATH/../release/assets/icon/server.png $SCRIPTPATH/../release/packages/server/appdir/server.png
cd ..

$SCRIPTPATH/appimageTool/appimagetool-x86_64.AppImage appdir
chmod +x $SCRIPTPATH/../release/packages/server/R-Type-Server-x86_64.AppImage

printf "\n### Cleaning server workspace...\n"
cp $SCRIPTPATH/../release/packages/server/R-Type-Server-x86_64.AppImage $SCRIPTPATH/../release/packages/
rm -R $SCRIPTPATH/../release/packages/server
printf "\n### Done generating server AppImage\n"


cd $SCRIPTPATH/..
printf "\n### Packaging the game client into a standalone AppImage....\n"

mkdir -p release/packages/client/appdir

cd release/packages/client/appdir

cat > AppRun << 'EOF'
#!/bin/sh
cd "$(dirname "$0")"
exec ./client "$@"
EOF

chmod +x AppRun

cat > R-Type-Client.desktop << 'EOF'
[Desktop Entry]
Type=Application
Name=R-Type-Client
Comment=The client of the R-Type project.
Icon=client
Categories=Game
EOF

cp $SCRIPTPATH/../release/client ./client
find $SCRIPTPATH/../release/ -type f \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $SCRIPTPATH/../release/packages/client/appdir \;
find $SCRIPTPATH/../release/ -type l \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $SCRIPTPATH/../release/packages/client/appdir \;
cp -R $SCRIPTPATH/../release/assets/ ./assets
cp $SCRIPTPATH/../release/assets/icon/client.png $SCRIPTPATH/../release/packages/client/appdir/client.png
cd ..

$SCRIPTPATH/appimageTool/appimagetool-x86_64.AppImage appdir
chmod +x $SCRIPTPATH/../release/packages/client/R-Type-Client-x86_64.AppImage

printf "\n### Cleaning client workspace...\n"
cp $SCRIPTPATH/../release/packages/client/R-Type-Client-x86_64.AppImage $SCRIPTPATH/../release/packages/
rm -R $SCRIPTPATH/../release/packages/client
printf "\n### Done generating client AppImage\n"

