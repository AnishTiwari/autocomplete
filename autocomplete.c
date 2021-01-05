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

  for(int i = 0; i < word_l ; i++){
    for(int j = 0; j < dist; j++){

      s_t new_s = malloc(sizeof(s));
      new_s->state = malloc(sizeof(char) * 2);
      sprintf(new_s->state, "%d%d", i, j);
      new_s->edge_count = -1;
      new_s->edges = NULL;
       
      /* add edge for correct symbol (same level but diff state 00, 01, 02...) */ 
      e_t new_e_correct = malloc(sizeof(e));
      new_e_correct->symbol = word[i];
      char* temp = NULL;
      temp = malloc(sizeof(char) * 2);
      sprintf(temp,"%d%d", i+1,j); 
      new_s->state = temp;
     
      if(new_s->edge_count == -1) {
	new_s->edges = malloc(sizeof(e));
	new_s->edges[new_s->edge_count++] = new_e_correct;   
      }
      
      /* add edge for deletion */
      e_t new_e_del = malloc(sizeof(e));
      new_e_del->symbol = '*';
      char* temp__ = NULL;
      
      temp__ = malloc(sizeof(char) * 2);
      sprintf(temp__, "%d%d", i, j+1); 
     
      new_e_del->state = temp__;

      if(new_s->edge_count == -1) {
	new_s->edges = malloc(sizeof(e));
	new_s->edges[new_s->edge_count++] = *new_e_del;   
      }
      
      else {
	new_s->edge_count++;
	new_s->edges = realloc(new_s->edges, sizeof(e) * (new_s->edge_count + 1));
	new_s->edges[new_s->edge_count] = *new_e_del;   
      }
   
      /* add edge for substitution / insertion */
      e_t new_e_ins = malloc(sizeof(e));
      new_e_ins->symbol = '*';
      char* temp_ = NULL;
      temp_ = malloc(sizeof(char) * 2);
      sprintf(temp_, "%d%d", i+1, j+1);
      new_e_ins->state = temp;
      
      if(new_s->edge_count == -1) {

	new_s->edges = malloc(sizeof(e));
	new_s->edges[new_s->edge_count++] = *new_e_ins;   
      }
      
      else {
	new_s->edge_count++;
	new_s->edges = realloc(new_s->edges, sizeof(e) * (new_s->edge_count +1));
	new_s->edges[new_s->edge_count] = *new_e_ins;   
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
  
  return nfa;
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
  printf("%d states\n",nfa->state_count);
    /* find_matches(word, lev_dist, word_len); */
  return 0;
}
