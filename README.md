# UROS-Kernel
The Operating System that I am developing.

~~This code is not organised yet. It needs a lot of changes, and not rely especially on headers.~~

Now the code is organised "properly", refer to `updates.txt` for more updates this time.

Compiling the project:
For now, you do need to compile this with linux, any distro will work, in my case, i use WSL.
If you find a way to do this in Windows, feel free to open an issue.
In any case, you need the following tools if you don't have them.

**Run these if you don't have any toolset to compile the project**
`sudo apt install gcc xorriso build-essential git`
`sudo apt install nasm`

Im not sure tho if any other cross-compiler would work, try luck modifying the Makefile.

How to compile the project: Simply run in the same directory with the Makefile `make all`,
and everything should be automated. In any case, **you should check for now updates.txt**
If you have all the osdev tools installed, you dont have. To execute this, run `make run`.
If for some reason doesn't work, proceed to run the following command.
`qemu-system-i386.exe -accel whpx -d int -no-shutdown -no-reboot UROS-Image.bin`
