/*
  File autogenerated by gengetopt version 2.23
  generated with the following command:
  gengetopt --file-name=cli --unnamed-opts

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "cli.h"

const char *gengetopt_args_info_purpose = "judger for the future yaoj";

const char *gengetopt_args_info_usage = "Usage: yaoj-judger [OPTION]... [FILE]...";

const char *gengetopt_args_info_versiontext = "Copyright (c) Sshwy 2022";

const char *gengetopt_args_info_description = "A set of program runners with resource limitation, syscall limitation and final\nstatus report.";

const char *gengetopt_args_info_help[] = {
  "  -h, --help                    Print help and exit",
  "  -V, --version                 Print version and exit",
  "\nRequired Options:",
  "\nA brief text description before the other options.\n",
  "  -r, --result=string           specify the result code using name",
  "      --log=filename            specify judger result file",
  "  -p, --policy=filename         specify policy name",
  "  -P, --policy-dir=filename     specify policy search directory",
  "  -j, --judger=judgername       specify which judger to use  (possible\n                                  values=\"traditional\", \"interactive\",\n                                  \"general\")",
  "\nResource Limitations:",
  "  the following options are all optional",
  "  -t, --timeout=integer         specify both time limits in milliseconds",
  "      --realtime=integer        specify the runtime limit in milliseconds",
  "      --cputime=integer         specify the cpu limit in milliseconds",
  "  -m, --memory=integer          specify all three memory limits in MB",
  "      --virtual-memory=integer  specify the virtual memory limit in MB",
  "      --real-memory=integer     specify the actual memory limit in MB",
  "      --stack-memory=integer    specify the stack memory limit in MB",
  "  -g, --output-size=integer     specify the output limit in MB",
    0
};

typedef enum {ARG_NO
  , ARG_STRING
  , ARG_INT
} cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error);

static int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error);

const char *cmdline_parser_judger_values[] = {"traditional", "interactive", "general", 0}; /*< Possible values for judger. */

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->result_given = 0 ;
  args_info->log_given = 0 ;
  args_info->policy_given = 0 ;
  args_info->policy_dir_given = 0 ;
  args_info->judger_given = 0 ;
  args_info->timeout_given = 0 ;
  args_info->realtime_given = 0 ;
  args_info->cputime_given = 0 ;
  args_info->memory_given = 0 ;
  args_info->virtual_memory_given = 0 ;
  args_info->real_memory_given = 0 ;
  args_info->stack_memory_given = 0 ;
  args_info->output_size_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->result_arg = NULL;
  args_info->result_orig = NULL;
  args_info->log_arg = NULL;
  args_info->log_orig = NULL;
  args_info->policy_arg = NULL;
  args_info->policy_orig = NULL;
  args_info->policy_dir_arg = NULL;
  args_info->policy_dir_orig = NULL;
  args_info->judger_arg = NULL;
  args_info->judger_orig = NULL;
  args_info->timeout_orig = NULL;
  args_info->realtime_orig = NULL;
  args_info->cputime_orig = NULL;
  args_info->memory_orig = NULL;
  args_info->virtual_memory_orig = NULL;
  args_info->real_memory_orig = NULL;
  args_info->stack_memory_orig = NULL;
  args_info->output_size_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{


  args_info->help_help = gengetopt_args_info_help[0] ;
  args_info->version_help = gengetopt_args_info_help[1] ;
  args_info->result_help = gengetopt_args_info_help[4] ;
  args_info->log_help = gengetopt_args_info_help[5] ;
  args_info->policy_help = gengetopt_args_info_help[6] ;
  args_info->policy_dir_help = gengetopt_args_info_help[7] ;
  args_info->judger_help = gengetopt_args_info_help[8] ;
  args_info->timeout_help = gengetopt_args_info_help[11] ;
  args_info->realtime_help = gengetopt_args_info_help[12] ;
  args_info->cputime_help = gengetopt_args_info_help[13] ;
  args_info->memory_help = gengetopt_args_info_help[14] ;
  args_info->virtual_memory_help = gengetopt_args_info_help[15] ;
  args_info->real_memory_help = gengetopt_args_info_help[16] ;
  args_info->stack_memory_help = gengetopt_args_info_help[17] ;
  args_info->output_size_help = gengetopt_args_info_help[18] ;
  
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(CMDLINE_PARSER_PACKAGE_NAME) ? CMDLINE_PARSER_PACKAGE_NAME : CMDLINE_PARSER_PACKAGE),
     CMDLINE_PARSER_VERSION);

  if (strlen(gengetopt_args_info_versiontext) > 0)
    printf("\n%s\n", gengetopt_args_info_versiontext);
}

static void print_help_common(void)
{
	size_t len_purpose = strlen(gengetopt_args_info_purpose);
	size_t len_usage = strlen(gengetopt_args_info_usage);

	if (len_usage > 0) {
		printf("%s\n", gengetopt_args_info_usage);
	}
	if (len_purpose > 0) {
		printf("%s\n", gengetopt_args_info_purpose);
	}

	if (len_usage || len_purpose) {
		printf("\n");
	}

	if (strlen(gengetopt_args_info_description) > 0) {
		printf("%s\n\n", gengetopt_args_info_description);
	}
}

void
cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);

  args_info->inputs = 0;
  args_info->inputs_num = 0;
}

void
cmdline_parser_params_init(struct cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct cmdline_parser_params *
cmdline_parser_params_create(void)
{
  struct cmdline_parser_params *params = 
    (struct cmdline_parser_params *)malloc(sizeof(struct cmdline_parser_params));
  cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}


static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{
  unsigned int i;
  free_string_field (&(args_info->result_arg));
  free_string_field (&(args_info->result_orig));
  free_string_field (&(args_info->log_arg));
  free_string_field (&(args_info->log_orig));
  free_string_field (&(args_info->policy_arg));
  free_string_field (&(args_info->policy_orig));
  free_string_field (&(args_info->policy_dir_arg));
  free_string_field (&(args_info->policy_dir_orig));
  free_string_field (&(args_info->judger_arg));
  free_string_field (&(args_info->judger_orig));
  free_string_field (&(args_info->timeout_orig));
  free_string_field (&(args_info->realtime_orig));
  free_string_field (&(args_info->cputime_orig));
  free_string_field (&(args_info->memory_orig));
  free_string_field (&(args_info->virtual_memory_orig));
  free_string_field (&(args_info->real_memory_orig));
  free_string_field (&(args_info->stack_memory_orig));
  free_string_field (&(args_info->output_size_orig));
  
  
  for (i = 0; i < args_info->inputs_num; ++i)
    free (args_info->inputs [i]);

  if (args_info->inputs_num)
    free (args_info->inputs);

  clear_given (args_info);
}

/**
 * @param val the value to check
 * @param values the possible values
 * @return the index of the matched value:
 * -1 if no value matched,
 * -2 if more than one value has matched
 */
static int
check_possible_values(const char *val, const char *values[])
{
  int i, found, last;
  size_t len;

  if (!val)   /* otherwise strlen() crashes below */
    return -1; /* -1 means no argument for the option */

  found = last = 0;

  for (i = 0, len = strlen(val); values[i]; ++i)
    {
      if (strncmp(val, values[i], len) == 0)
        {
          ++found;
          last = i;
          if (strlen(values[i]) == len)
            return i; /* exact macth no need to check more */
        }
    }

  if (found == 1) /* one match: OK */
    return last;

  return (found ? -2 : -1); /* return many values or none matched */
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  int found = -1;
  if (arg) {
    if (values) {
      found = check_possible_values(arg, values);      
    }
    if (found >= 0)
      fprintf(outfile, "%s=\"%s\" # %s\n", opt, arg, values[found]);
    else
      fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->result_given)
    write_into_file(outfile, "result", args_info->result_orig, 0);
  if (args_info->log_given)
    write_into_file(outfile, "log", args_info->log_orig, 0);
  if (args_info->policy_given)
    write_into_file(outfile, "policy", args_info->policy_orig, 0);
  if (args_info->policy_dir_given)
    write_into_file(outfile, "policy-dir", args_info->policy_dir_orig, 0);
  if (args_info->judger_given)
    write_into_file(outfile, "judger", args_info->judger_orig, cmdline_parser_judger_values);
  if (args_info->timeout_given)
    write_into_file(outfile, "timeout", args_info->timeout_orig, 0);
  if (args_info->realtime_given)
    write_into_file(outfile, "realtime", args_info->realtime_orig, 0);
  if (args_info->cputime_given)
    write_into_file(outfile, "cputime", args_info->cputime_orig, 0);
  if (args_info->memory_given)
    write_into_file(outfile, "memory", args_info->memory_orig, 0);
  if (args_info->virtual_memory_given)
    write_into_file(outfile, "virtual-memory", args_info->virtual_memory_orig, 0);
  if (args_info->real_memory_given)
    write_into_file(outfile, "real-memory", args_info->real_memory_orig, 0);
  if (args_info->stack_memory_given)
    write_into_file(outfile, "stack-memory", args_info->stack_memory_orig, 0);
  if (args_info->output_size_given)
    write_into_file(outfile, "output-size", args_info->output_size_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char **argv, struct gengetopt_args_info *args_info)
{
  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser_ext (int argc, char **argv, struct gengetopt_args_info *args_info,
                   struct cmdline_parser_params *params)
{
  int result;
  result = cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser2 (int argc, char **argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  int result = EXIT_SUCCESS;

  if (cmdline_parser_required2(args_info, prog_name, 0) > 0)
    result = EXIT_FAILURE;

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error)
{
  int error_occurred = 0;
  FIX_UNUSED (additional_error);

  /* checks for required options */
  if (! args_info->result_given)
    {
      fprintf (stderr, "%s: '--result' ('-r') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  if (! args_info->log_given)
    {
      fprintf (stderr, "%s: '--log' option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  if (! args_info->policy_given)
    {
      fprintf (stderr, "%s: '--policy' ('-p') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  if (! args_info->policy_dir_given)
    {
      fprintf (stderr, "%s: '--policy-dir' ('-P') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  if (! args_info->judger_given)
    {
      fprintf (stderr, "%s: '--judger' ('-j') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  
  /* checks for dependences among options */

  return error_occurred;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see cmdline_parser_params.check_ambiguity
 * @param override @see cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  if (possible_values && (found = check_possible_values((value ? value : default_value), possible_values)) < 0)
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s' (`-%c')%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt, short_opt,
          (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s'%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt,
          (additional_error ? additional_error : ""));
      return 1; /* failure */
    }
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_INT:
    if (val) *((int *)field) = strtol (val, &stop_char, 0);
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_INT:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}


int
cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error_occurred = 0;
  struct gengetopt_args_info local_args_info;
  
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  /* TODO: Why is this here? It is not used anywhere. */
  override = params->override;
  FIX_UNUSED(override);

  initialize = params->initialize;
  check_required = params->check_required;

  /* TODO: Why is this here? It is not used anywhere. */
  check_ambiguity = params->check_ambiguity;
  FIX_UNUSED(check_ambiguity);

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "result",	1, NULL, 'r' },
        { "log",	1, NULL, 0 },
        { "policy",	1, NULL, 'p' },
        { "policy-dir",	1, NULL, 'P' },
        { "judger",	1, NULL, 'j' },
        { "timeout",	1, NULL, 't' },
        { "realtime",	1, NULL, 0 },
        { "cputime",	1, NULL, 0 },
        { "memory",	1, NULL, 'm' },
        { "virtual-memory",	1, NULL, 0 },
        { "real-memory",	1, NULL, 0 },
        { "stack-memory",	1, NULL, 0 },
        { "output-size",	1, NULL, 'g' },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVr:p:P:j:t:m:g:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          cmdline_parser_print_help ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          cmdline_parser_print_version ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'r':	/* specify the result code using name.  */
        
        
          if (update_arg( (void *)&(args_info->result_arg), 
               &(args_info->result_orig), &(args_info->result_given),
              &(local_args_info.result_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "result", 'r',
              additional_error))
            goto failure;
        
          break;
        case 'p':	/* specify policy name.  */
        
        
          if (update_arg( (void *)&(args_info->policy_arg), 
               &(args_info->policy_orig), &(args_info->policy_given),
              &(local_args_info.policy_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "policy", 'p',
              additional_error))
            goto failure;
        
          break;
        case 'P':	/* specify policy search directory.  */
        
        
          if (update_arg( (void *)&(args_info->policy_dir_arg), 
               &(args_info->policy_dir_orig), &(args_info->policy_dir_given),
              &(local_args_info.policy_dir_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "policy-dir", 'P',
              additional_error))
            goto failure;
        
          break;
        case 'j':	/* specify which judger to use.  */
        
        
          if (update_arg( (void *)&(args_info->judger_arg), 
               &(args_info->judger_orig), &(args_info->judger_given),
              &(local_args_info.judger_given), optarg, cmdline_parser_judger_values, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "judger", 'j',
              additional_error))
            goto failure;
        
          break;
        case 't':	/* specify both time limits in milliseconds.  */
        
        
          if (update_arg( (void *)&(args_info->timeout_arg), 
               &(args_info->timeout_orig), &(args_info->timeout_given),
              &(local_args_info.timeout_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "timeout", 't',
              additional_error))
            goto failure;
        
          break;
        case 'm':	/* specify all three memory limits in MB.  */
        
        
          if (update_arg( (void *)&(args_info->memory_arg), 
               &(args_info->memory_orig), &(args_info->memory_given),
              &(local_args_info.memory_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "memory", 'm',
              additional_error))
            goto failure;
        
          break;
        case 'g':	/* specify the output limit in MB.  */
        
        
          if (update_arg( (void *)&(args_info->output_size_arg), 
               &(args_info->output_size_orig), &(args_info->output_size_given),
              &(local_args_info.output_size_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "output-size", 'g',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          /* specify judger result file.  */
          if (strcmp (long_options[option_index].name, "log") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->log_arg), 
                 &(args_info->log_orig), &(args_info->log_given),
                &(local_args_info.log_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "log", '-',
                additional_error))
              goto failure;
          
          }
          /* specify the runtime limit in milliseconds.  */
          else if (strcmp (long_options[option_index].name, "realtime") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->realtime_arg), 
                 &(args_info->realtime_orig), &(args_info->realtime_given),
                &(local_args_info.realtime_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "realtime", '-',
                additional_error))
              goto failure;
          
          }
          /* specify the cpu limit in milliseconds.  */
          else if (strcmp (long_options[option_index].name, "cputime") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->cputime_arg), 
                 &(args_info->cputime_orig), &(args_info->cputime_given),
                &(local_args_info.cputime_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "cputime", '-',
                additional_error))
              goto failure;
          
          }
          /* specify the virtual memory limit in MB.  */
          else if (strcmp (long_options[option_index].name, "virtual-memory") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->virtual_memory_arg), 
                 &(args_info->virtual_memory_orig), &(args_info->virtual_memory_given),
                &(local_args_info.virtual_memory_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "virtual-memory", '-',
                additional_error))
              goto failure;
          
          }
          /* specify the actual memory limit in MB.  */
          else if (strcmp (long_options[option_index].name, "real-memory") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->real_memory_arg), 
                 &(args_info->real_memory_orig), &(args_info->real_memory_given),
                &(local_args_info.real_memory_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "real-memory", '-',
                additional_error))
              goto failure;
          
          }
          /* specify the stack memory limit in MB.  */
          else if (strcmp (long_options[option_index].name, "stack-memory") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->stack_memory_arg), 
                 &(args_info->stack_memory_orig), &(args_info->stack_memory_given),
                &(local_args_info.stack_memory_given), optarg, 0, 0, ARG_INT,
                check_ambiguity, override, 0, 0,
                "stack-memory", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */



  if (check_required)
    {
      error_occurred += cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  cmdline_parser_release (&local_args_info);

  if ( error_occurred )
    return (EXIT_FAILURE);

  if (optind < argc)
    {
      int i = 0 ;
      int found_prog_name = 0;
      /* whether program name, i.e., argv[0], is in the remaining args
         (this may happen with some implementations of getopt,
          but surely not with the one included by gengetopt) */

      i = optind;
      while (i < argc)
        if (argv[i++] == argv[0]) {
          found_prog_name = 1;
          break;
        }
      i = 0;

      args_info->inputs_num = argc - optind - found_prog_name;
      args_info->inputs =
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        if (argv[optind++] != argv[0])
          args_info->inputs[ i++ ] = gengetopt_strdup (argv[optind-1]) ;
    }

  return 0;

failure:
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
/* vim: set ft=c noet ts=8 sts=8 sw=8 tw=80 nojs spell : */
