#!/bin/bash 
echo $@
#find . -type f -name "*_out.cpp" -delete
/Users/alasdairmorrison/Library/Developer/Xcode/DerivedData/ClangInstrumentationPlugin-amqlzsryrxvvgkehenrlpeypglea/Build/Products/Debug/aliclang -isysroot "/Applications/Xcode5-DP3.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk" $@ # -I"${HEADER_SEARCH_PATHS// /\-I}" -I"/Users/alasdairmorrison/Projects/EnigmaCompiler/EnigmaCompiler/JDI/src" -I"/Users/alasdairmorrison/Projects/EnigmaCompiler/EnigmaCompiler/" -I "/usr/include/c++/4.2.1/" "${INPUT_FILE_PATH}"
#clang++ $@ #works
#clang++ -isysroot "/Applications/Xcode5-DP3.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk" $@