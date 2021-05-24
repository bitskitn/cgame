int
main(int argc, char *argv[])
{
    // notes:
    //
    //   we use yacc+lex for parsing, so we need to fork/exec the
    //   c preprocessor and pipe the output to this program.
    //
    //   to support *.S/*.s files as input, we need as. to support
    //   the -S option, we need dis.

    for (int i = 1; i < argc; i++)
    {
        char *arg = argv[i];
        if (arg[0] != '-')
        {
            // todo: add to list of files to compile
            continue;
        }
        switch (arg[1])
        {
            case 'D':  // define a preprocessor macro
            case 'E':  // preprocess only
            case 'I':  // add search path to preprocessor
            case 'L':  // add search path to linker
            case 'O':  // set optimization level
            case 'S':  // output disassembled code
            case 'U':  // undefine a preprocessor macro
            case 'c':  // compile, but do not link
            case 'g':  // generate debugging information
            case 'l':  // link library
            case 's':  // strip symbols
                break;
        }
    }
}

