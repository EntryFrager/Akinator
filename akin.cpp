#include "akin.h"

static int guess_object (NODE *node);
static int give_determ (NODE *node);
static void compare_object (NODE *node);
static void print_database (TREE *tree);
static int exit_save (TREE *tree);
static void print_determ_object (NODE *node, char *defined_word, STACK *stack);
static NODE *print_part_determ (NODE *node, STACK *stack);
static bool find_object (NODE *node, char *search_objects, STACK *stack);
static int check_new_objects (NODE *node);
static void add_objects (NODE *node);
static void check_answer (char *answer);

int game_run ()
{
    TREE tree = {};

    create_tree (&tree, 0);
    input_base (&tree);

    int mode = 0;

    while (true)
    {
        printf ("Hello, you got into the game \"Akinator\". Select game mode:\n");
        printf ("\t1. Guess the object[1];\n");
        printf ("\t2. Define an object[2];\n");
        printf ("\t3. Compare objects[3];\n");
        printf ("\t4. Get the base[4];\n");
        printf ("\t5. Exit saving the database[5];\n");
        printf ("\t6. Exit without saving the database[6];\n");

        printf ("Select game mode: ");
        scanf ("%d", &mode);

        switch (mode)
        {
            case (1):
            {
                CHECK_ERROR_RETURN (guess_object (tree.root));
                break;
            }
            case (2):
            {
                CHECK_ERROR_RETURN (give_determ (tree.root));
                break;
            }
            case (3):
            {
                compare_object (tree.root);
                break;
            }
            case (4):
            {
                print_database (&tree);
                break;
            }
            case (5):
            {
                CHECK_ERROR_RETURN (exit_save (&tree));

                destroy_tree (&tree);

                return ERR_NO;
            }
            case (6):
            {
                destroy_tree (&tree);

                return ERR_NO;
            }
            default:
            {
                printf ("I don't understand, please enter the correct command\n\n\n");
                break;
            }
        }

        printf ("\n\n");
    }

    return ERR_NO;
}

int guess_object (NODE *node)
{
    printf ("Is it %s? ", node->value);

    char answer[STR_SIZE] = "";
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

    return ERR_NO;
}

int give_determ (NODE *node)
{
    STACK stack = {};
    stack_ctor (&stack, STACK_SIZE);

    char defined_word[STR_SIZE] = "";

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
            return ERR_NO;
        }
    }

    print_determ_object (node, defined_word, &stack);

    stack_dtor (&stack);

    return ERR_NO;
}

void print_determ_object (NODE *node, char *defined_word, STACK *stack)
{
    printf ("The %s is: ", defined_word);

    while (stack->position != 0)
    {
        node = print_part_determ (node, stack);
    }

    printf (".\n");
}

NODE *print_part_determ (NODE *node, STACK *stack)
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

    return node;
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
    
    char answer[STR_SIZE] = "";
    scanf ("%s", answer);

    check_answer (answer);

    if (strcasecmp (answer, "yes") == 0)
    {
        guess_object (node);
        return YES;
    }
    else
    {
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
}

void compare_object (NODE *node)
{
    STACK stack_1 = {};
    STACK stack_2 = {};

    stack_ctor (&stack_1, STACK_SIZE);
    stack_ctor (&stack_2, STACK_SIZE);

    char compare_object_1[STR_SIZE] = "";
    char compare_object_2[STR_SIZE] = "";

    printf ("Enter the name of the first object to compare: ");
    scanf ("%s", compare_object_1);
    printf ("Enter the name of the second object being compared: ");
    scanf ("%s", compare_object_2);

    find_object (node, compare_object_1, &stack_1);
    find_object (node, compare_object_2, &stack_2);

    printf ("They are similar in that both: ");

    while ((stack_1.data[stack_1.position - 1] == stack_2.data[stack_2.position - 1]) &&
           (stack_1.position != 0 && stack_2.position != 0))
    {
        node = print_part_determ (node, &stack_1);
        stack_pop (&stack_2);
    }

    NODE *node_1 = node;
    NODE *node_2 = node;

    printf ("\nBut they have differences:\n\t");

    print_determ_object (node_1, compare_object_1, &stack_1);
    printf ("\t");
    print_determ_object (node_2, compare_object_2, &stack_2);

    stack_dtor (&stack_1);
    stack_dtor (&stack_2);
}

int exit_save (TREE *tree)
{
    _FOPEN (fp, tree->info.fp_name_base, "w+");

    print_tree (tree->root, fp);

    _FCLOSE (fp);

    return ERR_NO;
}

void print_database (TREE *tree)
{
    tree_dump_graph_viz (tree, __FILE__, __func__, __LINE__);
    system ("dot.svg");
}

void check_answer (char *answer)
{
    while (strncasecmp (answer, "no", 2) != 0 && strncasecmp (answer, "yes", 3) != 0)
    {
        printf ("\nI don't understand this answer, enter yes or no: ");
        scanf ("%s", answer);
    }
}