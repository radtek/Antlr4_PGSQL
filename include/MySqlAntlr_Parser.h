#include <iostream>
#include <string>

#include <antlr4-runtime.h>
#include "MySqlLexer.h"
#include "MySqlParser.h"

using namespace antlr4;
using namespace std;
int ANTLR_Parse( char *sql_name);