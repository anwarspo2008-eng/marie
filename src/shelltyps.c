#include <string.h>
#include <stddef.h>
#include "shelltypes.h"

// The dispatch table: one entry per shell. To add a new shell later,
// write its handler in its own file, declare it in shelltypes.h, and
// add one line here. dispatch_shell_command() is the only thing that
// reads this table, so nothing else in the codebase needs to change.
typedef struct {
    const char *name;
    void (*handler)(shell_ctx_t *ctx);
} shell_command_t;

static const shell_command_t shell_table[] = {
    { "loginshell", loginshell_shell },
    { "help",       help_shell       },
    { "about",      about_shell      },
};

#define SHELL_COUNT (sizeof(shell_table) / sizeof(shell_table[0]))

int dispatch_shell_command(const char *input, shell_ctx_t *ctx)
{
    for (size_t i = 0; i < SHELL_COUNT; i++) {
        if (strcmp(input, shell_table[i].name) == 0) {
            shell_table[i].handler(ctx);
            return 1;
        }
    }
    return 0;
}
