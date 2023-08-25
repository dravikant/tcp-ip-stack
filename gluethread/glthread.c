#include "glthread.h"
#include <stdlib.h>

void init_glthread(glthread_t *glthread){
    glthread->left = NULL;
    glthread->right = NULL;
}

void glthread_add_next(glthread_t *base_glthread, glthread_t *new_glthread){

    if(!base_glthread->right){
        base_glthread->right = new_glthread;
        new_glthread->left = base_glthread;
        return;
    }

    glthread_t *temp = base_glthread->right;
    base_glthread->right = new_glthread;
    new_glthread->left = base_glthread;
    new_glthread->right = temp;
    temp->left = new_glthread;
    return;
}

void glthread_add_before(glthread_t *base_glthread, glthread_t *new_glthread){

    if(!base_glthread->left){
        base_glthread->left = new_glthread;
        new_glthread->right = base_glthread;
        return;
    }

    glthread_t *temp = base_glthread->left;
    base_glthread->left = new_glthread;
    new_glthread->right = base_glthread;
    new_glthread->left = temp;
    temp->right = new_glthread;
    return;
}


void remove_glthread(glthread_t *curr_glthread){

    if(!curr_glthread->left){
        if(curr_glthread->right){
            curr_glthread->right->left = NULL;
            curr_glthread->right = 0;
            return;
        }
        return;
    }
    if(!curr_glthread->right){
        curr_glthread->left->right = NULL;
        curr_glthread->left = NULL;
        return;
    }

    curr_glthread->left->right = curr_glthread->right;
    curr_glthread->right->left = curr_glthread->left;
    curr_glthread->left = 0;
    curr_glthread->right = 0;
}

void delete_glthread_list(glthread_t *base_glthread){

    glthread_t *glthreadptr = NULL;

    ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr){
        remove_glthread(glthreadptr);
    } ITERATE_GLTHREAD_END(base_glthread, glthreadptr);
}

void glthread_add_last(glthread_t *base_glthread, glthread_t *new_glthread){

    glthread_t *glthreadptr = NULL,
               *prevglthreadptr = NULL;

    ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr){
        prevglthreadptr = glthreadptr;
    } ITERATE_GLTHREAD_END(base_glthread, glthreadptr);

    if(prevglthreadptr) 
        glthread_add_next(prevglthreadptr, new_glthread); 
    else
        glthread_add_next(base_glthread, new_glthread);
}

unsigned int get_glthread_list_count(glthread_t *base_glthread){

    unsigned int count = 0;
    glthread_t *glthreadptr = NULL;

    ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr){
        count++;
    } ITERATE_GLTHREAD_END(base_glthread, glthreadptr);
    return count;
}

void glthread_priority_insert(glthread_t *base_glthread, 
                         glthread_t *glthread,
                         int (*comp_fn)(void *, void *),
                         int offset){


    glthread_t *curr = NULL,
               *prev = NULL;

    init_glthread(glthread);

    if(IS_GLTHREAD_LIST_EMPTY(base_glthread)){
        glthread_add_next(base_glthread, glthread);
        return;
    }

    /* Only one node*/
    if(base_glthread->right && !base_glthread->right->right){
        if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread->right, offset), 
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset)) == -1){
            glthread_add_next(base_glthread->right, glthread);
        }
        else{
            glthread_add_next(base_glthread, glthread);
        }
        return;
    }

     if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset), 
            GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread->right, offset)) == -1){
        glthread_add_next(base_glthread, glthread);
        return;
    }

    ITERATE_GLTHREAD_BEGIN(base_glthread, curr){

        if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset), 
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(curr, offset)) != -1){
            prev = curr;
            continue;
        }

        glthread_add_next(curr, glthread);
        return;

    }ITERATE_GLTHREAD_END(base_glthread, curr);

    /*Add in the end*/
    glthread_add_next(prev, glthread);
} 