#define DISTANCE 2

typedef struct nfa n;
typedef n* nfa_t;

typedef struct state s;
typedef s* s_t;

typedef struct edge e;
typedef e* e_t;

struct state {
  char* state;
  e_t edges;
  int edge_count;
};

struct edge {
  char symbol;
  int mem_count;
 char state[][2];
 
};

struct nfa{
  s_t states;
  int state_count;
};
