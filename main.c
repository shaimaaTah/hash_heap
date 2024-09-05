// shaimaa dar taha
//1222440
//section 1

// include the libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // define some constant
#define MAX_BUILDINGS 100
#define MAX_NAME_LEN 50

#define MAX_COURSES 300
#define MAX_PREREQUISITES 10
#define MAX_COURSE_NAME 20

#define MAX   100


typedef struct {
    char name[MAX_NAME_LEN];
} Building;
// Struct to store the name of a building.
 // MAX_NAME_LEN is a constant that defines the maximum length of a building's name.



typedef struct {
    int matrix[MAX_BUILDINGS][MAX_BUILDINGS];
    Building buildings[MAX_BUILDINGS]; //
    int size;
} Graph;

 /*Struct to create
  the graph.
 * matrix: Adjacency matrix to store distances between buildings.
 * buildings: Array to store Building structs.
 * size: The number of buildings (vertices) in the graph.
*/

typedef struct {
    int vertex;
    int dist;
} MinHeapNode;

 // Struct to represent a node in the min-heap.
 // vertex: The index of the vertex in the graph.
// dist: The current shortest distance from the source to this vertex.


typedef struct {
    MinHeapNode nodes[MAX_BUILDINGS];
    int size;
    int pos[MAX_BUILDINGS];
} MinHeap;
/*
 * Struct to represent a min-heap.
 * nodes: Array to store the heap nodes.
 * size: The current number of nodes in the heap.
 * pos: Array to store the position of each vertex in the heap. This is used to decrease the key value of a vertex.
 */



typedef struct {
    char course[MAX_COURSE_NAME];// Name of the course
    char prerequisites[MAX_PREREQUISITES][MAX_COURSE_NAME]; // Array to store prerequisites of the course
    int prereq_count; // Number of prerequisites for the course
} Course;

typedef struct {
    char course[MAX_COURSE_NAME];
    int indegree;  // Number of incoming edges
    int outdegree; // Number of outgoing edges
    char adj_out[MAX_COURSES][MAX_COURSE_NAME]; // Outgoing edges (dependencies)
    int out_count;
} Node;

typedef struct {
int size;// Number of elements in the queue
int front; // Index of the front element in the array
int rear; // Index of the rear element in the array
char  array[MAX]; // Array to store elements of the queue

}Queue;

 //  the graph function to dijkstra  algorithm
void readBuildingsFromFile();
int findBuildingIndex(Graph *graph, char *name);
void addBuilding(Graph *graph, char *name);
void addEdge(Graph *graph, char *name1, char *name2, int distance);
void printGraph(Graph *graph);
void printAllShortestPaths(Graph *graph);
void findAndPrintShortestPath(Graph *graph,int b);
void displayMenu();

// function to heap I use in dijkstra
MinHeap* createMinHeap(int capacity);
void minHeapify(MinHeap* minHeap, int idx);
int isEmpty(MinHeap* minHeap);
MinHeapNode extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap, int v, int dist);
int isInMinHeap(MinHeap* minHeap, int v);



// functions to topological sort
int  readCoursesFromFile(Course Courses[]);
Node* find_node(Node nodes[], int node_count, char *course);
int build_graph(Course courses[], int course_count, Node nodes[]);
void topological_sort(Node nodes[], int node_count,int b);

// functions to queue I use in the topological sort
Queue* createQueue() ;
void enqueue(Queue* q, int value) ;
int dequeue(Queue* q);
int isEmpty2(Queue *q);


Graph *graph;
char sorted_order[MAX_COURSES][MAX_COURSE_NAME];


int main() {
    Course courses[MAX_COURSES];
Node nodes[MAX_COURSES];
int course_count =0;

    graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Memory allocation failed\n");
    }
// Initialize graph's adjacency matrix to 0 (no edges)
    graph->size = 0;
    for (int i = 0; i < MAX_BUILDINGS; i++) {
        for (int j = 0; j < MAX_BUILDINGS; j++) {
            graph->matrix[i][j] = 0;
        }
    }
    int choice =0;// Variable to store user's menu choice
printf("welcome to my program \n the menu:\n");
    displayMenu();

     printf("Enter your choice:\n ");
        scanf("%d", &choice);
int load =0;// Flag to check if data has been loaded from files
    // Main loop to process user choices
     while (1)  {
       switch (choice) {
            case 1:
                // Load building data from file
                  readBuildingsFromFile();
                 printf("the building file load successfully \n");
               course_count = readCoursesFromFile(courses);
                printf("the courses file load successfully \n");
                load=1;
                break;
// Find and print shortest path without showing the path
                 case 2:
                     if (load)
                  findAndPrintShortestPath(graph,0);
                // printAllShortestPaths(graph);
                  else
                    printf("please click 1 to load the data from file ");
                break;

                 case 3: // Find and print shortest path with path details
                     if (load)
                     findAndPrintShortestPath(graph,1);
                     else
                       printf("please click 1 to load the data from file ");
                break;

                 case 4:
                     // Perform topological sort without prerequisites
                     if (load ){
  // course_count = readCoursesFromFile(courses);
    if (course_count == -1) {
        return 1;
    }
    int node_count = build_graph(courses, course_count, nodes);
    if (node_count == -1) {
        return 1;
    }
    topological_sort(nodes, node_count,0);
                     }else
                      printf("please click 1 to load the data from file ");

                break;
                 case 5:// Perform topological sort with prerequisites
                        if (load ){
  // course_count = readCoursesFromFile(courses);
    if (course_count == -1) {
        return 1;
    }
    int node_count = build_graph(courses, course_count, nodes);
    if (node_count == -1) {
        return 1;
    }
    topological_sort(nodes, node_count,1);
                     }
                     else{
                      printf("please click 1 to load the data from file \n");
                     }
                break;
                 case 6:// Exit the program and free allocated memory
                      free(graph);
                printf("thank you for using my programm\n good bye \n");
                    break;
                 default :
                       printf("Invalid choice. Please try again.\n");
                    break;

       }
       displayMenu();
         printf("Enter your choice: \n");
        scanf("%d", &choice);

     }
 //   readBuildingsFromFile();
  //  findAndPrintShortestPath(graph);

  //  free(graph);
    return 0;
}

void readBuildingsFromFile() {
   // Declare a file pointer
    FILE *file;
    // Open the file "input_buildings.txt" in read mode
    file = fopen("input_buildings.txt", "r");


    // Check if the file was successfully opened
    if (file == NULL) {
             // Print an error message and return if the file cannot be opened
        printf("error! can't open this file ");
        return;
    }

    char line[100];

        // Read each line from the file until the end of the file is reached
    while (fgets(line, sizeof(line), file)) {
        // Declare variables to store the names of the buildings and the distance
        char name1[MAX_NAME_LEN], name2[MAX_NAME_LEN];
        int distance;
        // store the line in multi variables
        sscanf(line, "%[^#]#%[^#]#%d", name1, name2, &distance);
        // Add an edge between the buildings in the graph with the specified distance
        addEdge(graph, name1, name2, distance);
    }
 // Close the file after reading all lines
    fclose(file);
}

int findBuildingIndex(Graph *graph, char *name) {
     // Iterate through the array of buildings in the graph
   for (int i = 0; i < graph->size; ++i) {
       // Check if the name of the current building matches the given name
        if (strcmp(graph->buildings[i].name, name) == 0) {
           // If a match is found, return the index of the building
            return i;
        }
    }
       // If the building is not found, return -1 to indicate failure
    return -1;
}

void addBuilding(Graph *graph, char *name) {
    // Copy the name of the building to the name field of the next available slot in the buildings array
   strcpy(graph->buildings[graph->size].name, name);
    // Increment the size of the graph to reflect the addition of a new building
    graph->size++;
}

void addEdge(Graph *graph, char *name1, char *name2, int distance) {
   // Find the indices of the buildings corresponding to the given names
    int index1 = findBuildingIndex(graph, name1);
    int index2 = findBuildingIndex(graph, name2);

     // If the first building does not exist in the graph, add it
    if (index1 == -1) {
        addBuilding(graph, name1); // Add the first building to the graph
        index1 = graph->size - 1;  // Update the index of the first building
    }

    // If the second building does not exist in the graph, add it
    if (index2 == -1) {
        addBuilding(graph, name2); // Add the second building to the graph
        index2 = graph->size - 1;  // Update the index of the second building
    }
  // Set the distance between the first and second buildings in the adjacency matrix
    graph->matrix[index1][index2] = distance;
     // Set the distance between the second and first buildings (since the graph is undirected)
   // graph->matrix[index2][index1] = distance;
}

void printGraph(Graph *graph) {
     // Print a header indicating that this is the representation of the graph
    printf("Graph representation:\n");

    // Loop through each row of the adjacency matrix
    for (int i = 0; i < graph->size; i++) {
          // Loop through each column of the adjacency matrix
        for (int j = 0; j < graph->size; j++) {
             // Check if there is a non-zero distance between buildings i and j
            if (graph->matrix[i][j] != 0 && graph->matrix[i][j] != INT_MAX) {
                // Print the connection between buildings i and j along with the distance
                printf("%s - %s: %d\n", graph->buildings[i].name, graph->buildings[j].name, graph->matrix[i][j]);
            }
        }
    }
}

MinHeap* createMinHeap(int capacity) {
    // Allocate memory for the MinHeap structure
   MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));

    // Initialize the size of the MinHeap to 0
    minHeap->size = 0;
    // Initialize the position array of the MinHeap to -1
    // This array will be used to keep track of the position of each vertex in the heap
    // Initially, all positions are set to -1 to indicate that the vertices are not yet in the heap
    for (int i = 0; i < capacity; ++i) {
        minHeap->pos[i] = -1;
    }
    // Return the pointer to the created MinHeap
    return minHeap;
}

void minHeapify(MinHeap* minHeap, int idx) {
    // Initialize the index of the smallest element as the current index
    int smallest = idx;
    // Calculate the indices of the left and right children of the current node
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
// Check if the left child exists and its distance is smaller than the current smallest
    if (left < minHeap->size && minHeap->nodes[left].dist < minHeap->nodes[smallest].dist) {
        smallest = left; // Update the index of the smallest element
    }
     // Check if the right child exists and its distance is smaller than the current smallest

    if (right < minHeap->size && minHeap->nodes[right].dist < minHeap->nodes[smallest].dist) {
        smallest = right;// Update the index of the smallest element
    }
 // Check if the right child exists and its distance is smaller than the current smallest
    if (smallest != idx) {
         // Swap the nodes at the smallest and current indices in the heap
        MinHeapNode smallestNode = minHeap->nodes[smallest];
        MinHeapNode idxNode = minHeap->nodes[idx];

        // Update the positions of the vertices in the position array
        minHeap->pos[smallestNode.vertex] = idx;
        minHeap->pos[idxNode.vertex] = smallest;
  // Swap the nodes in the heap
        MinHeapNode temp = minHeap->nodes[smallest];
        minHeap->nodes[smallest] = minHeap->nodes[idx];
        minHeap->nodes[idx] = temp;

        // Recursively call minHeapify on the subtree rooted at the smallest index
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap* minHeap) {
    // Check if the size of the minHeap is equal to 0
    // If it is, the heap is empty, so return 1 (true), otherwise return 0 (false)
    return minHeap->size == 0;
}

MinHeapNode extractMin(MinHeap* minHeap) {
     // Check if the heap is empty
    if (isEmpty(minHeap)) {
             // If the heap is empty, return a dummy node with a vertex -1 and distance INT_MAX
        MinHeapNode minNode = {-1, INT_MAX};
        return minNode;
    }

    // Extract the root node of the heap
    MinHeapNode root = minHeap->nodes[0];

    // Replace the root node with the last node of the heap
    MinHeapNode lastNode = minHeap->nodes[minHeap->size - 1];
    minHeap->nodes[0] = lastNode;

     // Update the positions of the vertices in the position array
    minHeap->pos[root.vertex] = minHeap->size - 1;
    minHeap->pos[lastNode.vertex] = 0;

    // Decrease the size of the heap
    minHeap->size--;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap* minHeap, int v, int dist) {
     // Get the index of vertex 'v' in the minHeap
    int i = minHeap->pos[v];
      // Update the distance of vertex 'v' to 'dist'
    minHeap->nodes[i].dist = dist;

      // While 'i' is not the root and the distance of 'v' is less than its parent's distance
    while (i && minHeap->nodes[i].dist < minHeap->nodes[(i - 1) / 2].dist) {
       // Swap the positions of 'v' and its parent in the position array
        minHeap->pos[minHeap->nodes[i].vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->nodes[(i - 1) / 2].vertex] = i;

         // Swap the nodes of 'v' and its parent in the minHeap
        MinHeapNode temp = minHeap->nodes[i];
        minHeap->nodes[i] = minHeap->nodes[(i - 1) / 2];
        minHeap->nodes[(i - 1) / 2] = temp;

        // Move up in the heap to continue the process
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap* minHeap, int v) {
     // Check if the position of vertex 'v' in the minHeap is less than the heap size
    // If it is, vertex 'v' is in the heap, so return 1 (true), otherwise return 0 (false)
    return minHeap->pos[v] < minHeap->size;
}

void dijkstra(Graph *graph, int src, int dist[], int prev[]) {
   // Get the size of the graph
    int size = graph->size;
    // Create a min heap to store vertices and their distances
    MinHeap* minHeap = createMinHeap(size);

 // Initialize distances and previous vertices
    for (int v = 0; v < size; ++v) {
        dist[v] = INT_MAX;
        prev[v] = -1;
        // Initialize nodes in the min heap
        minHeap->nodes[v].vertex = v;
        minHeap->nodes[v].dist = dist[v];
        // Set positions of vertices in the min heap
        minHeap->pos[v] = v;
    }
// Initialize source vertex distance to 0 and decrease its key
    minHeap->nodes[src].vertex = src;
    minHeap->nodes[src].dist = dist[src] = 0;
    minHeap->pos[src] = src;
    minHeap->size = size;

 // Decrease the key of the source vertex in the min heap
    decreaseKey(minHeap, src, dist[src]);

// Dijkstra's algorithm
    while (!isEmpty(minHeap)) {
             // Extract the minimum node from the min heap
        MinHeapNode minNode = extractMin(minHeap);
        int u = minNode.vertex;
// Explore neighbors of the extracted vertex
        for (int v = 0; v < size; ++v) {
            if (graph->matrix[u][v] && isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
               dist[u] + graph->matrix[u][v] < dist[v]) {
                 // Update distance and previous vertex
                dist[v] = dist[u] + graph->matrix[u][v];
                prev[v] = u;
                 // Decrease the key of the updated vertex in the min heap
                decreaseKey(minHeap, v, dist[v]);
            }
        }
    }
// Free the memory allocated for the min heap
    free(minHeap);
}

void printShortestPath(Graph *graph, int src, int dest, int prev[]) {
    // Check if source and destination are the same
    if (dest == src) {
             // If source and destination are the same, print only the source building name
       printf("%s", graph->buildings[src].name);
    }
     // Check if there is no path from source to destination
    else if (prev[dest] == -1) {
            // If there is no path, print a message indicating so
        printf("No path from %s to %s\n", graph->buildings[src].name, graph->buildings[dest].name);
    }
    // Otherwise, print the shortest path recursively
    else {
           // Recursively print the shortest path from source to the previous node of the destination
        printShortestPath(graph, src, prev[dest], prev);
       // Print the destination building name along with an arrow
        printf(" -> %s", graph->buildings[dest].name);
    }
}

void printAllShortestPaths(Graph *graph) {
    // Iterate over each vertex in the graph
    for (int i = 0; i < graph->size; i++) {
             // Arrays to store distances and previous vertices for Dijkstra's algorithm
        int dist[MAX_BUILDINGS];
        int prev[MAX_BUILDINGS];
     // Run Dijkstra's algorithm from vertex i to calculate shortest paths to all other vertices
        dijkstra(graph, i, dist, prev);

     // Iterate over all vertices to print shortest paths from vertex i to each of them
        for (int j = 0; j < graph->size; j++) {
             // Ensure that the source and destination vertices are different
            if (i != j) {
            // Print the header indicating the source and destination buildings

                printf("Shortest path from %s to %s: ", graph->buildings[i].name, graph->buildings[j].name);
               // Check if there is a path from vertex i to vertex j
                if (dist[j] == INT_MAX) {

                    // If there's no path, print a message indicating so
                    printf("No path between the building (infinity)\n");
                } else {
                     // If there is a path, print the shortest path and its distance
                    printShortestPath(graph, i, j, prev);
                    printf(" (Distance: %d)\n", dist[j]);
                }
            }
        }
    }
}
void findAndPrintShortestPath(Graph *graph,int b) {
  // Arrays to store the names of the buildings
    char name1[MAX_NAME_LEN], name2[MAX_NAME_LEN];
    printf("the building in the graph  is \n   Masri  A.Shaheen   Bamieh   Sh.Shaheen   Aggad   AL.Juraysi  KNH  Alsadik \n");

    // Prompt the user to enter the names of the two buildings
    printf("Enter the name of the first building: ");
    scanf("%s", name1);
    printf("Enter the name of the second building: ");
    scanf("%s", name2);

// Find the indices of the two buildings in the graph
    int index1 = findBuildingIndex(graph, name1);
    int index2 = findBuildingIndex(graph, name2);

    // Check if one or both of the buildings do not exist in the graph
    if (index1==-1||index2==-1){
    if (index1 == -1) {
        printf("the %s buildings   is not in the graph.\n",name1);
    }
         if (index2==-1){
        printf("the %s buildings   is not in the graph.\n",name2);
        }
        return;
    }

// Arrays to store distances and previous vertices for Dijkstra's algorithm
    int dist[MAX_BUILDINGS];
    int prev[MAX_BUILDINGS];
    // Run Dijkstra's algorithm from the index of the first building
    dijkstra(graph, index1, dist, prev);
 // Check if there is a path from the first building to the second building
    if (dist[index2] == INT_MAX) {
             // If there's no path, print a message indicating so
        printf("\nNo path from %s to %s  (infinity)\n", name1, name2);
    } else {
         // If there is a path, print the shortest path and its distance
       printf("\nShortest path from %s to %s: ", name1, name2);
       if (b){
       printShortestPath(graph, index1, index2, prev);
       }
        printf(" (Distance: %d)\n", dist[index2]);
    }
}
void displayMenu(){

        printf("\n1. Load the two files.\n");
        printf("2. Calculate the shortest distance between two buildings.\n");
        printf("3. Print the shortest route between two buildings and the total distance.\n");
        printf("4. Sort the courses using the topological sort.\n");
        printf("5. Print the sorted courses.\n");
        printf("6. Exit the application.\n");
}

int readCoursesFromFile(Course courses[]) {
     // Open the input file in read mode
    FILE *file = fopen("input_courses.txt", "r");
    if (!file) {
            // Print an error message if the file cannot be opened
        printf("Error opening file");
        return -1;
    }

    char line[256]; // array of char to hold each line from the file
    int course_count = 0;// Counter for the number of courses
// Read each line from the file
    while (fgets(line, sizeof(line), file)) {
           // Remove the newline character from the end of the line
        line[strcspn(line, "\n")] = 0;

         // Split the line into course and prerequisites using "#" as delimiter
        char *token = strtok(line, "#");
        if (token == NULL) {
            continue;// Skip if the line is empty or doesn't contain a course
        }
// Copy the course name to the courses arra
        strcpy(courses[course_count].course, token);
        courses[course_count].prereq_count = 0;
  // Process the prerequisites
        while ((token = strtok(NULL, "#"))) {
                // Split the prerequisites using " # " as delimiter
            char *prereq_token = strtok(token, " # ");
            while (prereq_token != NULL) {
                // Check if the number of prerequisites is within the limit
                if (courses[course_count].prereq_count < MAX_PREREQUISITES) {
                    // Copy each prerequisite to the courses array
                    strcpy(courses[course_count].prerequisites[courses[course_count].prereq_count], prereq_token);
                    courses[course_count].prereq_count++;
                }

                prereq_token = strtok(NULL, " # ");
            }
        }

        course_count++;
    }

    fclose(file); // to close the file
    return course_count;
}

Queue* createQueue() {
     // Allocate memory for the queue structure
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
      printf("Failed to create queue");
      return NULL;
    }
     // Initialize the queue attributes
    q->size = 0;
    q->front = 0;
    q->rear = 0;
    return q;
}

int isEmpty2(Queue *q) {
    return q->size == 0;
}
void enqueue(Queue* q, int value) {
    // Check if the queue is full
    if (q->size == MAX) {
        printf("Queue is full\n");
        return;
    }
      // Insert the value into the queue and update rear pointer
    q->array[q->rear] = value;
    q->rear ++; // Move rear pointer to the next position
    // Increment the size of the queue
    q->size++;
}
int dequeue(Queue* q) {
    if (isEmpty2(q)) {
        printf("Queue is empty\n");
        return -1; // Return -1 indicating failure
    }
    int value = q->array[q->front];
    q->front ++;  // Move front pointer to the next position
    q->size--;
    return value;
}

void freeQueue(Queue* q) {
    free(q);
}

Node* find_node(Node nodes[], int node_count, char *course) {
   // Iterate through the array of nodes
    for (int i = 0; i < node_count; i++) {
        // Check if the course name matches the given course
        if (strcmp(nodes[i].course, course) == 0) {
            // Return a pointer to the matching node
            return &nodes[i];
        }
    }
    // If no matching node is found, return NULL
    return NULL;
}

int build_graph(Course courses[], int course_count, Node nodes[]) {
    int node_count = 0;
 // Iterate through each course in the list
    for (int i = 0; i < course_count; i++) {
         // Find or create a node for the current course
        Node* node = find_node(nodes, node_count, courses[i].course);
        if (node == NULL) {
                 // If the node doesn't exist, create a new one
            node = &nodes[node_count++];
            strcpy(node->course, courses[i].course);
            node->indegree = 0;
            node->indegree = 0;
            node->out_count = 0;
        }
       // Process prerequisites for the current course
        for (int j = 0; j < courses[i].prereq_count; j++) {
           // Find or create a node for the prerequisite course
            Node* prereq_node = find_node(nodes, node_count, courses[i].prerequisites[j]);
            if (prereq_node == NULL) {
                prereq_node = &nodes[node_count++];
                strcpy(prereq_node->course, courses[i].prerequisites[j]);
                prereq_node->indegree = 0;
                prereq_node->outdegree = 0;
                prereq_node->out_count = 0;
            }
          // Update adjacency lists and degrees
            strcpy(prereq_node->adj_out[prereq_node->out_count++], node->course);
            node->indegree++;
            prereq_node->outdegree++;
        }
    }

    return node_count;
}
void topological_sort(Node nodes[], int node_count,int b) {
     // Create a queue for nodes with zero indegree
    Queue* queue = createQueue();
    // Array to store the sorted order of courses
  //  char sorted_order[MAX_COURSES][MAX_COURSE_NAME];
    int sorted_count = 0;
// Enqueue nodes with zero indegree
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].indegree == 0) {
            enqueue(queue, i);
        }
    }
 // Perform topological sorting
    while (!isEmpty2(queue)) {
        int node_idx = dequeue(queue);
        // Add the course to the sorted order
        strcpy(sorted_order[sorted_count++], nodes[node_idx].course);
     // Update indegree of adjacent nodes
        for (int i = 0; i < nodes[node_idx].out_count; i++) {
            Node *adj_node = find_node(nodes, node_count, nodes[node_idx].adj_out[i]);
            if (adj_node != NULL) {
                adj_node->indegree--;
                // If indegree becomes zero, enqueue the node
                if (adj_node->indegree == 0) {
                    enqueue(queue, adj_node - nodes); // Calculate index difference
                }
            }
        }
    }
if (b==1){
    if (sorted_count != node_count) {
        printf("There exists a cycle in the graph\n");
    } else {
        //print the function
        printf("Topological Sort of Courses:\n");
        for (int i = 0; i < sorted_count; i++) {
            printf("%s\n", sorted_order[i]);
        }
    }
}
else {
printf("the courses sort sucssesfully\n");
}

    freeQueue(queue);
}
