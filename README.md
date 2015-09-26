# my NEMU


Try to learn computer architecture by lab

## Aim: 
Implement a simplified x86 simulator.

## Running environment:
- Virtualbox (Debian netinst CD image with i386 architecture.)
- Debian:
	- make
	- lreadline
	- gcc
	- ld
- [more detail](http://cslab.nju.edu.cn/ics/index.php/Ics:2013/PA0)

------------------------------

Aug 12 2015 update:

## The functionalities that I have already implemented:
### Simple debugger
breakpoints: set, delete  
single step through a binary executable file  
expression evaluation  
print in debugging  

### ISA
cmpl  

------------------


## NOTICE in the future:
- the adt itself not must to be synchronized, the user choose whether to synchronize.  
- can add NOINTR to make sure user add synchronization if necessary  

- put the original global variable to parameter can better re-use some data structure.(ListHead)
- using macro when need different type(Tree<>)
- using special name convention when implement data structure, e.g. `_bitmap_array`

- design an error return system between 'client' and 'server'(in os is 'os user' and 'os kernel')
	- `enum`: not a simple `FAIL` and `SUCC` but `no such directory or file`, `Segmentation fault`.
	- `struct`: a int indicate state, and a string indicate error message.

- if fail to allocate some resources when allocating a serial of resources,  
	remember to free already allocated resources.

