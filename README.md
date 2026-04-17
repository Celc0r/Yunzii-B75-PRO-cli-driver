# Yunzii-B75-PRO-cli-driver
Simple driver for keyboard YUNZII B75 PRO written in C++ for my needs

Currently available animations:
1: Wave
2: Riple
3: Starlight
4: Stream
5: Shadow
6: Mountain Wave
7: Sine Wave
8: Color Spring
9: Snow Trace
10: Flower Wave
11: Kill Two Birds
12: Circle Wave
13: Colorful Cross
14: Snow Fall
15: Meteor
16: Dynamic Breathing
17: Spectrum Cycle

All colors available.

Some animations have directions, i did'nt write them up per animation, maybe will do that later, but you can choose from 1-4 or 1-2, depends on animation.

You can set static color or "per key", but there is no way to setup keys to light up in compiled program, you need to change source code for your key map, compile it and run.

There is a option "random color" which is just rainbow in most of animations.

Animation's "Light shadow" and "Music follow" will NOT be implemented by me (for now maybe, but it is pretty difficult for me).

There is currently no value check, so if you are going to input speed/RGB/brightness value above what it needs to be, it can follow to unexecperted behaivour. For example, speed can be a lot slower that 1 and a lot faster than 4 if you input different value's, don't think it works on all of animations, but still pretty funny to play with.

No macros for now.

Program automaticly finds your keyboard by VID PID, so should be working on all linux systems with HIDAPI. I reverse-engineered(if you even can it that) original driver using WireShark and replicated packets from different options.

For all wondering, why is there a "blank's" and skipping in function "set_perkey" by switch statement, some packets from original driver were like this, if data is gonna written to them -- nothing will happen, so i'm just skipping them and properly adjesting enum key map.

I'm too lazy to put it into OpenRGB GUI and if someone is going to do that, please give some credit, thank you.

Command to compile i use:
g++ -o main main.cpp -lhidapi-hidraw

