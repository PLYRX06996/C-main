Node* insertAtPosition(Node** head, int pos, int value){
    Node* temp = newNode(value);
    if(pos < 0) return head;
    if(pos == 1) {
        temp->next = *head;
        return temp;
        }
        
    Node* prev = head;
    int count = 1;
    
    while(count < pos - 1 && prev->next != NULL){
        prev = prev->next;
        count++;
        }
    if(prev == NULL){
        prev->next = temp;
        return head;
        }
    temp->next = prev->next;
    prev->next = temp;
    return head;
}