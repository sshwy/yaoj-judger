/**
 * @file signal_pipe.c
 * @author sshwy (jy.cat@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-04-17
 *
 * @copyright (c) 2022
 *
 * pipe delivering simple signal
 */

/**
 * @brief create a pipe
 *
 * @param pipe
 * @return int
 */
int signal_pipe(int pipe[2]);
int send_signal(int pipe[2], int signal);
int wait_signal(int pipe[2]);