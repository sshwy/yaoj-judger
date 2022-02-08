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

struct rsclim_ctxt rctxt = {
    .time = 1500,
    .virtual_memory = 30 * MB,
    .actual_memory = 3 * MB,
    .stack_memory = 2 * MB,
    .output_size = 1 * KB,
};

char *parse_argv[500], _parsed_argv[500][500];
char policy[500];
char policy_dir[500];
int parsed_argc = 0;
int result_code = -1;

int parse_opt(int key, char *arg, struct argp_state *state) {
  switch (key) {
  case 't':
    rctxt.time = atoi(arg);
    break;
  case 'm':
    rctxt.actual_memory = rctxt.stack_memory = rctxt.virtual_memory =
        atoi(arg) * MB;
    break;
  case 771:
    rctxt.virtual_memory = atoi(arg) * MB;
    break;
  case 772:
    rctxt.actual_memory = atoi(arg) * MB;
    break;
  case 773:
    rctxt.stack_memory = atoi(arg) * MB;
    break;
  case 774:
    set_logfile(arg);
  case 'g':
    rctxt.output_size = atoi(arg) * MB;
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
    if (log_fp == NULL) {
      argp_error(state, "log file didn't specified.");
    }
    break;
  }
  return 0;
}

int main(int argc, char **argv, char **env) {
  struct argp_option options[] = {
      {"timeout", 't', "TIMEOUT", 0, "specify the time limit in milliseconds"},
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

  struct policy_ctxt pctxt = create_policy_ctxt(policy_dir, policy);

  struct runner_ctxt ectxt = {
      .argc = parsed_argc, .argv = parse_argv, .env = env};
  struct hook_ctxt hctxt = create_hook_ctxt();

  struct perform_ctxt ctxt = {
      .ectxt = &ectxt,
      .pctxt = &pctxt,
      .rctxt = &rctxt,
      .hctxt = &hctxt,
  };

  perform(&ctxt);

  fprint_result(log_fp, &ctxt.result);

  ASSERT((int)ctxt.result.code == result_code,
         "test failed (result=%d, expect=%d)\n", (int)ctxt.result.code,
         result_code);
  fclose(log_fp);
  return 0;
}