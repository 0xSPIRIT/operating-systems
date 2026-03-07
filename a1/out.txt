#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Incorrect usage.\n  Usage: %s <input-file> <output-file>\n", argv[0]);
    return 1;
  }

  const char *input_file  = argv[1];
  const char *output_file = argv[2];

  int p[2];
  pipe(p);

  char buffer[512];
  ssize_t bytes_read;

  pid_t pid = fork();

  if (pid == -1) {
    fprintf(stderr, "Error forking the process! Exiting...\n");
    return 1;
  }

  if (!pid) {
    // Child process
    close(p[1]);

    // Opens output_file with write access;
    // if it doesn't exist, we create it and truncate the length to 0.
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
      fprintf(stderr, "Error opening the output file %s! Exiting...\n", output_file);
      return 1;
    }

    while ((bytes_read = read(p[0], buffer, sizeof(buffer))) > 0) {
      write(fd, buffer, bytes_read);
    }

    close(fd);
    close(p[0]);
  } else {
    // Parent process
    close(p[0]);

    int fd = open(input_file, O_RDONLY);

    if (fd == -1) {
      fprintf(stderr, "Couldn't find input file %s! Exiting...\n", input_file);
      return 1;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
      write(p[1], buffer, bytes_read);
    }

    close(fd);
    close(p[1]);

    wait(NULL); // Waits for child to finish
  }

  return 0;
}

