#ifndef PIPES_WARNINGS_HPP
#define PIPES_WARNINGS_HPP

#if defined(_MSC_VER)
    #define PIPES_DISABLE_WARNING_PUSH __pragma(warning( push ))
    #define PIPES_DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))
    #define PIPES_DISABLE_WARNING_POP __pragma(warning( pop ))

    #define PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED      PIPES_DISABLE_WARNING(4522)

#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define PIPES_DISABLE_WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
    #define PIPES_DISABLE_WARNING(warningName) DO_PRAGMA(GCC diagnostic ignored #warningName)
    #define PIPES_DISABLE_WARNING_POP DO_PRAGMA(GCC diagnostic pop)

    #define PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED

#else
    #define PIPES_DISABLE_WARNING_PUSH
    #define PIPES_DISABLE_WARNING_POP
    #define PIPES_DISABLE_WARNING_MULTIPLE_ASSIGNMENT_OPERATORS_SPECIFIED
#endif

#endif /* PIPES_WARNINGS_HPP */
