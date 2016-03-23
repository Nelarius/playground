
class Random {
    // seed the random number generator with a non-deterministic seed
    foreign static randomize()
    // seed with an integer
    foreign static seed(s)
    // generate a random integer in [a, b]
    foreign static int(a, b)
    // generate a random real number in [0, 1)
    foreign static real()
    // generate a static real number in [a, b)
    foreign static real(a ,b)
}
