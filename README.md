# fizzbuzz_swiftnav
###FizzBuzz for Swift Navigation Application Process

This is a modified FizzBuzz algorithm. The usual rules for the game apply, with the exceptions that only Fibonacci numbers are considered, and when a number is prime it should be reported with "BuzzFizz" instead of the number itself. The exact, given specification is as follows:

> In the programming language of your choice, write a program generating the first n Fibonacci numbers F(n), printing
>
> * "Buzz" when F(n) is divisible by 3.
> * "Fizz" when F(n) is divisible by 5.
> * "FizzBuzz" when F(n) is divisible by 15.
> * "BuzzFizz" when F(n) is prime.
> * the value F(n) otherwise.

The chosen language is C.

The default implementation attempts to follow the spec as closely as possible and so the default options employ the following assumptions. These defaults can be over-ridden by command line options.

#### Output Formatting Commandline Options
* **-z** Classically, the Fibonacci sequence begins with 1, thus by default F\_1 == 1 (F\_0 is not defined). Use the option **-z** to make F\_1 == 0.
* **-s3,5p** 3 and 5 are divisible by 3 and 5 respectively, of course, but also prime. Because the specification doesn't imply the different reporting options are mutually exclusive, by default, the divisibility of these is reported first, followed immediately by their primality reports (same order as the requirement is listed in the spec). The Divisibilty reports are not supressible as this aspect of these particular numbers, and their direct naming in the specificiation, is very difficult to dismiss. Primality reports for 3 and 5 may be supressed, however. To do this, use the **-s3,5p** option.
* **-lf** By default there are no seperators between reports.  So, for example, the output begins: "11BuzzFizzBuzzBuzzFizzFizzBuzzFizz" for 1,1,2,3,5. To insert a linefeed between reports, use the **-lf** option. Note that use of the **-lf** option also results in the insertion of a space character between the divisiblity report and primality report for 3 and 5. See examples below.

#### Other Commandline Options:
* **-h**  print a helpful message
* **-?**  see: -h
* **-v**  Increase verbosity. This causes the program to abandon the spec, but this can be useful. '-v' causes F_%d to be printed for every report. '-v -v' causes the Fibonacci number itself to be printed as part of every report also.
* **-a**  The primality test is probabalistic rather than deterministic, which allows for efficient testing of very large candidate primes. If one wishes to see an indication of potential false-positive primality reports, use -a to add an asterisk to the BuzzFizz report, like so: "BuzzFizz*".  Note that according to (gmplib documentation on this primality test)[https://gmplib.org/manual/Number-Theoretic-Functions.html#Number-Theoretic-Functions], the probability that a composite is mistakenly reported to be prime is less than 1/(4^50), a number obtained from the fact that each possible prime is tested 50 times (each goes through 50 passes). This reported statistic implies that each pass is an independent trial, a doubtable claim. However, even if the actual probability of this claim were underestimated by a factor of 10^20, the probability of even one false-positive in the first 50,000 prime reports is 3.94e-6. A very comfortably low probability. Because of this, especially with respect to FizzBuzz, there is not compelling case to be made _for_ a truly deterministic primality test.
* &lt;n&gt; Finally, the number of Fibonacci numbers to test is given as an integer. If no integer is given then the sequence runs until it receives a Ctrl+C or is otherwise killed.

The purpose of providing these options, in the absence of a perfectly clear specification, is that the program will be flexible enough that the desired format should be obtainable.


--

Build Requirements:

* (CMake)[https://cmake.org/] and a standard Linux-style build environment should be installed and working
* gmplib, the (GNU Multiple Precision Arithmetic Library)[https://gmplib.org/] and its 'dev' packages, must be installed


Build and Test:

1. Checkout this repository
2. cd fizzbuzz\_swiftnav/
3. mkdir build
4. cd build
5. cmake ../
6. make


After the above steps are complete, you should find an executable file named fizzbuzz in the current directory. This program accepts zero or more command-line arguments. For usage documentation, please run the program with the -h option.

```
ana@trifle:~/code/experiments/fizzbuzz\_swiftnav/build (dev)$ ./fizzbuzz -h
usage: ./fizzbuzz [options] [&lt;n&gt;]
  options:
    -? | -h   print this message and exit
    -v        increase verbosity. -v may be specified many times
    -z        use zero as the first Fibonacci number
    -s3,5p    supress the report of 3 and 5 as primes
    -lf       add linefeed after each F\_n report
    -a        add an asterisk after each BuzzFizz which is not deterministically prime
    &lt;n&gt;     consider only the first &lt;n&gt; Fibonacci numbers. (The default is to run until Ctrl+C is pressed)
```

Running a quick test of the first few Fibonacci numbers yields the following. Note that zero is not classified as a Fibonacci number in this run and numbers 3 and 5, because they're both prime and are divisible by 3 and 5 respectively, report both the divisibility and the primality.

```
ana@trifle:~/code/experiments/fizzbuzz\_swiftnav/build (dev)$ ./fizzbuzz -lf 10
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

...or, without the '-lf' option:
```
ana@trifle:~/code/experiments/fizzbuzz\_swiftnav/build (dev)$ ./fizzbuzz 10 ; echo
11BuzzFizzBuzzBuzzFizzFizzBuzzFizz8BuzzFizzBuzz34Fizz
```


Here we specify that zero is to be recognized as the first prime and 3 and 5 should not be reported as prime. Note that zero divides all integers, so it is reported as divisible by 15.
```
ana@trifle:~/code/experiments/fizzbuzz\_swiftnav/build (dev)$ ./fizzbuzz -lf -z -s3,5p 10
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
```

Here is a longer run that uses one verbosity flag, the effect of which is to prepend the line with the Fibonacci number name. `grep` is used to trim the output to the interesting piece of this result.
```
ana@trifle:~/code/experiments/fizzbuzz\_swiftnav/build (dev)$ time ./fizzbuzz -lf -z -v 3000 | grep BuzzFizz
F\_4: BuzzFizz
F\_5: Buzz BuzzFizz
F\_6: Fizz BuzzFizz
F\_8: BuzzFizz
F\_12: BuzzFizz
F\_14: BuzzFizz
F\_18: BuzzFizz
F\_24: BuzzFizz
F\_30: BuzzFizz
F\_44: BuzzFizz
F\_48: BuzzFizz
F\_84: BuzzFizz
F\_132: BuzzFizz
F\_138: BuzzFizz
F\_360: BuzzFizz
F\_432: BuzzFizz
F\_434: BuzzFizz
F\_450: BuzzFizz
F\_510: BuzzFizz
F\_570: BuzzFizz
F\_572: BuzzFizz
F\_2972: BuzzFizz

real	0m1.418s
user	0m1.416s
sys	0m0.004s
```

And the same run again with two visiblity flags so that the Fibonacci number is question is printed for every report, and the -a flag to marks BuzzFizz with an asterisk whenever the primality is not strictly guaranteed.
```
ana@trifle:~/code/experiments/fizzbuzz\_swiftnav/build (dev)$ time ./fizzbuzz -lf -z -vv -a 3000 | grep BuzzFizz
F\_4: 2: BuzzFizz
F\_5: 3: Buzz BuzzFizz
F\_6: 5: Fizz BuzzFizz
F\_8: 13: BuzzFizz
F\_12: 89: BuzzFizz
F\_14: 233: BuzzFizz
F\_18: 1597: BuzzFizz
F\_24: 28657: BuzzFizz
F\_30: 514229: BuzzFizz
F\_44: 433494437: BuzzFizz*
F\_48: 2971215073: BuzzFizz*
F\_84: 99194853094755497: BuzzFizz*
F\_132: 1066340417491710595814572169: BuzzFizz*
F\_138: 19134702400093278081449423917: BuzzFizz*
F\_360: 475420437734698220747368027166749382927701417016557193662268716376935476241: BuzzFizz*
F\_432: 529892711006095621792039556787784670197112759029534506620905162834769955134424689676262369: BuzzFizz*
F\_434: 1387277127804783827114186103186246392258450358171783690079918032136025225954602593712568353: BuzzFizz*
F\_450: 3061719992484545030554313848083717208111285432353738497131674799321571238149015933442805665949: BuzzFizz*
F\_510: 10597999265301490732599643671505003412515860435409421932560009680142974347195483140293254396195769876129909: BuzzFizz*
F\_570: 36684474316080978061473613646275630451100586901195229815270242868417768061193560857904335017879540515228143777781065869: BuzzFizz*
F\_572: 96041200618922553823942883360924865026104917411877067816822264789029014378308478864192589084185254331637646183008074629: BuzzFizz*
F\_2972: 357103560641909860720907774139063454445569926582843306794041997476301071102767570483343563518510007800304195444080518562630900027386498933944619210192856768352683468831754423234217978525765921040747291316681576556861490773135214861782877716560879686368266117365351884926393775431925116896322341130075880287169244980698837941931247516010101631704349963583400361910809925847721300802741705519412306522941202429437928826033885416656967971559902743150263252229456298992263008126719589203430407385228230361628494860172129702271172926469500802342608722006420745586297267929052509059154340968348509580552307148642001438470316229: BuzzFizz*

real	0m1.274s
user	0m1.268s
sys	0m0.008s
```
--

The (GNU Multiple Precision Arithmetic Library)[https://gmplib.org/] makes these extremely large numbers easy to work with.
