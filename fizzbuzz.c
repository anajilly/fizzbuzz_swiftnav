#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include <gmp.h>


// the non-deterministic primality results have been checked up to Fibonnaci: 354224848179261915075

#define FIZZstr "Fizz"
#define BUZZstr "Buzz"

// quick, simple program options structure. FizzBuzz should never 
// be complex enough require complex option processing. keep it simple.
typedef struct {
    int zero_is_fib;
    int verbose;
    int use_deterministic_primes;
    int supress_threeandfive_prime_report;         // 3 and 5 are Fibonacci numbers, primes, and  divisible by 3 and 5
} prog_opts_t;
static prog_opts_t opts = {0};


// for deterministic prime testing.  a bad implementation.
static int is_deterministically_prime( mpz_t F_np1 );
static int is_prime__against_list( mpz_t candidate );
static int generate_primes_up_to( mpz_t max );
static void deallocate_primes();
static void initialize_primes();



static void fibn_step( mpz_t F_np1, mpz_t F_n, mpz_t F_nm1 )
{
    mpz_add( F_np1, F_n, F_nm1 );  // np1 is a placeholder only
    mpz_set( F_nm1, F_n );          // nm1 and n are used to track the sequence
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

        if( primeclass == 1 )                                        // primality is uknown. With 50 passes, however, the probability
        {                                                            // that it's not prime, according to docs, is ~ 7.9e-31
            if( opts.use_deterministic_primes )
            {
                if( is_deterministically_prime( F_n ) )
                    primeclass = 2;                                  // classification is changed
            }
        }

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

    if( opts.use_deterministic_primes )
        initialize_primes();

    if( opts.zero_is_fib )
    {
        run_checks_and_report( n, F_nm1 );
        n++;
    }

    for( ; n_F == (uint64_t)-1 || n <= n_F;  n++ )                   // n_F == (u64)-1  means unbounded. (practical limit is lower.)
    {
        run_checks_and_report( n, F_n );
        fibn_step( F_np1, F_n, F_nm1 );
    }

    mpz_clear( F_nm1 );  // always cleanup
    mpz_clear( F_n );
    mpz_clear( F_np1 );

    if( opts.use_deterministic_primes )
    {
        deallocate_primes();
    }

    return 0;
}

static const char * progname = "fizzbuzz";

static void print_usage_and_exit()
{
    fprintf( stderr, "usage: %s [options] [<n>]\n", progname );
    fprintf( stderr, "  options:\n" );
    fprintf( stderr, "    -? | -h   print this message and exit\n" );
    fprintf( stderr, "    -d        use a deterministic prime number check\n" );
    fprintf( stderr, "    -v        increase verbosity. -v may be specified many times\n" );
    fprintf( stderr, "    -z        use zero as the first Fibonacci number\n" );
    fprintf( stderr, "    -s3,5p    supress the report of 3 and 5 as primes\n" );
    fprintf( stderr, "    <n>       consider only the first <n> Fibonacci numbers. (default is to run forever)\n\n" );
    exit(0);
}

int main( int argc, char ** argv )
{
    uint64_t n_F = (uint64_t)-1;    // max(u64) means print F_n without limit
    int i = 0;

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

    if( n_F < 1 )
        n_F = (uint64_t)-1;    // treat a request for zero Fibonacci numbers as a no-limit request

    if( opts.verbose > 1 && n_F < (uint64_t)-1 )
    {
        mpz_t expected_largest_fib;
        mpf_t expected_largest_fib_d;

        mpz_init( expected_largest_fib );
        mpf_init_set_d( expected_largest_fib_d, 1.61803398875 );
        mpf_pow_ui( expected_largest_fib_d, expected_largest_fib_d, n_F - (2 + opts.zero_is_fib));
        mpz_set_f( expected_largest_fib, expected_largest_fib_d );
        mpf_sqrt( expected_largest_fib_d, expected_largest_fib_d );

        gmp_printf( "approx(F_%u) will be approximately: %Zd.\nsqrt( approx(F_%u) ) == %Ff\n",
                n_F, expected_largest_fib, n_F, expected_largest_fib_d );

        mpz_clear( expected_largest_fib );
        mpf_clear( expected_largest_fib_d );
    }

    return execute_request( n_F );
}



// this is a simple list struct made for naive but deterministic primality testing
typedef struct s_mpz_list s_mpz_list_t;
struct s_mpz_list
{
    s_mpz_list_t * next;
    mpz_t v;
};
static s_mpz_list_t primes;
static mpz_t max_prime_in_list;

static int is_deterministically_prime( mpz_t F_np1 )
{
    int isprime = 0;
    static int first_run = 1;
    static mpz_t F_np1_sqrt;

    if( first_run )
    {
        mpz_init( F_np1_sqrt );                         // doing this frequently is slow. so make it static
        first_run = 0;                                   // and deal with a tiny bit of uncool mess.
    }

    if( mpz_root( F_np1_sqrt, F_np1, 2 ) )             // if the result of this sqrt() isn't exact,
        mpz_add_ui( F_np1_sqrt, F_np1_sqrt, 2 );       // add two to round up, maintaining even/odd.

    if( mpz_cmp( max_prime_in_list, F_np1_sqrt ) < 0 )  // if the max is less than the sqrt, generate more primes.
    {
        if( generate_primes_up_to( F_np1_sqrt ) )       // if generate_primes_up_to returns an error, we maxed out memory
        {
            opts.use_deterministic_primes = 0;          // so we need to stop trying deterministic checks
            deallocate_primes();                        // this list is no longer useful and is taking up space
            return 0;
        }
    }

    if( mpz_cmp( max_prime_in_list, F_np1_sqrt ) >= 0 ) // if the max no no less than the sqrt, do the deterministic check
    {
        if( is_prime__against_list( F_np1 ) )
            isprime = 1;
    }

    return isprime;
}


static int is_prime__against_list( mpz_t candidate )
{
    int iscomposite = 0;

    s_mpz_list_t * p = &primes;

    do {
        iscomposite = mpz_divisible_p( candidate, p->v );
        if( iscomposite )
            p = p->next;
    } while( iscomposite && p );

    return ! iscomposite;
}

static int generate_primes_up_to( mpz_t max )
{
    int primes_count = 0;
    int error = 0;
    s_mpz_list_t * p = &primes;

    do {
        if( mpz_cmp( max, p->v ) < 0 )
            break;

        if( p->next )
        {
            p = p->next;
            primes_count++;
        }
        else
        {
            p->next = calloc( 1, sizeof(s_mpz_list_t) );
            if( p->next )
            {
                s_mpz_list_t * np = p->next;

                mpz_nextprime( np->v, p->v );                // this probability of the result being composite is very low.
                while( ! is_prime__against_list( np->v ) )   // but we're generating a deterministic check list, so check it.
                    mpz_nextprime( np->v, np->v );

                mpz_init_set( max_prime_in_list, np->v );    // once we get here, we're sure that our result is correct
                p = np;

                primes_count++;
            }
            else
                error = -1;
        }

        if( opts.verbose )
        {
            printf( "\rprimes in list: %d", primes_count );
            fflush( stdout );
        }
    }
    while( p && ! error );

    if( opts.verbose )
        printf( "\n" );

    return error;
}

static void deallocate_primes()
{
    s_mpz_list_t * p = primes.next;

    while( p )
    {
        s_mpz_list_t * n = p->next;

        mpz_clear( p->v );
        free( p );

        if( n )
            p = n;
    }

    mpz_clear( primes.v );
    primes.next = NULL;  // list is empty
}

// just add the first 3 primes to our list.
static void initialize_primes()
{
    s_mpz_list_t * p = &primes;

    mpz_init_set_ui( p->v, 2 );

    p->next = calloc( 1, sizeof(s_mpz_list_t) );
    p = p->next;
    mpz_init_set_ui( p->v, 3 );

    p->next = calloc( 1, sizeof(s_mpz_list_t) );
    p = p->next;
    mpz_init_set_ui( p->v, 5 );

    mpz_init_set( max_prime_in_list, p->v );
}

