#ifndef SHELLTYPES_H
#define SHELLTYPES_H

// Passed to every shell handler. Not every handler uses every field
// (help_shell and about_shell only need socket_fd), but giving every
// handler the same signature is what lets them all sit in one table
// in typeshell.c.
typedef struct {
    int   socket_fd;
    int  *authenticated; // pointer so loginshell_shell can flip it in server.c
    char *username;      // buffer, at least 128 bytes, owned by server.c
    char *password;      // buffer, at least 128 bytes, owned by server.c
} shell_ctx_t;

// Looks up "input" in the shell table (defined in shelltyps.c) and runs
// the matching handler. Returns 1 if a match was found and run, 0 if
// "input" isn't a known shell name.
int dispatch_shell_command(const char *input, shell_ctx_t *ctx);

// Individual shells. Each lives in its own .c file. To add a new shell:
// 1. write "void my_shell(shell_ctx_t *ctx)" in its own src/my_shell.c
// 2. declare it here
// 3. add one line to the shell_table in src/shelltyps.c
// Nothing else needs to change.
void help_shell(shell_ctx_t *ctx);
void about_shell(shell_ctx_t *ctx);
void loginshell_shell(shell_ctx_t *ctx);

#endif
