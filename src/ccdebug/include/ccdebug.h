/* I also wanted a FTRACE macro that printed fn call + argument values
 * but the way the preprocessor works and the way va_list work means that
 * to use it you'd have to basically define the format string yourself
 * and that is the same as just calling ccLogTrace by hand */

#ifdef DEBUG
#define DebugEnabledExpression(expr) \
    { expr }
#else 
#define DebugEnabledExpression(expr) \
    (void)0
#endif /* DEBUG */