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


main_t global_state_table = NULL;

nfa_table_t dfa_state = NULL;

sym_t symb_table = NULL;

void fill_symbol_table(char* in_string){
  sym_t temp_sym = NULL;
  for(int i =0; i < (int)strlen(in_string); i++){
    HASH_FIND_STR(symb_table, &in_string[i], temp_sym);
    if(temp_sym == NULL){
      temp_sym= malloc(sizeof(sym));
      strcpy(temp_sym->symbol, &in_string[i]);
      HASH_ADD_STR(symb_table, symbol, temp_sym);
    }
  }
}

int INITIAL_COUNT = -1;

s_t find_state(char* state_name, nfa_t nfa){

  for(int p=0; p < nfa->state_count; p++){
    if( strcmp(nfa->states[p].state, state_name) == 0){
      return &nfa->states[p];
    }
  }

  printf("Not found any state with state name %s\n", state_name);
  return NULL;
}

void insert_dfa(char* dfa_state_name){
  nfa_table_t new_dfa_state = NULL;
  new_dfa_state = malloc(sizeof(nfa_table));
  new_dfa_state->state_name = malloc(sizeof(char) * strlen(dfa_state_name));
  strcpy(new_dfa_state->state_name , dfa_state_name);
  
  /* since it is already checked that the state name doesnt exists no need to check here */
  HASH_ADD_STR(dfa_state, state_name, new_dfa_state);
}

void insert_into_state(char* state_name, int is_multiple, int idx_v){

  main_t new_idx = malloc(sizeof(main_hash));
  new_idx->idx = idx_v;
  main_t temp_main = NULL;
  HASH_FIND_INT(global_state_table, &idx_v, temp_main);
  if (temp_main == NULL){
    HASH_ADD_INT(global_state_table, idx, new_idx);
  }
  else{
    perror("Unexpected Behavior\n");
    exit(1);
  }
   
  nfa_table_t new_state = malloc(sizeof(nfa_table));
  new_state->is_multiple = is_multiple;
  new_state->state_name = malloc(sizeof(char) * strlen(state_name));
  strcpy(new_state->state_name, state_name);
  nfa_table_t state_exists = NULL;
   
  HASH_FIND_STR(global_state_table,new_state->state_name, state_exists);
  if (state_exists == NULL){
    HASH_ADD_STR(global_state_table->nfa_table, state_name, new_state);
  }

  INITIAL_COUNT += 1; 

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
       
      /* add edge for correct symbol (same level but diff state 00, 01, 02...) */ 
      e_t new_e_correct = malloc(sizeof(e));
      new_e_correct->mem_count = -1;
      new_e_correct->symbol = word[i];
      char* temp = NULL;
      temp =malloc(sizeof(char)* 2);
      sprintf(temp ,"%d%d", i+1, j);
      ++new_e_correct->mem_count;
      new_e_correct->estate[new_e_correct->mem_count] = temp;
      if(new_s->edge_count == -1) {
	new_s->edges = malloc(sizeof(e));
	++new_s->edge_count;
	new_s->edges[0] = *new_e_correct;
       	new_s->edges[0].estate[0] = temp;
	printf("7777 %s %s %s -\n", new_e_correct->estate[0], temp, new_s->edges[0].estate[0]);
      }
     
      /* add edge for deletion */
      e_t new_e_star = malloc(sizeof(e));
      new_e_star->symbol = '*';
      new_e_star->mem_count = -1;
      char* temp1=NULL ;
      temp1 = malloc(sizeof(char)*2);
      char* temp2 = NULL;
      temp2 = malloc(sizeof(char)*2);
      sprintf(temp1 ,"%d%d", i,j+1);
      ++new_e_star->mem_count;
      new_e_star->estate[0]= temp1;
      sprintf(temp2 ,"%d%d", i+1, j+1);
      ++new_e_star->mem_count;
      new_e_star->estate[1]= temp2;
      
      if(new_s->edge_count == -1) {
	/* new_s->edges = malloc(sizeof(e)); */
	/* new_s->edges[new_s->edge_count++] = *new_e_star;    */
	/* printf("9999 %s %s %s -\n", new_e_star->estate[0], temp, new_s->edges[0].estate[0]); */

      }
      
      else {
	++new_s->edge_count;
	printf("###%d\n",new_s->edge_count);
	new_s->edges = realloc(new_s->edges, sizeof(e) * (new_s->edge_count + 1));
	new_s->edges[new_s->edge_count] = *new_e_star;
       	new_s->edges[new_s->edge_count].estate[0] = temp1; 
	
       	new_s->edges[new_s->edge_count].estate[1] = temp2;
	printf("5555 %d %s %s %s -\n", new_e_star->mem_count, new_e_star->estate[0], temp1, new_s->edges[1].estate[1]);
      }
      
      /* ========================================= */
      /* add state to nfa table */

      if(nfa->state_count == -1) {
	nfa->states = malloc(sizeof(s));
	insert_into_state(new_s->state, new_s->edges[0].mem_count, 0);
	insert_dfa(new_s->state);
	nfa->states[0] = *new_s;
	printf("8888 %s %s %s -\n", new_e_correct->estate[0], temp, new_s->edges[0].estate[0]);
	
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
      char* last_temp =NULL;
      last_temp = malloc(sizeof(char) *2);
      sprintf(last_temp, "%d%d", i, j+1);
      ++last_edge->mem_count;
      last_edge->estate[last_edge->mem_count] = last_temp;
      last_states->edges = malloc(sizeof(e));
      last_states->edges = last_edge;

      
      if(nfa->state_count == -1) {
	nfa->states = malloc(sizeof(s));
	insert_into_state(last_states->state, last_states->edges[0].mem_count, 0);

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
      char* final_temp =NULL;
      final_temp = malloc(sizeof(char)*2);
      sprintf(final_temp, "%d%d", i+1, j);
      
      last_edge->estate[++last_edge->mem_count] = final_temp;
      last_states->edges = malloc(sizeof(e));
      last_states->edges = last_edge;

      if(nfa->state_count == -1) {
	nfa->states = malloc(sizeof(s));

	insert_into_state( last_states->state, last_states->edges[0].mem_count, 0);

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

    insert_into_state(final_state->state, final_state->edges[0].mem_count, 0);

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
  printf("%s -----> NULL EOA\n",nfa->states[8].state);
}

void print_dfa_hash(){
  main_t s;

  for(s=global_state_table; s != NULL; s=s->hh.next) {
    printf("user id %d: name %s\n", s->idx, s->nfa_table->state_name);
  }
}

void post_merge(m_t merge, nfa_t nfa, char* new_state_name){
  m_t tem = NULL;

    s_t new_state = NULL;
    new_state = malloc(sizeof(s));
    new_state->state = malloc(sizeof(char) * strlen(new_state_name));
    strcpy(new_state->state, new_state_name);
    new_state->edge_count = -1;
    int is_mul = -1;

    /* iterate through all symbols */
  for(tem=merge; tem!= NULL; tem=tem->hh.next){
    tr_t tr_temp = NULL;
    new_state->edges = malloc(sizeof(e));
    new_state->edge_count++;
    new_state->edges[new_state->edge_count].mem_count = -1;
    strcpy(&new_state->edges[new_state->edge_count].symbol, tem->symbol);
    int v= 0;
    for(tr_temp = tem->tstate; tr_temp != NULL; tr_temp = tr_temp->hh.next){

      (v > is_mul)?is_mul=v:is_mul;
      new_state->edges[new_state->edge_count].mem_count++;
      new_state->edges[new_state->edge_count].estate[new_state->edges[new_state->edge_count].mem_count] = malloc(sizeof(char) * strlen(tr_temp->tarnsfer_state));
      strcpy(new_state->edges[new_state->edge_count].estate[new_state->edges[new_state->edge_count].mem_count] , tr_temp->tarnsfer_state);

    }

  }
      nfa->state_count++;
    
    nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
    nfa->states[nfa->state_count] = *new_state;	

    
    insert_into_state(new_state->state, is_mul, INITIAL_COUNT );
     

    /* a new entry to dfa */
   
}

void merge_states(s_t state, nfa_t nfa){

  for(int i=0; i < state->edge_count; i++){

    if(state->edges[i].mem_count > 0){
      m_t merged =NULL;

      tr_t tra = NULL;
      char* temp =NULL;
      for(int j =0 ; j < state->edges[i].mem_count; j++){
	if(temp == NULL){
	  temp = malloc(sizeof(char) * strlen(state->edges[i].estate[j]));
	  strcpy(temp, state->edges[i].estate[j]);
	}
	else{
	  temp = realloc(temp, sizeof(char) * (strlen(temp)+strlen(state->edges[i].estate[j])));
	  strcat(temp, state->edges[i].estate[j]);
	}

	s_t curr_state = find_state(state->edges[i].estate[j], nfa);

	for(int x=0; x <= curr_state->edge_count; x++){
	  m_t temp_merged =NULL;
	  HASH_FIND_STR(merged, &curr_state->edges[x].symbol, temp_merged);
	  if(temp_merged == NULL){
	    temp_merged = malloc(sizeof(m));
	    temp_merged->symbol =malloc(sizeof(char) * 2);
	    strcpy(temp_merged->symbol ,&curr_state->edges[x].symbol);
	    HASH_ADD_STR(merged, symbol, temp_merged);
	  }
	  for(int y=0; y <= curr_state->edges[x].mem_count; y++){
	    tr_t temp_trans = NULL;
	    
	    HASH_FIND_STR(tra, curr_state->edges[x].estate[y], temp_trans);
	    if( temp_trans == NULL){
	      temp_trans = malloc(sizeof(tr));
	      temp_trans->tarnsfer_state = malloc(sizeof(char) * strlen(curr_state->edges[x].estate[y]));
	      HASH_ADD_STR(tra, tarnsfer_state, temp_trans);
	    }
	  }

	}
	/* add to the key {'o' : {'10'}, {'11'} } */
	HASH_ADD_STR(merged->tstate, tarnsfer_state, tra); 
	/* check if the merged state name already exists
	   in the nfa table if it exists do nothing else
	   use the mergerd hash table to form a new entry in nfa
*/
	if(find_state(temp,nfa) != NULL)
	post_merge(merged, nfa, temp);
	else{

      nfa_table_t dfa_exists = NULL;
      /* check if state already exists in dfa state hash */
      HASH_FIND_STR(dfa_state, temp, dfa_exists );
      if(dfa_exists == NULL){

	  printf("MERGED STATES %s ALREADY EXISTS both in nfa, dfa\n", temp);

	  }
	  else{
	insert_dfa(temp);

	  }
	}
      }
    }
    else{

      /*  add the state if not exists */

      nfa_table_t dfa_exists = NULL;
      /* check if state already exists in dfa state hash */
      HASH_FIND_STR(dfa_state, state->edges[i].estate[0]  , dfa_exists );
      if(dfa_exists == NULL){
	insert_dfa(state->edges[i].estate[0]);
	INITIAL_COUNT ++;
	int is_multiple = 0;
	for(int e_c =0; e_c < state->edge_count; e_c++){
	  if(state->edges[e_c].mem_count > is_multiple)
	    {  is_multiple = state->edges[e_c].mem_count;
	      break;
	    }
	}
	insert_into_state(state->state, state->edges->mem_count, is_multiple);
	  
      }

    }

  }
}


void construct_dfa(nfa_t nfa){

  print_dfa_hash();  
  int i = 0;

  main_t get_state = NULL;
  while( i <= INITIAL_COUNT){
    HASH_FIND_INT(global_state_table, &i , get_state);
    if(get_state == NULL){
      perror("Unexpected Behavior : COuld not get state"); 
    }
    
    else {
      s_t st = find_state(get_state->nfa_table->state_name, nfa);

      if(st != NULL){

	if( get_state->nfa_table->is_multiple > 0){
	  /* merge the multiple states for all input symbols */
	   
	  merge_states(st, nfa);
	}

	else {
	    
	  nfa_table_t dfa_exists = NULL;
	  /* check if state already exists in dfa state hash */
	  HASH_FIND_STR(dfa_state, st->state , dfa_exists );
	  if(dfa_exists == NULL){
	    insert_dfa(st->state);
	    INITIAL_COUNT ++;
	    int is_multiple = 0;
	    for(int e_c =0; e_c < st->edge_count; e_c++){
	      if(st->edges[e_c].mem_count > is_multiple)
		is_multiple = st->edges[e_c].mem_count;
	    }
	    insert_into_state(st->state, st->edges->mem_count, is_multiple);
	  }
       
	}

      }
      /* if state not in original nfa */

      else{




      }
	
    }
    ++i;
  }
  

}


void print_dfa(nfa_t nfa){

  for(int i=0; i < nfa->state_count; i++){

    for(int j=0; j <= nfa->states[i].edge_count; j++){


      printf("%s ---( %c )---> %s\n", nfa->states[i].state, nfa->states[i].edges[j].symbol, nfa->states[i].edges[j].estate[0]);


    }
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
  construct_dfa(nfa);
  /* find_matches(word, lev_dist, word_len); */

  printf("%d states\n", nfa->state_count+1);

  print_dfa(nfa);

  print_nfa(nfa);

 
  return 0;
}
