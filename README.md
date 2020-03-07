# Google Perf Tools使用
代码:https://github.com/gperftools/gperftools  
使用说明:https://gperftools.github.io/gperftools/cpuprofile.html

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
下载好的安装包在https://github.com/NIGHTFIGHTING/gperftools-tutorial/src/
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
我已经下载好的安装包在https://github.com/NIGHTFIGHTING/gperftools-tutorial/src/
### 2.1问题
安装时可能出现configure: WARNING: No frame pointers and no libunwind. Using experimental backtrace capturing via libgcc. 
Expect crashy cpu profiler。是因为没有安装libunwind。在gperftools工具的INSTLL例有说明，64位系统需要安装。使用yum search libunwind查找，
然后选择需要的安装。


