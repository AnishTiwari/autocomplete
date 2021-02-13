#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>

#include "autocomplete.h"

main_t global_state_table = NULL;

nfa_table_t dfa_state = NULL;

int INITIAL_COUNT = -1;

s_t find_state(char* state_name, nfa_t nfa){

	for(int p=0; p <= nfa->state_count; p++){
		if( strcmp(nfa->states[p].state, state_name) == 0){
			return &nfa->states[p];
		}
	}
	printf("Not found any state with state name %s\n", state_name);
	return NULL;
}

void find_and_change(char* new_state_name, char* state_name, nfa_t nfa_, int x) {

	for(int p=0; p <= nfa_->state_count; p++){
		if( strcmp(nfa_->states[p].state, state_name) == 0){
			nfa_->states[p].edges[x].mem_count = 0;

			nfa_->states[p].edges[x].estate[0] = realloc(nfa_->states[p].edges[x].estate[0],sizeof(char) * strlen(new_state_name));
			strcpy(nfa_->states[p].edges[x].estate[0] , new_state_name);
			return;
		}
	}
}

void insert_dfa(char* dfa_state_name, int idx){

	printf("---------------ADDING %s to final state table with idx %d --------------\n", dfa_state_name, idx);
	nfa_table_t new_dfa_state = NULL;
	new_dfa_state = malloc(sizeof(nfa_table));
	new_dfa_state->state_name = malloc(sizeof(char) * strlen(dfa_state_name));
	new_dfa_state->state_name = dfa_state_name;
	new_dfa_state->state_idx = idx;
	/* since it is already checked that the state name doesnt exists no need to check here */
	HASH_ADD_STR(dfa_state, state_name, new_dfa_state);

}

void insert_into_state(char* s_name, int is_multiple){

	INITIAL_COUNT += 1;
	int idx_v = INITIAL_COUNT;
	main_t new_idx = malloc(sizeof(main_hash));
	new_idx->idx = idx_v;
	main_t temp_main = NULL;

	/* HASH_FIND_INT(global_state_table, &idx_v, temp_main); */
	if (temp_main == NULL){
		HASH_ADD_INT(global_state_table, idx, new_idx);
	}
	else{
		perror("Unexpected Behavior\n");
		exit(1);
	}

	nfa_table_t new_state = NULL;
	new_state = malloc(sizeof(nfa_table));
	nfa_table_t state_exists = NULL;

	new_state->is_multiple = is_multiple;
	new_state->state_name = malloc(sizeof(char) * (strlen(s_name) +1));
	strcpy(new_state->state_name, s_name);
	new_idx->nfa_table =NULL;
	HASH_FIND_STR(global_state_table->nfa_table, s_name, state_exists);
	if (state_exists == NULL){
		HASH_ADD_STR(new_idx->nfa_table, state_name, new_state);
	}
	else{
		printf("GOLDEN ERROR\n");
	}

	/* print the global state table */
	/* main_t te = NULL; */
	/* for(te=global_state_table; te != NULL; te=te->hh.next){ */
	/*   printf("%d %s++\n",te->idx, te->nfa_table->state_name); */

	/* } */
}

nfa_t construct_nfa(nfa_t nfa, char* word, int dist, int word_l){

	for(int i = 0; i < word_l ; i++){
		for(int j = 0; j < dist; j++){

			s_t new_s = malloc(sizeof(s));
			new_s->state = malloc(sizeof(char) * 2);
			sprintf(new_s->state, "%d%d", i, j);
			new_s->edge_count = -1;

			/* add edge for correct symbol (same level but diff state 00, 01, 02...) */ 
			e_t new_e_correct = malloc(sizeof(e));
			new_e_correct->mem_count = -1;
			new_e_correct->symbol = NULL;
			new_e_correct->symbol = malloc(sizeof(char) * 2);
			strncpy(new_e_correct->symbol, &word[i], 1) ;
				new_e_correct->symbol[1]='\0';
			/* new_e_correct->symbol = &word[i]; */

			printf("||==%s==||",new_e_correct->symbol);

			char* temp = NULL;
			temp = malloc(sizeof(char)* 2);
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
			new_e_star->symbol =malloc(sizeof(char) * 2);
			strcpy(new_e_star->symbol, "*");
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
				new_s->edges = realloc(new_s->edges, sizeof(e) * (new_s->edge_count + 1));
				new_s->edges[new_s->edge_count] = *new_e_star;
				new_s->edges[new_s->edge_count].estate[0] = temp1; 
	
				new_s->edges[new_s->edge_count].estate[1] = temp2;
				/* printf("5555 %d %s %s %s -\n", new_e_star->mem_count, new_e_star->estate[0], temp1, new_s->edges[1].estate[1]); */
			}
      
			/* ========================================= */
			/* add state to nfa table */

			if(nfa->state_count == -1) {
				nfa->states = malloc(sizeof(s));
				/* insert the initial state for dfa construction */
				insert_into_state(new_s->state, 1);
				insert_dfa(new_s->state, 0);
				nfa->states[0] = *new_s;
				/* printf("8888 %s %s %s -\n", new_e_correct->estate[0], temp, new_s->edges[0].estate[0]); */
	
				nfa->state_count++;
			}
      
			else {
				nfa->state_count++;
				nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
				new_s->state_idx = nfa->state_count;
				nfa->states[nfa->state_count] = *new_s;	
			}      
		}
	} /* end of for loops */

	for(int i=word_l; i==word_l; i++){
		for(int j=0;j < dist; j++){
			s_t last_states = NULL;
			last_states = malloc(sizeof(s));
			last_states->edge_count = -1;
			last_states->edges = NULL;

			last_states->state = malloc(sizeof(char) *2);
			sprintf(last_states->state, "%d%d", i,j);

			e_t last_edge = malloc(sizeof(e));
			last_edge->symbol = malloc(sizeof(char) * 2);
			strcpy(last_edge->symbol, "*");
			last_edge->mem_count = -1;
			char* last_temp =NULL;
			last_temp = malloc(sizeof(char) *2);
			sprintf(last_temp, "%d%d", i, j+1);
			++last_edge->mem_count;
			last_edge->estate[last_edge->mem_count] = last_temp;
      
			last_states->edges = malloc(sizeof(e));
			last_states->edge_count++;
			last_states->edges[0] = *last_edge;
      
      
			if(nfa->state_count == -1) {
				nfa->states = malloc(sizeof(s));
				insert_into_state(last_states->state, 1);

				nfa->states[0] = *last_states;
				nfa->state_count++;
			}
      
			else {
				nfa->state_count++;
				last_states->state_idx = nfa->state_count;
				nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
				nfa->states[nfa->state_count] = *last_states;
			}

		}
	}

	for(int i=0; i < word_l; i++){
		for(int j=dist; j==dist; j++){

			s_t last_states = NULL;
			last_states = malloc(sizeof(s));
			last_states->edge_count = -1;
			last_states->edges = NULL;

			last_states->state = malloc(sizeof(char) *2);
			sprintf(last_states->state, "%d%d", i,j);

			e_t last_edge = malloc(sizeof(e));
			last_edge->symbol = NULL;
			last_edge->symbol = malloc(sizeof(char) * 2);
			strncpy(last_edge->symbol, &word[i], 1) ;
			last_edge->symbol[1]='\0';
			
			printf("|||||(%s)%d%d\n", last_edge->symbol,i,j);
			/* strcpy(last_edge->symbol , &word[i]); */
			last_edge->mem_count = -1;
			char* final_temp =NULL;
			final_temp = malloc(sizeof(char)*2);
			sprintf(final_temp, "%d%d", i+1, j);
      
			last_edge->estate[++last_edge->mem_count] = final_temp;
			last_states->edges = malloc(sizeof(e));
			last_states->edge_count++;
			last_states->edges[0] = *last_edge;

			if(nfa->state_count == -1) {
				nfa->states = malloc(sizeof(s));

				insert_into_state( last_states->state, 1);

				nfa->states[0] = *last_states;
				nfa->state_count++;
			}
      
			else {
				nfa->state_count++;

				nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
				last_states->state_idx =nfa->state_count;
				nfa->states[nfa->state_count] = *last_states;	
			}
      
		}
	}

	s_t final_state = NULL;
	final_state  = malloc(sizeof(s));
	final_state->edges=NULL;
	final_state->edge_count = -1;
	final_state->state = malloc(sizeof(char) * 2);
	sprintf(final_state->state, "%d%d", word_l,dist);

	if(nfa->state_count == -1) {
		nfa->states = malloc(sizeof(s));

		insert_into_state(final_state->state, 1);

		nfa->states[0] = *final_state;
		nfa->state_count++;
	}
      
	else {
		nfa->state_count++;
		final_state->state_idx = nfa->state_count;
		nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count +1) );
		nfa->states[nfa->state_count] = *final_state;	
	}
  
	return nfa;
}

void print_nfa(nfa_t nfa){

	printf("GOING TO PRINT %d states in NFA\n", nfa->state_count);
	for(int i=0; i <= nfa->state_count ;i++){

		if(nfa->states[i].edges != NULL)
		{
			for(int j=0; j <= nfa->states[i].edge_count; j++){
				for(int k =0;k <= nfa->states[i].edges[j].mem_count;k++){
					printf("%s ---( %s )--> %s\n", nfa->states[i].state, nfa->states[i].edges[j].symbol, nfa->states[i].edges[j].estate[k]);
				}
			}
		}
		else
			printf("NON EDGE states %s\n", nfa->states[i].state);
	}
}


void post_merge(m_t merge, nfa_t nfa, char* new_state_name){

#ifdef DEBUG
	/* DEBUG */
	m_t s, tmp;
	HASH_ITER(hh, merge, s, tmp) {
		printf("symbol name %s\n", s->symbol);
		tr_t q, rmp;
		HASH_ITER(hh, s->tstate, q, rmp){
			printf(" transition %s\n", q->tarnsfer_state);
		}
		/* ... it is safe to delete and free s here */
	}

	/* END DEBUG */
#endif

	m_t tem = NULL;

	s_t new_state = NULL;
	new_state = malloc(sizeof(s));
	new_state->state = NULL;
	new_state->state = malloc(sizeof(char) * ( strlen(new_state_name) + 1 ));
	strcpy(new_state->state, new_state_name);
	new_state->edge_count = -1;
	int is_mul = -1;

	/* iterate through all symbols */
	for(tem=merge; tem!= NULL; tem=tem->hh.next){
		int v=0;
				/* printf("**************(%s)", tem->symbol); */
			
		tr_t tr_temp = NULL;
		e_t ed =NULL;
		ed = malloc(sizeof(e));
		ed->symbol = malloc(sizeof(char) *1);
		strcpy(ed->symbol , tem->symbol);
		ed->mem_count = -1;
		for(tr_temp = tem->tstate; tr_temp != NULL; tr_temp = tr_temp->hh.next){
			v++;
			if( v > is_mul ) is_mul = v;

			ed->estate[++ed->mem_count] = malloc(sizeof(char) *strlen(tr_temp->tarnsfer_state));
			strcpy(ed->estate[ed->mem_count], tr_temp->tarnsfer_state);

		}
		if(new_state->edge_count == -1){
			new_state->edges = malloc(sizeof(e));
			new_state->edges[0] = *ed;
			new_state->edge_count++;
		}
		else{
			new_state->edge_count++;
			new_state->edges = realloc(new_state->edges, sizeof(e) * (new_state->edge_count +1));
			new_state->edges[new_state->edge_count] = *ed;

		}

	}

	/* printf("NEW STATE PRINT ====>\n %s state name, %d state edges\n",new_state->state,new_state->edge_count); */
	nfa->state_count +=1;
	/* printf("ADDED A NEW STATE %s with count %d sym %s\n",new_state->state,nfa->state_count+1, new_state->edges[0].symbol); */

	/* nfa->states = realloc(nfa->states, sizeof(s) * (nfa->state_count + 1 ) ); */

	new_state->state_idx = nfa->state_count;
	nfa->states[nfa->state_count] = *new_state;
	/* nfa->states[nfa->state_count].state = malloc(sizeof(char)* strlen(new_state->state)); */
	/* nfa->states[nfa->state_count].state = new_state->state; */
	/* nfa->states[nfa->state_count].edge_count = new_state->edge_count; */
	/* nfa->states[nfa->state_count].edges = malloc(sizeof(e)); */
	/* nfa->states[nfa->state_count].edges = new_state->edges; */
	/* print_nfa(nfa); */
	insert_into_state(new_state->state, is_mul);
}

void merge_states(s_t state, nfa_t nfa){

	for(int i=0; i <= state->edge_count; i++){
		if(state->edges[i].mem_count > 0){
			m_t merged =NULL;
			m_t temp_merged =NULL;
	        	tr_t temp_trans = NULL;

			char* temp =NULL;

			for(int j =0 ; j <= state->edges[i].mem_count; j++){

				if(temp == NULL){
					temp = malloc(sizeof(char) * strlen(state->edges[i].estate[j]));
					strcpy(temp, state->edges[i].estate[j]);
				}
				else{
					temp = realloc(temp, sizeof(char) * (strlen(temp)+strlen(state->edges[i].estate[j])));
					strcat(temp, state->edges[i].estate[j]);
				}
				s_t curr_state = find_state(state->edges[i].estate[j], nfa);
				if(curr_state->edge_count == -1)	    continue;

				for(int x=0; x <= curr_state->edge_count; x++){
					/* printf("--------=======->%s", curr_state->edges[x].symbol); */
					HASH_FIND_STR(merged, curr_state->edges[x].symbol, temp_merged);
					if(temp_merged == NULL){
						temp_merged = malloc(sizeof(m));
						temp_merged->symbol = malloc(sizeof(char) * 2);
						strcpy(temp_merged->symbol, curr_state->edges[x].symbol);
						HASH_ADD_STR(merged, symbol, temp_merged);
					}

					for(int y=0; y <= curr_state->edges[x].mem_count; y++){

						HASH_FIND_STR(temp_merged->tstate, curr_state->edges[x].estate[y], temp_trans);
						if( temp_trans == NULL){
							temp_trans = malloc(sizeof(tr));
							temp_trans->tarnsfer_state = malloc(sizeof(char) * strlen(curr_state->edges[x].estate[y]));
							strcpy(temp_trans->tarnsfer_state, curr_state->edges[x].estate[y]);
							HASH_ADD_STR(temp_merged->tstate, tarnsfer_state, temp_trans);
						}
					}
				}

			}

			/* DEBUG */
			/* m_t s, tmp; */
			/* HASH_ITER(hh, merged, s, tmp) { */
			/* 	printf(" name %s\n", s->symbol); */
			/* 	tr_t q, rmp; */
			/* 	HASH_ITER(hh, s->tstate, q, rmp){ */
			/* 		printf(" transition %s\n", q->tarnsfer_state); */
			/* 	} */
			/* 	/\* ... it is safe to delete and free s here *\/ */
			/* } */

			/* END DEBUG */

			/*  check if the merged state name already exists
			 *  in the nfa table if it exists do nothing else
			 *  use the mergerd hash table to form a new entry in nfa
			 */
			find_and_change(temp, state->state, nfa,i);

			if(find_state(temp,nfa) == NULL)
			{
				post_merge(merged, nfa, temp);
				printf("%s already exists in nfa\n", temp);
				nfa_table_t dfa_exists = NULL;
				/* check if state already exists in dfa state hash */
				HASH_FIND_STR(dfa_state, temp, dfa_exists );
				if(dfa_exists == NULL){
					insert_dfa(temp, nfa->state_count);
				}

			}
			else{
				printf("MERGED STATES %s ALREADY EXISTS both in nfa, dfa\n", temp);

			}

		}

		else{

			printf("SINGLE STATE: %s\n",state->edges[i].estate[0]);

			/*  add the state if not exists */

			nfa_table_t dfa_exists = NULL;
			/* check if state already exists in dfa state hash */
			HASH_FIND_STR(dfa_state, state->edges[i].estate[0], dfa_exists );
			if(dfa_exists == NULL){
				s_t g = find_state(state->edges[i].estate[0], nfa);
				insert_dfa(state->edges[i].estate[0], g->state_idx);
				int is_multiple = 0;
				s_t foreign = find_state(state->edges[i].estate[0] ,nfa);
				for(int e_c =0; e_c <= foreign->edge_count; e_c++){
					if(foreign->edges[e_c].mem_count > is_multiple)
					{  is_multiple = foreign->edges[e_c].mem_count;
						break;
					}
				}
				insert_into_state(state->edges[i].estate[0], is_multiple);

			}

		}

	}

}


void construct_dfa(nfa_t nfa){

	int i = 0;

	while( i <= INITIAL_COUNT){
		main_t get_state = NULL;

		HASH_FIND_INT(global_state_table, &i , get_state);
		if(get_state == NULL){
			printf("Unexpected Behavior : COuld not get state %d\n",i); 
		}

		else {
			s_t st = find_state(get_state->nfa_table->state_name, nfa);

			if(st != NULL){

				if( get_state->nfa_table->is_multiple > 0){

					/* merge the multiple states for all input symbols */
					printf("GOT TO MERGE STATES\n");
					merge_states(st, nfa);
				}

				else {

					printf("DEAD for %s with is multiple of %d\n", get_state->nfa_table->state_name, get_state->nfa_table->is_multiple);
					nfa_table_t dfa_exists = NULL;
					/* check if state already exists in dfa state hash */
					HASH_FIND_STR(dfa_state, st->state , dfa_exists );
					if(dfa_exists == NULL){
						insert_dfa(st->state, st->state_idx); /* TODO: Not needed */

						int is_multiple = 0;
						for(int e_c =0; e_c <= st->edge_count; e_c++){
							if(st->edges[e_c].mem_count > is_multiple)
								is_multiple = st->edges[e_c].mem_count;
						}
						insert_into_state(st->state, st->edges->mem_count);
					}
       
				}

			}
      
			/* if state not in original nfa */
			else{
				printf("Could not find state %s in nfa\n", get_state->nfa_table->state_name);
			}
	
		}
		++i;
	}
}


void find_all_words(nfa_t nfa){


   DIR *dir;
    struct dirent *de;

    dir = opendir("."); /*your directory*/
    while(dir)
    {
        de = readdir(dir);
        if (!de) break;
        printf("%i %s\n", de->d_type, de->d_name);
    }
    closedir(dir);
    
	FILE *fp = fopen("./words/words.txt", "r");


    if(fp == NULL){
		printf("Could not read input word list\n");
		return;
	}
	printf("MATCHD WORDS ARE:\n");
	char curr_char;
	char curr_word[20] = "";
	int initial = 0;
	int idx = 0;
	int wc=0;
	int break_word=0;
	while ((curr_char = fgetc(fp)) != EOF)
	{
		/* printf("%c",curr_char); */
		if(curr_char == '\n'){
			if(initial == 8 && break_word != 1){
				wc++;
				curr_word[idx] ='\0';

				idx=0;
				initial = 0;
				printf("%s\n",curr_word);
			}
			else{
				initial=0;
				idx = 0;
				  
			}
			break_word=0;
		}
		else{
			if(break_word){
				initial = -1;
				continue;
			}
			curr_word[idx++] = curr_char;
			s_t state = &nfa->states[initial];
			int prev_init = initial;
			for(int c=0; c<= state->edge_count; c++){

				if(state->edges[c].symbol[0] == curr_char || state->edges[c].symbol[0] == '*'){
					nfa_table_t exists = NULL;
					HASH_FIND_STR(dfa_state, state->edges[c].estate[0],exists );
					initial = exists->state_idx;
					break;
				}

				else{
					/* if(state->edges[c].symbol[0] == '*'){ */
					/* 	  nfa_table_t exists = NULL; */
					/* 	  HASH_FIND_STR(dfa_state, state->edges[c].estate[0],exists ); */
					/* 	  initial = exists->state_idx; */
					/* 	  break; */
					/* } */
				}
			}
			if(prev_init == initial)
				break_word  =1;
		}
	}
	fclose (fp);
     
	fprintf(stdout, "WORD COUNT IS %d\n", wc);       
}

int main(int argc, char *argv[]){

	char* word = NULL;
	if (argc == 2)
		word = argv[1];

	else {
		printf( "Invalid arguments EXITING\n\n");
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


	printf("FINAL DFA STATE TABLE \n");
	nfa_table_t final,temp =NULL;
	HASH_ITER(hh, dfa_state, final, temp)
		printf("%s %d\n", final->state_name, final->state_idx);


	find_all_words(nfa);
	
	return 0;
}
