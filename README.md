# Advent of Code 2024
Here's the link if you want to do it yourself: [Advent of Code 2024](https://adventofcode.com/2024)

## Things I'd like to improve

### Problem 6
- Make it so it first grabs strings where the muls are valid and THEN just parses all the muls inside them
    - Insert a "do()" in the beginning of the string and a "don't()" at the end
    - Grab all the strings between "do()"'s and "don't()"'s
    - Those are the valid strings

### Problem 10
- Make it not disgusting
    - Currently recursively swaps values until it is valid
    - Possibly a way to find a chain of rules
