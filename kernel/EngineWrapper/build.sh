#!/bin/bash
sudo apt-get install qt5-defalut
mkdir "3thparty"
cd "3thparty"
git clone https://gitee.com/shenzhen_chuangxiang_3d/CuraEngine.git
git clone https://gitee.com/shenzhen_chuangxiang_3d/protobuf.git
git clone https://gitee.com/shenzhen_chuangxiang_3d/trimesh2.git
cd ..
mkdir build
cd build
cmake ../
make

