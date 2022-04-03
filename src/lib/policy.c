#include <linux/filter.h>
#include <linux/seccomp.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>

#include "async_log.h"
#include "common.h"
#include "judger.h"
#include "kafel.h"
#include "policy.h"

static const char prefix[] = "builtin:";
static const int prefix_len = 8;

static int is_builtin_policy(const char *policy) {
  if (strlen(policy) >= prefix_len &&
      strncmp(policy, prefix, prefix_len) == 0) {
    return 1;
  } else {
    return 0;
  }
}

static int policy_set_builtin(policy_ctxt_t ctxt, const char *policy) {
  const char *policyname = policy + prefix_len;
  LOG_DEBUG("load builtin policy: \"%s\"", policyname);

  struct builtin_policy *list = policy_list_get();
  char tdir[] = "/tmp/yjudger.XXXXXX";
  if (mkdtemp(tdir) == NULL) {
    yreturn(E_BULTIN_POL);
  }
  char *path = NULL;
  LOG_DEBUG("create tmp dir: %s", tdir);
  for (unsigned i = 0; i < policy_num; i++) {
    LOG_DEBUG("find %s (%d bytes)", list[i].filename, list[i].len);
    path = path_join(tdir, '/', list[i].filename);

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
      yreturn(E_FP);
    }
    if (fwrite(list[i].content, sizeof(unsigned char), list[i].len, fp) !=
        list[i].len) {
      yreturn(E_BULTIN_POL);
    }
    fclose(fp);
    free(path);
  }

  free(list);
  ctxt->is_builtin = 1;
  return policy_set(ctxt, tdir, policyname);
}

int policy_set(policy_ctxt_t ctxt, const char *dirname, const char *policy) {
  if (is_builtin_policy(policy)) {
    return policy_set_builtin(ctxt, policy);
  }

  char *tmp = path_join(dirname, '/', policy);
  char *filename = path_join(tmp, '.', "policy");

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    yreturn(E_FP);
  }
  ctxt->content = ftos(fp);
  ctxt->dirname = strdup(dirname);
  ctxt->policy = strdup(policy);

  free(tmp);
  free(filename);
  return 0;
}

/**
 * @brief replace identifier in content.
 */
int policy_identifier_handler(const char *content,
                              const yjudger_ctxt_t per_ctxt, char **res) {
  int res_len = 1;
  char *result = malloc(res_len * sizeof(char));
  *result = '\0';

  // use regular expression for pattern replacement
  regex_t regex;
  char re[] = "%[0-9][0-9]*s";
  if (regcomp(&regex, re, REG_NEWLINE)) {
    yreturn(E_ERROR);
  }

  const char *s = content;
  regmatch_t pmatch[1];
  int offset, len = 0, reg_status;
  int las = 0;

  while (1) {
    reg_status = regexec(&regex, s, 1, pmatch, 0);
    int num = 0;
    char num_str[10], address[50];
    *address = '\0'; // init

    if (reg_status == 0) {
      // parse match
      offset = pmatch[0].rm_so + (s - content);
      len = pmatch[0].rm_eo - pmatch[0].rm_so;
      strncpy(num_str, content + offset + 1, len - 2);
      num_str[len - 1] = 0;
      num = atoi(num_str);
      if (!(0 <= num && num < per_ctxt->ectxt->argc)) {
        LOG_DEBUG("num_str=\"%s\", num=%d, argc=%d", num_str, num,
                  per_ctxt->ectxt->argc);
        yreturn(E_POL_IDT);
      }
      sprintf(address, "%p", per_ctxt->ectxt->argv[num]);
    } else {
      offset = strlen(content);
    }

    int extra_len = offset - las + strlen(address);
    res_len += extra_len;

    result = realloc(result, (res_len + 5) * sizeof(char));
    if (result == NULL) {
      yreturn(E_ERROR);
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
  *res = result;
  return 0;
}

int compile_policy_before_fork(yjudger_ctxt_t per_ctxt) {
  char *esc_content;
  if (policy_identifier_handler(per_ctxt->pctxt->content, per_ctxt,
                                &esc_content))
    yreturn(yerrno);
  struct sock_fprog prog;
  kafel_ctxt_t kctxt = kafel_ctxt_create();
  kafel_set_input_string(kctxt, esc_content);
  kafel_add_include_search_path(kctxt, per_ctxt->pctxt->dirname);
  if (kafel_compile(kctxt, &prog)) {
    LOG_DEBUG("policy compilation failed: %s", kafel_error_msg(kctxt));
    yreturn(E_KAFEL);
  }
  kafel_ctxt_destroy(&kctxt);
  LOG_INFO("compile policy \"%s\" succeed", per_ctxt->pctxt->policy);
  per_ctxt->pctxt->prog = prog;
  return 0;
}

int apply_policy_prog(struct sock_fprog prog) {
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) ||
      prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0)) {
    yreturn(E_APP_POL);
  }
  LOG_INFO("apply policy succeed");
  return 0;
}

int apply_policy(yjudger_ctxt_t per_ctxt) {
  return apply_policy_prog(per_ctxt->pctxt->prog);
}