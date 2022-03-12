/**
 * @file judger.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Public judger library.
 * @date 2022-02-01
 * @copyright Copyright (c) 2022
 *
 * The implementation of judger consists of two parts: common part and perform
 * part. The common part lies in `helper.c` while perform part (i.e., definition
 * of perform()) lies in `src/judger/`.
 */
#ifndef YAOJUDGER_H
#define YAOJUDGER_H

#include <sys/resource.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "yerr.h"

#define KB (1024)
#define MB (1024 * KB)

enum rsclim_type {
  REAL_TIME,      //!< set real time limitation
  CPU_TIME,       //!< set cpu time limitation (rlimit)
  TIME,           //!< set both time limitations
  VIR_MEM,        //!< shortcut for VIRTUAL_MEMORY
  VIRTUAL_MEMORY, //!< limit virtual memory usage (rlimit)
  ACT_MEM,        //!< shortcut for ACTUAL_MEMORY
  ACTUAL_MEMORY,  //!< limit actual memory usage recorded by getrusage()
  STK_MEM,        //!< shortcut for STACK_MEMORY
  STACK_MEMORY,   //!< limit stack memory usage (rlimit)
  OUT,            //!< shortcut for OUTPUT_SIZE
  OUTPUT_SIZE,    //!< limit output file size (rlimit)
  MEM,            //!< shortcut for MEMORY
  MEMORY,         //!< set all three memory limitations
};

enum result_code {
  OK = 0,  //!< all correct
  RE = 1,  //!< runtime error
  MLE = 2, //!< memory limit exceed
  TLE = 3, //!< time limit exceed
  OLE = 4, //!< output limit exceed
  SE = 5,  //!< system error (judger error)
  DSC = 6, //!< dangerous system call
  ECE = 7, //!< exit code exception
};

/**
 * @brief transform string code name (uppercase) to corresponding code.
 * @param arg
 * @return int
 */
enum result_code atorc(char *arg);

/**
 * @brief Describe result of a terminated process.
 */
struct perform_result {
  /// return code of the judgement (often set during perform)
  enum result_code code;
  /**
   * @brief terminate signal raised by child process
   *
   * if child process exit normally, it shall be 0.
   */
  int signal;
  /**
   * @brief exit code of child process
   *
   * if child process terminated by signal, it shall be 0.
   */
  int exit_code;
  int real_time;   //!< in milliseconds.
  int cpu_time;    //!< in milliseconds.
  int real_memory; //!< in bytes.
};

typedef struct perform_ctxt *perform_ctxt_t;

/**
 * @brief Perform a judgement.
 */
int perform_general(perform_ctxt_t ctxt);
int perform_interactive(perform_ctxt_t ctxt);
int perform_traditional(perform_ctxt_t ctxt);

/**
 * @brief Create a perform context
 */
perform_ctxt_t perform_ctxt_create();

void perform_ctxt_free(perform_ctxt_t ctxt);

/**
 * @brief Set kafel policy (enhanced) for the runner.
 *
 * @param ctxt
 * @param dirname see policy_ctxt
 * @param policy see policy_ctxt
 * @return 0 on success, 1 otherwise
 */
int perform_set_policy(perform_ctxt_t ctxt, char *dirname, char *policy);

/**
 * @brief Set runner (program to be judged) arguments
 *
 * It's greatly dependent on the implementation of perform().
 *
 * @return 0 on success, 1 otherwise
 */
int perform_set_runner(perform_ctxt_t ctxt, int argc, char **argv, char **env);

/**
 * @brief Set resource limitation for the runner.
 *
 * @param ctxt
 * @param type see enum rsclim_type
 * @param lim see rsclim_ctxt
 * @return 0 on success, 1 otherwise
 */
int perform_set_limit(perform_ctxt_t ctxt, int type, int lim);

/**
 * @brief get result of perform
 *
 * shall be valid before calling perform_ctxt_free().
 *
 * @param ctxt
 * @return struct result
 */
struct perform_result perform_result(perform_ctxt_t ctxt);

char *json_result(struct perform_result);

/**
 * @brief set perform log file.
 */
void log_set(const char *filename);

/**
 * @brief print a result message to log file.
 */
void log_print_result(struct perform_result *pres);

/**
 * @brief close log file.
 */
void log_close();

#ifdef __cplusplus
}
#endif

#endif /* YAOJUDGER_H */