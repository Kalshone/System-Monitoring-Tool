/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 * Name: Katherine Pravdin
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- to help designing your code
 *************************************************************************/

/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap* heap, int maybeIdx){
       if (heap != NULL && (maybeIdx > 0 && maybeIdx <= heap->size)){
              return true;
       }
       return false;
}

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex){
       return heap->arr[nodeIndex].priority;
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap* heap, int nodeIndex){
       int left = 2 * nodeIndex;
       if (isValidIndex(heap, left)){ // covers NULL case
              return left;
       }
       return NOTHING;
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap* heap, int nodeIndex){
       int right = 2 * nodeIndex + 1;
       if (isValidIndex(heap, right)){ // covers NULL case
              return right;
       }
       return NOTHING;
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex){
       int parent = (nodeIndex) / 2; // already floored bc integer division
       if (isValidIndex(heap, parent)){ // covers NULL case
              return parent;
       }
       return NOTHING;
}

/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap *heap, int index1, int index2) {
       if (heap == NULL)
              return;
       if (isValidIndex(heap, index1) && isValidIndex(heap, index2)){
              int id1 = heap->arr[index1].id;
              int id2 = heap->arr[index2].id;
              HeapNode temp = heap->arr[index1];
              heap->arr[index1] = heap->arr[index2];
              heap->arr[index2] = temp;
              heap->indexMap[id1] = index2;
              heap->indexMap[id2] = index1;
              heap->arr[index1].id = id2;
              heap->arr[index2].id = id1;
       }
}

/* Returns the the index of the child with the minimum priority.
 */
int minIdx(MinHeap *heap, int parent_node) {
  int left_child = leftIdx(heap, parent_node);
  int right_child = rightIdx(heap, parent_node);
  return priorityAt(heap, left_child) < priorityAt(heap, right_child)
             ? left_child
             : right_child;
}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap *heap, int nodeIndex) {
  if (heap == NULL || !isValidIndex(heap, nodeIndex))
    return;
  int child_node = nodeIndex;
  while (isValidIndex(heap, parentIdx(heap, child_node))) {
    int parent_node = parentIdx(heap, child_node);
    if (priorityAt(heap, child_node) < priorityAt(heap, parent_node)) {
      swap(heap, parent_node, child_node);
    }
    child_node = parent_node;
  }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap *heap) {
  if (heap == NULL)
    return;
  int parent = ROOT_INDEX;
  while (isValidIndex(heap, minIdx(heap, parent))) {
    int min_child = minIdx(heap, parent);
    if (priorityAt(heap, min_child) < priorityAt(heap, parent)) {
      swap(heap, parent, min_child);
    }
    parent = min_child;
  }
}


/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex){
       return heap->arr[nodeIndex];
}


/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex){
       return nodeAt(heap, nodeIndex).id;
}


/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id){
       return heap->indexMap[id];
}

/*********************************************************************
 * Required functions
 ********************************************************************/

/* Returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode getMin(MinHeap* heap){
       return heap->arr[ROOT_INDEX];
}

/* Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode extractMin(MinHeap* heap){
       HeapNode min = getMin(heap);
       swap(heap, ROOT_INDEX, heap->size);
       heap->indexMap[min.id] = ROOT_INDEX - 1;
       heap->size--;
       bubbleDown(heap);
       return min;
}

/* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */
void insert(MinHeap* heap, int priority, int id){
       heap->size++;
       heap->arr[heap->size].priority = priority;
       heap->arr[heap->size].id = id;
       heap->indexMap[heap->arr[heap->size].id] = heap->size;
       bubbleUp(heap, heap->size);
}

/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap* heap, int id){
       int index = indexOf(heap, id);
       if (heap != NULL && isValidIndex(heap, index)){
              return priorityAt(heap, index);
       }
       return NOTHING;
}

/* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
bool decreasePriority(MinHeap* heap, int id, int newPriority){
       if (heap != NULL){
              int index = indexOf(heap, id);
              if (isValidIndex(heap, index) && priorityAt(heap, index) > newPriority) {
                     heap->arr[index].priority = newPriority;
                     bubbleUp(heap, index);
                     return true;
              }  
       }
       return false;
}

/* Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap* newHeap(int capacity){
       if (capacity <= 0){
              return NULL;
       }
       MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
       // MinHeap* heap = (MinHeap*)calloc(1, sizeof(MinHeap));
       if (heap == NULL){
              return NULL;
       }
       heap->capacity = capacity;
       heap->size = 0;
       heap->arr = (HeapNode*)malloc(sizeof(HeapNode) * (capacity + ROOT_INDEX)); // +1 for 1-based indexing
       if (heap->arr == NULL){
              free(heap->arr);
              free(heap);
              return NULL;
       }
       heap->indexMap = (int*)malloc(sizeof(int) * (capacity + ROOT_INDEX));
       if(heap->indexMap == NULL){
              free(heap->indexMap);
              free(heap);
              return NULL;
       }
       return heap;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap* heap){
       if (heap != NULL){
              free(heap->arr);
              free(heap->indexMap);
              free(heap);
       }
}

/*********************************************************************
 ** Helper function provided
 *********************************************************************/

/* Prints the contents of this heap, including size, capacity, full index
 * map, and, for each non-empty element of the heap array, that node's ID and
 * priority. */
void printHeap(MinHeap* heap) {
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}
