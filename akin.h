#ifndef AKIN_H
#define AKIN_H

#include <stdio.h>
#include <string.h>
#include <TXLib.h>

#include "include/tree.h"
#include "include/stack.h"
#include "include/error.h"
#include "include/utils.h"

const bool VALUE_FIND = true;
const bool VALUE_NOT_FIND = false;

const ELEMENT YES = 1;
const ELEMENT NO = 0;

const size_t STACK_SIZE = 10;

int game_run (TREE *tree);

int guess_words (NODE *node);

void add_objects (NODE *node);

void give_determ (NODE *node);

bool find_value (NODE *node, char *search_value, STACK *sstack);

#endif //AKIN_H