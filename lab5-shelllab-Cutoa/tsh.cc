// 
// tsh - A tiny shell program with job control
// 
// Jason Swartz
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"

//
// Needed global variable definitions
//

static char prompt[] = "tsh> ";
int verbose = 0;

//
// You need to implement the functions eval, builtin_cmd, do_bgfg,
// waitfg, sigchld_handler, sigstp_handler, sigint_handler
//
// The code below provides the "prototypes" for those functions
// so that earlier code can refer to them. You need to fill in the
// function bodies below.
// 

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

//
// main - The shell's main routine 
//
int main(int argc, char **argv) 
{
  int emit_prompt = 1; // emit prompt (default)

  //
  // Redirect stderr to stdout (so that driver will get all output
  // on the pipe connected to stdout)
  //
  dup2(1, 2);

  /* Parse the command line */
  char c;
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
    case 'h':             // print help message
      usage();
      break;
    case 'v':             // emit additional diagnostic info
      verbose = 1;
      break;
    case 'p':             // don't print a prompt
      emit_prompt = 0;  // handy for automatic testing
      break;
    default:
      usage();
    }
  }

  //
  // Install the signal handlers
  //

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT,  sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
  Signal(SIGCHLD, sigchld_handler);  // Terminated or stopped child

  //
  // This one provides a clean way to kill the shell
  //
  Signal(SIGQUIT, sigquit_handler); 

  //
  // Initialize the job list
  //
  initjobs(jobs);

  //
  // Execute the shell's read/eval loop
  //
  for(;;) {
    //
    // Read command line
    //
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }

    char cmdline[MAXLINE];

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      app_error("fgets error");
    }
    //
    // End of file? (did user type ctrl-d?)
    //
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }

    //
    // Evaluate command line
    //
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); //control never reaches here
}
  









/////////////////////////////////////////////////////////////////////////////
//
// eval - Evaluate the command line that the user has just typed in
// 
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.  Note:
// each child process must have a unique process group ID so that our
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when we type ctrl-c (ctrl-z) at the keyboard.
//
void eval(char *cmdline) 
{
    /* Parse command line */
    //
    // The 'argv' vector is filled in by the parseline
    // routine below. It provides the arguments needed
    // for the execve() routine, which you'll need to
    // use below to launch a process.
    //

    //
    // The 'bg' variable is TRUE if the job should run
    // in background mode or FALSE if it should run in FG
    //
    
    char *argv[MAXARGS]; //argument list
    char buf[MAXLINE]; //command line
    int bg; //if process should run in background or foreground
    pid_t pid; //process id
    sigset_t set; //signal set to block certain signals
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;        /* Ignore empty lines */
    
    
    if (!builtin_cmd(argv)) { //if a command is read
        
        //block sigchild. This prevents race; prevents child from being reaped before being added to job list
        sigemptyset(&set); //clean up sets, initialize
        sigaddset(&set, SIGCHLD); //add SIGCHLD to the set
        sigprocmask(SIG_BLOCK, &set, NULL); //block signals in the sets
        
        
        //Process groups: every process is part of a process group, with a unique id returned by getpgid(). Children belong to the same group as their parent, but can change it's own or another's group by using setpgid. Implementation is sending a signal to an entire group using -pid.s
        
        //Fork() is when a parent process creates a new child process that has a copy of the parent's adress space, file descriptors, but different PID's. It is called once, but returns twice, once in the parent process (returns the PID of the child), and zero in the child. Parent and child run concurrently. 
        //Child runs user job
        if ((pid = fork()) == 0) { //if 0, is child process.
            setpgid(0, 0); //set child's group to a new process group (this is identical to the child's PID)
            sigprocmask(SIG_UNBLOCK, &set, NULL); //Unblock SIGCHLD
            
            //execve function loads and runs a new program in the context of the current process. 
            //By convention, argv[0] is the name of the executable object file.
            //argv is the pointer to the first command in the arguments list.
            //environ is the environment, an array of strings.
            //upon failure, control returns to caller of execve() which handles the error. Normally, execve does not return unless there is an error, upon which the shell exits.
            
            if (execve(argv[0], argv, environ) < 0) { //load and run program. exeve() returns -1 if command is not found. argv[0] 
                printf("%s: Command not found\n", argv[0]);
                exit(0);
            }
        }
        
        //Parent waits in for foreground job to terminate
        if (!bg) { //Foreground
            sigprocmask(SIG_UNBLOCK, &set, NULL); //Unblocks SIGCHLD signal
            addjob(jobs, pid, FG, cmdline); //Add this process to the fg job list
            waitfg(pid); //Parent waits for foreground job to terminate
        } else { //Background
            sigprocmask(SIG_UNBLOCK, &set, NULL); //Unblocks SIGCHLD signal
            addjob(jobs, pid, BG, cmdline); //Add this process to the bg job list
            printf("[%d] (%d) %s", pid2jid(pid), (int)pid, cmdline); //Print bg process info
        }
    }

    return;
}






/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char**argv){
    char *delim; /* Points to first space delimiter */
    int argc; /* number of args */
    int bg; /*background job?*/
    
    buf[strlen(buf)-1] = ' '; //replace trailing '\n' with space
    while(*buf && (*buf == ' ')) //ignore leading spaces
        buf++;
    
    //build the argv list
    argc = 0;
    while ((delim = strchr(buf, ' '))){
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while(*buf && (*buf == ' ')) //ignore spaces
            buf++;
    }
    argv[argc] = NULL;
    
    if(argc == 0) //ignore blank lines
        return 1;
    
    //should the job run in the background?
    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;
    
    return bg;
}




/////////////////////////////////////////////////////////////////////////////
//
// builtin_cmd - If the user has typed a built-in command then execute
// it immediately. The command name would be in argv[0] and
// is a C string. We've cast this to a C++ string type to simplify
// string comparisons; however, the do_bgfg routine will need 
// to use the argv array as well to look for a job number.
//
int builtin_cmd(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "jobs")){ //list the running and stopped background jobs
        listjobs(jobs);
        return 1;
    }
    if (!strcmp(argv[0], "bg")){ //change a stopped BG job to a running BG job
        do_bgfg(argv);
        return 1;
    }
    if (!strcmp(argv[0], "fg")){ //change a stopped or running BG job to a running BG job
        do_bgfg(argv);
        return 1;
    }
    if (!strcmp(argv[0], "&")){ //ignore singleton, do nothing
        return 1;
    }
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
  struct job_t *jobp=NULL;
    
  /* Ignore command if no argument */
  if (argv[1] == NULL) {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }
    
  /* Parse the required PID or %JID arg */
  if (isdigit(argv[1][0])) {
    pid_t pid = atoi(argv[1]);
    if (!(jobp = getjobpid(jobs, pid))) {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%') {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid))) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }	    
  else {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.
  //
  string cmd(argv[0]);

    if(cmd == "bg") { //if background
        jobp->state = BG; //change job state to BG
        printf("[%d] (%d) %s", jobp->jid, jobp->pid, jobp->cmdline); //print status of job
        kill(-jobp->pid, SIGCONT); //Send SIGCONT signal to entire group (hence the - sign)
    } else {
        jobp->state = FG; //change job state to FG
        kill(-jobp->pid, SIGCONT); //Send SIGCONT signal to entire group (hence the - sign)
        waitfg(jobp->pid); //wait for the foreground child process
    }
    
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid)
{
    while (pid == fgpid(jobs)) { //while process is in the foreground, sleep
        sleep(1);
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// Signal handlers
//


/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.  
//
void sigchld_handler(int sig) 
{
    //SIGCHLD sent to shell whenever a child job teerminates or stops. This handler reaps all zombie children it can. It does not wait for any other running children to terminate.
    pid_t pid;
    int status;
    int jobid;
    
    //Detect any terminated or stopped jobs (Do not wait for any other running children to terminate)
    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0){
        jobid = pid2jid(pid);
        
        if (WIFEXITED(status)) { //Terminated normally
            deletejob(jobs, pid); 
        }
        
        else if (WIFSIGNALED(status)) { //Terminated by the receipt of an uncaught signal. WIFSIGNALED() returns true if the child process is terminated by a signal that was not caught. This is for SIGINT (CTRL-C)
            printf("Job [%d] (%d) terminated by signal %d\n", jobid, (int) pid, WTERMSIG(status));
            deletejob(jobs,pid); 
        }
        
        else if (WIFSTOPPED(status)) { //Terminated by receipt of a signal. WIFSTOPPED() returns true if the child that caused the return is currently stopped. CTRL-Z sends a SIGTSP
            printf("Job [%d] (%d) stopped by signal %d\n", jobid, (int) pid, WSTOPSIG(status));
            getjobpid(jobs, pid)->state = ST;  //change job state to ST
        }
        
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//    user types ctrl-c at the keyboard.  Catch it and send it along
//    to the foreground job.  
//
void sigint_handler(int sig) 
{
    pid_t pid = fgpid(jobs); //current foreground job
    if (pid != 0) {
        kill(-pid, sig); //send SIGINT to entire fg group with - sign
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP.  
//

//CTRL+Z in the shell stops the foreground processes, but does not terminate them, via SIGSTP. User is taken back to shell, where they can say bg to put the stopped process in the background or fg to continue it in the foreground.
void sigtstp_handler(int sig) 
{
    pid_t pid = fgpid(jobs); //current foreground job 
    if (pid != 0) {
        kill(-pid, sig); //send SIGSTP to entire fg group with - sign
    }
    return;
}

/*********************
 * End signal handlers
 *********************/