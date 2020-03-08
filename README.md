# Google Perf Tools安装以及使用
代码:https://github.com/gperftools/gperftools  
使用说明:  
https://gperftools.github.io/gperftools/cpuprofile.html  
http://goog-perftools.sourceforge.net/doc/cpu_profiler.html  
https://github.com/gperftools/gperftools/wiki  

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
### 2.2问题  
编译时打开了./configure –enable-frame-pointers ，这要求被测试的程序在编译时要加上gcc编译选项，否则某些多线程程序可能会 core:CCFLAGS=-fno-omit-frame-pointer  


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
g++ src/test-normal/test-normal.cpp -lprofiler -o test-normal  
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
   env LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=test-normal.prof ./test-normal  
```
   可以看到profiler的文件test-normal.prof  

### 3.2Running the Code  
#### (1)将环境变量CPUPROFILE定义为将概要文件转储到的文件名。例如，如果您有一个/bin/ls与libprofiler链接的版本 ，则可以运行:  
```
    env CPUPROFILE=ls.prof /bin/ls
```
```
    env CPUPROFILE=test-normal.prof ./test-normal # 我们可以接着3.1(1)或者3.1(2)或者3.1(3)继续执行  
```
可以看到profiler的文件test-normal.prof  
#### (2)除了定义环境变量CPUPROFILE外，您还可以定义CPUPROFILESIGNAL。这样可以通过您指定的信号号来控制性能分析。在正常操作下，这个信号编号必须没有被程序中使用过。在内部，它充当由信号触发的开关，默认情况下处于关闭状态。例如，如果您有一个/bin/chrome与libprofiler链接的版本 ，则可以运行:   
```
    env CPUPROFILE=chrome.prof CPUPROFILESIGNAL=12 /bin/chrome &
    killall -12 chrome # 然后，您可以触发分析以开始
    killall -12 chrome # 然后，在一段时间后，您可以告诉它停止以生成profile文件
```
##### [2.1]测试test-server  
```
    g++ src/test-server/test-server.cpp -lprofiler -o test-server
    env CPUPROFILE=test-server.prof CPUPROFILESIGNAL=12 ./test-server>log 2>&1 &
    killall -12 test-server
    killall -12 test-server #看见生成的profiler文件test-server.prof.0
```
##### [2.2]测试test-server  
```
    export CPUPROFILE=test-server.prof CPUPROFILESIGNAL="12" LD_PRELOAD="/usr/local/lib/libprofiler.so"
    g++ src/test-server/test-server.cpp -o test-server
    killall -12 test-server
    killall -12 test-server #看见生成的profiler文件test-server.prof.0
```
```
[xiaoju@flliuqi gperftools-tutorial]$ pprof --text test-server test-server.prof.0
Using local file test-server.
Using local file test-server.prof.0.
Total: 1616 samples
     817  50.6%  50.6%     1616 100.0% loopop
     799  49.4% 100.0%      799  49.4% loopop_callee
       0   0.0% 100.0%     1616 100.0% __libc_start_main
       0   0.0% 100.0%     1616 100.0% _start
       0   0.0% 100.0%     1616 100.0% main
```
#### (3)在您的代码中，将您要分析的代码括在对ProfilerStart()和的调用中 ProfilerStop()。（这些函数在<gperftools/profiler.h>中声明。） ProfilerStart()将使用profiler文件名[profile-filename]作为参数。  
```
    g++ src/test-normal/test-normal-profiler.cpp -lprofiler -o test-normal
    ./test-normal # 会产生profiler文件test-normal.prof
```
```
    g++ src/test-server/test-server-profiler.cpp -lprofiler -o test-server
    ./test-server >log 2>&1 &
    kill -SIGUSR1 `ps -ef | grep test-server | grep -v grep | awk '{print $2}'`    # 或者使用killall -10 test-server(kill -l可以查看信号的编号)
    kill -SIGUSR2 `ps -ef | grep test-server | grep -v grep | awk '{print $2}'`    # 或者使用killall -12 test-server
    #得到profiler文件test-server.prof
```


## 4.参考
https://blog.csdn.net/10km/article/details/83820080  
https://blog.csdn.net/dolphin98629/article/details/80932848  
https://www.jianshu.com/p/8b996698e2e3  
https://www.iteye.com/blog/xiaotaoge-1565059  
https://www.cnblogs.com/foxmailed/archive/2012/04/08/2437207.html  
https://www.cnblogs.com/Lipp/archive/2012/05/28/2521382.html  
https://www.cnblogs.com/dylancao/p/7683960.html  
https://blog.csdn.net/weixin_41376894/article/details/78793321  
https://www.cnblogs.com/lenolix/archive/2010/12/13/1904868.html  
https://www.kancloud.cn/subin/blog/619133  

字段名  | 描述
---- | ----- | ------ 
Class Name  | 类名，非类成员函数此项为空
Method Name  | 函数名 

字段名  | 描述  | 表头
---- | ----- | ------ 
单元格内容  | 单元格内容 | 单元格内容
单元格内容  | 单元格内容 | 单元格内容 
