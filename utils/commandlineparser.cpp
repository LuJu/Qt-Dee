#include "commandlineparser.h"


CommandLineParser::CommandLineParser (int argc, char *argv [], void * p)
{
    /* Wow, simple and smart... */
    Parse (argc, argv, p);
}

CommandLineParser::~CommandLineParser ()
{
}

//int FindParent (int i, char *argv [])
//{
//    /* Go back until an option's found */
//    do {
//        i --;
//    } while ((argv [i][0] != opt_prefix [0] ||
//             (argv [i][0] != opt_long_prefix [0]   &&
//              argv [i][1] != opt_long_prefix [1])) && i > 0);

//    return i;
//}

/* Which type of char has been passed? (long option / short option / arg) */
void CommandLineParser::Parse (int argc, char *argv [], void * p)  /* Func1 */
{
//    int i;
//    QString argument;
//    for (i=1; i<argc; i++) {

//        // long option
//        if (argument.startsWith(_long_option)) {
//            if (argument.contains("=")){
//               QStringList list = argument.split("=");
//            } else {
//                QString option = argument.remove(2,2);
//                _long_option.append(option);
//            }
//        }
//        // short option
//        else if (argument.startsWith(_short_option)) {
//            if (Find (argv[i], SHORT) == 0) {
//                qWarning()<<"No "<< argv[i] <<"option";
//            }

//        }
//        /* No match: it's an argument! */
//        else {
//            int x = FindParent (i, argv);

//            /* If no option's been found something wrong happened */
//            if (x == 0) {
//            /* Error2 */
//                fprintf (stderr, "ERROR: mis-typed word '%s': should be an argument but no option were found\n", argv [i]);
//                exit (12);
//            }
//            /* If everything is ok, fill in the args arrays */
//            else {
//                FillArgs (i, x, argv);
//            }
//        }
//    }


//    /* Scans the array of options... */
//    for (i=0; i<NUM_OPTS; i++)
//        /* ...to find a called one! */
//        if (options[i].called) {
//            /* All args have been specified: ok, we continue */
//            if (options[i].done    == options[i].numArgs ||
//               (options[i].numArgs == 0 && options[i].done == 0)) {
//                options[i].target (options[i].args,p);
//            }
//            /* Got 12 out of 10? wow sounds incredible... */
//            else if (options[i].done > options[i].numArgs) {
//            /* Error0 */
////                fprintf (stderr, "ERROR: too many arguments for option '%s'! Try '%s' for info.\n", options[i].name, help);
//                exit (30);
//            }
//            /* Did you forget something? (wake up!) */
//            else if (options[i].done < options[i].numArgs) {
//             /* Error1 */
////                fprintf (stderr, "ERROR: missing operand(s). Try '%s' for info\n", help);
//                exit (31);
//            }
//        }
}

//char *allocName (int i)
//{
//    char *name;

//    if (options[i].format == SHORT) {
//        name = (char*)malloc (strlen (options[i].name) + strlen (opt_prefix) + 1);
//        strcpy (name, opt_prefix);
//    }
//    else if (options[i].format == LONG) {
//        name = (char*)malloc (strlen (options[i].name) + strlen (opt_long_prefix) + 1);
//        strcpy (name, opt_long_prefix);
//    }

//    strcat (name, options[i].name);

//    return name;
//}

//int CommandLineParser::Find (char *input, int type)
//{
//    int i;
//    char *name;

//    for (i=0; i<NUM_OPTS; i++) {
//        name = allocName (i);

//        /* Scan options to check the existence of the one passed */
//        if (!strcmp (name, input) && options[i].format == type) {
//            /* called flag to 1: the option has been called! */
//            options[i].called = 1;

//            free (name);

//            return i+1;
//        }

//        free (name);
//    }

//    return 0;
//}

///* Put args in their owner's array */
//void CommandLineParser::FillArgs (int i, int x, char *argv []) /* Func2 */
//{
//    int k;
//    char *name;

//    if (DEBUG && VERBOSE)
//        printf ("'%s' is an argument of the option '%s'\n", argv[i], argv[x]);

//    for (k=0; k<NUM_OPTS; k++) {
//        name = allocName (k);

//        /* Parent option found */
//        if (!strcmp(name, argv [x])) {
//            if (options[k].done < MAX_ARGS) {
//                /* Add arg in array */
//                options[k].args[options[k].done] = argv[i];

//                /* Wow, we've done another argument matching! */
//                options[k].done ++;
//            }
//            /* Hey! Too many arguments... Argh... */
//            else {
//            /* Error0 */
//                fprintf (stderr, "ERROR: too many arguments for option '%s'! Try '%s' for info.\n", options[k].name, help);

//                free (name);

//                exit (20);
//            }
//        }

//        free (name);
//    }
//}
