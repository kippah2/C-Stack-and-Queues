#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Description: Program that provides users with multiple options regarding data
// structure manipulation and linking. Focus on stacks and queues.


// adding cyan color to program outputs. not required, added as a fun
// experiment.
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

////////////////////////////////////////////////

// define the structure of node, which will be stored in stacks and queues
typedef struct node {
    char *first_name;
    char *last_name;
    int puid, age;
    struct node *next;
} node_t;

// initializing node specific functions
node_t *node_new(void);
void node_print(node_t *node);
void node_free(node_t *node);

////////////////////////////////////////////////

// defining a structure to store the top of stack pointer so that we can
// maintain multiple stacks
// not required but added for robustness
typedef struct {
    node_t *top;
} stack_t;

// declaring functions relating to stacks
void stack_push(stack_t *stack, node_t *node);
node_t *stack_pop(stack_t *stack);
bool stack_is_empty(stack_t *stack);
void stack_clear(stack_t *stack);
void stack_print(stack_t *stack);
size_t stack_size(stack_t *stack);

////////////////////////////////////////////////

// defining a structure that contains front and back pointers so that we can
// maintain multiple queues
// not required but added for robustness
typedef struct {
    node_t *front, *back;
} queue_t;

// declaring functions relating to queues
void queue_enqueue(queue_t *queue, node_t *node);
node_t *queue_dequeue(queue_t *queue);
bool queue_is_empty(queue_t *queue);
void queue_clear(queue_t *queue);
void queue_print(queue_t *queue);
size_t queue_size(queue_t *queue);

////////////////////////////////////////////////

// node_new allocates memory for the node on the heap
node_t *node_new() {
    // allocate memory and declare variables
    node_t *newNode = malloc(sizeof(node_t));
    char first_name[256];
    char last_name[256];
    int puid, age;

    // request user input to be put into the contents of the node
    printf("Enter first name: ");
    scanf("%s", first_name);
    printf("Enter last name: ");
    scanf("%s", last_name);
    printf("Enter your PUID: ");
    scanf("%d", &puid);
    printf("Enter your age: ");
    scanf("%d", &age);

    // move inputted values into the newly created node
    newNode->first_name = strdup(first_name);
    newNode->last_name = strdup(last_name);
    newNode->puid = puid;
    newNode->age = age;
    newNode->next = NULL;

    return newNode;
}

// function to display the contents of a node
void node_print(node_t *node) {
    printf("Node(first_name: %s, last_name: %s, puid: %d, age: %d, next: %p)\n",
           node->first_name, node->last_name, node->puid, node->age,
           (void *) node->next);
}

// function to free a node and all the data it owns if first_name and last_name 
// aren't freed when the node is freed, there will be a memory leak
void node_free(node_t *node) {
    if (node != NULL) {
        free(node->first_name);
        free(node->last_name);
        free(node);
    }
}

////////////////////////////////////////////////

// function to push new node on the top of the stack
void stack_push(stack_t *stack, node_t *node) {
    // set the previous top node's next pointer to the one being put on top of
    // it since it's a stack, this means it'll be the top pointer as that's the
    // only part of the stack you can access
    node->next = stack->top;
    stack->top = node;
}

// function that creates a tempory node and points to the previous top of stack
node_t *stack_pop(stack_t *stack) {
    // adds placeholder node that equals previous top of stack
    node_t *node = stack->top;
    // set top of stack pointer to one node below previous top, so that it may
    // become the new top of stack
    stack->top = node->next;
    // set new top of stack node's next pointer to NULL, indicating it is now
    // at the top of the stack
    node->next = NULL;
    // returns previously top of stack node
    return node;
}
// function to check if the stack is empty. if the top of the stack is empty, we
// can assume the stack is empty.
bool stack_is_empty(stack_t *stack) {
    return stack->top == NULL;
}
// function to clear all nodes from the stack
void stack_clear(stack_t *stack) {
    // so long as the stack isn't empty, we're going to use the pop function to
    // remove the nodes. stack being empty is end condition of the loop.
    while (!stack_is_empty(stack))
        // need to free the memory so that it does not leak upon clearing
        node_free(stack_pop(stack));
}
// function that prints out the nodes of the stack
void stack_print(stack_t *stack) {
    // for each node in the stack, starting from the top node, until we reach a
    // null node, we will print the contents of the node
    for (node_t *node = stack->top; node != NULL; node = node->next)
        node_print(node);
}
// function that finds the number of nodes present in the stack
size_t stack_size(stack_t *stack) {
    size_t size = 0;
    // iterates through the stack starting at the top, adding one to the size
    // counter each iteration. reaching a null node is the end condition
    for (node_t *node = stack->top; node != NULL; node = node->next)
        size++;
    return size;
}

////////////////////////////////////////////////

// function that adds a new node to the back of the queue
void queue_enqueue(queue_t *queue, node_t *node) {
    if (queue_is_empty(queue)) {
        // if the queue is empty, the node that gets added is both the front and
        // the back simultaneously
        queue->front = node;
        queue->back = node;
    } else {
        // if the queue isn't empty, the pointer to the node that was previously
        // the back of the queue needs to point to the newly added node so it's
        // no longer NULL
        queue->back->next = node;
        // then we make the back pointer point to the newly added node
        queue->back = node;
    }
}

// function that removes the front of the queue and returns it
node_t *queue_dequeue(queue_t *queue) {
    // temporary pointers for the front of the queue
    node_t *node = queue->front;
    // sets front of queue pointer to point to new front of queue
    // which is node linked by previous front of queue
    queue->front = node->next;
    if (queue_is_empty(queue))
        // if the queue is empty, set pointer to back of queue to NULL
        // because there's nothing there
        queue->back = NULL;
    // set next pointer of previous front of queue to NULL so it's no
    // longer linked to the queue
    node->next = NULL;
    // return the node that was removed from the front of the queue
    return node;
}

// function that checks if the queue contains any nodes
bool queue_is_empty(queue_t *queue) {
    // if the queue->front pointer is NULL, we can assume the queue is empty
    // this is because we point queue-front to a node when we add our first node
    // and remove it when we clear the queue
    return queue->front == NULL;
}

// function for clearing all nodes from the queue
void queue_clear(queue_t *queue) {
    while (!queue_is_empty(queue))
        // loop through dequeue function until the queue is empty
        node_free(queue_dequeue(queue));
}

// function for printing each node and the contents of the queue
void queue_print(queue_t *queue) {
    // iterates through the queue starting at the front, printing the contents
    // of each node
    for (node_t *node = queue->front; node != NULL; node = node->next)
        node_print(node);
}

// function for counting the number of nodes in the queue
size_t queue_size(queue_t *queue) {
    size_t size = 0;
    // iterates through the queue starting at the front, adding one to the size
    // counter each iteration. stops when encounters a node that is NULL
    for (node_t *node = queue->front; node != NULL; node = node->next)
        size++;
    return size;
}

////////////////////////////////////////////////

// first function that contains the menu
int main() {
    // since this program supports multiple stacks and queues,
    // we are assigning one top pointer per stack, and one back and front
    // pointers per queue.
    stack_t stack = { .top = NULL };
    queue_t queue = { .back = NULL, .front = NULL };

    while (true) {
        // print user options
        printf(RESET
               "Choose a function you'd like to run (1-10):\n"
               "  1) Push - Push a node onto the top of the stack\n"
               "  2) Pop - Pop a node off of the top of the stack\n"
               "  3) Enqueue - Enqueue a node at the back of the queue\n"
               "  4) Dequeue - Dequeue a node from the front of the queue\n"
               "  5) Clear Stack - Remove all nodes from the stack\n"
               "  6) Clear Queue - Remove all nodes from the queue\n"
               "  7) Print Stack - Print all nodes in the stack\n"
               "  8) Print Queue - Print all nodes in the queue\n"
               "  9) Reverse Queue - Use the stack to reverse the queue\n"
               "  10) Exit - Exit the program\n"
               "?: ");

        // initialize user choice variable and scan input from user to be stored
        int choice;
        scanf("%d", &choice);

        // output will now be cyan colored. not required but done for funsies :)
        printf(CYAN "\n");

        // switch statement to compare user input to options available
        switch (choice) {
        case 1:
            // call push function, pass node_new() function, which means user
            // will be prompted to input content for the node being pushed
            stack_push(&stack, node_new());
            break;
        case 2:
            // check if stack is empty and notify user if true
            if (stack_is_empty(&stack)) {
                printf("Stack is empty!\n");
            } else {
                // if stack isn't empty, set node pointer equal to results of
                // returned node from pop function
                node_t *node = stack_pop(&stack);
                node_print(node);
                node_free(node);
            }
            break;
        case 3:
            queue_enqueue(&queue, node_new());
            break;
        case 4:
            // if the queue is empty, notify the user
            if (queue_is_empty(&queue)) {
                printf("Queue is empty!\n");
            } else {
                // if queue is not empty, we are setting a node pointer to the
                // item at the front of the queue, which is removed from queue
                // in the function
                node_t *node = queue_dequeue(&queue);
                // print the node that was removed from the front of the queue
                node_print(node);
                // free allocated memory for node removed from the queue
                node_free(node);
            }
            break;
        // for case 5-9, the %zu format specifier is used for printing out the size_t type,
        // which is whats returned by the sizeof() function.
        case 5:
            printf("Cleared %zu node(s).\n", stack_size(&stack));
            stack_clear(&stack);
            break;
        case 6:
            printf("Cleared %zu node(s).\n", queue_size(&queue));
            queue_clear(&queue);
            break;
        case 7:
            printf("There are %zu node(s) in the stack:\n", stack_size(&stack));
            stack_print(&stack);
            break;
        case 8:
            printf("There are %zu node(s) in the queue:\n", queue_size(&queue));
            queue_print(&queue);
            break;
        case 9:
            // this will reverse the queue by putting the contents of the queue
            // into a stack
            {
                // we get the size of the queue first, so we know how many
                // entries to pop from the stack later. this way we can flip the
                // queue without touching the entries already on the stack.
                size_t queue_size_before = queue_size(&queue);
                while (!queue_is_empty(&queue))
                    // we are pushing the node that is being returned by dequeue
                    // onto the stack
                    stack_push(&stack, queue_dequeue(&queue));
                // we are then going to put the entries back into the queue with
                // enqueue. this only happens as many times as there was
                // originally nodes in the queue
                for (size_t i = 0; i < queue_size_before; ++i)
                    queue_enqueue(&queue, stack_pop(&stack));
                printf("Queue reversed.\n");
            }
            break;
        case 10:
            stack_clear(&stack);
            queue_clear(&queue);
            return 0;
        }

        printf("\n");
    }
}
