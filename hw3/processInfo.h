struct processInfo {
  int state;    // process state
  int ppid;     // parent PID
  int sz;       // ssize of process memory
  int nfd;      // number of open file descriptors in the process
  int nrswitch; // number context swithces in
};

