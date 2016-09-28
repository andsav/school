/*
 * A very simple program to submit a file - use at your own risk ;)
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <regex.h>

#define SUBMIT_DIRECTORY "/usr/share/submit"
#define LOG_FILE "submit.log"
#define MESSAGE_LEN 512

typedef struct {
	unsigned int submitted;
	unsigned int version;
	unsigned int help;
	char * path;
	char * message;
} submit_args;

//Prints version
static
void print_version(char* cmd) 
{
	char output[128];
	char buffer[128];
	int cmd_address = (int) cmd;
	
	snprintf(buffer, sizeof(buffer), "Submission program (%x) version 0.1 (%s)\n", cmd_address, cmd);
	sprintf(output, buffer);
	printf(output);
}

static
int run_cmd(char * cmd, ... ) {
	va_list vl;
	char * arg[8];
	pid_t pid;
	int i, status;

	i = 1;
	va_start(vl, cmd);
	while(i < 7 && (arg[i++] = va_arg(vl, char*)));
	va_end(vl);
	arg[7] = NULL;
	arg[0] = rindex(cmd, '/');
	if (arg[0] == NULL)
		arg[0] = cmd;
	else
		arg[0]++;

	pid = fork();
	// error
	if (pid < 0) {
		fprintf(stderr, "Fork failed\n");
		return 1;
	}
	if (pid > 0) {
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) == 0 || WEXITSTATUS(status) < 0)
			return 1;
	}
	else {
		execvp(cmd, arg);
		return 1;
	}
	return 0;
}

static
int copy_file(const char* src_name, const char* dst_name)
{
	char buf[2048], *p1, *p2;
	FILE *src_file, *dst_file;
	int c;
	if (link(src_name, dst_name) == 0) {
		unlink(src_name);
		return 0;
	} else {
		src_file = fopen(src_name, "r");
		if (src_file == NULL) {
			fprintf(stderr, "Failed to open source file: %s\n",
				src_name);
			return 1;
		}
		p1 = buf;
		while ((c = fgetc(src_file)) != EOF) {
			*p1 = c;
			p1++;
		}

		fclose(src_file);

		dst_file = fopen(dst_name, "w");
		if (dst_file == NULL) {
			fprintf(stderr, "Failed to open destination file: %s\n",
				dst_name);
			return 1;
		}
		p2 = buf;
		while (p2 != p1) {
			fputc(*p2, dst_file);
			p2++;
		}

		fclose(dst_file);
	}
	return 0;
}

static
int dir_exists(char *dir) {
	struct stat s;
	int err = stat(dir, &s);
	if(err == -1 && errno == ENOENT) {
		return 0;
	} else {
		if(S_ISDIR(s.st_mode)) {
			return 1;
		} else {
			unlink(dir);
			return 0;
		}
	}
	return 0;
}
	
static
char * get_submit_dir() {
	char * subdir_name;
	uid_t uid;
	struct passwd *entry;

	uid = getuid();
	entry = getpwuid(uid);
	if (entry == NULL) {
		fprintf(stderr, "Failed to find pwd entry\n");
		return NULL;
	}

	subdir_name = malloc(strlen(SUBMIT_DIRECTORY) + 1
		+ strlen(entry->pw_name) + 1);
	if (subdir_name == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}
	
	strcpy(subdir_name, SUBMIT_DIRECTORY);
	strcat(subdir_name, "/");
	strcat(subdir_name, entry->pw_name);
	if (!dir_exists(subdir_name))
		run_cmd("mkdir", subdir_name, NULL);
	return subdir_name;
}

static
char * get_dst_name(char * src_name) {
	char * dst_name;
	char * subdir_name;

	subdir_name = get_submit_dir();

	dst_name = malloc(strlen(subdir_name) + 1 + strlen(src_name) + 1);
	if (dst_name == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}
	
	strcpy(dst_name, subdir_name);
	strcat(dst_name, "/");
	strcat(dst_name, src_name);
	free(subdir_name);

	return dst_name;
}

static
char * get_logfile_name() {
	uid_t userid;
	gid_t groupid;
	struct passwd *entry;
	char *pathname, *safepath, *ptr;
	struct stat buf;
	int fd;

	userid = getuid();
	groupid = getgid();
	entry = getpwuid(userid);
	if (entry == NULL) {
		fprintf(stderr, "Failed to find pwd entry\n");
		return NULL;
	}

	pathname = malloc(6 + strlen(entry->pw_name) + 1
		+ strlen(LOG_FILE) + 1);
	if (pathname == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	safepath = malloc(6 + strlen(entry->pw_name) + 1);
	if (safepath == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return NULL;
	}

	strcpy(pathname, "/home/");
	strcat(pathname, entry->pw_name);
	strcpy(safepath, pathname);
	strcat(pathname, "/");
	strcat(pathname, LOG_FILE);		

	ptr = realpath(pathname, NULL);
	if (ptr == NULL) {		
		fd = creat(pathname, S_IWUSR | S_IRUSR);
		if (fd < 0) {
			fprintf(stderr, "Failed to create log file: %s\n",
				pathname);
			return NULL;
		}
		if (fchown(fd, userid, groupid) < 0) {
			fprintf(stderr,
				"Failed to change ownership of log file: %s\n", 
				pathname);
			return NULL;
		}
	}
	else {
		if (strncmp(ptr, safepath, strlen(safepath))) {
			fprintf(stderr, "Invalid log file: %s\n", ptr);
			return NULL;
		}
	
		if (stat(pathname, &buf) != 0) {
			fprintf(stderr, "Failed to stat\n");
			return NULL;
		}
		
		if (buf.st_uid != userid) {
			fprintf(stderr, "Not your log file: %s\n", pathname);
			return NULL;
		}
	}

	return pathname;
}

//Checks for substrings in the program that have been connected to troublesome submissions
static
int check_for_viruses(char * filename) {
	char buf[1024], *p1;
	FILE *src_file;
	int c, ctr;
	regex_t regex;
	int reti;

	printf("Checking %s for viruses...\n", filename);
	
	src_file = fopen(filename, "r");
	if (src_file == NULL) {
		fprintf(stderr, "Failed to open source file: %s\n",
			filename);
		return 1;
	}
	while (c != EOF) {
		p1 = buf;
		ctr = 0;
		while(((c = fgetc(src_file)) != EOF) && (ctr < 1024)){
			*p1 = c;
			p1++;
			ctr++;
		}
	
		if(regcomp(&regex, "bin/sh", 0)){
			fprintf(stderr, "Could not compile known virus signatures\n");
			return 1;
		}

		reti = regexec(&regex, buf, 0, NULL, 0);
		if(!reti){
			printf("Alert! Detected possibly malicious submission! Terminating.\n");
			return  1;
		}
	}
		
	printf("No viruses found! :)\n");

	return 0;
}

static
int log_message(submit_args args, char* logfile_name) {
	FILE * logfile;

	if (args.message == NULL)
		args.message = "n/a";

	if ((logfile = fopen(logfile_name, "w")) == NULL)
		return 1;

	fputs(args.message, logfile);
	fclose(logfile);
	return 0;
}

//Parses arguments from commandline
//Returns struct with the appropriate flags set
static
submit_args parse_args(int argc, char* argv[]) {
	int c, opts;
	submit_args args;

	struct option long_options[] = {
		{"submitted", 0, NULL, 's'},
		{"version", 0, NULL, 'v'},
		{"help", 0, NULL, 'h'},
		{0, 0, 0, 0}
	};

	memset(&args, 0, sizeof(submit_args));

	c = 0;
	opts = 0;

	while (1) {
		c = getopt_long(argc, argv, "svh", long_options, NULL);
		if (c == -1) break;

		switch (c) {
			case 's':
				opts = 1;
				args.submitted = 1;
				break;
			case 'v':
				opts = 1;
				args.version = 1;
				break;
			default:
				opts = 1;
				args.help = 1;
				break;
		}
		if (args.help) break;
	}
	
	if (!opts && argc >= 2) {
		args.path = argv[1];
		if (argc >= 3) {
			args.message = argv[2];
		}
	} else if (argc < 2)
		args.help = 1;

	return args;
}

//Prints usage
static
void print_usage(char* cmd) 
{
	char txt[420+1];
	char txt2[128];
	int i;

	snprintf(txt, 640, "Syntax:\n\t%s <path> [log message]\n"
		"-s, --submitted	Show your submitted files\n"
		"-v, --version		Show version\n"
		"-h, --help		Show this usage message\n", cmd);

	for (i = 0; i < 3; i++) {
		snprintf(txt2, 128, "Exiting program... (%d)", i);
	}

	printf("%s\n", txt);
	printf("%s\n", txt2);
}

static
int show_confirmation() {
	char *subdir_name;
	subdir_name = get_submit_dir();
	run_cmd("/bin/ls", "-la", subdir_name, NULL);
	return 0;
}

static
int check_forbidden(char * source) {
	char * forbidden_char;
	forbidden_char = (char*) malloc(sizeof(char));
	if (forbidden_char == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		return 1;
	}

	forbidden_char[0] = '/';

	printf("Checking source: ");
	printf(source);
	printf("\n");

	if (index(source, (int)(forbidden_char[0])) != NULL) {
		fprintf(stderr, "File name includes slash\n");
		return 1;
	}

	free(forbidden_char);
	return 0;
}

int main(int argc, char* argv[])
{
	char *dst, *logfile_name;
	int safe;
	submit_args args;

	//parse arguments
	args = parse_args(argc, argv);
	if (args.help) {
		print_usage(argv[0]);
		return 0;
	}

	if (args.version) {
		print_version(argv[0]);
		return 0;
	}

	if (args.submitted) {
		show_confirmation();
		return 0;
	}

	//check to make sure the source directory doesn't contain invalid
	//characters
	if (check_forbidden(args.path))
		return 1;

	//get logfile name
	logfile_name = get_logfile_name();
	if (logfile_name == NULL)
		return 1;
	
	//check for viruses
	safe = 1;
	if (check_for_viruses(args.path))
		safe = 0;
		
	//log submission
	if (log_message(args, logfile_name))
		return 1;

	//copy the file
	dst = get_dst_name(args.path);
	if (safe) {
		if(copy_file(args.path, dst))
			return 1;
	}
	free(dst);

	//show confirmation
	show_confirmation();

	return 0;
}

