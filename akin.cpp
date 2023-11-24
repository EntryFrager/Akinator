#include "akin.h"

#define _FREE(name) free(name); name = NULL;

static void print_determ_object (NODE *node, char *defined_word, STACK *stack);
static bool find_object (NODE *node, char *search_objects, STACK *stack);
static int check_new_objects (NODE *node);
static void add_objects (NODE *node);
static void check_answer (char *answer);
static char *create_str (size_t size);

int game_run (TREE *tree)
{    
    printf ("Hello, you got into the game \"Akinator\". Select game mode:\n");
    printf ("\t1. Guess the object[1];\n");
    printf ("\t2. Define an object[2];\n");
    printf ("\t3. Compare objects[3];\n");
    printf ("\t4. Get the base[4];\n");
    printf ("\t5. Exit saving the database[5];\n");
    printf ("\t6. Exit without saving the database[6];\n");

    printf ("Select game mode: ");
    int mode = 0;
    scanf ("%d", &mode);

    switch (mode)
    {
        case (1):
        {
            CHECK_ERROR_RETURN (guess_object (tree->root));
            break;
        }
        case (2):
        {
            CHECK_ERROR_RETURN (give_determ (tree->root));
            break;
        }
        case (3):
        {
            CHECK_ERROR_RETURN (compare_object (tree->root));
            break;
        }
        case (4):
        {

            break;
        }
        case (5):
        {
            FILE *fp = fopen (tree->info.fp_name_base, "w+");
            if (fp == NULL)
            {
                return ERR_FOPEN;
            }

            print_tree (tree, tree->root, fp);

            if (fclose (fp) != 0)
            {
                return ERR_FCLOSE;
            }

            return ERR_NO;
        }
        case (6):
        {
            return ERR_NO;
        }
        default:
        {
            printf ("I don't understand, please enter the correct command\n\n\n");
            break;
        }
    }

    game_run (tree);

    return ERR_NO;
}

int guess_object (NODE *node)
{
    printf ("It is %s? ", node->value);
    char *answer = create_str (STR_SIZE);
    scanf ("%s", answer);

    check_answer (answer);

    if (strcasecmp (answer, "yes") == 0)
    {
        if (node->left)
        {
            guess_object (node->left);
        }
        else
        {
            printf ("Yes, I guessed it right.\n");
        }
    }
    else
    {
        if (!node->right)
        {
            add_objects (node);

            printf ("Now I know everything\n");
        }
        else
        {
            guess_object (node->right);
        }
    }

    _FREE(answer);

    return ERR_NO;
}

int give_determ (NODE *node)
{
    STACK stack = {};
    stack_ctor (&stack, STACK_SIZE);

    char *defined_word = create_str (1000);

    printf ("Enter the expression to be defined: ");

    while (true)
    {
        scanf ("%s", defined_word);

        if (find_object (node, defined_word, &stack))
        {
            break;
        }

        if (check_new_objects (node) == YES)
        {
            _FREE(defined_word);
            return ERR_NO;
        }
    }

    print_determ_object (node, defined_word, &stack);

    _FREE(defined_word);
    return ERR_NO;
}

void print_determ_object (NODE *node, char *defined_word, STACK *stack)
{
    printf ("The %s is: ", defined_word);

    while (stack->position != 0)
    {
        ELEMENT value = stack_pop (stack);

        if (value == 1)
        {
            printf ("%s", node->value);
            node = node->left;
        }
        else
        {
            printf ("not %s", node->value);
            node = node->right;
        }

        if (stack->position != 0)
        {
            printf (", ");
        }
    }

    printf (".\n\n\n");
}

bool find_object (NODE *node, char *search_objects, STACK *stack)
{
    if (!node)
    {
        return OBJECT_NOT_FIND;
    }

    if (strcasecmp (node->value, search_objects) == 0)
    {
        return OBJECT_FIND;
    }
        
    if (find_object (node->left, search_objects, stack))
    {
        stack_push (stack, YES);
        return OBJECT_FIND;
    }    

    if (find_object (node->right, search_objects, stack))
    {
        stack_push (stack, NO);
        return OBJECT_FIND;
    }

    return OBJECT_NOT_FIND;
}

int check_new_objects (NODE *node)
{
    printf ("This element is not in the database, do you want to add it?\n");
    
    char *answer = create_str (STR_SIZE);

    check_answer (answer);

    if (strcasecmp (answer, "yes") == 0)
    {
        guess_object (node);
        _FREE(answer);
        return YES;
    }
    else
    {
        _FREE(answer);
        return NO;
    }

    return NO;
}

void add_objects (NODE *node)
{
    printf ("What is this?\n"   \
            "It is... ");
    
    char *new_object = create_str (STR_SIZE);
    scanf ("%s", new_object);

    printf ("How is %s different from %s?\n", new_object, node->value);

    char* difference = create_str (STR_SIZE);
    scanf ("%s", difference);

    const char *temp_old_object = node->value;

    node->value = difference;
    add_node (node, new_object, LEFT);
    add_node (node, temp_old_object, RIGHT);

    _FREE(new_object);
}

int compare_object (NODE *node)
{
    STACK stack_1 = {};
    STACK stack_2 = {};

    stack_ctor (&stack_1, STACK_SIZE);
    stack_ctor (&stack_2, STACK_SIZE);

    char *compare_object_1 = create_str (STR_SIZE);
    char *compare_object_2 = create_str (STR_SIZE);

    printf ("Enter the name of the first object to compare: ");
    scanf ("%s", compare_object_1);
    printf ("\nEnter the name of the second object being compared: ");
    scanf ("%s", compare_object_2);

    CHECK_ERROR_RETURN (find_object (node, compare_object_1, &stack_1));
    CHECK_ERROR_RETURN (find_object (node, compare_object_2, &stack_2));


}

void check_answer (char *answer)
{
    while (strncasecmp (answer, "no", 2) != 0 && strncasecmp (answer, "yes", 3) != 0)
    {
        printf ("\nI don't understand this answer, enter yes or no: ");
        scanf ("%s", answer);
    }
}

char *create_str (size_t size)
{
    char *str = (char *) calloc (size, sizeof (char));
    my_assert (str != NULL);

    return str;
}