# Google Perf Tools使用
https://github.com/gperftools/gperftools

## 1.安装libunwind
### (1)源码安装libunwind
wget https://github.com/libunwind/libunwind/archive/v0.99.tar.gz
tar -xvf v0.99.tar.gz
cd libunwind-0.99
autoreconf --force -v --install
./configure --prefix=gperftools-tutorial/output 
make 
make install
这里我安装在https://github.com/NIGHTFIGHTING/gperftools-tutorial/output这个目录
如果prefix不指定安装的目录lib会产生在系统/usr/local/lib,头文件在/usr/local/include
### (2)yum安装
yum search libunwind查找，然后选择需要的安装
yum install libunwind-devel.x86_64

# 2.安装gperftools
源码安装google-perf-tools/gperftools
wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.6.1/gperftools-2.6.1.tar.gz
tar -xvf  gperftools-2.6.1.tar.gz # 解压
./configure --prefix=gperftools-tutorial/output 
make 
make install
这里我安装在https://github.com/NIGHTFIGHTING/gperftools-tutorial/output这个目录
如果prefix不指定安装的目录lib会产生在系统/usr/local/lib,头文件在/usr/local/include


