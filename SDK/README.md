
# Build Executable With Fibol  

consider Fibol output is a simple object file,  
if we want to run it, we also need linker with  
target platform.  

For Linux, we can use 'ld', but this will not work  
for now, because unlike C compiler Fibol does not give   
the prologue of the program, one way to do that is we can  
just use gcc or clang link the object file, they will generate  
the prologue and link with each other.  

```
gcc helloworld.fbl.o -o hellworld

```