#include <stdio.h>
#include <math.h>
#include <queue>
#include <graphics.h>
#include <string.h>  // Header for memset, strcpy functions

#define MAX_SIZE 100
#define M_PI 3.14159265358979323846
#define RADIUS 20

// Function declarations
void drawframe();                                                                     // Draw interface frame
void drawButton(int left, int top, int right, int bottom, const char* label);        // Draw button
bool isMouseHover(int left, int top, int right, int bottom);                         // Check mouse hover
void handleMouseHover(int left, int top, int right, int bottom, const char* label, bool &hovered); // Handle mouse hover
void drawgraph();                                                                     // Draw graph
void clearGraph();                                                                   // Clear graph
void cleanGraph();                                                                   // Clean graph
void bfs(int start, int end, int n);                                                // BFS algorithm
void BFS_display();                                                                  // Display BFS
void openAndReadFile(const char* filePath);                                          // Open and read file
void handleClick(int x, int y);                                                      // Handle mouse click
void mouse();                                                                        // Handle mouse events
void waitForKeypress();                                                              // Wait for keypress

// Status flags
bool isResetMode = false;      // Reset mode flag
bool isGraphLoaded = false;    // Graph loaded status
bool isBFSExecuted = false;    // BFS execution status
bool isAdjSymmetry = false;    // Adjacency matrix symmetry check

// BFS arrays
int visited[MAX_SIZE];
int parent[MAX_SIZE];
int path[MAX_SIZE];
int len = 0;

// Graph data
int numV;                      // Number of vertices
struct Coord {                 // Coordinate structure
    int x;
    int y;
    int label;
};
struct Coord vertices[MAX_SIZE];
FILE *fp;
char buffer[10];
int adjMatrix[MAX_SIZE][MAX_SIZE]; // Adjacency matrix 

// Draw button function
void drawButton(int left, int top, int right, int bottom, const char* label) {
    setcolor(WHITE);
    setlinestyle(0, 1, 3);
    rectangle(left, top, right, bottom);
    settextstyle(10, 0, 1);

    char buffer[MAX_SIZE];
    strcpy(buffer, label);
    outtextxy(left + 10, top + 10, buffer);
}

// Draw graphics frame
void drawframe() {
    setcolor(WHITE);
    setlinestyle(0, 1, 3);
    rectangle(10, 10, 550, 370);

    rectangle(10, 380, 550, 480);
    settextstyle(10, 0, 1);
    
    drawButton(567, 10, 700, 50, "Infor");
    drawButton(567, 70, 700, 110, "Load file");
    drawButton(567, 130, 700, 170, "Start");
    drawButton(567, 190, 700, 230, "Reset");
    drawButton(567, 250, 700, 290, "Delete");
    drawButton(567, 310, 700, 350, "Exit");
}

// Read coordinates and adjacency matrix from file
void openAndReadFile(const char* filePath) {
    fp = fopen(filePath, "r");
    if (fp == NULL) {
        printf("File not found!");
    }
    else {
        fscanf(fp, "%d", &numV);
        if (numV < 2 || numV > 15) {
            printf("Invalid number of vertices. It must be between 2 and 15.\n");
            fclose(fp);  // Close file before return
            return;
        }
        for (int i = 0; i < numV; i++) {
            for (int j = 0; j < numV; j++) {
                fscanf(fp, "%d", &adjMatrix[i][j]);
            }
        }
        // Check matrix symmetry
        isAdjSymmetry = true;
        for (int i = 0; i < numV; i++) {
            for (int j = 0; j < numV; j++) {
                if (adjMatrix[i][j] != adjMatrix[j][i]) {
                    isAdjSymmetry = false;
                    break;
                }
            }
        }
        fclose(fp);  // Move fclose inside else block
    }
    isGraphLoaded = true; 
}

// Draw circle function
void drawCircle(int x, int y, int label) {
    setcolor(WHITE);
    circle(x, y, RADIUS);
    char buffer[10];
    sprintf(buffer, "%d", label);
    outtextxy(x - 10, y - 10, buffer);
}

// Draw line with arrow function
void drawLine(int x1, int y1, int x2, int y2) {
    const int arrow_size = 10; 
    
    // Calculate angle between two points
    double angle = atan2(y2 - y1, x2 - x1);
    x1 = x1 + RADIUS * cos(angle);
    y1 = y1 + RADIUS * sin(angle);
    x2 = x2 - RADIUS * cos(angle);
    y2 = y2 - RADIUS * sin(angle);
    
    line(x1, y1, x2, y2);

    // Draw arrow for directed graph
    if(!isAdjSymmetry) {
        int x3 = x2 - arrow_size * cos(angle - M_PI / 6);
        int y3 = y2 - arrow_size * sin(angle - M_PI / 6);
        int x4 = x2 - arrow_size * cos(angle + M_PI / 6);
        int y4 = y2 - arrow_size * sin(angle + M_PI / 6);
        
        line(x2, y2, x3, y3);
        line(x2, y2, x4, y4);
    }
}

// Draw edges function
void drawEdges() {
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            // Undirected graph (symmetric matrix)
            if (isAdjSymmetry) {
                if (adjMatrix[i][j] == 1 && i < j) {  
                    drawLine(vertices[i].x, vertices[i].y, vertices[j].x, vertices[j].y);
                }
            } 
            // Directed graph
            else {
                if (adjMatrix[i][j] == 1) {
                    drawLine(vertices[i].x, vertices[i].y, vertices[j].x, vertices[j].y);
                }
            }
        }
    }
}

// Draw graph function
void drawgraph() {
    int vertexCount = 0;
    
    settextstyle(10, 0, 1);
    setcolor(WHITE);
    outtextxy(20, 390, "CLICK LEFT MOUSE BUTTON TO ADD A NODE");

    while (vertexCount < numV && !kbhit()) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x = mousex();
            int y = mousey();

            if (x >= 10 + RADIUS && x <= 550 - RADIUS && y >= 10 + RADIUS && y <= 370 - RADIUS) {
                bool canPlace = true;

                // Check distance with existing vertices
                for (int i = 0; i < vertexCount; i++) {
                    double distance = sqrt(pow(vertices[i].x - x, 2) + pow(vertices[i].y - y, 2));
                    if (distance < 3 * RADIUS) {  
                        canPlace = false;
                        break;
                    }
                }

                if (canPlace) {
                    vertices[vertexCount].x = x;
                    vertices[vertexCount].y = y;
                    vertices[vertexCount].label = vertexCount;  

                    drawCircle(x, y, vertexCount);  
                    vertexCount++;  
                }
            }
            clearmouseclick(WM_LBUTTONDOWN);  
        }
        delay(10);
    }

    setcolor(BLACK);
    outtextxy(20, 390, "CLICK LEFT MOUSE BUTTON TO ADD A NODE");
    setcolor(WHITE);
    drawEdges();  
}

// Redraw graph function
void redrawgraph() {
    for (int i = 0; i < numV; i++) {
        drawCircle(vertices[i].x, vertices[i].y, vertices[i].label);
    }
    drawEdges();
}

// Clear graph function
void clearGraph() {
    isBFSExecuted = false;
    memset(visited, 0, sizeof(visited));
    memset(path, 0, sizeof(path));
    len = 0;
    cleardevice();
}

// Clean graph and reset state
void cleanGraph() {
    memset(visited, 0, sizeof(visited));
    memset(path, 0, sizeof(path));
    len = 0;
    cleardevice();
    isGraphLoaded = false; 
}

// Handle click events
void handleClick(int x, int y) {
    // Information button
    if(x >= 567 && x <= 700 && y >= 10 && y <= 50) {
        cleanGraph(); 
        drawframe();
        settextstyle(10, 0, 2);
        setcolor(RED);
        outtextxy(80, 50, "BFS Visualization for Graphs");

        settextstyle(8, 0, 1);
        setcolor(WHITE);
        outtextxy(80, 110, "Supervisor: M.Sc. DOAN VU THINH");
        outtextxy(80, 160, "Student: TRAN DUC NGAN");
        outtextxy(80, 210, "Student ID: 64131460");

        settextstyle(6, 0, 1);
        setcolor(YELLOW);        
        outtextxy(80, 270, "Thank you for reviewing the project!");
    }
    
    // Load file button - Need to add windows.h header and fix implementation
    if(x >= 567 && x <= 700 && y >= 70 && y <= 110) {
        // Temporarily use existing file or input path
        char filePath[256];
        printf("Enter file path: ");
        scanf("%s", filePath);
        
        isAdjSymmetry = true;
        openAndReadFile(filePath);
        clearGraph();
        drawframe();
        drawgraph();
    }

    // Start button
    if (x >= 567 && x <= 700 && y >= 130 && y <= 170) {    
        if (isGraphLoaded) {
            if (!isBFSExecuted) {
                BFS_display(); 
                isBFSExecuted = true; 
            } else {
                clearGraph();      
                drawframe();       
                redrawgraph();     
                BFS_display();     
            }
        } else {
            setcolor(WHITE); 
            cleanGraph();          
            drawframe();           
            outtextxy(195, 120, "NO GRAPH TO START");  
            outtextxy(60, 170, "CLICK THE 'Load file' BUTTON TO ADD GRAPH"); 
        }
    }
    
    // Reset button
    if (x >= 567 && x <= 700 && y >= 190 && y <= 230) {
        if (isGraphLoaded) {
            isBFSExecuted = false; 
            clearGraph();          // Fixed from cleanGraph to clearGraph
            drawframe();           
            drawgraph();           
        } else {
            cleanGraph();  
            drawframe();   
            setcolor(WHITE);
            settextstyle(10, 0, 1);
            outtextxy(195, 120, "NO GRAPH TO RESET");  
            outtextxy(60, 170, "CLICK THE 'Load file' BUTTON TO ADD GRAPH");
        }
    }
    
    // Delete button
    if (x >= 567 && x <= 700 && y >= 250 && y <= 290) {
        cleanGraph();  
        drawframe();   
        outtextxy(180, 120, "DATA HAS BEEN DELETED");  
        outtextxy(60, 170, "CLICK THE 'Load file' BUTTON TO ADD GRAPH"); 
    }

    // Exit button
    if (x >= 567 && x <= 700 && y >= 310 && y <= 350) {
        closegraph();  
    }
}

// BFS algorithm implementation
void bfs(int start, int end, int n) {
    if (start < 0 || start >= n || end < 0 || end >= n) {
        setcolor(WHITE);
        settextstyle(10, 0, 1);
        outtextxy(20, 390, "INVALID START OR END NODE");
        return;
    }

    std::queue<int> q;
    memset(visited, 0, sizeof(visited));
    memset(parent, -1, sizeof(parent));

    q.push(start);
    visited[start] = 1;

    // Display BFS search path
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        setcolor(RED);
        circle(vertices[current].x, vertices[current].y, 20);
        delay(500);

        if (current == end) {
            break;
        }

        for (int i = 0; i < n; i++) {
            if (adjMatrix[current][i] == 1 && !visited[i]) {
                q.push(i);
                visited[i] = 1;
                parent[i] = current;
                
                setcolor(RED);
                drawLine(vertices[current].x, vertices[current].y, vertices[i].x, vertices[i].y);

                delay(400);
            }
        }
    }

    // Display path
    if (visited[end]) {
        // Build path
        int path[MAX_SIZE];
        int len = 0;
        for (int v = end; v != -1; v = parent[v]) {
            path[len++] = v;
        }

        setcolor(YELLOW);  
        for (int i = len - 1; i > 0; i--) {
            drawLine(vertices[path[i]].x, vertices[path[i]].y, vertices[path[i - 1]].x, vertices[path[i - 1]].y);
            delay(200);
        }

        for (int i = 0; i < len; i++) {
            setcolor(YELLOW);
            circle(vertices[path[i]].x, vertices[path[i]].y, 20);
            delay(100);
        }
        
        settextstyle(10, 0, 1);
        setcolor(WHITE);
        outtextxy(20, 390, "PATH: ");
        for (int i = len - 1; i >= 0; i--) {
            char pathBuffer[10];  // Use local variable instead of global buffer
            sprintf(pathBuffer, "%d", path[i]);
            outtextxy(120 + (len - 1 - i) * 30, 390, pathBuffer);
        }
    }
    else {
        settextstyle(10, 0, 1);
        setcolor(WHITE);
        outtextxy(20, 390, "NO PATH FOUND");
    }
}

// Display BFS function
void BFS_display() {
    int start, end;
    printf("Enter the starting vertex: ");
    scanf("%d", &start);
    printf("Enter the ending vertex: ");
    scanf("%d", &end);
    bfs(start, end, numV);
}

// Check mouse hover function
bool isMouseHover(int left, int top, int right, int bottom) {
    int x = mousex();
    int y = mousey();
    if (x >= left && x <= right && y >= top && y <= bottom) {
        return true; 
    }
    return false; 
}

// Handle mouse hover events
void handleMouseHover(int left, int top, int right, int bottom, const char* label, bool &hovered) {
    char buffer[MAX_SIZE]; 
    if (isMouseHover(left, top, right, bottom)) {
        setcolor(RED);
        rectangle(left, top, right, bottom);
        settextstyle(10, 0, 1);

        sprintf(buffer, "%s", label); 
        outtextxy(left + 10, top + 10, buffer); 

        hovered = true;
    } else if (hovered) {
        setcolor(WHITE);
        rectangle(left, top, right, bottom);
        settextstyle(10, 0, 1);
        
        sprintf(buffer, "%s", label); 
        outtextxy(left + 10, top + 10, buffer);
        hovered = false;
    }
}

// Handle mouse events
void mouse() {
    int x;
    int y;
    
    bool hovered = false;
    bool hovered1 = false; 
    bool hovered2 = false; 
    bool hovered3 = false; 
    bool hovered4 = false; 
    bool hovered5 = false; 
    
    while (!kbhit()) {
        if (ismouseclick(WM_MOUSEMOVE)) {
            getmouseclick(WM_MOUSEMOVE, x, y);
            handleMouseHover(567, 10, 700, 50, "Infor", hovered);
            handleMouseHover(567, 70, 700, 110, "Load file", hovered1);
            handleMouseHover(567, 130, 700, 170, "Start", hovered2);
            handleMouseHover(567, 190, 700, 230, "Reset", hovered3);
            handleMouseHover(567, 250, 700, 290, "Delete", hovered4);
            handleMouseHover(567, 310, 700, 350, "Exit", hovered5);
            clearmouseclick(WM_MOUSEMOVE);
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            handleClick(x, y);
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}

// Main function
int main() {
    initwindow(720, 500);
    drawframe();
    mouse();
    
    getch();
    closegraph();
    
    return 0;
}
                visited[i] = 1;
                parent[i] = current;
                
                setcolor(RED);
                drawLine(vertices[current].x, vertices[current].y, vertices[i].x, vertices[i].y);

                delay(400);
            }
        }
    }

    // Hien thi duong di
    if (visited[end]) {
        // Xay dung duong di
        int path[MAX_SIZE];
        int len = 0;
        for (int v = end; v != -1; v = parent[v]) {
            path[len++] = v;
        }

        setcolor(YELLOW);  
        for (int i = len - 1; i > 0; i--) {
            drawLine(vertices[path[i]].x, vertices[path[i]].y, vertices[path[i - 1]].x, vertices[path[i - 1]].y);
            delay(200);
        }

        for (int i = 0; i < len; i++) {
            setcolor(YELLOW);
            circle(vertices[path[i]].x, vertices[path[i]].y, 20);
            delay(100);
        }
        
        settextstyle(10, 0, 1);
        setcolor(WHITE);
        outtextxy(20, 390, "PATH: ");
        for (int i = len - 1; i >= 0; i--) {
            char pathBuffer[10];  // Sử dụng biến local thay vì global buffer
            sprintf(pathBuffer, "%d", path[i]);
            outtextxy(120 + (len - 1 - i) * 30, 390, pathBuffer);
        }
    }
    else {
        settextstyle(10, 0, 1);
        setcolor(WHITE);
        outtextxy(20, 390, "NO PATH FOUND");
    }
}

void BFS_display() {
    int start, end;
    printf("Enter the starting vertex: ");
    scanf("%d", &start);
    printf("Enter the ending vertex: ");
    scanf("%d", &end);
    bfs(start, end, numV);
}

// Check mouse hover function
bool isMouseHover(int left, int top, int right, int bottom) {
    int x = mousex();
    int y = mousey();
    if (x >= left && x <= right && y >= top && y <= bottom) {
        return true; 
    }
    return false; 
}

// Handle mouse hover events
void handleMouseHover(int left, int top, int right, int bottom, const char* label, bool &hovered) {
    char buffer[MAX_SIZE]; 
    if (isMouseHover(left, top, right, bottom)) {
        setcolor(RED);
        rectangle(left, top, right, bottom);
        settextstyle(10, 0, 1);

        sprintf(buffer, "%s", label); 
        outtextxy(left + 10, top + 10, buffer); 

        hovered = true;
    } else if (hovered) {
        setcolor(WHITE);
        rectangle(left, top, right, bottom);
        settextstyle(10, 0, 1);
        
        sprintf(buffer, "%s", label); 
        outtextxy(left + 10, top + 10, buffer);
        hovered = false;
    }
}

// Handle mouse events
void mouse() {
    int x;
    int y;
    
    // Hover status flags
    bool hovered = false;
    bool hovered1 = false; 
    bool hovered2 = false; 
    bool hovered3 = false; 
    bool hovered4 = false; 
    bool hovered5 = false; 
    
    while (!kbhit()) {
        if (ismouseclick(WM_MOUSEMOVE)) {
            getmouseclick(WM_MOUSEMOVE, x, y);
            handleMouseHover(567, 10, 700, 50, "Infor", hovered);
            handleMouseHover(567, 70, 700, 110, "Load file", hovered1);
            handleMouseHover(567, 130, 700, 170, "Start", hovered2);
            handleMouseHover(567, 190, 700, 230, "Reset", hovered3);
            handleMouseHover(567, 250, 700, 290, "Delete", hovered4);
            handleMouseHover(567, 310, 700, 350, "Exit", hovered5);
            clearmouseclick(WM_MOUSEMOVE);
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            handleClick(x, y);
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}

// Main function
int main() {
    initwindow(720, 500);
    drawframe();
    mouse();
    
    getch();
    closegraph();
    
    return 0;
}