/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge) {
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList* head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex* vertex) {
  if (vertex == NULL) {
    printf("NULL");
  } else {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph* graph) {
  if (graph == NULL) {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/

/* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
Edge* newEdge(int fromVertex, int toVertex, int weight){
  Edge* edge = (Edge*)malloc(sizeof(Edge));
  if (edge == NULL) {
    fprintf(stderr, "Error: malloc failed in newEdge\n");
    exit(1);
  }
  edge->fromVertex = fromVertex;
  edge->toVertex = toVertex;
  edge->weight = weight;

  return edge;
}

/* Returns a newly created EdgeList containing 'edge' and pointing to the next
 * EdgeList node 'next'.
 */
EdgeList* newEdgeList(Edge* edge, EdgeList* next){
  EdgeList* edgeList = (EdgeList*)malloc(sizeof(EdgeList));
  if (edgeList == NULL) {
    fprintf(stderr, "Error: malloc failed in newEdgeList\n");
    exit(1);
  }
  edgeList->edge = edge;
  edgeList->next = next;

  return edgeList;
}

/* Returns a newly created Vertex with ID 'id', value 'value', and adjacency
 * list 'adjList'.
 * Precondition: 'id' is valid for this vertex
 */
Vertex* newVertex(int id, void* value, EdgeList* adjList){
  Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
  if (vertex == NULL) {
    fprintf(stderr, "Error: malloc failed in newVertex\n");
    exit(1);
  }
  vertex->id = id;
  vertex->value = value;
  vertex->adjList = adjList;

  return vertex;
}

/* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
Graph* newGraph(int numVertices){
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  if (graph == NULL) {
    fprintf(stderr, "Error: malloc failed in newGraph\n");
    exit(1);
  }
  graph->numVertices = numVertices;
  graph->numEdges = 0;
  graph->vertices = (Vertex**)malloc(numVertices * sizeof(Vertex*));
  if (graph->vertices == NULL) {
    fprintf(stderr, "Error: malloc failed in newGraph\n");
    exit(1);
  }
  for (int i = 0; i < numVertices; i++) {
    graph->vertices[i] = NULL;
  }

  return graph;
}

/* Frees memory allocated for EdgeList starting at 'head'.
 */
void deleteEdgeList(EdgeList* head){
  EdgeList* temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp->edge);
    free(temp);
  }
}

/* Frees memory allocated for 'vertex' including its adjacency list.
 */
void deleteVertex(Vertex* vertex){
  if (vertex != NULL) {
    deleteEdgeList(vertex->adjList);
    free(vertex);
  }
}

/* Frees memory allocated for 'graph'.
 */
void deleteGraph(Graph* graph){
  if (graph != NULL) {
    for (int i = 0; i < graph->numVertices; i++) {
      deleteVertex(graph->vertices[i]);
    }
    free(graph->vertices);
    free(graph);
  }
}