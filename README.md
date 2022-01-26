# Ricochet Robot Solver
Ricochet Robot is one of my most favorite board games of all time. So, I am developing an algorithm that solves Ricochet Robot game.
### Goal
- Given destination coordinates and current robot's coordinates, the algorithm solves the fastest and shortest way for robot to reach final destination. 

## Field

### Tiles
I am going to represent each tiles in 4bits. That will be represented as `UDLR` order. For example, if a tile can move just **left and right**, that tile will be represented as `0x3`.  To save memory space and computing size, I am going to merge two tiles into one `uint8_t` variable. That means, a single `uint8_t` variable will store 2 tiles. The total field is `16 * 16` size. That being said, it can be represented into `8 * 16` of `uint8_t` variables. That will be less than `1KB` to represent the whole field. 

### Robot Coordinates
The maximum coordinates of robots can be (0,0) to (15, 15). Also, I am going to represent robot's coordinates into a single `uint8_t` variable. Since the size is `16 * 16`, each axis can be represented in 4bit. The coordinates will be represented in `XXXX YYYY` order. For example, if a robot is in coordinate (3, 10), the coordinates will be represented as `0x3A`. 

### Why Bit Operations?
I get the question that, it's 21st century and we got `128GB` of RAM in computers. Why use bit operations and a `uint8_t` to save memory space, when you can use `Class` , `Struct` , `Boolean` s? Using Bit operations instead of `if` and `for` loops are extremely '**cheap**' solutions in terms of computer resources and its computing power cost. Also I prefer using old bit operation techniques rather than *High Leveled* operations.

## Algorithm
### My personal algorithm is like following. 
![n1](https://raw.githubusercontent.com/gooday2die/RicochetAI/main/pic/n1.PNG)
- Let's suppose a field that looks like this. It is **NOT** possible for the robot in (1,0) to reach (2,5) without help of other robots. Thus, we have to move robots.

![n2](https://raw.githubusercontent.com/gooday2die/RicochetAI/main/pic/n2.PNG)
- Let's assume that there are some robots placed 'right neighbor' to the destination (2,5). By 'right neighbor' it means that those neighbors are UP, DOWN, LEFT, RIGHT positions of destination position. Meaning that there would be (1,5) , (3,5) , (2,4) , (2,6). Let's assume if it is possible to reach destination with the help of that robot. The answer would be?
- **Yes. You can visit (2,5) with help of (3,5).**
- As you can see, we assumed that there is a robot placed in (3,5) and with help of that virtual robot, it is possible to reach destination. However, as you know, that virtual robot is **NOT** there yet. 
- Let's find if robot 0, 1, 2 can visit (3,5) itself. The answer would be?
- **NO. Not a single robot 0, 1, 2 can visit (3,5) without help of others.** 

![n3](https://raw.githubusercontent.com/gooday2die/RicochetAI/main/pic/n3.PNG)
- Then let's assume if there is 'right neighbor' virtual block near (3,5). That would be (2,5), (4,5), (3,4), (3,6). 
- With help of its neighbor robots, is it possible to put any other robots into (3,5)? The answer would be?
- **Yes, With help of (4,5) it is possible to set a single robot into (3,5)**
- So, then is it possible to set a single at (4,5) ? 
- **Yes, any robot 0, 1, 2 can visit (4,5) without help of other virtual blocks**
- So we can now put a robot at (4,5)
- So we can now put a robot at (3,5)
- So, finally we can now put a robot at (2,5)

The basic idea of this is 'recursion'. **Assume that there are neighbor robots** and then check if that robot can be set by setting another **Virtual robots**. If we can find that one recursion base case with possible robot's position, it is possible to break down the recursion process and find out the optimal way of solving ricochet robot puzzle. In short, this is my algorithm.

1. Simulate 'right neighbor's and check if it is possible to visit destination.
2. Check 'right neighbor' is possible to place. If not, make another 'right neighbor' and do the same process recursively.

### This algorithm does not take using more than 3 robot moves into account. 

For example, if we need to move robot in order 0, 1, 2, 1, 0, this algorithm does NOT recognize the fact that it can move 'already used' robot afterwards. (Till now)
