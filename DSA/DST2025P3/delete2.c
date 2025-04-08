void delOfeveryI(Node** head, int i){
    if(*head == NULL || i < 1){     /**/
        return;
    }
    if(i == 1){                     /**/
        Node* current = *head;      /**/
        while(current != NULL){     /**/
            Node* temp = current;   /**/
            current = current->next;/**/
            free(temp);             /**/
        }
        *head = NULL;
        return;
    }
    int l = 0;
    Node* temp = *head;
    while(temp != NULL){
        temp = temp->next;
        l++;
    }
    Node* current = *head;
    for(int j = i - 1; j < l; j += i){
        Node* temp = *head;
        if(current->next == NULL) break;
        while(j--){
            current = current->next;
        }
        temp = current->next;
        current->next = temp->next;
        free(temp);
    }
}