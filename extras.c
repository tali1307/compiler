/* void classTable()
{
    ParserInfo pi;
    pi.er = none;
    symbols_table[symbols_table_index]->scope_number = ++scope_count;
    pi.tk = GetNextToken();
    strcpy(className, pi.tk.lx);
    while (symbols_table_index < TABLE_SIZE)
    {
        pi.tk = GetNextToken();
        if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
        {
            char *kind, *type;
            strcpy(symbols_table[symbols_table_index]->kind, pi.tk.lx);
            strcpy(kind, pi.tk.lx);
            pi.tk = GetNextToken();
            strcpy(symbols_table[symbols_table_index]->type, pi.tk.lx);
            strcpy(type, pi.tk.lx);
            pi.tk = GetNextToken();
            strcpy(symbols_table[symbols_table_index]->name, pi.tk.lx);
            symbols_table[symbols_table_index]->scope_level = CLASS_SCOPE;
            symbols_table[symbols_table_index]->scope_number = scope_count;
            if (strcmp(GetNextToken().lx, ",") == 0)
            {
                while (strcmp(PeekNextToken().lx, ";") != 0)
                {
                    pi.tk = GetNextToken();
                    strcpy(symbols_table[symbols_table_index]->name, pi.tk.lx);
                    strcpy(symbols_table[symbols_table_index]->type, type);
                    strcpy(symbols_table[symbols_table_index]->kind, kind);
                    symbols_table[symbols_table_index]->scope_level = CLASS_SCOPE;
                    symbols_table[symbols_table_index]->scope_number = scope_count;
                }
            }
        }
        else if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "method") == 0 || strcmp(pi.tk.lx, "function") == 0)
            break;
        symbols_table_index++;
    }
}

void methodTable()
{
    ParserInfo pi;
    pi.er = none;
    table *t = NULL;
    strcpy(t->name, "this");
    strcpy(t->kind, "argument");
    strcpy(t->type, className);
    t->scope_level = SUBROUTINE_SCOPE;
    t->scope_number = ++scope_count;

    if (symbolsTableLookup(t) == true)
    {
        pi.er = redecIdentifier;
        return pi;
    }
    else
    {
        symbols_table[symbols_table_index] = t;
        symbols_table_index++;
    }

    while (symbols_table_index < TABLE_SIZE)
    {
        pi.tk = GetNextToken();
        if (strcmp(pi.tk.lx, "(") == 0)
        {
            while (strcmp(pi.tk.lx, ")") != 0)
            {
                pi = type();
                if (pi.er == none)
                {
                    strcpy(t->type, pi.tk.lx);
                    pi.tk = GetNextToken();
                    strcpy(t->name, pi.tk.lx);
                    strcpy(t->kind, "argument");
                    t->scope_level = SUBROUTINE_SCOPE;
                    t->scope_number = scope_count;

                    if (symbolsTableLookup(t) == true)
                    {
                        pi.er = redecIdentifier;
                        return pi;
                    }
                    else
                    {
                        symbols_table[symbols_table_index] = t;
                        symbols_table_index++;
                    }
                }
            }
        }
        if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "function") == 0 || strcmp(pi.tk.lx, "while") == 0 || strcmp(pi.tk.lx, "do") == 0 || strcmp(pi.tk.lx, "if") == 0)
            break;
        symbols_table_index++;
    }
}

void CreateTables()
{
    ParserInfo pi;
    pi.er = none;
    pi.tk = GetNextToken();
    if (strcmp(pi.tk.lx, "class") == 0)
    {
        classTable();
    }
    else if (strcmp(pi.tk.lx, "method") == 0)
    {
        methodTable();
    }
} */