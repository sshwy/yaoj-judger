#include <linux/filter.h>
#include <linux/seccomp.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>

#include "common.h"
#include "kafel.h"
#include "policy.h"

struct policy_ctxt create_policy_ctxt(char *dirname, char *policy) {
  const char *filename =
      path_join(path_join(dirname, '/', policy), '.', "policy");
  struct policy_ctxt ctxt = {
      .dirname = dirname,
      .policy = policy,
      .content = ftos(fopen(filename, "r")),
  };
  return ctxt;
}

/**
 * @brief replace identifier in content.
 *
 * @param content
 * @param per_ctxt
 * @param result
 */
char *policy_identifier_handler(const char *content,
                                const perform_ctxt_t per_ctxt) {
#define ALLOC_UNIT 100
  int res_size = strlen(content) + ALLOC_UNIT, res_len = 0;
  char *result = malloc(res_size * sizeof(char));
  *result = '\0';

  // use regular expression for pattern replacement
  regex_t regex;
  char re[] = "%[0-9][0-9]*s";
  ASSERT(regcomp(&regex, re, REG_NEWLINE) == 0, "regex compile failed.");

  const char *s = content;
  regmatch_t pmatch[1];
  int offset, len = 0, reg_status;
  int las = 0;

  while (1) {
    reg_status = regexec(&regex, s, 1, pmatch, 0);
    int resize_flag = 0, num = 0;
    char num_str[10], address[50];
    *address = '\0'; // init

    if (reg_status == 0) {
      // parse match
      offset = pmatch[0].rm_so + (s - content);
      len = pmatch[0].rm_eo - pmatch[0].rm_so;
      strncpy(num_str, content + offset + 1, len - 2);
      num_str[len - 1] = 0;
      num = atoi(num_str);
      ASSERT(0 <= num && num < per_ctxt->ectxt->argc, "invalid num (%d)", num);
      sprintf(address, "%p", per_ctxt->ectxt->argv[num]);
    } else {
      offset = strlen(content);
    }

    int extra_len = offset - las + strlen(address);

    // realloc result in need
    while (res_len + extra_len + 5 > res_size) {
      resize_flag = 1;
      res_size += ALLOC_UNIT;
    }
    if (resize_flag) {
      result = realloc(result, res_size * sizeof(char));
      ASSERT(result != NULL, "realloc error\n");
    }

    // update result
    if (offset > las) {
      strncat(result, content + las, offset - las);
    }
    strcat(result, address);
    las = offset + len;

    if (reg_status != 0)
      break;

    s += pmatch[0].rm_eo;
  }
  return result;
}

int compile_policy_before_fork(perform_ctxt_t per_ctxt) {
  char *esc_content =
      policy_identifier_handler(per_ctxt->pctxt->content, per_ctxt);
  struct sock_fprog prog;
  kafel_ctxt_t kctxt = kafel_ctxt_create();
  kafel_set_input_string(kctxt, esc_content);
  kafel_add_include_search_path(kctxt, per_ctxt->pctxt->dirname);
  if (kafel_compile(kctxt, &prog)) {
    SET_ERRORF("policy compilation failed: %s", kafel_error_msg(kctxt));
    return 1;
  }
  kafel_ctxt_destroy(&kctxt);
  LOG_INFO("compile policy \"%s\" succeed.\n", per_ctxt->pctxt->policy);
  per_ctxt->pctxt->prog = prog;
  return 0;
}

int apply_policy_prog(struct sock_fprog prog) {
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) ||
      prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0)) {
    SET_ERRORF("error applying policy");
    return 1;
  }
  free(prog.filter);
  LOG_INFO("apply policy succeed.\n");
  return 0;
}

int apply_policy(perform_ctxt_t per_ctxt) {
  return apply_policy_prog(per_ctxt->pctxt->prog);
}