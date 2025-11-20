set -e

cd /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run
/opt/homebrew/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
