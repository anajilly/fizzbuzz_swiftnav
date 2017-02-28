/*
 * Copyright (C) 2017 Christiana Johnson
 * Contact: Christiana Johnson <anajilly@gmail.com>
 *
 * This source is subject to the license found in the file 'LICENSE' which must
 * be be distributed together with this source. All other rights reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * \brief FizzBuzz, a standard coding test.
 *
 * The specification was found here:
 * https://github.com/swift-nav/screening_questions/blob/master/questions.md#swift-navigation-application-questions
 * and reads as follows:
 *
 * In the programming language of your choice, write a program generating the first n Fibonacci numbers F(n), printing
 * * "Buzz" when F(n) is divisible by 3.
 * * "Fizz" when F(n) is divisible by 5.
 * * "FizzBuzz" when F(n) is divisible by 15.
 * * "BuzzFizz" when F(n) is prime.
 * * the value F(n) otherwise.
 *
 * We encourage you to compose your code for this question in a way that represents the quality of
 * code you produce in the workplace - e.g. tests, documentation, linting, dependency management
 * (though there's no need to go [too] far).
 *
 * Please upload your code to GitHub or bitbucket and post a link to the repo or gist as your answer in the Lever form.
 */

/**
 * Finding primes is fun, but it can go on forever. Deciding where to stop is important.
 *
 * Non-deterministic primality tests have been used to check the output of this program
 * up to Fibonnaci: 354224848179261915075 which means that for all probable prime
 * F_n <= 354224848179261915075 generated, where mpz_probab_prime_p(p,50) == 1,
 * have been shown to accurately report primality. This is expected because, according to
 * the documentation on mpz_probab_prime_p(), with 50 passes the probability that a
 * composite is falsely reported to be prime is less than 1/(4^50). Very good odds.
 *
 * Because number, 3.5422+20, is so small, as Fibonnaci numbers go, the usefullness of
 * more complete deterministic prime checking is doubted, and is left as an excersize for
 * interested parties. See previous versions in this repo for clues.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include <gmp.h>


#define FIZZstr "Fizz"
#define BUZZstr "Buzz"

// needed prototypes
static void execute_request( const uint64_t n_F );

//! A quick and simple struct for holding command-line program options.
/** FizzBuzz should never be complex enough require complex option
 *  processing than this. Keep it simple.
 */
typedef struct {
    int zero_is_fib;                                                 /*!< if(zero_is_fib != 0) F_1 is zero */
    int verbose;                                                     /*!< 0 produces standar output greater values produce more, but non-standard output. */
    int supress_threeandfive_prime_report;                           /*!< 3 and 5 are prime, and divisible by 3 and 5. This adds reporting guidence. */
} prog_opts_t;

static prog_opts_t opts = {0};
static const char * progname = "fizzbuzz";

/** Prints a messsage which documents the use of the fizzbuzz binary, and terminates program execution.  */
static void print_usage_and_exit()
{
    fprintf( stderr, "usage: %s [options] [<n>]\n", progname );
    fprintf( stderr, "  options:\n" );
    fprintf( stderr, "    -? | -h   print this message and exit\n" );
    fprintf( stderr, "    -v        increase verbosity. -v may be specified many times\n" );
    fprintf( stderr, "    -z        use zero as the first Fibonacci number\n" );
    fprintf( stderr, "    -s3,5p    supress the report of 3 and 5 as primes\n" );
    fprintf( stderr, "    <n>       consider only the first <n> Fibonacci numbers. (default is to run forever)\n\n" );
    exit(0);
}

/** main() processes command-line arguments as documented in print_usage_and_exit() and runs the requested action. */
int main( int argc, char ** argv )
{
    uint64_t n_F = (uint64_t)-1;                                     // max(u64) means print F_n without limit
    int i = 0;

    progname = argv[0];

    for( i = 1; i < argc; i++ )
    {
        if( 0 == strcmp( argv[i], "-v" ) )   opts.verbose += 1;
        if( 0 == strcmp( argv[i], "-vv" ) )  opts.verbose += 2;
        if( 0 == strcmp( argv[i], "-vvv" ) ) opts.verbose += 3;

        if( 0 == strcmp( argv[i], "-z" ) )
            opts.zero_is_fib = 1;

        if( 0 == strcmp( argv[i], "-?" ) || 0 == strcmp( argv[i], "-h" ) )
            print_usage_and_exit();

        if( 0 == strcmp( argv[i], "-s3,5p" ) )
            opts.supress_threeandfive_prime_report = 1;

        if( isdigit( argv[i][0] ) )
            n_F = strtoull( argv[i], NULL, 0 );
    }

    if( n_F < 1 )                                                    // treat a request for zero Fibonacci
        n_F = (uint64_t)-1;                                          // numbers as a no-limit request

    execute_request( n_F );

    return 0;
}



/** \brief Advance the Fibonacci series by one step.
 * 
 * Preconditions:
 * - All parameters must be mpz_init initialized before this runs.
 * - F_n and F_nm1 are successive Fibonacci numbers, where F_n > F_nm1.
 *
 * Afterward, F_nm1 will be equal what F_n was set to at the start and
 * F_n will again be the next Fibonnaci number following F_nm1.  F_np1
 * has no use outside of fibn_step.
 *
 * \param F_np1  F_(n+1)    The initial value is ignored.
 * \param F_n    F_(n)
 * \param F_nm1  F_(n-1)
 */
static void fibn_step( mpz_t F_np1, mpz_t F_n, mpz_t F_nm1 )         // use 3 arguments to avoid initialization of mpz_t for each call
{
    mpz_add( F_np1, F_n, F_nm1 );                                    // np1 is a placeholder only
    mpz_set( F_nm1, F_n );                                           // nm1 and n are used to track the sequence
    mpz_set( F_n, F_np1 );
}

/** \brief Execute the FizzBuzz checks and produce output for one Fibonacci number.
 * 
 * \param n    The 'n' number used to name the Fibonacci number in the sequence.
 * \param F_n  An allocate Fibonacci number, to be evaluated and reported on.
 */
static void run_checks_and_report( const uint64_t n, const mpz_t F_n )
{
    int printFIZZ = mpz_divisible_ui_p( F_n, 5 );
    int printBUZZ = mpz_divisible_ui_p( F_n, 3 );

    if( opts.verbose > 1 ) printf("F_%lu: ", n);                     // useful info for checking the algorithm.
    if( opts.verbose > 2 ) gmp_printf( "%Zd: ", F_n );

    if( printFIZZ ) printf( "%s", FIZZstr );                         // handle the Fizz case first because if n is divisible by 3
    if( printBUZZ ) printf( "%s", BUZZstr );                         // also then Buzz is appended to Fizz, which also handles 15

    // F_ 1,2,3,4,5,6
    //    0,1,1,2,3,5
    //    1,1,2,3,5,8
    if(    n < 7                                                     // for F_n in {2,3,5,8}
        && n > 3                                                     // testing n instead of F_n is simple and fast
        && ! opts.supress_threeandfive_prime_report )                // and this block is all about reporting 3,5 as prime
    {
        if( printFIZZ || printBUZZ )                                 // divisibility by 3 or 5 also implies primality
            printf( " %s%s", BUZZstr, FIZZstr );                     // so report it as prime in addition to divisible
    }

    if( ! printFIZZ && ! printBUZZ )
    {
        int passes = 50;                                             // mpz_probab_prime_p() doc says 50 is upper reasonable number
        int primeclass = mpz_probab_prime_p( F_n, passes );          // if primeclass == 1; P( F_n is composite ) <= 1/(4^passes)

        switch( primeclass )
        {
        case 1:                                                      // Almost certainly prime.
            printf( "%s%s%s", BUZZstr, FIZZstr,                      // BuzzFizz,
                    (opts.verbose ? "*" : "") );                     // with verbose-optional caveat '*'
            break;
        case 2:                                                      // Definitely prime. BuzzFizz
            printf( "%s%s", BUZZstr, FIZZstr );
            break;
        default:                                                     // Definitely not prime and not divisible by 3 or 5. show F_n
            gmp_printf( "%Zd", F_n );
            break;
        }
    }

    printf( "\n" );
    fflush( stdout );
}

/** \brief Execute the FizzBuzz algorithm for each selected Fibonacci number.
 *
 * The Fibonacci sequence is generated, and each number is fed to the function:
 * run_checks_and_report(). execute_request() handles whether 0 is reported
 * as the first Fibonacci number, and it optionally terminates when these
 * operations have been executed for n_F Fibonacci numbers.
 * 
 * \param n_F  The 'n' of 'F_n', of the *final* Fibonacci number to be tested.
 *             If n_F == (uint64_t)-1 then the sequence runs until signaled.
 */
static void execute_request( const uint64_t n_F )
{
    uint64_t n = 1;
    mpz_t F_nm1, F_n, F_np1;                                         // Fibonacci numbers n-1, n, and n+1

    mpz_init_set_ui( F_nm1, 0 );                                     // handle 0,1,1,2,3,5 as special cases
    mpz_init_set_ui( F_n, 1 );
    mpz_init( F_np1 );

    if( opts.zero_is_fib )
    {
        run_checks_and_report( n, F_nm1 );
        n++;
    }

    for( ; n_F == (uint64_t)-1 || n <= n_F; n++ )                    // n_F == (u64)-1  means unbounded. (practical limit is lower.)
    {
        run_checks_and_report( n, F_n );
        fibn_step( F_np1, F_n, F_nm1 );
    }

    mpz_clear( F_nm1 );  // always cleanup
    mpz_clear( F_n );
    mpz_clear( F_np1 );
}

