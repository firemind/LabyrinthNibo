#ifndef MYLOG_H
#define MYLOG_H

int log_line = 0;

void mylog(const char *text){
  gfx_move(0, log_line);
  log_line += 7;
  if (log_line > 50) {
    log_line = 0;
  }
  gfx_print_text(text);
}

#endif /*MYLOG_H*/
