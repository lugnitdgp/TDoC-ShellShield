#include <iostream>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include<cstdlib>
#define  concat(a,b) (a"" b)
#define CGROUP_MEM "/sys/fs/cgroup/cntr/memory.max"
#define CGROUP_CPU "/sys/fs/cgroup/cntr/cpu.max"
#define CGROUP_SUBTREE "/sys/fs/cgroup/cgroup.subtree_control"
#define REQ_CGROUP "/sys/fs/cgroup/cntr"