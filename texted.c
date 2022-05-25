#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct terminos orig_terminos;

void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termonis);
}

void enableRawMode(){
  tcgetattr(STDIN_FILENO, &orig_terminos);
  atexit(disableRawMode);
  
  struct terminos raw = orig_terminos;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |=  (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char *argv[]){
  enableRawMode();
  char c = '\0';
  read(STDIN_FILENO, &c, 1);
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
	printf("%d('%c')\r\n", c, c);
    }
  }
  return 0;
}
