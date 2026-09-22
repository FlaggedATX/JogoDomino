Domino Game

![Language](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)
![Status](https://img.shields.io/badge/status-in%20development-yellow)

A domino game built in C with the goal of reinforcing the language fundamentals and the MVC (Model-View-Controller) software architecture.
## About

The project aims to implement the complete game logic: tiles, board, turns, and win detection.



## Prerequisites:
A C compiler such as [GCC](https://gcc.gnu.org/) or Clang.




## How to Play

Classic double-six dominoes:

- 28 tiles (`[0|0]` to `[6|6]`), 7 per player; the rest form the boneyard
- The highest double starts
- On your turn, match one end of the board; if you can't, draw or pass
- The first to empty their hand wins; if the game is blocked, the lowest pip count wins

## Project Structure

```
JogoDomino/
├── Dom_JLRV_Projeto.cpp        # Entry point
├── Dom_JLRV_Model.cpp/.h       # Game data & logic
├── Dom_JLRV_View.cpp/.h        # Display/output
├── Dom_JLRV_Controller.cpp/.h  # Game flow & input
└── README.md
```
