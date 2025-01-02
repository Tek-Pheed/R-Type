#! /bin/bash

install_name_tool -add_rpath @loader_path/ ./release/client
install_name_tool -add_rpath @loader_path/ ./release/server