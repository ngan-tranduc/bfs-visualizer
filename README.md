# BFS Graph Visualization  

A C++ application that provides an interactive visualization of the Breadth-First Search (BFS) algorithm on graphs using the BGI (Borland Graphics Interface) graphics library.  

**Completion date:** Dec 29, 2024  

## Features

- **Interactive Graph Creation**: Click to place vertices on the canvas
- **File-based Graph Loading**: Load graphs from text files containing adjacency matrices
- **BFS Visualization**: Step-by-step visualization of the BFS algorithm
- **Path Highlighting**: Visual representation of the shortest path found by BFS
- **Graph Type Support**: Works with both directed and undirected graphs
- **User-friendly Interface**: Clean GUI with hover effects and intuitive buttons

## Screenshots

The application features a clean interface with:
- Graph drawing area (left panel)
- Control buttons (right panel): Info, Load File, Start, Reset, Delete, Exit
- Status display area (bottom panel)

## Requirements

### For Quick Start (Ready-to-run)
- Windows OS
- No additional software needed - just click `BFS.lnk`!

### For Development (Optional)
- C++ compiler (GCC, Dev-C++, Code::Blocks, etc.)
- BGI Graphics Library (graphics.h)
  - For modern systems: WinBGIm library
- Graphics support
- Mouse and keyboard

## Installation

### Quick Start (Recommended)
1. Clone this repository:
   ```bash
   git clone https://github.com/ngan-tranduc/bfs-visualizer
   ```
2. Navigate to the project folder
3. **Double-click `BFS.lnk`** to run the application
4. That's it! No compilation needed.

### Development Setup (Optional)
If you want to modify the code:

**Option 1: Using Dev-C++ with WinBGIm**
1. Download and install Dev-C++
2. Download WinBGIm package
3. Extract WinBGIm to your Dev-C++ directory
4. Open the project in Dev-C++
5. Compile and run

**Option 2: Using Code::Blocks**
1. Install Code::Blocks with MinGW
2. Setup graphics library (WinBGIm)
3. Create new project and add the source file
4. Configure project settings for graphics
5. Build and run

## Usage

### Loading a Graph
1. Click the **"Load file"** button
2. Enter the path to your input file
3. The graph will be loaded and you can place vertices by clicking

### Input File Format
Create a text file with the following format:
```
5
0 1 0 0 1
1 0 1 1 0
0 1 0 1 0
0 1 1 0 1
1 0 0 1 0
```

- First line: Number of vertices (2-15)
- Following lines: Adjacency matrix (0 = no edge, 1 = edge exists)
- For undirected graphs: Matrix should be symmetric
- For directed graphs: Matrix can be asymmetric

### Running BFS
1. After loading a graph and placing vertices, click **"Start"**
2. Enter the starting vertex number in the console
3. Enter the ending vertex number in the console
4. Watch the BFS visualization:
   - Red circles: Vertices being explored
   - Red lines: Edges being traversed
   - Yellow path: Final shortest path (if exists)

### Control Buttons
- **Info**: Display project information
- **Load file**: Load graph from file
- **Start**: Begin BFS visualization
- **Reset**: Reset the current graph (keep loaded data)
- **Delete**: Delete all data and start fresh
- **Exit**: Close the application

## Algorithm Details

The application implements the standard BFS algorithm:

1. **Initialization**: Mark all vertices as unvisited
2. **Queue Operations**: Use a queue to store vertices to visit
3. **Exploration**: Visit all neighbors of current vertex
4. **Path Reconstruction**: Use parent array to rebuild the shortest path
5. **Visualization**: Color-coded representation of the search process

### Time Complexity
- **Time**: O(V + E) where V is vertices and E is edges
- **Space**: O(V) for the queue and visited arrays

## Code Structure

```
├── BFS.lnk                 # Quick launch shortcut                
├── main.cpp                # Main source file
├── ...
├── README.md              # This file
└── graph/                 # Sample input files
    ├── test0.txt
    ├── test1.txt
    └── ...
```

### Key Functions
- `bfs()`: Core BFS algorithm implementation
- `drawgraph()`: Interactive graph vertex placement
- `drawEdges()`: Render graph edges based on adjacency matrix
- `handleClick()`: Process mouse clicks and button actions
- `openAndReadFile()`: Parse input files

## Sample Input Files

### Undirected Graph
```
4
0 1 1 0
1 0 0 1
1 0 0 1
0 1 1 0
```

### Directed Graph
```
5
0 1 0 0 0
0 0 1 0 0
0 0 0 1 0
0 0 0 0 1
1 0 0 0 0
```

## Troubleshooting

### Common Issues

**Application won't start**
- Make sure you're running on Windows
- Try right-click on `BFS.lnk` → "Run as administrator"
- Check if antivirus is blocking the executable

**Graphics initialization failed**
- Ensure you have graphics drivers installed
- Try running on a different Windows machine
- Contact support if issue persists

**File not found error**
- Verify file path is correct
- Ensure file format matches specification
- Use forward slashes (/) or double backslashes (\\\\) in file paths

**Vertices too close together**
- Minimum distance between vertices is enforced
- Try placing vertices with more spacing
- The system prevents overlapping vertices

**BFS not starting**
- Make sure graph is loaded first
- Verify vertices are placed on canvas
- Check if start/end vertex numbers are valid

## Author

**TRAN DUC NGAN**  
Student ID: 64131460  
Supervisor: M.Sc. DOAN VU THINH  

## Acknowledgments

- Thanks to the Computer Science Department for project guidance
- BGI Graphics Library developers
- Open source community for inspiration

---

⭐ **If you found this project helpful, please give it a star!**
