#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "hook.h"
#include "judger.h"
#include "lib/policy.h"
#include "lib/resource.h"

static rsclim_ctxt_t rsclim_ctxt_create() {
  rsclim_ctxt_t ctxt = malloc(sizeof(struct rsclim_ctxt));
  ctxt->actual_memory = 0;
  ctxt->cpu_time = 0;
  ctxt->output_size = 0;
  ctxt->real_time = 0;
  ctxt->stack_memory = 0;
  ctxt->virtual_memory = 0;
  return ctxt;
}

static runner_ctxt_t runner_ctxt_create() {
  runner_ctxt_t ctxt = malloc(sizeof(struct runner_ctxt));
  ctxt->argc = 0;
  ctxt->argv = NULL;
  ctxt->env = NULL;
  return ctxt;
}

static policy_ctxt_t policy_ctxt_create() {
  policy_ctxt_t ctxt = malloc(sizeof(struct policy_ctxt));
  ctxt->policy = NULL;
  ctxt->content = NULL;
  ctxt->dirname = NULL;
  ctxt->prog.filter = NULL;
  ctxt->is_builtin = 0;
  return ctxt;
}

static hook_ctxt_t hook_ctxt_create() {
  hook_ctxt_t ctxt = malloc(sizeof(struct hook_ctxt));
  ctxt->after_fork = hook_chain_create();
  ctxt->before_fork = hook_chain_create();
  ctxt->after_wait = hook_chain_create();
  return ctxt;
}

perform_ctxt_t perform_ctxt_create() {
  perform_ctxt_t ctxt = malloc(sizeof(struct perform_ctxt));
  ctxt->pself = -1;
  ctxt->pchild = -1;
  ctxt->status = -1;
  ctxt->pctxt = policy_ctxt_create();
  ctxt->ectxt = runner_ctxt_create();
  ctxt->rctxt = rsclim_ctxt_create();
  ctxt->hctxt = hook_ctxt_create();
  return ctxt;
}

int perform_set_policy(perform_ctxt_t ctxt, char *dirname, char *policy) {
  return policy_set(ctxt->pctxt, dirname, policy);
}

/**
 * @brief duplicate char** and add a NULL end
 */
static char **strsndupn(char **strs, size_t n) {
  char **ps = (char **)(malloc((n + 1) * sizeof(char *)));
  for (size_t i = 0; i < n; i++) {
    ps[i] = strdup(strs[i]);
  }
  ps[n] = NULL;
  return ps;
}

/**
 * @brief duplicate char** ended by NULL
 */
static char **strsdupn(char **strs) {
  size_t n = 0;
  while (strs[n] != NULL)
    ++n;
  return strsndupn(strs, n);
}

int perform_set_runner(perform_ctxt_t ctxt, int argc, char **argv, char **env) {
  ctxt->ectxt->argc = argc;
  ctxt->ectxt->argv = strsndupn(argv, argc);
  ctxt->ectxt->env = strsdupn(env);
  return 0;
}

int perform_set_limit(perform_ctxt_t ctxt, int type, int lim) {
  rsclim_ctxt_t rctxt = ctxt->rctxt;
  switch (type) {
  case REAL_TIME:
    rctxt->real_time = lim;
    break;
  case CPU_TIME:
    rctxt->cpu_time = lim;
    break;
  case TIME:
    rctxt->real_time = lim;
    rctxt->cpu_time = lim;
    break;
  case VIR_MEM:
  case VIRTUAL_MEMORY:
    rctxt->virtual_memory = lim;
    break;
  case ACT_MEM:
  case ACTUAL_MEMORY:
    rctxt->actual_memory = lim;
    break;
  case STK_MEM:
  case STACK_MEMORY:
    rctxt->stack_memory = lim;
    break;
  case OUT:
  case OUTPUT_SIZE:
    rctxt->output_size = lim;
    break;
  case MEM:
  case MEMORY:
    rctxt->virtual_memory = lim;
    rctxt->actual_memory = lim;
    rctxt->stack_memory = lim;
    break;
  default:
    return yerr(E_HELPER_INVALID_LIM);
  }
  return 0;
}

const char result_code_name[][10] = {
    [OK] = "OK",   [RE] = "RE", [MLE] = "MLE", [TLE] = "TLE",
    [OLE] = "OLE", [SE] = "SE", [DSC] = "DSC", [ECE] = "ECE",
};

enum result_code atorc(char *arg) {
  for (int i = 0; i < 8; i++) {
    if (strcmp(arg, result_code_name[i]) == 0)
      return i;
  }
  fprintf(stderr, "\"%s\" doesn't match any result code name.\n", arg);
  exit(1);
}

static void free_argv(char **p) {
  if (p == NULL)
    return;
  for (size_t i = 0; p[i] != NULL; i++) {
    free(p[i]);
    p[i] = NULL;
  }
  free(p);
}

struct perform_result perform_result(perform_ctxt_t ctxt) {
  return ctxt->result;
}

char *json_result(struct perform_result result) {
  char *s = malloc(200);
  sprintf(s,
          "{\"result\":%d,"
          "\"signal\":%d,"
          "\"exit_code\":%d,"
          "\"real_time\":%d,"
          "\"cpu_time\":%d,"
          "\"memory\":%d"
          "}",
          result.code, result.signal, result.exit_code, result.real_time,
          result.cpu_time, result.real_memory);
  return s;
}

void perform_ctxt_free(perform_ctxt_t ctxt) {
  if (ctxt->pctxt->is_builtin) {
    rmtree_depth1(ctxt->pctxt->dirname);
  }

  free(ctxt->pctxt->content);
  free(ctxt->pctxt->dirname);
  free(ctxt->pctxt->policy);
  free(ctxt->pctxt->prog.filter);

  free_argv(ctxt->ectxt->argv);
  free_argv(ctxt->ectxt->env);

  hook_chain_free(ctxt->hctxt->after_fork);
  hook_chain_free(ctxt->hctxt->before_fork);
  hook_chain_free(ctxt->hctxt->after_wait);

  free(ctxt->pctxt);
  free(ctxt->ectxt);
  free(ctxt->rctxt);
  free(ctxt->hctxt);

  free(ctxt);
}
