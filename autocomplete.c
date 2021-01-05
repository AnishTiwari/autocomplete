#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "autocomplete.h"

int** create2d(int row, int col){

  int **arr = calloc(row , sizeof( int *)); 
  for(int i = 0; i < row; i++) 
    arr[i] = calloc(col , sizeof(int)); 

  return arr;
}

nfa_t construct_nfa(nfa_t nfa, char* word, int dist, int word_l){

  
  printf("%d\n", word_l);
  printf("%d\n", dist);
  for(int i = 0; i < word_l ; i++){
    for(int j = 0; j < dist; j++){

      s_t new_s = malloc(sizeof(s));
      new_s->state = malloc(sizeof(char) * 2);
      sprintf(new_s->state, "%d%d", i, j);
      new_s->edge_count = -1;
      new_s->edges = NULL;
       
      /* add edge for correct symbol (same level but diff state 00, 01, 02...) */ 
      e_t new_e_correct = malloc(sizeof(e));
      new_e_correct->mem_count = -1;
      new_e_correct->symbol = word[i];

      sprintf(new_e_correct->estate[++new_e_correct->mem_count] ,"%d%d", i+1, j);
      
      if(new_s->edge_count == -1) {
	new_s->edges = malloc(sizeof(e));
	
	memcpy(&new_s->edges[++new_s->edge_count] , new_e_correct, sizeof(e));   
	printf("7777 %s\n", new_s->edges[0].estate[0]);
      }
      
      /* add edge for deletion */
      e_t new_e_star = malloc(sizeof(e));
      new_e_star->symbol = '*';
      sprintf(new_e_star->estate[++new_e_correct->mem_count] ,"%d%d", i,j+1);
      sprintf(new_e_star->estate[++new_e_correct->mem_count] ,"%d%d", i+1, j+1);
  
      
      if(new_s->edge_count == -1) {
	new_s->edges = malloc(sizeof(e));
	new_s->edges[new_s->edge_count++] = *new_e_star;   
      }
      
      else {
	new_s->edge_count++;
	new_s->edges = realloc(new_s->edges, sizeof(e) * (new_s->edge_count + 1));
	new_s->edges[new_s->edge_count] = *new_e_star;   
      }
   
      if(nfa->state_count == -1) {
	nfa->states = malloc(sizeof(s));
	nfa->states[0] = *new_s;
	nfa->state_count++;
      }
      
      else {
	nfa->state_count++;
	nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
	nfa->states[nfa->state_count] = *new_s;	
      }

      
    }
  } /* end of for loops */

  for(int i=word_l; i==word_l; i++){
    for(int j=0;j < dist; j++){
      s_t last_states = malloc(sizeof(s));
      last_states->edge_count = -1;
      last_states->edges = NULL;

      last_states->state = malloc(sizeof(char) *2);
      sprintf(last_states->state, "%d%d", i,j);

      e_t last_edge = malloc(sizeof(e));
      last_edge->symbol = '*';
      last_edge->mem_count = -1;
      sprintf(last_edge->estate[++last_edge->mem_count], "%d%d", i, j+1);

      last_states->edges = malloc(sizeof(e));
      last_states->edges = last_edge;

      if(nfa->state_count == -1) {
	nfa->states = malloc(sizeof(s));
	nfa->states[0] = *last_states;
	nfa->state_count++;
      }
      
      else {
	nfa->state_count++;
	nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
	nfa->states[nfa->state_count] = *last_states;	
      }
      
    }
  }

  for(int i=0; i < word_l; i++){
    for(int j=dist; j==dist; j++){

      s_t last_states = malloc(sizeof(s));
      last_states->edge_count = -1;
      last_states->edges = NULL;

      last_states->state = malloc(sizeof(char) *2);
      sprintf(last_states->state, "%d%d", i,j);

      e_t last_edge = malloc(sizeof(e));
      last_edge->symbol = word[i];
      last_edge->mem_count = -1;
      sprintf(last_edge->estate[++last_edge->mem_count], "%d%d", i+1, j);

      last_states->edges = malloc(sizeof(e));
      last_states->edges = last_edge;

      if(nfa->state_count == -1) {
	nfa->states = malloc(sizeof(s));
	nfa->states[0] = *last_states;
	nfa->state_count++;
      }
      
      else {
	nfa->state_count++;
	nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
	nfa->states[nfa->state_count] = *last_states;	
      }
      
    }
  }

  s_t final_state = malloc(sizeof(s));
  final_state->edges=NULL;
  final_state->edge_count = -1;
  final_state->state = malloc(sizeof(char) * 2);
  sprintf(final_state->state, "%d%d", word_l,dist);

  if(nfa->state_count == -1) {
    nfa->states = malloc(sizeof(s));
    nfa->states[0] = *final_state;
    nfa->state_count++;
  }
      
  else {
    nfa->state_count++;
    nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
    nfa->states[nfa->state_count] = *final_state;	
  }
  
  return nfa;
}

void print_nfa(nfa_t nfa){

  for(int i=0; i < nfa->state_count ;i++){

    printf("%s ---( %c )--> %s\n",nfa->states[i].state, nfa->states[i].edges[0].symbol, nfa->states[i].edges[0].estate[0]);


  }

}

int main(int argc, char *argv[]){

  char* word = NULL;
  if (argc == 2)
    word = argv[1];

  else
    {
      fprintf(stderr, "Invalid arguments EXITING\n\n");
      exit(1);
    }

  int lev_dist = DISTANCE;

  int word_len = strlen(word);

  nfa_t nfa = malloc(sizeof(n));
  nfa->states=NULL;
  nfa->state_count =-1;
  
  nfa = construct_nfa(nfa, word, lev_dist, word_len);
  printf("%d states\n", nfa->state_count+1);
  print_nfa(nfa);

  /* find_matches(word, lev_dist, word_len); */
  return 0;
}
