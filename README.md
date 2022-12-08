# Pintos
Labs for Principles of Operating Systems course (CompSci 143A) at UCI. [Pintos](http://pintos-os.org) 
is a teaching operating system for x86, challenging but not overwhelming, small
but realistic enough to understand OS in depth (it can run x86 machine and simulators 
including QEMU, Bochs and VMWare Player!). The main source code, documentation and assignments 
are developed by Ben Pfaff and others from Stanford (refer to its [LICENSE](./LICENSE)).
This repository also contains some changes made by [Ryan Huang](huang@cs.jhu.edu).

## Running Tests

```bash
cd ~/Pintos/pintos/src/threads/build
make check
```

pintos-mkdisk filesys.dsk --filesys-size=2
pintos -p ~/Pintos/pintos/src/examples/echo -a echo -- -f -q run 'echo x'


```bash
cd ~/Pintos/pintos/src/userprog && make
cd ~/Pintos/pintos/src/userprog/build
make check
```