#ifndef APPLETS_H
#define APPLETS_H

extern int ls_main(int argc, char **argv);
extern int cat_main(int argc, char **argv);
extern int echo_main(int argc, char **argv);
extern int cp_main(int argc, char **argv);
extern int mv_main(int argc, char **argv);
extern int rm_main(int argc, char **argv);
extern int mkdir_main(int argc, char **argv);
extern int rmdir_main(int argc, char **argv);
extern int ln_main(int argc, char **argv);
extern int chmod_main(int argc, char **argv);
extern int pwd_main(int argc, char **argv);
extern int touch_main(int argc, char **argv);
extern int sh_main(int argc, char **argv);
extern int head_main(int argc, char **argv);
extern int tail_main(int argc, char **argv);
extern int wc_main(int argc, char **argv);
extern int grep_main(int argc, char **argv);
extern int sort_main(int argc, char **argv);
extern int uniq_main(int argc, char **argv);
extern int cut_main(int argc, char **argv);
extern int tr_main(int argc, char **argv);
extern int basename_main(int argc, char **argv);
extern int dirname_main(int argc, char **argv);
extern int whoami_main(int argc, char **argv);
extern int id_main(int argc, char **argv);
extern int hostname_main(int argc, char **argv);
extern int uname_main(int argc, char **argv);
extern int date_main(int argc, char **argv);
extern int sleep_main(int argc, char **argv);
extern int true_main(int argc, char **argv);
extern int false_main(int argc, char **argv);
extern int yes_main(int argc, char **argv);
extern int seq_main(int argc, char **argv);
extern int kill_main(int argc, char **argv);
extern int ps_main(int argc, char **argv);
extern int printf_main(int argc, char **argv);
extern int test_main(int argc, char **argv);
extern int expr_main(int argc, char **argv);
extern int tee_main(int argc, char **argv);
extern int xargs_main(int argc, char **argv);
extern int find_main(int argc, char **argv);
extern int du_main(int argc, char **argv);
extern int df_main(int argc, char **argv);
extern int chown_main(int argc, char **argv);
extern int chgrp_main(int argc, char **argv);
extern int stat_main(int argc, char **argv);
extern int file_main(int argc, char **argv);
extern int readlink_main(int argc, char **argv);
extern int realpath_main(int argc, char **argv);
extern int which_main(int argc, char **argv);
extern int env_main(int argc, char **argv);
extern int nproc_main(int argc, char **argv);
extern int arch_main(int argc, char **argv);
extern int split_main(int argc, char **argv);
extern int tac_main(int argc, char **argv);
extern int rev_main(int argc, char **argv);
extern int nl_main(int argc, char **argv);
extern int od_main(int argc, char **argv);
extern int comm_main(int argc, char **argv);
extern int join_main(int argc, char **argv);
extern int paste_main(int argc, char **argv);
extern int fold_main(int argc, char **argv);
extern int fmt_main(int argc, char **argv);
extern int strings_main(int argc, char **argv);
extern int cksum_main(int argc, char **argv);
extern int md5sum_main(int argc, char **argv);
extern int sha1sum_main(int argc, char **argv);
extern int base64_main(int argc, char **argv);
extern int tty_main(int argc, char **argv);
extern int clear_main(int argc, char **argv);
extern int mktemp_main(int argc, char **argv);
extern int mknod_main(int argc, char **argv);
extern int mkfifo_main(int argc, char **argv);
extern int mount_main(int argc, char **argv);
extern int umount_main(int argc, char **argv);
extern int chroot_main(int argc, char **argv);
extern int sync_main(int argc, char **argv);
extern int reboot_main(int argc, char **argv);
extern int halt_main(int argc, char **argv);
extern int poweroff_main(int argc, char **argv);
extern int dmesg_main(int argc, char **argv);
extern int free_main(int argc, char **argv);
extern int uptime_main(int argc, char **argv);
extern int nice_main(int argc, char **argv);
extern int nohup_main(int argc, char **argv);
extern int pidof_main(int argc, char **argv);
extern int link_main(int argc, char **argv);
extern int unlink_main(int argc, char **argv);
extern int truncate_main(int argc, char **argv);
extern int dd_main(int argc, char **argv);
extern int cal_main(int argc, char **argv);
extern int factor_main(int argc, char **argv);
extern int logname_main(int argc, char **argv);
extern int groups_main(int argc, char **argv);
extern int printenv_main(int argc, char **argv);
extern int timeout_main(int argc, char **argv);
extern int renice_main(int argc, char **argv);
extern int vmstat_main(int argc, char **argv);
extern int swapon_main(int argc, char **argv);
extern int swapoff_main(int argc, char **argv);
extern int setsid_main(int argc, char **argv);
extern int unshare_main(int argc, char **argv);
extern int ionice_main(int argc, char **argv);
extern int sed_main(int argc, char **argv);
extern int diff_main(int argc, char **argv);
extern int tar_main(int argc, char **argv);
extern int ping_main(int argc, char **argv);
extern int netstat_main(int argc, char **argv);
extern int ifconfig_main(int argc, char **argv);
extern int nc_main(int argc, char **argv);
extern int expand_main(int argc, char **argv);
extern int unexpand_main(int argc, char **argv);
extern int column_main(int argc, char **argv);
extern int shuf_main(int argc, char **argv);
extern int tsort_main(int argc, char **argv);
extern int umask_main(int argc, char **argv);
extern int ulimit_main(int argc, char **argv);
extern int install_main(int argc, char **argv);
extern int type_main(int argc, char **argv);
extern int hexdump_main(int argc, char **argv);
extern int shred_main(int argc, char **argv);
extern int host_main(int argc, char **argv);
extern int csplit_main(int argc, char **argv);
extern int look_main(int argc, char **argv);
extern int watch_main(int argc, char **argv);
extern int lsblk_main(int argc, char **argv);
extern int losetup_main(int argc, char **argv);
extern int blockdev_main(int argc, char **argv);
extern int fsync_main(int argc, char **argv);
extern int flock_main(int argc, char **argv);
extern int uuidgen_main(int argc, char **argv);
extern int sha256sum_main(int argc, char **argv);
extern int nologin_main(int argc, char **argv);
extern int lscpu_main(int argc, char **argv);
extern int top_main(int argc, char **argv);
extern int xxd_main(int argc, char **argv);
extern int sum_main(int argc, char **argv);
extern int more_main(int argc, char **argv);
extern int tput_main(int argc, char **argv);
extern int stty_main(int argc, char **argv);
extern int useradd_main(int argc, char **argv);
extern int userdel_main(int argc, char **argv);
extern int groupadd_main(int argc, char **argv);
extern int groupdel_main(int argc, char **argv);
extern int wall_main(int argc, char **argv);
extern int write_main(int argc, char **argv);
extern int banner_main(int argc, char **argv);
extern int chsh_main(int argc, char **argv);
extern int chfn_main(int argc, char **argv);
extern int reset_main(int argc, char **argv);

typedef int (*applet_fn)(int argc, char **argv);

struct applet {
    const char *name;
    applet_fn main;
};

static const struct applet applets[] = {
    { "ls",       ls_main       },
    { "cat",      cat_main      },
    { "echo",     echo_main     },
    { "cp",       cp_main       },
    { "mv",       mv_main       },
    { "rm",       rm_main       },
    { "mkdir",    mkdir_main    },
    { "rmdir",    rmdir_main    },
    { "ln",       ln_main       },
    { "chmod",    chmod_main    },
    { "pwd",      pwd_main      },
    { "touch",    touch_main    },
    { "sh",       sh_main       },
    { "head",     head_main     },
    { "tail",     tail_main     },
    { "wc",       wc_main       },
    { "grep",     grep_main     },
    { "sort",     sort_main     },
    { "uniq",     uniq_main     },
    { "cut",      cut_main      },
    { "tr",       tr_main       },
    { "basename", basename_main },
    { "dirname",  dirname_main  },
    { "whoami",   whoami_main   },
    { "id",       id_main       },
    { "hostname", hostname_main },
    { "uname",    uname_main    },
    { "date",     date_main     },
    { "sleep",    sleep_main    },
    { "true",     true_main     },
    { "false",    false_main    },
    { "yes",      yes_main      },
    { "seq",      seq_main      },
    { "kill",     kill_main     },
    { "ps",       ps_main       },
    { "printf",   printf_main   },
    { "test",     test_main     },
    { "expr",     expr_main     },
    { "tee",      tee_main      },
    { "xargs",    xargs_main    },
    { "find",     find_main     },
    { "du",       du_main       },
    { "df",       df_main       },
    { "chown",    chown_main    },
    { "chgrp",    chgrp_main    },
    { "stat",     stat_main     },
    { "file",     file_main     },
    { "readlink", readlink_main },
    { "realpath", realpath_main },
    { "which",    which_main    },
    { "env",      env_main      },
    { "nproc",    nproc_main    },
    { "arch",     arch_main     },
    { "split",    split_main    },
    { "tac",      tac_main      },
    { "rev",      rev_main      },
    { "nl",       nl_main       },
    { "od",       od_main       },
    { "comm",     comm_main     },
    { "join",     join_main     },
    { "paste",    paste_main    },
    { "fold",     fold_main     },
    { "fmt",      fmt_main      },
    { "strings",  strings_main  },
    { "cksum",    cksum_main    },
    { "md5sum",   md5sum_main   },
    { "sha1sum",  sha1sum_main  },
    { "base64",   base64_main   },
    { "tty",      tty_main      },
    { "clear",    clear_main    },
    { "mktemp",   mktemp_main   },
    { "mknod",    mknod_main    },
    { "mkfifo",   mkfifo_main   },
    { "mount",    mount_main    },
    { "umount",   umount_main   },
    { "chroot",   chroot_main   },
    { "sync",     sync_main     },
    { "reboot",   reboot_main   },
    { "halt",     halt_main     },
    { "poweroff", poweroff_main },
    { "dmesg",    dmesg_main    },
    { "free",     free_main     },
    { "uptime",   uptime_main   },
    { "nice",     nice_main     },
    { "nohup",    nohup_main    },
    { "pidof",    pidof_main    },
    { "link",     link_main     },
    { "unlink",   unlink_main   },
    { "truncate", truncate_main },
    { "dd",       dd_main       },
    { "cal",      cal_main      },
    { "factor",   factor_main   },
    { "logname",  logname_main  },
    { "groups",   groups_main   },
    { "printenv", printenv_main },
    { "timeout",  timeout_main  },
    { "renice",   renice_main   },
    { "vmstat",   vmstat_main   },
    { "swapon",   swapon_main   },
    { "swapoff",  swapoff_main  },
    { "setsid",   setsid_main   },
    { "unshare",  unshare_main  },
    { "ionice",   ionice_main   },
    { "sed",       sed_main       },
    { "diff",      diff_main      },
    { "tar",       tar_main       },
    { "ping",      ping_main      },
    { "netstat",   netstat_main   },
    { "ifconfig",  ifconfig_main  },
    { "nc",        nc_main        },
    { "expand",    expand_main    },
    { "unexpand",  unexpand_main  },
    { "column",    column_main    },
    { "shuf",      shuf_main      },
    { "tsort",     tsort_main     },
    { "umask",     umask_main     },
    { "ulimit",    ulimit_main    },
    { "install",   install_main   },
    { "type",      type_main      },
    { "hexdump",   hexdump_main   },
    { "shred",     shred_main     },
    { "host",      host_main      },
    { "csplit",    csplit_main    },
    { "look",      look_main      },
    { "watch",     watch_main     },
    { "lsblk",     lsblk_main     },
    { "losetup",   losetup_main   },
    { "blockdev",  blockdev_main  },
    { "fsync",     fsync_main     },
    { "flock",     flock_main     },
    { "uuidgen",   uuidgen_main   },
    { "sha256sum", sha256sum_main },
    { "nologin",   nologin_main   },
    { "lscpu",     lscpu_main     },
    { "top",       top_main       },
    { "xxd",       xxd_main       },
    { "sum",       sum_main       },
    { "more",      more_main      },
    { "tput",      tput_main      },
    { "stty",      stty_main      },
    { "useradd",   useradd_main   },
    { "userdel",   userdel_main   },
    { "groupadd",  groupadd_main  },
    { "groupdel",  groupdel_main  },
    { "wall",      wall_main      },
    { "write",     write_main     },
    { "banner",    banner_main    },
    { "chsh",      chsh_main      },
    { "chfn",      chfn_main      },
    { "reset",     reset_main     },
    { NULL,        NULL           }
};

#endif
