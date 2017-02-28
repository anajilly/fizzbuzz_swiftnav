#Notes

###Primality testing

As mentioned in the README document, an effort is made to allow a truly arbitrary 'n' to be selected for the largest F(n). Testing primality of such large numbers, however, is very difficult. A solution to this problem is available in the gmplib package. The [mpz\_probab\_prime\_p( mpz\_t n, int reps)](https://gmplib.org/manual/Number-Theoretic-Functions.html#Number-Theoretic-Functions) function is guaranteed to never produce a false-negative. That is, if the return value indicates that the tested number is composite, it is definitely composite. It can, however, return false-positives.

According to the documentation, which can be found through the above link, the probability of a false-positive (i.e. the probability that a composite is mistakenly reported to be prime) is less than 1/(4^reps). This assertion implies that each 'rep' is produces a random variable which is statistically independent of every other 'rep' in the test. This claim is one that I have yet to research and verify. The 'reps' value used in fizzbuzz.c is hardcoded to 50, which seems to run with reasonable quickness and, if the claimed probability of 1/(4^reps) is to be believed, the chance of receiving a false-positive is astronomically small. Modelling the trials performed in fizzbuzz as Bernoulli trials, one can assert that even if the actual probability of this claim were underestimated by a factor of 10^20, that is if the true probability of a false-positive is (10^20)/(4^50), then the probability of even one false-positive occurring in the first 50,000 BuzzFizz reports is .00000394. This is clearly a comfortably low probability that "BuzzFizz" will ever be incorrectly reported, especially as it will likely take years to obtain the 50,000th uncertain probability outocome.
         
Because of this, especially with respect to the importance of the FizzBuzz routine, there is no compelling case to be made for a truly deterministic primality test.

If the purpose of writing fizzbuzz.c weren't merely for obtaining gainful employment at a cool company, research on the claim of the 1/(4^reps) probability would be warranted, so is presently not on my to-do list.

