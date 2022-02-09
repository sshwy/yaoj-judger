#include <argp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "judger.h"

perform_ctxt_t cctxt;

char *parse_argv[100], _parsed_argv[100][100];
int parsed_argc = 0;
int result_code = -1;

int parse_opt(int key, char *arg, struct argp_state *state) {
  switch (key) {
  case 't':
    perform_set_limit(cctxt, REAL_TIME, atoi(arg));
    break;
  case 'm':
    perform_set_limit(cctxt, MEM, atoi(arg) * MB);
    break;
  case 771:
    perform_set_limit(cctxt, VIR_MEM, atoi(arg) * MB);
    break;
  case 772:
    perform_set_limit(cctxt, ACT_MEM, atoi(arg) * MB);
    break;
  case 773:
    perform_set_limit(cctxt, STK_MEM, atoi(arg) * MB);
    break;
  case 'g':
    perform_set_limit(cctxt, OUT, atoi(arg) * MB);
    break;
  case 'r':
    result_code = atoi(arg);
    break;
  case ARGP_KEY_ARG:
    // printf("%s\n", arg);
    strcpy(_parsed_argv[parsed_argc], arg);
    parse_argv[parsed_argc] = _parsed_argv[parsed_argc];
    ++parsed_argc;
    break;
  case ARGP_KEY_END:
    if (result_code == -1) {
      argp_error(state, "result_code didn't specified.");
    }
    break;
  }
  return 0;
}

int main(int argc, char **argv, char **env) {
  cctxt = perform_ctxt_create();

  struct argp_option options[] = {
      {"timeout", 't', "TIMEOUT", 0, "specify the time limit in milliseconds"},
      {"memory", 'm', "MEMORY", 0, "specify all three memory limits in MB"},
      {"virtualmem", 771, "VIRTUAL_MEMORY", 0,
       "specify the virtual memory limit in MB"},
      {"actualmem", 772, "ACTUAL_MEMORY", 0,
       "specify the actual memory limit in MB"},
      {"stackmem", 773, "STACK_MEMORY", 0,
       "specify the stack memory limit in MB"},
      {"output", 'g', "OUTPUT", 0, "specify the output limit in MB"},
      {"result", 'r', "RESULT", 0, "specify the result code"},
      {0},
  };
  struct argp argp = {
      options,
      parse_opt,
      "exec input output errput",
  };
  argp_parse(&argp, argc, argv, 0, 0, 0);

  parse_argv[parsed_argc] = NULL;
  // for (int i = 0; i < parsed_argc; i++) printf(": %s\n", parse_argv[i]);

  log_set(".log.local");

  // please replace the first argument with yours <yaoj-judger>/policy (absolue
  // path is recommended)
  char pdir[] = "/home/sshwy/桌面/yaoj-judger/policy", po[] = "c_std_io";
  perform_set_policy(cctxt, pdir, po);
  perform_set_runner(cctxt, parsed_argc, parse_argv, env);

  perform(cctxt);

  log_print_result(&cctxt->result);

  log_close();
  if ((int)cctxt->result.code != result_code) {
    fprintf(stderr, "test failed (result=%d, expect=%d)\n",
            (int)cctxt->result.code, result_code);
  }
  return 0;
}