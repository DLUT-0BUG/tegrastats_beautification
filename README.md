# tegrastats-display
change the output of tegrastats on TX2
  
Catch the tegrastats' output through pipe. Then transform the string into various data.  
ONLY tx2Status_ver2.c is usable.
  
# Build  
> gcc tx2Status_ver2.c -o tx2status  

> sudo ./tx2status

=================================================================
|  2018/1/28   Sun   20:34:07					|
=================================================================
|  CPU0		346MHz	0%	|  GPU		0%	114MHz	|
|  CPU1		499MHz	0%	|  EMC		2%	665MHz	|
|  CPU2		499MHz	3%	|  APE		150		|
|  CPU3		345MHz	0%	|  RAM	1855	7852	23%	|
|  CPU4		346MHz	0%	|  SWAP	0	3926	0%	|
=================================================================
|  MPCU	40.50C	  |  PLL	40.50C'	|   Board   39.00C'	|
|  BPCU	40.50C	  |  PMIC	100C'	|   Diode   39.25C'	|
|		  |			|   thermal 40.30C'	|
=================================================================
