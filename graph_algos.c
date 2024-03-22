/*
 * Graph algorithms.
 *
 * Author (of starter code): A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1
#define DEBUG 0

typedef struct records {
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 * As a reminder: 
 typedef struct heap_node {
  int priority;  // priority of this node
  int id;        // the unique ID of this node (vertex ID); 0 <= id < size
} HeapNode;

typedef struct min_heap {
  int size;       // the number of nodes in this heap; 0 <= size <= capacity
  int capacity;   // the number of nodes that can be stored in this heap
  HeapNode* arr;  // the array that stores the nodes of this heap
  int* indexMap;  // indexMap[id] is the index of node with ID id in array arr
} MinHeap;
 */
MinHeap* initHeap(Graph* graph, int startVertex){
  MinHeap* heap = malloc(sizeof(MinHeap));
  if (heap == NULL) {
    return NULL;
  }
  
  heap->size = graph->numVertices;
  heap->capacity = graph->numVertices;
  heap->arr = malloc(graph->numVertices * sizeof(HeapNode));
  heap->indexMap = malloc(graph->numVertices * sizeof(int));
  
  if (heap->arr == NULL || heap->indexMap == NULL) {
    free(heap->arr);
    free(heap->indexMap);
    free(heap);
    return NULL;
  }
  
  for (int i = 0; i < graph->numVertices; i++) {
    heap->arr[i].priority = INT_MAX;
    heap->arr[i].id = i;
    heap->indexMap[i] = i;
  }
  
  heap->arr[startVertex].priority = 0;
  heap->arr[startVertex].id = startVertex;
  heap->indexMap[startVertex] = startVertex;
  
  return heap;
}

/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records* initRecords(Graph* graph, int startVertex){
  Records* records = malloc(sizeof(Records));
  if (records == NULL) {
    return NULL;
  }
  
  records->numVertices = graph->numVertices;
  records->heap = initHeap(graph, startVertex);
  records->finished = calloc(graph->numVertices, sizeof(bool));
  records->predecessors = malloc(graph->numVertices * sizeof(int));
  records->tree = malloc((graph->numVertices - 1) * sizeof(Edge));
  records->numTreeEdges = 0;
  
  if (records->heap == NULL || records->finished == NULL || records->predecessors == NULL || records->tree == NULL) {
    free(records->finished);
    free(records->predecessors);
    free(records->tree);
    free(records);
    return NULL;
  }
  
  for (int i = 0; i < graph->numVertices; i++) {
    records->predecessors[i] = NOTHING;
  }
  
  return records;
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap* heap){
  if (heap == NULL || heap->size == 0) {
    return true;
  }
  return false;
}

/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight){
  records->tree[ind].fromVertex = fromVertex;
  records->tree[ind].toVertex = toVertex;
  records->tree[ind].weight = weight;
  records->numTreeEdges++;
}

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex){
  EdgeList* path = NULL;
  EdgeList* head = NULL;
  while (vertex != startVertex) {
    EdgeList* newEdge = (EdgeList*)malloc(sizeof(EdgeList));
    if (newEdge == NULL) {
      fprintf(stderr, "Error: malloc failed in makePath\n");
      exit(1);
    }
    newEdge->edge = &distTree[vertex];
    newEdge->next = NULL;
    if (path == NULL) {
      path = newEdge;
      head = path;
    } else {
      path->next = newEdge;
      path = path->next;
    }
    vertex = distTree[vertex].fromVertex;
  }
  return head;
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
/* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.

Pseudocode for Prim's algorithm from lecture:
0. T := new container for edges
1. PQ := new min-heap()
2. start := pick a vertex
3. PQ.insert(0, start)
4. for each vertex v != start: PQ.insert(inf, v)
5. while not PQ.is-empty():
6.   u := PQ.extract-min()
7.   T.add((u.pred, u))
8.   for each v in u's adjacency list:
9.     if v in PQ and w(u, v) < priority(v):
10.      PQ.decrease-priority(v, w(u,v))
11.      v.pred := u
12. return T
 */
 
int isInMinHeap(MinHeap* heap, int id) {
  return heap->indexMap[id] < heap->size;
} 

void setPred(MinHeap* heap, int id, int pred) {
  heap->arr[heap->indexMap[id]].pred = pred;
}

EdgeList* getMSTprim(Graph* graph, int startVertex) {
  // Create a container for edges
  EdgeList* T = NULL;

  // Create a min-heap
  MinHeap* PQ = newHeap(graph->numVertices);

  // Insert startVertex with priority 0 into PQ
  insert(PQ, 0, startVertex);

  // Insert all other vertices into PQ with priority infinity
  for (int v = 0; v < graph->numVertices; v++) {
    if (v != startVertex) {
      insert(PQ, INT_MAX, v);
    }
  }

  // Main loop
  while (!isEmpty(PQ)) {
    // Extract the node with minimum priority from PQ
    HeapNode u = extractMin(PQ);

    // Add the edge (u.pred, u) to T
    if (u.pred != -1) {
      Edge* edge = newEdge(u.pred, u.id, getPriority(PQ, u.id));
      T = newEdgeList(edge, T);
    }

    // Update priorities of adjacent vertices
    EdgeList* adjList = graph->vertices[u.id]->adjList;
    while (adjList != NULL) {
      int v = adjList->edge->toVertex;
      int weight = adjList->edge->weight;

      // Check if v is in PQ and w(u, v) < priority(v), then PQ.decrease-priority(v, w(u,v)) and set v.pred := u
      if (isInMinHeap(PQ, v) && weight < getPriority(PQ, v)) {
        decreasePriority(PQ, v, weight);
        setPred(PQ, v, u.id);
      }

      adjList = adjList->next;
    }
  }

  // Free the min-heap
  deleteHeap(PQ);

  // Return the resulting MST
  return T;
}

/* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex) {
  MinHeap* PQ = newHeap(graph->numVertices);
  Edge* distTree = malloc(graph->numVertices * sizeof(Edge));
  if (PQ == NULL || distTree == NULL) {
    return NULL;
  }

  // Step 1: Insert start vertex with priority 0
  insert(PQ, 0, startVertex);
  distTree[startVertex].fromVertex = startVertex;
  distTree[startVertex].toVertex = startVertex;
  distTree[startVertex].weight = 0;

  // Step 2: Initialize distances of all other vertices to infinity
  for (int i = 0; i < graph->numVertices; i++) {
    if (i != startVertex) {
      insert(PQ, INT_MAX, i);
      distTree[i].fromVertex = i;
      distTree[i].toVertex = i;
      distTree[i].weight = INT_MAX;
    }
  }

  // Step 6: Dijkstra's algorithm
  while (!isEmpty(PQ)) {
    HeapNode u = extractMin(PQ);

    // Step 8: Relax edges from u to its adjacent vertices
    EdgeList* adjList = graph->vertices[u.id]->adjList;
    while (adjList != NULL) {
      int v = adjList->edge->toVertex;
      int weight = adjList->edge->weight;
      int newDist = distTree[u.id].weight + weight;

      if (newDist < distTree[v].weight) {
        decreasePriority(PQ, v, newDist);
        distTree[v].fromVertex = u.id;
        distTree[v].toVertex = v;
        distTree[v].weight = newDist;
      }

      adjList = adjList->next;
    }
  }

  deleteHeap(PQ);
  return distTree;
}

/* Creates and returns an array 'paths' of shortest paths from every vertex
 * in the graph to vertex 'startVertex', based on the information in the
 * distance tree 'distTree' produced by Dijkstra's algorithm on a graph with
 * 'numVertices' vertices and with the start vertex 'startVertex'.  paths[id]
 * is the list of edges of the form
 *   [(id -- id_1, w_0), (id_1 -- id_2, w_1), ..., (id_n -- start, w_n)]
 *   where w_0 + w_1 + ... + w_n = distance(id)
 * Returns NULL if 'startVertex' is not valid in 'distTree'.
 */
EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex){
  EdgeList** paths = malloc(numVertices * sizeof(EdgeList*));
  if (paths == NULL) {
    return NULL;
  }
  
  for (int i = 0; i < numVertices; i++) {
    paths[i] = makePath(distTree, i, startVertex);
  }
  
  return paths;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records) {
  if (records == NULL) return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}
