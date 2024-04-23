#include "../../main/utils/utils.h"
#include <assert.h>

int main(void)
{
    char testInput[64] = "LD 123";
    char *command = get_command(testInput);
    assert(strcmp(command, "LD") == 0);
    return 0;
}
