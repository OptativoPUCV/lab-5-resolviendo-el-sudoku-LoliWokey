#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n) {
    int f, c;
    for (f = 0; f < 9; f++) {
        int fila[10] = {0};
        int col[10] = {0};
        for (c = 0; c < 9; c++) {
            int val_fila = n->sudo[f][c];
            if (val_fila && fila[val_fila]++) return 0;
            
            int val_col = n->sudo[c][f];
            if (val_col && col[val_col]++) return 0;
        }
    }

    for (int k = 0; k < 9; k++) {
        int box[10] = {0};
        for (int p = 0; p < 9; p++) {
            int i = 3*(k/3) + p/3;
            int j = 3*(k%3) + p%3;
            int val = n->sudo[i][j];
            if (val && box[val]++) return 0;
        }
    }

    return 1;
}

List* get_adj_nodes(Node* n) {
    List* list = createList();
    int fila = -1, col = -1;
    
    for (int a = 0; a < 9 && fila == -1; a++) {
        for (int b = 0; b < 9 && col == -1; b++) {
            if (n->sudo[a][b] == 0) {
                fila = a;
                col = b;
            }
        }
    }
    
    if (fila == -1 || col == -1) return list;
    
    for (int k = 1; k <= 9; k++) {
        Node* copia = copy(n);
        copia->sudo[fila][col] = k;
        if (is_valid(copia)) {
            pushBack(list, copia);
        } else {
            free(copia);
        }
    }
    
    return list;
}

int is_final(Node* n){
  for (int f = 0 ; f < 9 ; f++){
    for (int c = 0 ; c < 9 ; c++){
      int num = n->sudo[f][c] ;
      if (num == 0) return 0 ;
    }
  }
  return 1;
}

Node* DFS(Node* initial, int* cont) {
    Stack* S = createStack();
    push(S, initial);

    while (!is_empty(S)) {
        (*cont)++;
        Node* current = top(S);
        pop(S);

        if (is_final(current)) {
            while (!is_empty(S)) {
                Node* temp = top(S);
                pop(S);
                free(temp);
            }
            free(S);
            return current;
        }

        List* adj_nodes = get_adj_nodes(current);
        Node* adj_node = first(adj_nodes);
        
        while (adj_node != NULL) {
            push(S, adj_node);
            adj_node = next(adj_nodes);
        }

        free(adj_nodes);
        free(current);
    }

    free(S);
    return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/