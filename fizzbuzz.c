#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include <gmp.h>

#define ENABLE_DETERMINISTIC_PRIMES 1

// the non-deterministic primality results have been checked up to Fibonnaci: 354224848179261915075

#define FIZZstr "Fizz"
#define BUZZstr "Buzz"

// quick, simple program options structure. FizzBuzz should never
// be complex enough require complex option processing. keep it simple.
typedef struct {
    int zero_is_fib;
    int verbose;
    int use_deterministic_primes;
    int supress_threeandfive_prime_report;                           // 3 and 5 are Fibonacci numbers, primes, and divisible by 3 and 5
} prog_opts_t;
static prog_opts_t opts = {0};


#ifdef ENABLE_DETERMINISTIC_PRIMES
// for deterministic prime testing.  a bad implementation.
static int allocate_primes_list();
static void free_primes_list();
static int is_deterministically_prime( mpz_t F_n );
static int is_prime__against_list( mpz_t candidate );
static int generate_primes_up_to( mpz_t max );
#endif



static void fibn_step( mpz_t F_np1, mpz_t F_n, mpz_t F_nm1 )         // use 3 arguments to avoid initialization of mpz_t for each call
{
    mpz_add( F_np1, F_n, F_nm1 );                                    // np1 is a placeholder only
    mpz_set( F_nm1, F_n );                                           // nm1 and n are used to track the sequence
    mpz_set( F_n, F_np1 );
}

static void run_checks_and_report( uint64_t n, mpz_t F_n )
{
    int printFIZZ = mpz_divisible_ui_p( F_n, 5 );
    int printBUZZ = mpz_divisible_ui_p( F_n, 3 );

    if( opts.verbose > 0 ) printf("F_%lu: ", n);                     // useful info for checking the algorithm.
    if( opts.verbose > 1 ) gmp_printf( "%Zd: ", F_n );

    if( printFIZZ ) printf( "%s", FIZZstr );                         // handle the Fizz case first because if n is divisible by 3
    if( printBUZZ ) printf( "%s", BUZZstr );                         // also then Buzz is appended to Fizz, which handles 15 also


    // 0,1,1,2,3,5
    // 1,1,2,3,5,8
    if( n < 7 && n > 3 && ! opts.supress_threeandfive_prime_report ) // for F_n in {2,3,5,8} (testing n is simple and fast check)
    {
        if( printFIZZ || printBUZZ )                                 // divisibility by 3 or 5 also implies primality
            printf( " %s%s", BUZZstr, FIZZstr );                     // so report it as prime in addition to divisible
    }

    if( ! printFIZZ && ! printBUZZ )
    {
        int passes = 50;                                             // mpz_probab_prime_p() doc says 50 is upper reasonable number
        int primeclass = mpz_probab_prime_p( F_n, passes );          // if primeclass == 1; P( F_n is composite ) <= 1/(4^passes)

#ifdef ENABLE_DETERMINISTIC_PRIMES
        if( primeclass == 1 )                                        // primality is uknown. With 50 passes, however, the probability
        {                                                            // that it's not prime, according to docs, is ~ 7.9e-31
            if( opts.use_deterministic_primes )
            {
                if( is_deterministically_prime( F_n ) )
                    primeclass = 2;                                  // classification is changed
            }
        }
#endif

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

static int execute_request( uint64_t n_F )
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

    return 0;
}

static const char * progname = "fizzbuzz";

static void print_usage_and_exit()
{
    fprintf( stderr, "usage: %s [options] [<n>]\n", progname );
    fprintf( stderr, "  options:\n" );
    fprintf( stderr, "    -? | -h   print this message and exit\n" );
#ifdef ENABLE_DETERMINISTIC_PRIMES
    fprintf( stderr, "    -d        use a deterministic prime number check\n" );
#endif
    fprintf( stderr, "    -v        increase verbosity. -v may be specified many times\n" );
    fprintf( stderr, "    -z        use zero as the first Fibonacci number\n" );
    fprintf( stderr, "    -s3,5p    supress the report of 3 and 5 as primes\n" );
    fprintf( stderr, "    <n>       consider only the first <n> Fibonacci numbers. (default is to run forever)\n\n" );
    exit(0);
}

int main( int argc, char ** argv )
{
    uint64_t n_F = (uint64_t)-1;                                     // max(u64) means print F_n without limit
    int i = 0;
    int retval = 0;

    progname = argv[0];

    for( i = 1; i < argc; i++ )
    {
        if( 0 == strcmp( argv[i], "-d" ) )
            opts.use_deterministic_primes = 1;

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

    // here, we run quick checks to see how many primes we might need
    // if a deterministic check is desired.  deterministic checks are
    // not supported when the set of F_n is unbounded.
    if( n_F < (uint64_t)-1 )
    {
        mpz_t expected_largest_fib;
        mpf_t expected_largest_fib_d;
        mpz_t sqrt_expected_largest_fib;

        mpz_init( expected_largest_fib );
        mpz_init( sqrt_expected_largest_fib );
        mpf_init_set_d( expected_largest_fib_d, 1.61803398875 );

        mpf_pow_ui( expected_largest_fib_d, expected_largest_fib_d, n_F - (2 + opts.zero_is_fib));
        mpz_set_f( expected_largest_fib, expected_largest_fib_d );
        mpz_sqrt( sqrt_expected_largest_fib, expected_largest_fib );
        mpz_add_ui( sqrt_expected_largest_fib, sqrt_expected_largest_fib, 1 );  // we don't want the truncate sqrt

        if( opts.verbose > 1 )
        {
            gmp_printf( "approx(F_%u) will be approximately: %Zd.\nsqrt( approx(F_%u) ) == %Zd\n",
                    n_F, expected_largest_fib, n_F, sqrt_expected_largest_fib );
        }

#ifdef ENABLE_DETERMINISTIC_PRIMES
        if( opts.use_deterministic_primes && allocate_primes_list() )
        {
            fprintf( stderr, "error: failed to allocate primes list. disabling deterministic prime checking -- %m\n" );
            opts.use_deterministic_primes = 0;
        }
#endif

        mpz_clear( expected_largest_fib );
        mpf_clear( expected_largest_fib_d );
        mpz_clear( sqrt_expected_largest_fib );
    }

    retval = execute_request( n_F );

#ifdef ENABLE_DETERMINISTIC_PRIMES
    if( opts.use_deterministic_primes )
        free_primes_list();
#endif

    return retval;
}



#ifdef ENABLE_DETERMINISTIC_PRIMES
// this is a simple list struct made for naive but deterministic primality testing
typedef struct 
{
    mpz_t comp;
    mpz_t min;
} mpz_touple_t ;
static mpz_touple_t * primes = NULL;

static int primes_slots_taken = 0;
static int primes_maxed_out = 0;
static mpz_t max_prime_in_list;

#define PRIMES_LIST_SIZE (4 * 1024 * 1024)

static int allocate_primes_list()
{
    primes = malloc( PRIMES_LIST_SIZE * sizeof(mpz_touple_t) );
    if( ! primes )
        return -1;

    primes_slots_taken = 0;
    return 0;
}

static void free_primes_list()
{
    if( ! primes )
        return;

    while( primes_slots_taken > 0 )
    {
        primes_slots_taken--;
        mpz_clear( primes[primes_slots_taken].min );
        mpz_clear( primes[primes_slots_taken].comp );
    }

    free( primes );
    primes = NULL;
}

static int is_deterministically_prime( mpz_t F_n )
{
    int isprime = 0;
    static int first_run = 1;
    static mpz_t F_n_sqrt;

    if( first_run )
    {
        mpz_init( F_n_sqrt );                                        // doing this frequently is slow. so make it static
        first_run = 0;                                               // and deal with a tiny bit of uncool mess.
    }

    if( mpz_root( F_n_sqrt, F_n, 2 ) )                               // if the result of this sqrt() isn't exact,
        mpz_add_ui( F_n_sqrt, F_n_sqrt, 2 );                         // add two to round up, maintaining even/odd.

    if( mpz_cmp( max_prime_in_list, F_n_sqrt ) < 0 )                 // if the max is less than the sqrt, generate more primes.
    {
        if( generate_primes_up_to( F_n_sqrt ) )                      // if generate_primes_up_to returns an error, we maxed out memory
        {
            fprintf( stderr, "error: generate_primes_up_to() returned an error. freeing primes list and turning off deterministic primality checks -- %m\n" );
            if( opts.verbose ) gmp_printf( "max deterministic prime found: %Zd\n",  max_prime_in_list );

            opts.use_deterministic_primes = 0;                       // so we need to stop trying deterministic checks
            free_primes_list();                                      // the primes list is no longer useful. free it
            return 0;
        }
    }

    if( mpz_cmp( max_prime_in_list, F_n_sqrt ) >= 0 )                // if the max no less than the sqrt, do the deterministic check
    {
        if( is_prime__against_list( F_n ) )
            isprime = 1;
    }

    return isprime;
}


static int is_prime__against_list( mpz_t candidate )
{
    int iscomposite = 0;
    int i;

    static int first_run = 1;
    static mpz_t candidate_sqrt;
    static mpz_t gcd;

    assert( primes != NULL );

    if( first_run )
    {
        first_run = 0;
        mpz_init( candidate_sqrt );
        mpz_init( gcd );
    }

    mpz_add_ui( candidate_sqrt, candidate, 1 );
    mpz_sqrt( candidate_sqrt, candidate_sqrt );

    for( i = 0; ! iscomposite && i < primes_slots_taken && mpz_cmp( primes[i].min, candidate_sqrt ) < 0; i++ )
    {
        mpz_gcd( gcd, candidate, primes[i].comp );
        iscomposite = ( mpz_cmp_ui( gcd, 1 ) > 0 );  // if gcd > 1
    }

    return ! iscomposite;
}

static int generate_primes_up_to( mpz_t max )
{
    int error = 0;
    int primes_added = 0;
    mpz_t nextprime;

    if( primes_maxed_out )
        return -1;

    mpz_init( nextprime );

    if( primes_slots_taken == 0 )
    {
        mpz_init_set_ui( primes[primes_slots_taken].min, 2 );
        mpz_init_set_ui( primes[primes_slots_taken].comp, 2*3*5*7*11*13 );

        mpz_init_set_ui( max_prime_in_list, 13 );

        primes_slots_taken++;
    }

    if( opts.verbose > 2 )
        printf( "\n" );

    do {
        if( mpz_cmp( max, max_prime_in_list ) < 0 )
            break;

        mpz_nextprime( nextprime, max_prime_in_list );               // this probability of the result being composite is very low.

        while( ! is_prime__against_list( nextprime ) )           // but we're generating a deterministic check list, so check it.
            mpz_nextprime( nextprime, nextprime );

        primes_added++;

        if( 500 < mpz_sizeinbase( primes[primes_slots_taken-1].comp, 10 ) )  // approximately how many base-10 digits?
        {
            if( primes_slots_taken >= PRIMES_LIST_SIZE )
            {
                primes_added--;
                error = -1;
            }
            else
            {
                mpz_init_set( primes[primes_slots_taken].comp, nextprime );
                mpz_init_set( primes[primes_slots_taken].min, nextprime );
                mpz_init_set( max_prime_in_list, nextprime );
                primes_slots_taken++;
            }
        }
        else
        {
            mpz_mul( primes[primes_slots_taken-1].comp, primes[primes_slots_taken-1].comp, nextprime );
            mpz_init_set( max_prime_in_list, nextprime );
        }

        if( opts.verbose > 2 )
        {
            printf( "\rprimes added to list: %d", primes_added );
            fflush( stdout );
        }
    }
    while( ! error );

    if( opts.verbose > 2 )
        printf( "\n" );

    mpz_clear( nextprime );

    return error;
}

#endif /* #ifdef ENABLE_DETERMINISTIC_PRIMES */
