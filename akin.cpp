#include "akin.h"

int game_run (TREE *tree)
{
    printf ("Hello, you got into the game \"Akinator\". Select game mode:\n");
    printf ("\t1. Guess the object[1];\n");
    printf ("\t2. Define an object[2];\n");
    printf ("\t3. Compare objects[3];\n");
    printf ("\t4. Add new object[4];\n");
    printf ("\t5. Exit saving the database[5];\n");
    printf ("\t6. Exit without saving the database[6].\n");

    printf ("Select game mode: ");
    int mode = 0;
    scanf ("%d", &mode);

    print_tree (tree, tree->root, stdout);

    switch (mode)
    {
        case (1):
        {
            guess_words (tree->root);
            break;
        }
        case (2):
        {
            give_determ (tree->root);
            break;
        }
        case (3):
        {
            
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
            break;
        }
    }

    game_run (tree);

    return ERR_NO;
}

int guess_words (NODE *node)
{
    printf ("It is %s? ", node->value);
    char answer[] = "";
    scanf ("%s", answer);

    if (strcasecmp (answer, "yes") == 0)
    {
        if (node->left)
        {
            guess_words (node->left);
        }
        else
        {
            printf ("Yes, I guessed it right.\n");
        }
    }
    else if (strcasecmp (answer, "no") == 0)
    {
        if (!node->right)
        {
            add_objects (node);

            printf ("Now I know everything\n");
        }
        else
        {
            guess_words (node->right);
        }
    }

    return ERR_NO;
}

void add_objects (NODE *node)
{
    printf ("What is this?\n"   \
            "It is... ");
    
    int size_object = 10;
    char *object = (char *) calloc (size_object, sizeof (char));

    printf ("How is %s different from %s?", object, node->value);

    char difference[] = "";
    scanf ("%s", difference);

    printf ("%s %s", object, difference);
    add_node (node, difference, RIGHT);
    add_node (node->right, object, LEFT);
    add_node (node->right, node->value, RIGHT);
}

void give_determ (NODE *node)
{
    STACK stack = {};
    stack_ctor (&stack, STACK_SIZE);

    char defined_word[] = "";

    printf ("Enter the expression to be defined: ");

    while (true)
    {
        scanf ("%s", defined_word);

        if (find_value (node, defined_word, &stack))
        {
            break;
        }

        printf ("\nYou entered an incorrect definition expression, please repeat again: ");
    }

    for (int ip = 0; ip <= stack.position; ip++)
    {
        printf ("it is %s, ", node->value);

        if (stack.data[ip] == 1)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
}

bool find_value (NODE *node, char *search_value, STACK *stack)
{
    if (!node)
    {
        return VALUE_NOT_FIND;
    }

    if (strcasecmp (node->value, search_value) == 0)
    {
        return VALUE_FIND;
    }
    
    stack_push (stack, YES);
    
    if (find_value (node->left, search_value, stack))
    {
        return VALUE_FIND;
    }

    stack_pop (stack);
    stack_push (stack, NO);

    if (find_value (node->right, search_value, stack))
    {
        return VALUE_FIND;
    }

    return VALUE_NOT_FIND;
}