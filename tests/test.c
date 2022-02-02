#include <argp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../src/judger.h"

struct rsclim_ctxt rctxt = {
    .time = 1500,
    .virtual_memory = 30 * MB,
    .actual_memory = 3 * MB,
    .stack_memory = 2 * MB,
    .output_size = 1 * KB,
};

char *parse_argv[100], _parsed_argv[100][100];
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
  case 'g':
    rctxt.output_size = atoi(arg) * MB;
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
    if (parsed_argc != 4) {
      argp_error(state, "argc = %d != 4", parsed_argc);
    }
    if (result_code == -1) {
      argp_error(state, "result_code didn't specified.");
    }
    break;
  }
  return 0;
}

int main(int argc, char **argv, char **env) {
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

  log_fp = fopen(".log.local", "w");
  if (log_fp == NULL) {
    ERRNO_EXIT(-1, "log_fp == NULL");
  }

  // please replace the first argument with yours <yaoj-judger>/policy (absolue
  // path is recommended)
  struct policy_ctxt pctxt =
      create_policy_ctxt("/home/sshwy/桌面/yaoj-judger/policy", "policy");

  struct runner_ctxt ectxt = {
      .argc = parsed_argc, .argv = parse_argv, .env = env};
  struct hook_ctxt hctxt = create_hook_ctxt();

  struct perform_ctxt ctxt;

  perform(&ctxt, pctxt, rctxt, ectxt, hctxt);

  fprint_result(log_fp, &ctxt.result);

  fclose(log_fp);
  ASSERT(ctxt.result.code == result_code,
         "test failed (result=%d, expect=%d)\n", ctxt.result.code, result_code);
  return 0;
}