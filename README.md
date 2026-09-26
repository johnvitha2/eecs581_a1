# IPv4 Address Extractor

## Description

This program searches a line of text for a valid IPv4 address with an optional port number. It validates each candidate and converts valid addresses into their unsigned 32-bit decimal representation.

## How to Run

Compile the program using GCC:

gcc ipv4_extractor.c -o ipv4_extractor

Run the program:

./ipv4_extractor

Enter a string to search for an IPv4 address. Enter END to terminate the program.

## Testing

extract_ipv4.c includes a runTests() function containing 15 test cases.

To run the tests, uncomment runTests(); in main(), then compile and execute the program.