# AI Disclosure

### Tool

ChatGPT (GPT-5.6 Sol)

### Date Consulted

September 26, 2026

### Code Attribution

I used a generative AI tool for all of this assignment's code generation. ChatGPT generated extractIPv4() and main() from Prompt #1, and runTests() from Prompt #2. I added a call to runTests() in main() to run the test cases. I reviewed the code and added comments. 

### Exact prompts used to generate the code

#### Prompt 1

Write a complete C program that searches a line of text for a valid IPv4 address, optionally followed by a port number. Perform all parsing and validation manually, character by character.
Implement this function exactly:
int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);
Return 1 if a valid IPv4 address is found and 0 otherwise.
A valid IPv4 address contains exactly four octets separated by periods. Each octet must:
•	contain 1-3 decimal digits
•	have a value from 0–255
•	have no leading zero unless the octet is exactly 0
Examples of valid octets: 0, 10, 255.
Examples of invalid octets: 00, 01, 001, 256.
An IPv4 address may optionally be followed immediately by a colon and port number. A valid port must:
•	contain 1–5 decimal digits
•	have a value from 0–65535
•	have no leading zero unless the port is exactly 0
If a colon is present after the fourth octet, the port must be fully valid. If the port is invalid, reject the entire candidate, including the address portion.
Only digits, periods, and colons can be part of a candidate token. All other characters are garbage and should be skipped while searching.
For example:
192a168.1.1.1
should allow 168.1.1.1 to be found and accepted.
A continuous run consisting only of digits, periods, and colons must be validated in full. Do not extract a valid substring from a larger malformed token.
For example, all of these must be rejected as complete candidate tokens:
192.168.1.1.
1.2.3.4.5
.1.2.3.4
1.2.3.4:80.5
1.2.3.4::80
Reject candidates with any of the following:
•	fewer or more than four octets
•	empty octets
•	octets longer than three digits
•	octet values greater than 255
•	leading zeroes in multi-digit octets
•	a colon before the fourth octet is complete
•	more than one colon
•	a colon with no port
•	a port longer than five digits
•	a port value greater than 65535
•	leading zeroes in a multi-digit port
•	extra periods or colons adjacent to an otherwise valid address
Do not use any of the following built-in functionality:
•	string-to-number conversion functions
•	formatted input functions for numeric conversion
•	IP address or network address parsing functions
•	regular-expression libraries or facilities
Standard character-classification functions such as isdigit are allowed.
Accumulate every numeric value manually from its digit characters by multiplying the current value by 10 and adding the next digit.
When a valid address is found:
•	return 1
•	store its 32-bit numeric value in *outAddress
•	store its port in *outPort, or -1 if no port is present
Treat the first IPv4 octet as the most significant byte. For example:
192.168.1.1
must produce:
3232235777
If no valid IPv4 address is found:
•	return 0
•	set *outAddress to 0
•	set *outPort to -1
Search from left to right. If a candidate is malformed, reject that entire candidate and continue searching after it.
Main should repeatedly read full lines of input. Use a character array and fgets so spaces are preserved, and remove any trailing newlines. 
Prompt each time with:
Enter a string (or 'END' to quit):
If the input is exactly END, print:
Program terminated.
Then, end the input loop and allow the program to terminate normally.
Otherwise, call extractIPv4.
If a valid address is found, print exactly:
Extracted IPv4 address: A.B.C.D (decimal value: N, port: P) where A.B.C.D is the extracted address, N is its unsigned 32-bit decimal value, and P is the numeric port or the literal text none.
Example: Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
If no valid address is found, print:
Invalid input: no valid IPv4 address found

#### Prompt 2

Write a C function called runTests() that tests the following inputs:
1. 172.16.5.20
2. 0.0.0.0
3. 255.255.255.255
4. server=1.2.3.4:8080end
5. 192a168.1.1.1
6. 256.1.2.3
7. 1.2.03.4
8. 1.2.3
9. 1.2.3.4.5
10. 1..2.3.4
11. 1.2.3.4.
12. 1.2.3.4:65536
13. 1.2.3.4:01
14. 1.2.3.4::80
15. 999.1.1.1 abc 1.2.3.4
Store the test inputs in an array and use another array containing the expected return values (1 for valid and 0 for invalid). Loop through the tests and call extractIPv4() for each one. Print if they passed or failed based on whether the return value matches the expected result. For invalid inputs, check that the address is set to 0 and the port is set to -1.
Print the extracted decimal address and port for each test. At the end, print how many tests passed out of the total.

### Verification

After generating the code using ChatGPT, I reviewed the program to confirm I understood how the parsing and validation processes worked, and added comments. 

I used 15 test cases to verify the code output, and all 15 test cases passed.

The test function checks whether each input is accepted or rejected and confirms that invalid inputs reset the output parameters. It also displays the decimal address and port so I can verify them manually. 

I did not identify any bugs. 