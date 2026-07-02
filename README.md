# utilbox

A multi-call Unix utility binary, written entirely from scratch in C — no
BusyBox or toybox source involved. Inspired by BusyBox's single-binary
design, `utilbox` bundles 150 command-line utilities into one small
executable, dispatched via symlinks or a subcommand argument.

Originally built as the userland toolset for **Anvil Linux**, a
from-scratch Linux distribution, but usable standalone on any Linux system
(including Termux/Android).

## Why

- **Small footprint** — one binary, symlinks instead of 150 separate
  executables.
- **From scratch** — every applet is original code, easy to read, easy to
  extend or fork.
- **Batteries included shell** — the bundled `sh` supports pipes,
  redirection, variables, `~` expansion, `;` chaining, an interactive REPL,
  and even **typo auto-correction** (mistype a command and it suggests/runs
  the closest match).

## Build

```sh
git clone https://github.com/YOUR_USERNAME/utilbox.git
cd utilbox
make
```
Requires a C11 compiler (cc/gcc/clang) and a POSIX-ish libc. Tested on
Termux (Android/Bionic) and should build cleanly on any standard Linux libc
(glibc/musl) with minor adjustments.

## install

make install                      # installs to /usr/local/bin by default
make install DESTDIR=/custom/path # or a custom prefix
This copies the utilbox binary and creates a symlink for every applet
(ls, cat, grep, etc.) pointing back to it.

## usage

Two ways to invoke any applet:

- via symlink (after install, or manually: ln -s utilbox ls)
ls -l

- via subcommand, no symlink needed
./utilbox ls -l

## applets

• File & text: ls cat cp mv rm mkdir rmdir ln chmod chown
chgrp touch stat file readlink realpath head tail wc
grep sort uniq cut tr sed diff basename dirname split
tac rev nl od comm join paste fold fmt expand
unexpand column shuf tsort csplit look install link
unlink truncate shred mktemp mknod mkfifo tar xxd
hexdump strings sum cksum md5sum sha1sum sha256sum base64

• Shell & process: sh echo printf test expr xargs tee kill
ps top nice nohup renice ionice timeout watch setsid
unshare pidof nproc

• System & admin: mount umount chroot sync reboot halt
poweroff dmesg free uptime vmstat lscpu lsblk losetup
blockdev swapon swapoff fsync flock umask ulimit useradd
userdel groupadd groupdel chsh chfn nologin

• Networking: ping netstat ifconfig nc host

• Info & misc: pwd whoami id groups logname hostname uname
date sleep true false yes seq cal factor env printenv
which type arch uuidgen tty clear reset tput stty
more wall write banner

## The sh Shell

Beyond a plain command runner, the bundled shell supports:
cat file.txt | grep pattern | sort          # pipelines
echo hello > out.txt                        # redirection
NAME=world; echo hello $NAME                # variables + chaining
cd ~/projects                               # tilde expansion + builtin cd
unamw -a                                    # → did you mean 'uname'? runs it
Run ./utilbox sh with no arguments for an interactive REPL.

## Design Notes

• Single translation unit per applet (src/<name>.c), dispatched through a
static table in include/applets.h.
• md5sum, sha1sum, and sha256sum implement their hash algorithms from
scratch (no OpenSSL/libcrypto dependency).
• tar implements the ustar format directly (create/extract/list,
uncompressed).
• Typo correction uses Levenshtein edit distance against built-in applets
and everything in $PATH.

## Limitations

This is a from-scratch, educational-leaning implementation — not a drop-in
GNU coreutils replacement. Notable gaps: no regex-heavy sed/grep
features beyond basic POSIX regex, sh has no &&/||/background jobs
yet, diff uses a simple LCS without unified-diff formatting.
Contributions and forks welcome — see [LICENSE](LICENSE)

## LICENSE

GPL v3 -- see [LICENSE](LICENSE)
