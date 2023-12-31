#ifndef AKIN_H
#define AKIN_H

#include <stdio.h>
#include <string.h>
#include <TXLib.h>

#include "include/tree.h"
#include "include/stack.h"
#include "include/error.h"
#include "include/utils.h"

const bool OBJECT_FIND = true;
const bool OBJECT_NOT_FIND = false;

const ELEMENT YES = 1;
const ELEMENT NO = 0;

const size_t STR_SIZE = 100;
const size_t STACK_SIZE = 10;

enum cmd_code {
    GUESS = 1,
    DEFIN,
    COMPARE,
    GET_BASE,
    EXIT_SAVE,
    EXIT_NO_SAVE
};

int game_run ();

#endif //AKIN_H