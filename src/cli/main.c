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

struct gengetopt_args_info args_info;
struct yjudger_result result;

int main(int argc, char **argv, char **env) {
  result.code = OK;

  int result_code = -1;

  if (cmdline_parser(argc, argv, &args_info))
    exit(1);

  // required
  if (log_set(args_info.log_arg, 0, args_info.log_color_flag)) {
    exit(1);
  }

  yjudger_ctxt_t ctxt = yjudger_ctxt_create();
  if (args_info.timeout_given)
    yjudger_set_limit(ctxt, TIME, args_info.timeout_arg);
  if (args_info.cputime_given)
    yjudger_set_limit(ctxt, CPU_TIME, args_info.cputime_arg);
  if (args_info.realtime_given)
    yjudger_set_limit(ctxt, REAL_TIME, args_info.realtime_arg);
  if (args_info.memory_given)
    yjudger_set_limit(ctxt, MEM, args_info.memory_arg * MB);
  if (args_info.virtual_memory_given)
    yjudger_set_limit(ctxt, VIR_MEM, args_info.virtual_memory_arg * MB);
  if (args_info.real_memory_given)
    yjudger_set_limit(ctxt, ACT_MEM, args_info.real_memory_arg * MB);
  if (args_info.stack_memory_given)
    yjudger_set_limit(ctxt, STK_MEM, args_info.stack_memory_arg * MB);
  if (args_info.output_size_given)
    yjudger_set_limit(ctxt, OUT, args_info.output_size_arg * MB);
  if (args_info.fileno_given)
    yjudger_set_limit(ctxt, FILENO, args_info.fileno_arg);
  if (args_info.result_given)
    result_code = atorc(args_info.result_arg);

  // transform inputs to NULL ended char* array
  int inputs_num = args_info.inputs_num;
  char **inputs = (char **)(malloc((inputs_num + 1) * sizeof(char *)));
  memcpy(inputs, args_info.inputs, inputs_num * sizeof(char *));
  inputs[inputs_num] = NULL;

  if (yjudger_set_policy(ctxt, args_info.policy_dir_arg,
                         args_info.policy_arg)) {
  } else {
    yjudger_set_runner(ctxt, inputs_num, inputs, env);

    if (strcmp(args_info.judger_arg, "interactive") == 0) {
      yjudger_interactive(ctxt);
    } else if (strcmp(args_info.judger_arg, "general") == 0) {
      yjudger_general(ctxt);
    } else {
      exit(1);
    }
    if (yerrno == 0) {
      result = yjudger_result(ctxt);
    }

    log_print_result(&result);
  }

  int return_code = 0;

  if (yerrno) {
    fprintf(stderr, "System Error: %s\n", ystrerr(yerrno));
    result.code = SE;
  }

  if (args_info.result_given) {
    if ((int)result.code != result_code) {
      fprintf(stderr, "test failed (result=%d, expect=%d)\n", (int)result.code,
              result_code);
      return_code = 1;
    }
  }

  if (args_info.json_given && args_info.json_flag) {
    char *jdump = json_result(result);
    printf("%s\n", jdump);
    free(jdump);
  }

  yjudger_ctxt_free(ctxt);
  log_close();
  cmdline_parser_free(&args_info);
  return return_code;
}