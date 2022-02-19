/**
 * @file main.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Command line interface for yaoj-judger.
 * @date 2022-02-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <argp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "judger.h"

perform_ctxt_t cctxt;

char *parse_argv[500], _parsed_argv[500][500];
char policy[500];
char policy_dir[500];
int parsed_argc = 0;
int result_code = -1;
int log_flag = 0;

int parse_opt(int key, char *arg, struct argp_state *state) {
  switch (key) {
  case 't':
    perform_set_limit(cctxt, TIME, atoi(arg));
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
  case 774:
    log_flag = 1;
    log_set(arg);
    break;
  case 775:
    perform_set_limit(cctxt, REAL_TIME, atoi(arg));
    break;
  case 776:
    perform_set_limit(cctxt, CPU_TIME, atoi(arg));
    break;
  case 'g':
    perform_set_limit(cctxt, OUT, atoi(arg) * MB);
    break;
  case 'r':
    result_code = atorc(arg);
    break;
  case 'p':
    strcpy(policy, arg);
    break;
  case 'P':
    strcpy(policy_dir, arg);
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
    if (log_flag == 0) {
      argp_error(state, "log file didn't specified.");
    }
    break;
  }
  return 0;
}

int main(int argc, char **argv, char **env) {
  cctxt = perform_ctxt_create();

  struct argp_option options[] = {
      {"timeout", 't', "TIMEOUT", 0,
       "specify both time limits in milliseconds"},
      {"realtime", 775, "REAL_TIME", 0,
       "specify the runtime limit in milliseconds"},
      {"cputime", 776, "CPU_TIME", 0, "specify the cpu limit in milliseconds"},
      {"memory", 'm', "MEMORY", 0, "specify all three memory limits in MB"},
      {"virmem", 771, "VIRTUAL_MEMORY", 0,
       "specify the virtual memory limit in MB"},
      {"actmem", 772, "ACTUAL_MEMORY", 0,
       "specify the actual memory limit in MB"},
      {"stkmem", 773, "STACK_MEMORY", 0,
       "specify the stack memory limit in MB"},
      {"fsize", 'g', "OUTPUT", 0, "specify the output limit in MB"},
      {"result", 'r', "RESULT", 0, "specify the result code using name"},
      {"log", 774, "LOG_FILE", 0, "specify judger result file"},
      {"policy", 'p', "POLICY", 0, "specify policy name"},
      {"policydir", 'P', "POLICY_DIR", 0, "specify policy search directory"},
      {0},
  };
  struct argp argp = {
      options,
      parse_opt,
      "runner arguments...",
      "Command line interface of judger (for the future yaoj)",
  };
  argp_parse(&argp, argc, argv, 0, 0, 0);

  parse_argv[parsed_argc] = NULL;
  // for (int i = 0; i < parsed_argc; i++) printf(": %s\n", parse_argv[i]);

  if (perform_set_policy(cctxt, policy_dir, policy)) {
    cctxt->result.code = SE;
  } else {
    perform_set_runner(cctxt, parsed_argc, parse_argv, env);

    if (perform(cctxt)) {
      cctxt->result.code = SE;
      fprintf(stderr, "SE: %s\n", ystrerr(yerrno));
    }
    log_print_result(&cctxt->result);
  }

  if ((int)cctxt->result.code != result_code) {
    fprintf(stderr, "test failed (result=%d, expect=%d)\n",
            (int)cctxt->result.code, result_code);
    return 1;
  }
  log_close();
  return 0;
}