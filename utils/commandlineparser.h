#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H
#include <QString>
#include <QHash>
#include <QStringList>

class CommandLineParser
{
public:


QStringList _long_options;
QStringList _short_options;
QHash<QString,QString> _arguments;
const QString _short_option = "-";
const QString _long_option = "--";
/* The types an option may have
 * (obviously only SHORT and LONG :D)
 */
//enum types { SHORT = 1, LONG };

//typedef struct {
//    /* The user has to define these only */
//    const char *name; /* Name of the option */
//    int numArgs; /* Number of arguments the option takes */
//    enum types format; /* Type of the option's call */

//    /* The function you'll specify as target must exist and declared
//     * BEFORE this point.
//     */
//    int (*target)(char **, void *); /* Pointer to the function to initialize */

//    /* You may not touch these! -- Unless you know what you're doing */
//    int done; /* The number of args processed */
//    int called; /* Has the option been called? */

//    /* Size +1 to avoid overflows */
//    char *args [MAX_ARGS+1]; /* array of args (starting from 0!)*/
//} opt;
CommandLineParser (int argc, char *argv [], void * p);
~CommandLineParser ();
void Parse (int argc, char *argv [], void * p);
};


#endif // COMMANDLINEPARSER_H
