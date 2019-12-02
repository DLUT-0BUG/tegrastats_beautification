# tegrastats-display
Beautify the output of tegrastats on TX2
  
Catch the tegrastats' output through pipe. Then transform the string into various data.  
ONLY tx2Status_ver2.c is usable.
  
# Build  
### for tx2  
> gcc tx2Status_ver2.c -o tx2status  
> sudo ./tx2status
### for Manifold 2-G
> gcc ManifoldStatus.c -o ManifoldStatus  
> sudo ./ManifoldStatus  
*Be aware that this code base on the command "tegrastats", which Manifold 2 is not contained. You should copy one to the directory /usr/bin before you execution.*  

![Effect](https://github.com/Laurenfrost/tegrastats-display/blob/master/Annotation.png)  
