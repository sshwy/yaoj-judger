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

#include "cli.h"
#include "judger.h"

int main(int argc, char **argv, char **env) {
  struct gengetopt_args_info args_info;
  int result_code;

  if (cmdline_parser(argc, argv, &args_info))
    exit(1);

  perform_ctxt_t cctxt = perform_ctxt_create();
  if (args_info.timeout_given)
    perform_set_limit(cctxt, TIME, args_info.timeout_arg);
  if (args_info.cputime_given)
    perform_set_limit(cctxt, CPU_TIME, args_info.cputime_arg);
  if (args_info.realtime_given)
    perform_set_limit(cctxt, REAL_TIME, args_info.realtime_arg);
  if (args_info.memory_given)
    perform_set_limit(cctxt, MEM, args_info.memory_arg * MB);
  if (args_info.virtual_memory_given)
    perform_set_limit(cctxt, VIR_MEM, args_info.virtual_memory_arg * MB);
  if (args_info.real_memory_given)
    perform_set_limit(cctxt, ACT_MEM, args_info.real_memory_arg * MB);
  if (args_info.stack_memory_given)
    perform_set_limit(cctxt, STK_MEM, args_info.stack_memory_arg * MB);
  if (args_info.output_size_given)
    perform_set_limit(cctxt, OUT, args_info.output_size_arg * MB);

  // transform inputs to NULL ended char* array
  int inputs_num = args_info.inputs_num;
  char **inputs = (char **)(malloc((inputs_num + 1) * sizeof(char *)));
  memcpy(inputs, args_info.inputs, inputs_num * sizeof(char *));
  inputs[inputs_num] = NULL;

  // required
  log_set(args_info.log_arg);
  result_code = atorc(args_info.result_arg);
  if (perform_set_policy(cctxt, args_info.policy_dir_arg,
                         args_info.policy_arg)) {
    cctxt->result.code = SE;
  } else {
    perform_set_runner(cctxt, inputs_num, inputs, env);

    if (strcmp(args_info.judger_arg, "traditional") == 0) {
      if (perform_traditional(cctxt)) {
        cctxt->result.code = SE;
        fprintf(stderr, "SE: %s\n", ystrerr(yerrno));
      }
    } else if (strcmp(args_info.judger_arg, "interactive") == 0) {
      if (perform_interactive(cctxt)) {
        cctxt->result.code = SE;
        fprintf(stderr, "SE: %s\n", ystrerr(yerrno));
      }
    } else if (strcmp(args_info.judger_arg, "general") == 0) {
      if (perform_general(cctxt)) {
        cctxt->result.code = SE;
        fprintf(stderr, "SE: %s\n", ystrerr(yerrno));
      }
    } else {
      exit(1);
    }

    log_print_result(&cctxt->result);
  }

  if ((int)cctxt->result.code != result_code) {
    fprintf(stderr, "test failed (result=%d, expect=%d)\n",
            (int)cctxt->result.code, result_code);
    return 1;
  }

  log_close();
  cmdline_parser_free(&args_info);
  return 0;
}