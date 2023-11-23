#include "akin.h"

int main ()
{
    TREE tree = {};

    create_tree (&tree, 0);
    input_base (&tree);

    CALL_DUMP (&tree, 0);

    game_run (&tree);

    destroy_tree (&tree);
    
    return 0;
}