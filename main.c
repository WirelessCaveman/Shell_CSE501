/******************************************************************************
 *
 *  File Name........: main.c
 *
 *  Description......: Simple driver program for ush's parser
 *
 *  Author...........: Vincent W. Freeh
 *
 *  $Id: main.c,v 1.2 2004/11/05 12:10:49 vin Exp $
 *
 *****************************************************************************/

//general references: 
//http://www.unixwiz.net/techtips/remap-pipe-fds.html
 
#include <stdio.h>
#include "parse.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#define STDIN           0
#define STDOUT          1
#define STDERR          2
#define LEN							4096
#define FMODE						0666

// #define echh
// #define pwdd
// #define cddd
// #define sett
// #define uset
// #define quitt
// #define dirr
// #define prPipe_piped
// #define prPipe_check
// #define process_cmdd
// #define prCmdd

extern char **environ;
int ush_pid;
int ushrc_flag = 0, endflag = 0;


void process_cmd (Cmd c, int in_file, int out_file, int closefile, int errorfile)		//System calls syntaxes drawn from http://www.opengroup.org
{
	char *curr_dir, *dst_dir, *env, *env_val, *cmd_str, **env_ptr;
	int iter, iter2;
	//struct pid_t pid;
	int pid = 1, last = 0;
	int stdin_cpy, stdout_cpy, stderr_cpy;
	int nice_inc = 4;
	
										#ifdef process_cmdd
											printf("entering process_cmd\n");
										#endif
	
	//if(c->next != NULL || c->out != Tnil || c->in == Tin)
	//{
		stdin_cpy = dup(STDIN);
    stdout_cpy = dup(STDOUT);
    stderr_cpy = dup(STDERR);
    
    					#ifdef process_cmdd
								printf("in_file: %d, out_file: %d, errorfile: %d \n", in_file, out_file, errorfile);
							#endif
    
 		dup2(in_file, STDIN);
        			#ifdef process_cmdd
								printf("in_file: %d, out_file: %d, errorfile: %d \n", in_file, out_file, errorfile);
								perror(c->infile);
							#endif
 		dup2(out_file, STDOUT);
        			#ifdef process_cmdd
        				perror(c->infile);
								printf("in_file: %d, out_file: %d, errorfile: %d \n", in_file, out_file, errorfile);
							#endif
 		dup2(errorfile, STDERR);
    
    					#ifdef process_cmdd
								printf("process_cmdd entertop: my pid parent: %d\n", getpid());
							#endif
							
  //}
    
			
							
			//return;	//the parent process will return after finishing the #ifdef after executing the command
//		}
// 		
//      if(c->in == Tpipe || c->in == TpipeErr)
//      	close(closefile);
//       if(c != NULL)
//   		{
// 				printf("%s\n", curr_dir);	
//         if(c->next->in == Tpipe || c->next->in == TpipeErr)
//         	close(closefile);
//   		}
	   	
	
	//if ((!(strcmp(c->args[0], "pwd")) || !(strcmp(c->args[0], "echo"))) && (c->next != NULL))
	//if ((!(strcmp(c->args[0], "pwd")) || !(strcmp(c->args[0], "echo"))))	//?????????********????????
	if (c->next != NULL || !( !(strcmp(c->args[0], "pwd")) || !(strcmp(c->args[0], "nice")) || !(strcmp(c->args[0], "where")) || !(strcmp(c->args[0], "cd")) || !(strcmp(c->args[0], "echo")) || !(strcmp(c->args[0], "setenv")) || !(strcmp(c->args[0], "unsetenv")) || !(strcmp(c->args[0], "quit")) || !(strcmp(c->args[0], "exit")) || !(strcmp(c->args[0], "logout"))))
	{
		pid = fork();		//you might want to say if (!(strcmp(c->args[0], "pwd")) || !(strcmp(c->args[0], "echo"))... till the close(out_file) statement
		
	}
	else
	{
		last = 1;
	}
	
	
	/*********************PWD*****************************/
	
		
//		pid = fork();
// 		if (pid != 0)
// 		{
// 							#ifdef pwdd	
// 								printf("my pid parent: %d\n", getpid());
// 							#endif
// 			wait();
// 			return;
// 		}
//		else if (pid == 0)	
	if (!(strcmp(c->args[0], "pwd")))		
	{
							#ifdef pwdd	
								printf("my pid parent: %d\n", getpid());
							#endif
		
		if (pid == 0 || last == 1)	
		{
			curr_dir = (char *)malloc(LEN * sizeof(char));
			curr_dir = (char *)getcwd(curr_dir, LEN);	
						#ifdef pwdd	
							printf("my pid child: %d\n", getpid());
						#endif
	
			printf("testing pid: %d; %s\n", pid, curr_dir);
			free(curr_dir);
			//return;
			if (last != 1)
			{
						#ifdef pwdd	
							printf("exiting at pwdd\n");
						#endif	
				exit (1);
			}
		}
						#ifdef pwdd	
							printf("return: my pid parent: %d\n", getpid());
						#endif
		//return;
	}

	  
	  							
	
	/********************ECHO******************************/
	else if(!(strcmp(c->args[0], "echo")))
	{
// 		pid = fork();
// 		if (pid != 0)
// 			wait();
		//else if (pid == 0)
		if (pid == 0 || last == 1)
		{
							#ifdef echh	
								printf("my pid child: %d\n", getpid());
							#endif
			iter2 = 1;
			env = (char *)malloc(LEN * sizeof(char));
			env_val = (char *)malloc(LEN * sizeof(char));
			while (c->args[iter2] != NULL)
			{
				memset(env, '\0', LEN * sizeof(char));
				iter = 0;
				if (c->args[iter2][0] == '$' && c->args[iter2][1] != '\0')
				{
					while(c->args[iter2][iter + 1] != '\0')
					{ 
							env[iter] = c->args[iter2][iter + 1];
							iter++;
					}
					printf("printing env: %s\n", env);
					env_val = (char *)getenv(env);
					if(env_val == NULL)
						printf("%s: Undefined variable.\n", env);
					else
						printf("%s\n", env_val);
					
				}
				else
				{
					//for (iter = 1; c->args[iter2][iter] != '\0'; iter++)
					printf ("%s\n", c->args[iter2]);
					printf("\n");
				}
				iter2++;
				
			}
			//printf("child exiting\n");
			if (last != 1)
			{
											#ifdef echh
												printf("exiting at echh\n");
											#endif
				exit (1);
			}
		}
									#ifdef echh	
									printf("my pid parent: %d\n", getpid());
									#endif
		//return;
	}
	
	/********************CD********************************/
	else if(!strcmp(c->args[0], "cd"))
	{
		if (pid == 0 || last == 1)
		{
								#ifdef cddd
								printf("executing CD\n");
								#endif
		
			dst_dir = (char *)malloc(LEN * sizeof(char));
			if(c->args[1] == NULL)
				dst_dir = (char *)getenv("HOME");
			else if(strcmp(c->args[1], "/") == 0)
				//dst_dir = (c->args[1]);
				dst_dir = "/";
			else 
			{
				dst_dir = (char *)getcwd(dst_dir, LEN);
				strcat(dst_dir, "/");
				strcat(dst_dir, c->args[1]);		
			}
			if(chdir(dst_dir) < 0)
				printf("%s: No such file or directory.\n", dst_dir);
			free (dst_dir);
			
			if (last != 1)
			{
											#ifdef cddd
												printf("exiting at cddd\n");
											#endif
				exit (1);
			}
		}
		//return;
	}
	
	/*******************SETENV*****************************/
	else if(strcmp(c->args[0], "setenv") == 0)
	{
		if (pid == 0 || last == 1)
		{
									#ifdef sett
										printf("executing setenv\n");
									#endif		
			cmd_str = (char *)malloc(LEN * sizeof(char));
			env_ptr = environ;
			if (c->args[1] == NULL)
			{
				while (*env_ptr != NULL)
				{
					printf("%s\n", *env_ptr);
					env_ptr++;
				}
			}
			else if (c->args[1] != NULL)
			{
				if (c->args[2] == NULL)
					c->args[2] = "";
				sprintf(cmd_str, "%s= %s", c->args[1], c->args[2]);
				//printf("printing sprintf op: %s\n", cmd_str);
				if (0 != putenv(cmd_str))
					printf("SETENV unsuccessfull\n");
			}
			if (last != 1)
			{
											#ifdef sett
												printf("exiting at sett\n");
											#endif
				exit;
			}
		}
		//return;
	}
	/*******************UNSETENV*****************************/
	else if(strcmp(c->args[0], "unsetenv") == 0)
	{
		if (pid == 0 || last == 1)
		{
									#ifdef uset
										printf("executing setenv\n");
									#endif	
			iter = 1;
			while (c->args[iter] !=  NULL)
			{
				if (unsetenv(c->args[iter]) == -1)
					printf("UNSETENV unsuccessfull\n");	//if unsetenv fails there is no display
				iter++;
			}
			if (last != 1)
			{
											#ifdef uset
												printf("exiting at uset\n");
											#endif
				exit;
			}
		}
		//return;
	}
	
// 	/*******************NICE***********************************/
// 	else if( !strcmp( c->args[0],"nice" ) )
//   {
// 		nice_inc = 4;
// 		if (pid == 0 || last == 1)
// 		{
// 									#ifdef uset
// 										printf("executing setenv\n");
// 									#endif
//                         if( c != NULL )
//                         {
// 				if( c->args[1] == NULL )
// 					nice( nice_inc );
// 				else if( nice_inc = atoi( c->args[1] ) )
// 				{
// 					if( ( nice_inc > 19 ) || ( nice_inc < ( -20 ) ) )
// 						printf( "Invalid range \n" );
// 					else
// 						nice( nice_inc );
// 				}
//                                 if( c->next->in == Tpipe || c->next->in == TpipeErr )
//                                         close( file_close );
//                         }
//                         exit(0);
//                 }
//                 else
//                 {
//                         if( c->in == Tpipe || c->in == TpipeErr )
//                                 close( file_in );
//                         if( c->next != NULL )
//                         {
//                                  if( c->next->in == Tpipe || c->next->in == TpipeErr )
//                                  close( file_out );
//                         }
//                          wait( chldstatus );
//                 }
//          }
//          else
// 	 {
// 		if( c->args[1] == NULL )
//                          nice( nice_inc );
//                 else if( nice_inc = atoi( c->args[1] ) )
//                 {
//                           if( ( nice_inc > 19 ) || ( nice_inc < ( -20 ) ) )
//                           	printf( "Invalid range \n" );
//                           else
//                                 nice( nice_inc );
//                 }
// 	 }
//          return ;
//   } 
	
	/*******************QUIT*********************************/
	else if((!strcmp(c->args[0], "quit")) || (!strcmp(c->args[0], "exit")) || (!strcmp(c->args[0], "logout"))) 
	{
									#ifdef quitt
										printf("closing shell\n");
									#endif
		if(ushrc_flag == 1)
		{
			//printf("endflag = 1\n");
			endflag = 1;
		}
		else exit (1);
	}
	/********************DIRECT-NOT INBUILT******************/
	else
	{ 
// 		pid = fork();
// 		if (pid != 0)
// 			wait();
		if (pid == 0)
		{
									#ifdef dirr	
										printf("dirr my pid child: %d\n", getpid());
									#endif
			
// 			dup2(in_file, STDIN);
// 			dup2(out_file, STDOUT);
// 			dup2(errorfile, STDERR);
		
// 			if(c->in == Tpipe || c->in == TpipeErr)
// 				close(closefile);       
			if(c != NULL)
      {
        execvp(c->args[0], &c->args[0]);
// 				if(c->next->in == Tpipe || c->next->in == TpipeErr)
//           close(closefile);
      }
//			execvp(c->args[0], &c->args[0]);
											#ifdef dirr
												printf("dirr: have executed execvp\n");
											#endif
			//if (last != 1)
			//{
											#ifdef dirr
												printf("exiting at dirr\n");
											#endif
				
	  		exit (1);
  		//}
	  }
	  							#ifdef dirr	
										printf("my pid parent: %d\n", getpid());
									#endif
		//return;
	}
	
	/***********Main shell closes everything and returns*********/
	if (pid != 0 || last == 1)
	{
							#ifdef process_cmdd	
								printf("process_cmdd enter: my pid parent: %d; last = %d\n", getpid(), last);
							#endif
			
			//wait();	//what was that about wait not being inside the while loop since the pipe buffer that the forked process uses to pass data back to the main shell may fill up
// 							#ifdef pwdd	
// 								printf("parent came out of wait: %d\n", getpid());
// 							#endif
			
// 			if(c->in == Tpipe || c->in == TpipeErr)
// 		  	close(in_file);
//   		if(c->next != NULL)
//   		{
// 		 		if(c->next->in == Tpipe || c->next->in == TpipeErr)
//       		close(out_file);
//   		}
				
//  		return;
				dup2(stdin_cpy, STDIN);
				dup2(stdout_cpy, STDOUT);
				dup2(stderr_cpy, STDERR);
				
				return;
	}
	
	
		
}

static void prCmd(Cmd c, int in_file, int out_file, int closefile, int errorfile)	//discussed the flag modes for "open()" with Aubin Sanyal
{
  int i;
	
  if (c) 
  {
    printf("%s%s ", c->exec == Tamp ? "BG " : "", c->args[0]);	//t&: background
    											#ifdef prCmdd
														printf("prCmdd: c != 0 or NULL\n");
													#endif
    
    if (c->in == Tin)	//input from input file	
    {
	    printf("<(%s) ", c->infile);
	    in_file = open(c->infile, O_CREAT|O_RDWR, FMODE);
			if(in_file < 0) printf("in_file error\n ");
    }
    
    											#ifdef prCmdd
														printf("c->out = Tnil--?\n");
													#endif
    if (c->out != Tnil)
    {
	    										#ifdef prCmdd
														printf("c->out != Tnil\n");
													#endif
      switch (c->out) 
      {
	      case Tout:
					printf(">(%s) ", c->outfile);	//o/p to o/p file
					out_file = open(c->outfile, O_CREAT|O_WRONLY, FMODE);
					if(out_file < 0) printf("out_file error \n");	
													#ifdef prCmdd
														printf("c->out = Tout\n");
													#endif
					break;
					
	      case Tapp:
					printf(">>(%s) ", c->outfile);	//append
					out_file = open(c->outfile, O_APPEND|O_WRONLY, FMODE);
													#ifdef prCmdd
														printf("c->out = Tapp\n");
													#endif
					break;
					
	      case ToutErr:
					printf(">&(%s) ", c->outfile);	//error
					errorfile = open(c->outfile, O_CREAT|O_WRONLY, FMODE);
					if(errorfile < 0) printf("errorfile error \n");	
					out_file = errorfile;	//needs to be in both error and out file
													#ifdef prCmdd
														printf("c->out = ToutErr\n");
													#endif					
					break;
					
	      case TappErr:
					printf(">>&(%s) ", c->outfile);
					errorfile = open(c->outfile, O_APPEND|O_WRONLY, FMODE);
					out_file = errorfile;	//needs to be in both error and out file
													#ifdef prCmdd
														printf("c->out = TappErr\n");
													#endif					
					break;
					
	      default:
					fprintf(stderr, "Shouldn't get here\n");
					exit(-1);
      }
    }

    process_cmd(c, in_file, out_file, closefile, errorfile); 
   											#ifdef prCmdd
      											printf("prCmdd: executed process_cmd\n");
      										#endif
       
    if (c->nargs > 1) 
    {
      printf("[");
      for (i = 1; c->args[i] != NULL; i++)
				printf("%d:%s,", i, c->args[i]);
      printf("\b]");
    }
    putchar('\n');
    // this driver understands one command
    if (!strcmp(c->args[0], "end"))
    {
      										#ifdef prCmdd
      											printf("prCmdd: exiting because strcmp(c->args[0], end) = 0\n");
      										#endif
	    exit(0);
    }
  }
}



static void prPipe_pipe(Cmd c)	//some code in this function is a modification of examples in the MAN pages 
{
  int fildes[2];	
  int in_file = STDIN, out_file = STDOUT, closefile, errorfile = STDERR;
  static int ftemp1, ftemp2;

 	//the first iteration will not enter this block
	if(c->in == Tpipe || c->in == TpipeErr)
  {
							    #ifdef prPipe_piped
							    	printf("c->in == Tpipe/TpipeErr\n");
							    #endif
  	in_file = ftemp1;
    out_file = STDOUT; //remember to overwrite below if the c->next->in is a pipe********
  }
  if(c->in == TpipeErr)
  {
		errorfile = ftemp1;
  }
  //end of first iteration block

  if(c->next != NULL)	//executed if there are pipes after this command
	{
		if(c->next->in == Tpipe || c->next->in == TpipeErr)
		{
							    #ifdef prPipe_piped
							    	printf("c->in == TpipeErr/Tpipe\n");
							    #endif
			if(pipe(fildes) < 0)
			{
				printf("Fatal error- can not read pipe. Exiting\n");
				exit (-1);
			}
			ftemp1 = fildes[0];	//read end: input to the next command
			closefile = fildes[0];	//input to the next command
			ftemp2 = fildes[1];	//output of the present command
			out_file = fildes[1];	//output of the present command
		}		
		if(c->next->in == TpipeErr)
		{
			errorfile = fildes[1];
		}
	}
	prCmd(c, in_file, out_file, closefile, errorfile);
  								#ifdef prPipe_piped
							    	printf("prPipe_piped: executed prCmd\n");
							    #endif
							    
  if(in_file != STDIN)
  {
	  close(in_file);
  }
  if(out_file != STDOUT)
  {
	  close(out_file);
  }
  if(errorfile != STDERR)
  {
	  close(errorfile);
  }
//  wait(&in_file);
   
}




static void prPipe(Pipe p)
{
  int icmd = 0;
  Cmd c;

  if (p == NULL)
  {
	  endflag = 1;
    return;
  }
    

  printf("Begin pipe%s\n", p->type == Pout ? "" : " Error");
  for (c = p->head; c != NULL; c = c->next) 
  {
    printf("  Cmd #%d: ", ++icmd);
    
    prPipe_pipe(c);
  }
  printf("End pipe\n");
  while (wait() != -1);
														  #ifdef prPipe_check
															  if (p->next == NULL) printf("here1 prPipe\n");
															  printf("here2 prPipe\n");
														  #endif
					
  prPipe(p->next);
}

void shandle(int sig_num)	//this function is the slightly modified version of code found at www.gnu.org
{
	if(sig_num == SIGUSR1)
	{
		if(getpid() != ush_pid) exit(0);	//one of the forked processes
	}
	if(sig_num == SIGINT)
		kill(0, SIGUSR1);
}	

void signalling()			//this function is the slightly modified version of code found at www.gnu.org
{
	signal(SIGINT, shandle);
	signal(SIGUSR1, shandle);
  signal(SIGTERM, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGABRT, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);
}

void filing()
{
// 	int fpshell, fpstdin ;
// 	char *filepath, *shellpath;

// 	Pipe p;

// 	filepath = (char *)malloc(LEN * sizeof(char));
// 	shellpath = (char *)malloc(LEN * sizeof(char)); 

// 	shellpath = getenv("HOME");

// 	strcpy(filepath, shellpath);
// 	strcat(filepath, "/.ushrc");

// 	fpshell = open(filepath, O_RDONLY);
// 	
// 	if(fpshell > 0)
// 	{
// 		endflag = 0;
// 		fpstdin = dup(STDIN);
// 		dup2(fpshell, STDIN);
// 		ushrc_flag = 1;
// 		
// 		do
// 		{
// 			// printf(" before endflag - %d \n", endflag);
// 			p = parse();
// 		       	prPipe(p);

// 			// printf(" after endflag - %d \n", endflag);
// 	//		freePipe(p);
// 		}while(!endflag);
// 		
// 		freePipe(p);
// 		ushrc_flag = 0;
// 		endflag = 0;
// 		close(fpshell);
// 		dup2(fpstdin, STDIN);
// 		//close(STDIN);
// 	}
// 	
// 	free(shellpath);
// 	free(filepath);
// 	// printf(" back to main function \n");
}

int main(int argc, char *argv[])
{
  Pipe p;
  //char *host = "ush";
  char *host;
  host = (char *)malloc(LEN * sizeof(char));
  gethostname(host, 256);
  
  ush_pid = getpid();
  
  signalling();
  filing();
  while (1) 
  {
    printf("%s%% ", host);
    p = parse();
    prPipe(p);
    freePipe(p);
  }
}

/*........................ end of main.c ....................................*/
