#define DISTANCE 2

#include "include/lib/uthash.h"


typedef struct transition tr;
typedef tr* tr_t;

typedef struct nfa n;
typedef n* nfa_t;

typedef struct state s;
typedef s* s_t;

typedef struct edge e;
typedef e* e_t;

typedef struct nfa_state_table nfa_table;
typedef nfa_table* nfa_table_t;

typedef struct main_hash_table main_hash;
typedef main_hash* main_t;

typedef struct merger m;
typedef m* m_t;

struct state {
  char* state;
  e_t edges;
  int edge_count;
};

struct edge {
  char symbol;
  int mem_count;
 char *estate[2];
 
};

struct nfa{
  s_t states;
  int state_count;
};


struct nfa_state_table{

  char* state_name;
   UT_hash_handle hh;
  int is_multiple;
};

struct main_hash_table{
  int idx;
  nfa_table_t nfa_table;
  UT_hash_handle hh;

};

struct transition{
  char* tarnsfer_state;
  UT_hash_handle hh;
};

struct merger{
  char* symbol;
  tr_t tstate;
  UT_hash_handle hh;
};

typedef struct symbol_table sym;
typedef sym* sym_t;

struct symbol_table{
  char* symbol;
  UT_hash_handle hh;
};
