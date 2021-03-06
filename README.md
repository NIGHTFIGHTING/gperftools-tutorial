# Google Performance Tools安装以及使用
一个优化的内存管理算法—tcmalloc性能优于malloc。   
一个用于CPU profile的工具，用于检测程序的性能热点，这个功能和gprof类似。  
一个用于堆检查工具，用于检测程序在是够有内存泄露，这个功能和valgrind类似。  
一个用于Heap profile的工具，用于监控程序在执行过程的内存使用情况。  
```
1.使用其提供的内存管理函数---TC Malloc:
       gcc [...] -ltcmalloc  
2.使用其堆内存检查工具---Heap Checker:
       gcc [...] -o myprogram -ltcmalloc
       HEAPCHECK=normal ./myprogram   

3.使用Heap Profiler:
      gcc [...] -o myprogram -ltcmalloc
      HEAPPROFILE=/tmp/netheap ./myprogram  

4.使用Cpu Profiler:
      gcc [...] -o myprogram -lprofiler
      CPUPROFILE=/tmp/profile ./myprogram
```
代码:https://github.com/gperftools/gperftools  
官方文档：
http://code.google.com/p/google-perftools/wiki/GooglePerformanceTools  
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

### 3.3Analyzing the Output  
需要dot为任何图形输出,yum install graphviz,这里有一些调用pprof的方法。这些将在下面更详细地描述。  
```
    pprof /bin/ls ls.prof
                          Enters "interactive" mode
    pprof --text /bin/ls ls.prof
                          Outputs one line per procedure
    pprof --gv /bin/ls ls.prof
                          Displays annotated call-graph via 'gv'
    pprof --gv --focus=Mutex /bin/ls ls.prof
                          Restricts to code paths including a .*Mutex.* entry
    pprof --gv --focus=Mutex --ignore=string /bin/ls ls.prof
                          Code paths including Mutex but not string
    pprof --list=getdir /bin/ls ls.prof
                          (Per-line) annotated source listing for getdir()
    pprof --disasm=getdir /bin/ls ls.prof
                          (Per-PC) annotated disassembly for getdir()
    pprof --text localhost:1234
                          Outputs one line per procedure for localhost:1234
    pprof --callgrind /bin/ls ls.prof
                       Outputs the call information in callgrind format
```
```
    pprof --pdf test-server test-server.prof > test-server.pdf
```
#### (1)节点信息
查看test-server.pdf，在pprof的各种图形模式中，输出是带有时序信息注释的调用图  
每个节点代表一个过程。有向边指示调用者与被调用者的关系。每个节点的格式如下:  
Class Name  
Method Name  
local (percentage)  
of cumulative (percentage)  
最后一两行包含时序信息。（分析是通过采样方法完成的，默认情况下，我们每秒进行100次采样。因此，输出中的一个时间单位对应于大约10毫秒的执行时间。）"local"时间是执行指令所花费的时间。直接包含在过程中（以及该过程中内联的任何其他过程中）。“cumulative”时间是“local”时间与在任何被叫者身上花费的时间之和。如果累计时间与当地时间相同，则不会打印。  


字段名  | 描述
---- | -----
Class Name  | 类名，非类成员函数此项为空
Method Name  | 函数名
local (percentage)  | 当前函数直接执行的指令所消耗的CPU时间（包括内联函数）（百分比）
of cumulative (percentage)  | 当前函数的local时间及其调用的函数的local时间总和（百分比），如果与local相同，则不显示

#### (2)边信息
从一个节点到另一个节点的边表示调用者与被调用者的关系。每个边都标有被调用者代表调用者花费的时间。  
有向边：调用者指向被调用者，有向边上的时间表示被调用者所消耗的CPU时间  



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


