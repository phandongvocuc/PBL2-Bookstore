set -e

cd /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run
/opt/homebrew/bin/cmake -E cmake_autogen /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run/CMakeFiles/pbl2_book_autogen.dir/AutogenInfo.json ""
/opt/homebrew/bin/cmake -E touch /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run/pbl2_book_autogen/timestamp
/opt/homebrew/bin/cmake -E cmake_transform_depfile Ninja gccdepfile /Users/nguyenly/Documents/PBL2/PBL2-Bookstore /Users/nguyenly/Documents/PBL2/PBL2-Bookstore /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run/pbl2_book_autogen/deps /Users/nguyenly/Documents/PBL2/PBL2-Bookstore/build_run/CMakeFiles/d/43f98d898e4982dc4df8eed042de76c15f111bb92a882205756d39df2e60b48c.d
