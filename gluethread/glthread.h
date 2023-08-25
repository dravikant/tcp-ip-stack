#ifndef __GLUETHREAD__
#define __GLUETHREAD__

typedef struct _glthread{
    struct _glthread *left;
    struct _glthread *right;
} glthread_t;

//new type alias glthread_t
//glthread_t myNode1;
//glthread_t myNode2;

//methods
void glthread_add_next(glthread_t *base_glthread, glthread_t *new_glthread);

void glthread_add_before(glthread_t *base_glthread, glthread_t *new_glthread);

void remove_glthread(glthread_t *glthread);

void init_glthread(glthread_t *glthread);

void glthread_add_last(glthread_t *base_glthread, glthread_t *new_glthread);

//try using == NULL instead of ==0
#define IS_GLTHREAD_LIST_EMPTY(glthreadptr)         \
    ((glthreadptr)->right == 0 && (glthreadptr)->left == 0)


/*
designed to convert a given pointer to a glthread_t structure 
back into a pointer to the containing structure that the glthread_t node is a part of. 


By casting pointers to char *, you effectively treat the memory as an array of bytes. 
This allows you to perform byte-level arithmetic, which is essential for calculating byte offsets.

(char *)(glthreadptr) - (char *)&(((structure_name *)0)->field_name)

By subtracting these two char * pointers, you're effectively calculating the byte offset 
between the start of the glthread_t node and the start of the containing structure.
 This byte offset is then used to determine the actual address of the containing structure.


*/

#define GLTHREAD_TO_STRUCT(fn_name, structure_name, field_name)                        \
    static inline structure_name * fn_name(glthread_t *glthreadptr){                   \
        return (structure_name *)((char *)(glthreadptr) - (char *)&(((structure_name *)0)->field_name)); \
    }


#define BASE(glthreadptr) ((glthreadptr)->right)


/*
assists in iterating over a generic linked thread (GLThread) structure

ITERATE_GLTHREAD_BEGIN(glthreadptrstart, glthreadptr): This macro is used to begin an iteration over the GLThread. 
You provide two pointers: glthreadptrstart, which is the starting node of the iteration, and glthreadptr, 
which will be used as the loop variable

_glthread_ptr is declared and initialized to NULL
glthreadptr is assigned the value of BASE(glthreadptrstart). 

The macro enters a for loop that iterates as long as glthreadptr is not NULL.

Inside the loop, _glthread_ptr is assigned the value of (glthreadptr)->right, 
which is the right pointer of the current node. This will be used to move to the next node in the iteration.

The ITERATE_GLTHREAD_END macro simply closes the loop and the block that was opened by ITERATE_GLTHREAD_BEGIN

glthread_t *start_node = ...;  // The starting node of the GLThread
glthread_t *current_node;

ITERATE_GLTHREAD_BEGIN(start_node, current_node)
    // Code to process the current_node
ITERATE_GLTHREAD_END(start_node, current_node)

*/

#define ITERATE_GLTHREAD_BEGIN(glthreadptrstart, glthreadptr)                                      \
{                                                                                                  \
    glthread_t *_glthread_ptr = NULL;                                                              \
    glthreadptr = BASE(glthreadptrstart);                                                          \
    for(; glthreadptr!= NULL; glthreadptr = _glthread_ptr){                                        \
        _glthread_ptr = (glthreadptr)->right;

#define ITERATE_GLTHREAD_END(glthreadptrstart, glthreadptr)                                        \
        }}

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthreadptr, offset)  \
    (void *)((char *)(glthreadptr) - offset)


void delete_glthread_list(glthread_t *base_glthread);

unsigned int get_glthread_list_count(glthread_t *base_glthread);

void glthread_priority_insert(glthread_t *base_glthread,     
                         glthread_t *glthread,
                         int (*comp_fn)(void *, void *),
                         int offset);


/*
#if 0: This preprocessor directive checks if the constant expression 0 is true.
 Since 0 is false, the code block between #if 0 and #endif is excluded from compilation. 
 It's effectively treated as a comment block.
*/
#if 0
void *
gl_thread_search(glthread_t *base_glthread,
        void *(*thread_to_struct_fn)(glthread_t *),
        void *key,
        int (*comparison_fn)(void *, void *));

#endif

#endif


