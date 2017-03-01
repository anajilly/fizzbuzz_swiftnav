# fizzbuzz_swiftnav
###FizzBuzz for Swift Navigation Application Process

--

####Quick Start

#####Build Requirements:

* [CMake](https://cmake.org/) and a standard Linux-style build environment should be installed and working
* gmplib, the [GNU Multiple Precision Arithmetic Library](https://gmplib.org/) and its 'dev' packages, must be installed


#####Build Instructions:

1. Checkout this repository
2. cd fizzbuzz\_swiftnav/
3. mkdir build
4. cd build
5. cmake ../
6. make


After the above steps are complete, you should find an executable file named fizzbuzz in the current directory. This program accepts zero or more command-line arguments. For usage documentation, please run the program with the -h option.

```
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ ./fizzbuzz -h
usage: ./fizzbuzz [options] [<n>]
  options:
    -? | -h   print this message and exit
    -v        increase verbosity. -v may be specified many times
    -z        use zero as the first Fibonacci number
    -s3,5p    suppress the report of 3 and 5 as primes
    -lf       add linefeed after each F_n report
    -a        add an asterisk after BuzzFizz when the report, with very low probability, may be inaccurate
    <n>       consider only the first <n> Fibonacci numbers. (The default is to run until Ctrl+C is pressed)
```

--

####Modified FizzBuzz

This is a modified FizzBuzz algorithm. The usual rules for the game apply, with the exceptions that only Fibonacci numbers are considered, and when a number is prime it should be reported with "BuzzFizz" instead of the number itself. The full, given specification is as follows:

> In the programming language of your choice, write a program generating the first n Fibonacci numbers F(n), printing
>
> * "Buzz" when F(n) is divisible by 3.
> * "Fizz" when F(n) is divisible by 5.
> * "FizzBuzz" when F(n) is divisible by 15.
> * "BuzzFizz" when F(n) is prime.
> * the value F(n) otherwise.

The chosen language is C.

Because the spec may be subject to liberal interpretation, this program is implemented with command line options for the purpose of obtaining outputs which match the different possible interpretations. The rationale is that the true desired output should be obtainable through the use of different program options and that regardless of what that desired output is, the availability of different formats will likely be seen as useful by some. The default output attempts to produce maximally literal interpretation of the spec, using these assumptions:

1. The union criterion. For each F(n), all matching rules will be applied, as a union (i.e. without mutual exclusion). For example: because 3 and 5 are prime, the default produces both the divisibility output AND the primality output.
2. In absence of other guidance, unioned rules are applied in an order that is implied by the order of the rules given in the specification.
3. A concise output is preferred over a verbose output. When a number is divisible by 15, we merely invoke both the divisible by 3 and divisible by 5 rules in the order needed to produce the specified divisible by 15 output. In this way the union criterion is applied, producing the minimal output that satisfies all applicable rules.
4. As there is no mention of a limit to F(n)'s 'n' in the specification, an effort is made to remove artificial limits to n.
5. Because no standard record-separating characters were mentioned by the spec, none are used in the default output.
6. Classically, the Fibonacci sequence was defined to start with F(1) == 1. The convention is followed by default.

These are the assumptions made which are used in the *default* output. To achieve a different output, command-line options are available, and are listed above.

--

####Examples:
See above for an example of running with the **-h** option which explains all the command line options.

#####fizzbuzz default output for the first 10 Fibonacci numbers
Note that, for clarity, 'echo' is appended on this command-line to add a linefeed after fizzbuzz terminates.
```
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ ./fizzbuzz 10 ; echo
11BuzzFizzBuzzBuzzFizzFizzBuzzFizz8BuzzFizzBuzz34Fizz
```

#####fizzbuzz output for the first 10 Fibonacci numbers with report separators inserted.
Note that each response to a given F(n) is called a "report." When **-lf** is requested, a linefeed is inserted between the reports for each given F(n). When more than one report is indicated for a given F(n), all are given on one line, each separated by a single space character. As mentioned earlier, the chosen order for multiple reports given on one line is the order they're listed in the spec.
```
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ ./fizzbuzz -lf 10
1
1
BuzzFizz
Buzz BuzzFizz
Fizz BuzzFizz
8
BuzzFizz
Buzz
34
Fizz
```

#####fizzbuzz with other options and more outputs
Here we look at the first 12, specify that zero is to be recognized as the first Fibonacci, and that 3 and 5 should not be reported as prime. Note that zero divides all integers, so it is reported as divisible by 15.
```
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ ./fizzbuzz -lf -z -s3,5p 12
FizzBuzz
1
1
BuzzFizz
Buzz
Fizz
8
BuzzFizz
Buzz
34
Fizz
BuzzFizz
```

#####fizzbuzz verbosity flags


Here is the same 12 numbers as above but with one verbosity flag, and again with two verbosity flags. The effect is to prepend the line with the Fibonacci number name, and with two, the F(n) itself is shown.
```
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ ./fizzbuzz -lf -v -z -s3,5p 12
F_1: FizzBuzz
F_2: 1
F_3: 1
F_4: BuzzFizz
F_5: Buzz
F_6: Fizz
F_7: 8
F_8: BuzzFizz
F_9: Buzz
F_10: 34
F_11: Fizz
F_12: BuzzFizz
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ ./fizzbuzz -lf -v -v -z -s3,5p 12
F_1: 0: FizzBuzz
F_2: 1: 1
F_3: 1: 1
F_4: 2: BuzzFizz
F_5: 3: Buzz
F_6: 5: Fizz
F_7: 8: 8
F_8: 13: BuzzFizz
F_9: 21: Buzz
F_10: 34: 34
F_11: 55: Fizz
F_12: 89: BuzzFizz
```

Here is a longer run that uses the -a flag, which marks BuzzFizz with an asterisk whenever the primality is not strictly guaranteed. (See NOTES.md for more info about primality test.) Note that `grep` is used to limit the output to what is most interesting. 
```
ana@trifle:~/code/experiments/fizzbuzz_swiftnav/build (dev)$ time ./fizzbuzz -lf -z -vv -a 3000 | grep BuzzFizz
F_4: 2: BuzzFizz
F_5: 3: Buzz BuzzFizz
F_6: 5: Fizz BuzzFizz
F_8: 13: BuzzFizz
F_12: 89: BuzzFizz
F_14: 233: BuzzFizz
F_18: 1597: BuzzFizz
F_24: 28657: BuzzFizz
F_30: 514229: BuzzFizz
F_44: 433494437: BuzzFizz*
F_48: 2971215073: BuzzFizz*
F_84: 99194853094755497: BuzzFizz*
F_132: 1066340417491710595814572169: BuzzFizz*
F_138: 19134702400093278081449423917: BuzzFizz*
F_360: 475420437734698220747368027166749382927701417016557193662268716376935476241: BuzzFizz*
F_432: 529892711006095621792039556787784670197112759029534506620905162834769955134424689676262369: BuzzFizz*
F_434: 1387277127804783827114186103186246392258450358171783690079918032136025225954602593712568353: BuzzFizz*
F_450: 3061719992484545030554313848083717208111285432353738497131674799321571238149015933442805665949: BuzzFizz*
F_510: 10597999265301490732599643671505003412515860435409421932560009680142974347195483140293254396195769876129909: BuzzFizz*
F_570: 36684474316080978061473613646275630451100586901195229815270242868417768061193560857904335017879540515228143777781065869: BuzzFizz*
F_572: 96041200618922553823942883360924865026104917411877067816822264789029014378308478864192589084185254331637646183008074629: BuzzFizz*
F_2972: 357103560641909860720907774139063454445569926582843306794041997476301071102767570483343563518510007800304195444080518562630900027386498933944619210192856768352683468831754423234217978525765921040747291316681576556861490773135214861782877716560879686368266117365351884926393775431925116896322341130075880287169244980698837941931247516010101631704349963583400361910809925847721300802741705519412306522941202429437928826033885416656967971559902743150263252229456298992263008126719589203430407385228230361628494860172129702271172926469500802342608722006420745586297267929052509059154340968348509580552307148642001438470316229: BuzzFizz*

real	0m1.274s
user	0m1.268s
sys	0m0.008s
```
--

The [GNU Multiple Precision Arithmetic Library](https://gmplib.org/) makes working with these extremely large numbers remarkably easy.
