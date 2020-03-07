# Google Perf Tools安装以及使用
代码:https://github.com/gperftools/gperftools  
使用说明:  
https://gperftools.github.io/gperftools/cpuprofile.html  
http://goog-perftools.sourceforge.net/doc/cpu_profiler.html  

## 1.安装libunwind
### (1)源码安装libunwind
```
wget https://github.com/libunwind/libunwind/archive/v0.99.tar.gz  
tar -xvf v0.99.tar.gz  
cd libunwind-0.99  
autoreconf --force -v --install  
./configure --prefix=gperftools-tutorial/output   
make   
make install  
```
这里我安装产出在https://github.com/NIGHTFIGHTING/gperftools-tutorial/tree/master/output 这个目录  
如果prefix不指定安装的目录,lib会产生在系统/usr/local/lib,头文件在/usr/local/include  
我已经下载好的安装包在https://github.com/NIGHTFIGHTING/gperftools-tutorial/tree/master/gperftools  
### (2)yum安装
```
yum search libunwind # 查找，然后选择需要的安装  
yum install libunwind-devel.x86_64  
```

## 2.安装gperftools
源码安装google-perf-tools/gperftools  
```
wget https://github.com/gperftools/gperftools/releases/download/gperftools-2.6.1/gperftools-2.6.1.tar.gz  
tar -xvf  gperftools-2.6.1.tar.gz # 解压  
./configure --prefix=gperftools-tutorial/output   
make   
make install  
```
这里我安装产出在https://github.com/NIGHTFIGHTING/gperftools-tutorial/tree/master/output 这个目录  
如果prefix不指定安装的目录,lib会产生在系统/usr/local/lib,头文件在/usr/local/include  
我已经下载好的安装包在https://github.com/NIGHTFIGHTING/gperftools-tutorial/tree/master/gperftools  
### 2.1问题
安装时可能出现configure: WARNING: No frame pointers and no libunwind. Using experimental backtrace capturing via libgcc.Expect crashy cpu profiler。  
是因为没有安装libunwind。在gperftools工具的INSTLL例有说明，64位系统需要安装。使用yum search libunwind查找，  
然后选择需要的安装。  


## 3.gperftools使用
我们在Google使用的CPU分析器。使用它的过程分为三个部分：将库链接到应用程序，运行代码以及分析输出  

### 3.1Linking in the Library  
要将CPU profiler安装到可执行文件中，请添加-lprofiler链接到可执行文件。（也可能在运行时使用LD_PRELOAD，例如 在添加 % env LD_PRELOAD="/usr/lib/libprofiler.so" <binary>，但不一定建议这样做。）  
这并没有打开CPU性能分析; 它只是插入代码。因此，-lprofiler在开发过程中始终始终链接到二进制文件是可行的 。这就是我们在Google所做的。（但是，由于任何用户都可以通过设置环境变量来打开探查器，因此不一定建议将链接profiler的二进制文件安装到运行中的生产系统中。）  
比如：  
#### (1)使用prefix产出的lib  
```
g++ src/test-normal/test-normal.cpp -I output/include/ output/lib/libprofiler.a -o test-normal   
```
#### (2)如果是默认的prefix安装在/usr/local/lib/libprofiler.so  
```
g++ src/test-normal/test-normal.cpp -lprofile -o test-normal  
```
#### (3)按顺序执行下列shell  
```
   export LD_PRELOAD="/usr/local/lib/libprofiler.so"  
   g++ src/test-normal/test-normal.cpp -o test-normal  
   env CPUPROFILE=test-normal.prof ./test-normal  
```
   可以看到profiler的文件test-normal.prof  
#### (4)  
```
   env  LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=test-normal.prof ./test-normal  
```
   可以看到profiler的文件test-normal.prof  

### 3.2Running the Code  
#### (1)将环境变量CPUPROFILE定义为将概要文件转储到的文件名。例如，如果您有一个/bin/ls与libprofiler链接的版本 ，则可以运行:  
```
    env CPUPROFILE=ls.prof /bin/ls
```
    我们可以接着3.1(1)或者3.1(2)或者3.1(3)继续执行  
```
    env CPUPROFILE=test-normal.prof ./test-normal
```
