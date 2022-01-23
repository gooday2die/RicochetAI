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

## Features
- Can find path **without** moving other robots. If robot cannot find path to the destination, it gives up. (2022-01-23) 
