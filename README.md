## Build 

git clone https://github.com/microsoft/vcpkg.git .vcpkg

.vcpkg/bootstrap-vcpkg.sh
.vcpkg/vcpkg install

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=.vcpkg/scripts/buildsystems/vcpkg.cmake