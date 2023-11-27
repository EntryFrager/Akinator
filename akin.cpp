#include "akin.h"

static void guess_object (NODE *node);
static int give_determ (NODE *node);
static int compare_objects (NODE *node);
static int exit_save (TREE *tree);
static void print_database (TREE *tree);
static bool find_object (NODE *node, const char *search_object, STACK *stack);
static int check_new_object (NODE *node);
static void add_object (NODE *node);
static void print_determ_object (NODE *node, const char *defined_word, STACK *stack);
static NODE *print_part_determ (NODE *node, STACK *stack);
static void print_compare_determ (NODE *node, const char *object_1, const char *object_2, \
                                  STACK *stack_1, STACK *stack_2);
static void base_dump (TREE *tree);
static int create_node (NODE *node, FILE *stream, int ip_parent, int ip, char *color);
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
        printf ("\t3. Compare object[3];\n");
        printf ("\t4. Get the base[4];\n");
        printf ("\t5. Exit saving the database[5];\n");
        printf ("\t6. Exit without saving the database[6];\n");

        printf ("Select game mode: ");

        if (scanf ("%d", &mode) != 1)
        {
            printf ("I don't understand, please enter the correct command\n\n\n");
            clean_buffer ();
            continue;
        }

        clean_buffer ();

        switch (mode)
        {
            case (GUESS):
            {
                guess_object (tree.root);
                break;
            }
            case (DEFIN):
            {
                CHECK_ERROR_RETURN (give_determ (tree.root));
                break;
            }
            case (COMPARE):
            {
                CHECK_ERROR_RETURN (compare_objects (tree.root));
                break;
            }
            case (GET_BASE):
            {
                print_database (&tree);
                break;
            }
            case (EXIT_SAVE):
            {
                CHECK_ERROR_RETURN (exit_save (&tree));

                destroy_tree (&tree);

                return ERR_NO;
            }
            case (EXIT_NO_SAVE):
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

void guess_object (NODE *node)
{
    my_assert (node != NULL);

    printf ("Is it %s? ", node->value);

    char *answer = get_str (stdin);

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
            add_object (node);

            printf ("Now I know everything\n");
        }
        else
        {
            guess_object (node->right);
        }
    }

    free (answer);
}

int give_determ (NODE *node)
{
    my_assert (node != NULL);

    STACK stack = {};
    stack_ctor (&stack, STACK_SIZE);

    char *defined_word = NULL;

    printf ("Enter the expression to be defined: ");

    while (true)
    {
        defined_word = get_str (stdin);

        if (find_object (node, defined_word, &stack))
        {
            break;
        }

        if (check_new_object (node) == YES)
        {
            free (defined_word);

            return ERR_NO;
        }
    }

    print_determ_object (node, defined_word, &stack);

    stack_dtor (&stack);
    free (defined_word);

    return ERR_NO;
}

int compare_objects (NODE *node)
{
    my_assert (node != NULL);

    STACK stack_1 = {};
    STACK stack_2 = {};

    stack_ctor (&stack_1, STACK_SIZE);
    stack_ctor (&stack_2, STACK_SIZE);

    printf ("Enter the name of the first object to compare: ");
    char *object_1 = get_str (stdin);
    printf ("Enter the name of the second object being compared: ");
    char *object_2 = get_str (stdin);

    if (!find_object (node, object_1, &stack_1) || !find_object (node, object_2, &stack_2))
    {
        if (check_new_object (node) == YES)
        {
            free (object_1);
            free (object_2);
        }

        return ERR_NO;
    }

    print_compare_determ (node, object_1, object_2, &stack_1, &stack_2);

    stack_dtor (&stack_1);
    stack_dtor (&stack_2);

    return ERR_NO;
}

int exit_save (TREE *tree)
{
    my_assert (tree != NULL);

    FOPEN_ (fp, tree->info.fp_name_base, "w+");

    print_tree (tree->root, fp);

    FCLOSE_ (fp);

    return ERR_NO;
}

void print_database (TREE *tree)
{
    my_assert (tree != NULL);

    base_dump (tree);
    system ("dot.svg");
}

bool find_object (NODE *node, const char *search_object, STACK *stack)
{
    my_assert (search_object != NULL);
    my_assert (stack != NULL);

    if (!node)
    {
        return OBJECT_NOT_FIND;
    }

    if (strcasecmp (node->value, search_object) == 0)
    {
        return OBJECT_FIND;
    }
        
    if (find_object (node->left, search_object, stack))
    {
        stack_push (stack, YES);
        return OBJECT_FIND;
    }    

    if (find_object (node->right, search_object, stack))
    {
        stack_push (stack, NO);
        return OBJECT_FIND;
    }

    return OBJECT_NOT_FIND;
}

int check_new_object (NODE *node)
{
    my_assert (node != NULL);

    printf ("This element is not in the database, do you want to add it?\n");
    
    char *answer = get_str (stdin);

    check_answer (answer);

    if (strcasecmp (answer, "yes") == 0)
    {
        guess_object (node);
        free (answer);

        return YES;
    }
    else
    {
        free (answer);

        return NO;
    }

    free (answer);

    return NO;
}

void add_object (NODE *node)
{
    my_assert (node != NULL);

    printf ("What is this?\n"   \
            "It is... ");
    
    char *new_object = get_str (stdin);

    printf ("How is %s different from %s?\n", new_object, node->value);

    char* difference = get_str (stdin);

    const char *temp_old_object = node->value;

    node->value = difference;
    add_node (node, new_object, LEFT);
    add_node (node, temp_old_object, RIGHT);
}

void print_determ_object (NODE *node, const char *defined_word, STACK *stack)
{
    my_assert (node != NULL);
    my_assert (defined_word != NULL);
    my_assert (stack != NULL);

    printf ("The %s is: ", defined_word);

    while (stack->position != 0)
    {
        node = print_part_determ (node, stack);
    }

    printf (".\n");
}

NODE *print_part_determ (NODE *node, STACK *stack)
{
    my_assert (node != NULL);
    my_assert (stack != NULL);

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

void print_compare_determ (NODE *node, const char *object_1, const char *object_2, \
                          STACK *stack_1, STACK *stack_2)
{
    my_assert (node != NULL);
    my_assert (object_1 != NULL);
    my_assert (object_2 != NULL);
    my_assert (stack_1 != NULL);
    my_assert (stack_2 != NULL);

    printf ("They are similar in that both: ");

    while ((stack_1->data[stack_1->position - 1] == stack_2->data[stack_2->position - 1]) &&
           (stack_1->position != 0 && stack_2->position != 0))
    {
        node = print_part_determ (node, stack_1);
        stack_pop (stack_2);
    }

    NODE *node_1 = node;
    NODE *node_2 = node;

    printf ("\nBut they have differences:\n\t");

    print_determ_object (node_1, object_1, stack_1);
    printf ("\t");
    print_determ_object (node_2, object_2, stack_2);
}

#define DUMP_DOT(str) fprintf (fp_dot, str "\n");
#define DUMP_DOT_PARAM(str, ...) fprintf (fp_dot, str "\n", __VA_ARGS__);

void base_dump (TREE *tree)
{
    FILE *fp_dot = fopen (tree->info.fp_dot_name, "w+");

    if (fp_dot == NULL)
    {
        my_strerr (ERR_FOPEN, stderr);
    }
    else
    {
        if (tree != NULL)
        {
            DUMP_DOT ("digraph List {");
            DUMP_DOT ("\trankdir = HR;");
            DUMP_DOT ("\tbgcolor = " BACK_GROUND_COLOR ";");

            if (tree->root != NULL)
            {
                create_node (tree->root, fp_dot, -1, 0, RED_COLOR);
            }

            DUMP_DOT ("}");
        }
    }

    if (fclose (fp_dot) != 0)
    {
        my_strerr (ERR_FCLOSE, stderr);
    }

    char *command = "dot -Tsvg include/dump.dot -o dot.svg";

    system (command);

    tree_dump_html (tree);
}

#undef DUMP_DOT
#undef DUMP_DOT_PARAM

int create_node (NODE *node, FILE *stream, int ip_parent, int ip, char *color)
{
    if (!node)
    {
        return ip - 1;
    }

    fprintf (stream, "\tnode%d [shape = Mrecord, style = filled, fillcolor = %s, "
             "label = \"{idx: %p | value: %s | left: %p | right: %p | parent: %p}\"];\n",
             ip, color, node, node->value, node->left, node->right, node->parent);

    if (ip > 0)
    {
        fprintf (stream, "\tnode%d -> node%d [color = %s]\n", ip_parent, ip, color);
    }

    ip_parent = ip;

    ip = create_node (node->left, stream, ip_parent, ip + 1, BLUE_COLOR);
    ip = create_node (node->right, stream, ip_parent, ip + 1, LIGHT_GREEN_COLOR);

    return ip;
}

void check_answer (char *answer)
{
    my_assert (answer != NULL);

    while (strncasecmp (answer, "no", 2) != 0 && strncasecmp (answer, "yes", 3) != 0)
    {
        printf ("\nI don't understand this answer, enter yes or no: ");
        answer = get_str (stdin);
    }
}