# Belkan Game AI Project

This project was developed as part of the Artificial Intelligence course at the University of Granada. It involves creating an AI algorithm to play the game Belkan automatically. The game's objective is for an ant (the player) to collect as much food (aims) as possible on different maps within constraints such as time limits, memory usage, and battery life.

## Project Description

In this project, we implemented several search algorithms to enable the ant to navigate the map efficiently and collect the maximum number of aims. The following algorithms were implemented:

- **Breadth-First Search (BFS)**
- **Depth-First Search (DFS)**
- **Dijkstra's Algorithm**
- **A* Search Algorithm**

The graphical user interface (GUI) for the game was provided by the course instructors, and the main implementation work was done in the files `player.cpp` and `player.h`.

## Files

- **player.cpp**: Contains the implementation of the ant's decision-making logic using the specified search algorithms.
- **player.h**: Contains the definitions and necessary includes for the `player.cpp`.

## How to Run

1. **Compile the Code**: Ensure you have a C++ compiler installed. Use the following command to compile the project:
    ```bash
    g++ -o belkan player.cpp
    ```

2. **Run the Game**: Execute the compiled file:
    ```bash
    ./belkan
    ```

3. **Select Algorithm**: Upon starting, you may need to select which algorithm the ant should use. This can be done through the provided GUI.
