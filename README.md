<div align="center">

```
 __  __            _
|  \/  | __ _ _ __(_) ___
| |\/| |/ _` | '__| |/ _ \
| |  | | (_| | |  | |  __/
|_|  |_|\__,_|_|  |_|\___|

   a socket server with a shell of its own
```

[![language](https://img.shields.io/badge/language-C-00599C?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![build](https://img.shields.io/badge/build-make-blue?style=flat-square)](#building-and-running)
[![status](https://img.shields.io/badge/status-active%20development-yellow?style=flat-square)](#roadmap)
[![license](https://img.shields.io/badge/license-see%20LICENSE-lightgrey?style=flat-square)](LICENSE)

</div>

---

Marie is a lightweight, self-hosted TCP socket server for a home lab. Point `nc` at it, and you're dropped into a small login shell that gates access behind credentials and routes every command through an extensible dispatch table.

The login banner's ASCII art was pulled from the cover of Drake's *If You're Reading This It's Too Late* — because a server can have taste too.

## ✨ What it does

| | |
|---|---|
| 🔌 **One process per client** | Every connection gets its own forked, isolated process |
| 🗝️ **Gated login** | Nothing runs until `loginshell` checks out against `users.txt` |
| 🧩 **Pluggable shells** | `help`, `about`, `loginshell` today — each is one file, one line in a table |
| 🌍 **Multilingual help** | The `help` shell speaks English, French, and Arabic |
| 🎨 **A logo with a backstory** | Post-login ASCII art inspired by an album, not a template |

## 🖥️ What it looks like

```text
$ nc 127.0.0.1 8080
Connected to Marie Server.
> help

 __________________________________________
|            LANGUAGE_MODES               |
|__________________________________________|
|   [1] ENG   [2] FR   [3] AR              |
|__________________________________________|
| >>> Select an option (default eng): 1

=========================== [MARIE HELP SHELL] =========================
  help         - Show the available commands in this interaction window.
  loginshell   - Show the user login interaction menu.
  about        - Show information about the project state and commands.
========================================================================
> loginshell
Username: alice
Password: ********
Login successful!

=========================================
          login shell for marie
=========================================

⠀⣾⠙⢶⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⣏⠱⣌⠣⡈⠓⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢾⢺⣤⡘⢦⡈⢢⡀⠱⣤⢶⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢸⡿⡏⠻⡀⠳⡄⠹⡄⠹⡸⡇⠀⠀⠀⠀⠀⠀⠀⠀   [LOGGED IN USER]
⣸⠃⢳⡀⢳⠀⢹⡀⠱⡄⠳⣹⣄⠀⠀⠀⠀⠀⠀⠀   Username: alice
⠈⢧⠈⢿⣆⠀⠀⠀⠀⠀⠀⠀⠀⣏⠀⠀⠀⠀⠀⠀
⠀⠀⡇⠈⢹⡄⠀⠀⠀⠀⠀⠀⠀⠹⡀⠀⠀⠀⠀⠀
⠀⠀⢳⠀⠈⢧⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀
⠀⠀⠈⢧⠀⠈⣆⡀⠀⠀⠀⠀⠀⠀⢻⣤⣤⣤⣀⡀
⠀⠀⢀⣾⣦⠀⠈⠳⣄⠀⠀⠀⠀⢠⣾⣿⣿⣿⡟⠁
⠀⠀⣿⣿⣿⠀⠀⠀⠈⣳⣤⡀⣰⣿⣿⣿⣿⡟⠀⠀
⠀⠀⣿⣿⣿⣀⣀⣀⣴⣿⡗⠳⣿⣿⣿⣿⣿⠃⠀⠀
⠀⠀⠹⣿⣿⣿⣿⣿⣿⡿⠁⢰⣿⣿⣿⣿⡏⠀⠀⠀
⠀⠀⠀⠙⠻⠿⠿⠿⠛⠁⠀⠈⠛⠿⣿⣿⠁⠀⠀⠀

> quit
```

## 🧠 How it flows

```
        accept() blocks
              │
              ▼
      client connects ──► fork()
              │                │
   parent loops back      child owns this
   to accept()  ◄──┐       session from here
                    │           │
                    │           ▼
                    │   ┌─────────────────┐
                    │   │ unauthenticated │──► dispatch_shell_command()
                    │   │      shell      │      (help / about / loginshell)
                    │   └────────┬────────┘
                    │            │ valid login
                    │            ▼
                    │   ┌─────────────────┐
                    └───│  authenticated  │──► echoes commands (for now)
                        │      loop       │
                        └────────┬────────┘
                                 │ "quit"
                                 ▼
                          connection closed
```

### Why a dispatch table, not a wall of `if`s

Every shell is its own file with one function: `void my_shell(shell_ctx_t *ctx)`. A single table in `src/shelltyps.c` maps command names to those functions, and `server.c` calls `dispatch_shell_command()` without ever knowing which shells exist. Growing the shell set is always the same three moves:

1. Write the handler in its own `src/*.c` file.
2. Declare it in `includes/shelltypes.h`.
3. Add one line to `shell_table`.

Nothing else changes — the Makefile globs `src/*.c` automatically.

## 📁 Project layout

```
src/
├── main.c        entry point — calls start_server()
├── server.c      socket setup, accept/fork loop, per-client session loop
├── shelltyps.c   the shell dispatch table
├── helpshell.c   the "help" shell (multi-language command list)
├── about.c       the "about" shell
├── loginshell.c  the "loginshell" shell (credential prompt + check)
├── auth.c        check_credentials() — verifies against users.txt
└── Look.c        send_ascii_logo() — the post-login banner

includes/         matching headers for each module above
users.txt         username/password pairs, one per line
Makefile          builds every src/*.c → obj/*.o → ./marie
```

## 🚀 Building and running

```bash
make        # builds ./marie
./marie     # starts listening on port 8080
```

Connect from another terminal:

```bash
nc 127.0.0.1 8080
```

| Target | Does |
|---|---|
| `make` | build |
| `make clean` | remove object files |
| `make fclean` | remove object files + binary |
| `make re` | clean rebuild |

## 🔐 Current authentication

Credentials live in `users.txt` as plaintext `username password` pairs, checked with a direct string comparison. Simple on purpose, for now.

## 🗺️ Roadmap

- [ ] **Hash stored passwords** — stop storing and comparing plaintext credentials in `users.txt`
- [ ] **New shells** — grow beyond `help` / `about` / `loginshell`
- [ ] **Authenticated command table** — replace the current `ACK:` echo with real tools, dispatched the same way the login-side shells are

## 🎧 Credits

The post-login banner's owl silhouette is a nod to Drake's *If You're Reading This It's Too Late* — one of the maintainer's favorite albums. Log in and you'll see it.

## 📄 License

See [`LICENSE`](LICENSE).