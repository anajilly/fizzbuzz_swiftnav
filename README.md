# fizzbuzz_swiftnav
Fizz Buzz for Swift Navigation Application Process

This is a modified FizzBuzz algorithm which checks only Fibonacci numbers and prints BuzzFizz if the number is a prime. If the number is divisible by 15 then FizzBuzz is printed. If divisible by 3 then Buzz is printed and if divisible by 5 then Fizz is printed. The choice of programming language used is left to the programmer. This is essentially the whole specification of the task, thus clearly there are many details left unspecified.

For instance, 3 and 5 are both divisible by 3 and 5 respectively, but are also prime numbers. The question naturally arises, should one of the two possible interpretations of the specification take precedence, or should all applicable actions be executed? I have yet to ask this question and hesitate to do so because it's silly and can be handled using a program option. Perhaps this is part of the test though? This is an aspect of the spec which is decidedly unclear.

Also, because limits are not specified, one presumes this program should be constrained by natural system limits of memory and processor speed (i.e. time), instead of by some arbitrary limit having to do with variable type sizes, etc.. To all other unanswered questions, one naturally presumes that the typical FizzBuzz idioms apply, such as each printed object should occupy its own line, etc..  Because the project is so simple, and easily modified to meet some more precise specification, it is assumed that we can begin with the most straightforward implementation and then apply corrections if needed.

The focus of the work, as indicate by Swift Navigation, will be to create a project which is maintainable, portable, and how easy to use by 3rd parties. Clues in the specification page indicate that the amount of work put into this project should be in line with the expected complexity of the project itself. As FizzBuzz is simple by definition, and even this modified FizzBuzz involving Fibonacci numbers and primes is still simple, the corresponding amount of effort should be minimal, but adequate.

I've made this project public, so far, because I don't remember there being a requirement that it be private. I will likely make this a private project before long, however. Regardless of its visibility, one must presume that if you, the reader, wish to be considered for a position at Swift Navigation, that you should implement your own version of this project from scratch.

The below instructions indicate the plan for this repository and, as of yet, will not function as advertised. Expect this to change later today.

--

Build Requirements:

* cmake and a standard Linux-style build environment should be installed and working
* libgmp, the (GNU Multiple Precision Arithmetic Library)[https://gmplib.org/] and its 'dev' packages, must be installed


Build and Test:

1. Checkout this repository
2. cd fizzbuzz\_swiftnav/
3. mkdir build
4. cmake ../
5. make
6. make test

After the above steps are complete, you should find an executable file named fizzbuzz in the current directory. This program accepts zero or more command-line arguments. For usage documentation, please run the program with the -h option. This is what you should see:

```
ana@trifle:~/code/fizzbuzz\_swiftnav/build$ ./fizzbuzz -h
usage: ./fizzbuzz [-?|-h] [-d] [-v | [-v -v | [...]]] [-z] [<n>]
    -? | -h   print this message and exit
    -d        use a deterministic prime number check
    -v        increase verbosity. each added -v increases verbosity more
    -z        use zero as the first Fibonacci number
   <n>        consider the first <n> Fibonacci numbers. (otherwise run without limit)
```

