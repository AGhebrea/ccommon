/* I also wanted a FTRACE macro that printed fn call + argument values
 * but the way the preprocessor works and the way va_list work means that
 * to use it you'd have to basically define the format string yourself
 * and that is the same as just calling ccLogTrace by hand */

/* Can mess up the debug lineinfo if you
 * don't use it like: 
 * DebugEnabledExpression( 
 *      line1;
 *      line2;
 *      etc...
 * )
 * 
 * the release variant of the macro comments out
 * expression just because of the same reason
 * restriction is that you cannot have
 * comments inside of the DebugEnabledExpression
 * macro
 */
#ifdef DEBUG
#define DebugEnabled(expr)  \
                            \
    expr                    \
    
#else 
#define DebugEnabled(expr)  \
    /*                      \
    expr                    \
    */
#endif /* DEBUG */