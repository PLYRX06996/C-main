#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

/*Operations on Singly Linked List
Traversal
Searching
Length
Insertion:
Insert at the beginning
Insert at the end
Insert at a specific position
Deletion:
Delete from the beginning
Delete from the end
Delete a specific node*/

// Definition of a Node in a singly linked list
struct Node {
    int data;          
    struct Node* next;
}Node;

// Function to create a new Node
struct Node* newNode(int data) {
    //Allocate memory for new node
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    //Assigning data to new node
    temp->data = data;
    //initializing next pointer to NULL
    temp->next = NULL;
    return temp;
}


//Transversal of Linked List

// Function to traverse and print the elements 
// of the linked list
void traverseLinkedList(struct Node* head)
{
    // Start from the head of the linked list
    struct Node* current = head;

    // Traverse the linked list until reaching the end (NULL)
    while (current != NULL) {

        // Print the data of the current node
        printf("%d ", current->data);

        // Move to the next node
        current = current->next;
    }

    printf("\n");
}
//Searching in Linked List
// Function to search for a value in the Linked List
bool searchLinkedList(struct Node* head, int target)
{
    // Traverse the Linked List
    while (head != NULL) {

        // Check if the current node's
        // data matches the target value
        if (head->data == target) {
            printf("%d", head->data);
            return true; // Value found
        }

        // Move to the next node
        head = head->next;
    }

    return false; // Value not found
}
// Function to find the length of the linked list
int findLength(struct Node* head)
{
    // Initialize a counter for the length
    int length = 0;

    // Start from the head of the list
    struct Node* curr = head;

    // Traverse the list and increment
    // the length for each node
    while (curr != NULL) {
        length++;
        curr = curr->next;
    }

    // Return the final length of the linked list
    return length;
}
//Insertion in a Singly Linked List
// Function to insert a new node at the beginning of the linked list
struct Node* insertAtBeginning(struct Node* head, int value)
{
    // Create a new node with the given value
    struct Node* new_node = newNode(value);

    // Set the next pointer of the new node to the current head
    new_node->next = head;

    // Move the head to point to the new node
    head = new_node;

    // Return the new head of the linked list
    return head;
}

// Function to insert a node at the end of the linked list
struct Node* insertAtEnd(struct Node* head, int value)
{
    // Create a new node with the given value
    struct Node* new_node = newNode(value);

    // If the list is empty, make the new node the head
    if (head == NULL)
        return new_node;

    // Traverse the list until the last node is reached
    struct Node* curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    // Link the new node to the current last node
    curr->next = new_node;

    return head;
}

// Function to insert a node at a specified position
struct Node* insertPos(struct Node* head, int pos, int data) {
    if (pos < 1) {
        printf("Invalid position!\n");
        return head;
    }

    // Special case for inserting at the head
    if (pos == 1) {
        struct Node* temp = newNode(data);
        temp->next = head;
        return temp;
    }

    // Traverse the list to find the node 
    // before the insertion point
    struct Node* prev = head;
    int count = 1;
    while (count < pos - 1 && prev != NULL) {
        prev = prev->next;
        count++;
    }

    // If position is greater than the number of nodes
    if (prev == NULL) {
        printf("Invalid position!\n");
        return head;
    }

    // Insert the new node at the specified position
    struct Node* temp = newNode(data);
    temp->next = prev->next;
    prev->next = temp;

    return head;
}

int main(){
    
    return 0;
}