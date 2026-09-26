#include <stdio.h>
#include <string.h>
#include <ctype.h>

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

// Runs test cases to check functionality of extractIPv4()
void runTests(void)
{
    // Array containing the 15 test inputs
    const char *tests[] = {
        "172.16.5.20",
        "0.0.0.0",
        "255.255.255.255",
        "server=1.2.3.4:8080end",
        "192a168.1.1.1",
        "256.1.2.3",
        "1.2.03.4",
        "1.2.3",
        "1.2.3.4.5",
        "1..2.3.4",
        "1.2.3.4.",
        "1.2.3.4:65536",
        "1.2.3.4:01",
        "1.2.3.4::80",
        "999.1.1.1 abc 1.2.3.4"
    };

    // Array containing expected outputs of tests
    // 1 indicates valid address, 0 indicates invalid input)
    int expected[] = {
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1
    };

    int passed = 0;
    int total = sizeof(expected) / sizeof(expected[0]);

    // Run each test
    // Compare result to expected result
    for (int i = 0; i < total; i++)
    {
        unsigned long address = 0;
        int port = -1;

        int result = extractIPv4(tests[i], &address, &port);

        if (result == expected[i] &&
            (result || (address == 0 && port == -1)))
        {
            printf("PASS: %s\n", tests[i]);
            passed++;
        }
        else
        {
            printf("FAIL: %s\n", tests[i]);
        }

        // Display the address and port
        printf("Address: %lu, Port: %d\n\n", address, port);
    }

    // Display the number of tests that passed
    printf("RESULT: %d/%d tests passed\n", passed, total);
}

// Search the input string for the first valid IPv4 address
// Return 1 if found, 0 otherwise
// The numeric address and optional port are returned through pointers
int extractIPv4(const char* str, unsigned long* outAddress, int* outPort)
{
    int i = 0;

    // Initialize the outputs to required failure values
    *outAddress = 0;
    *outPort = -1;

    // Search through the string until null terminator is reached
    while (str[i] != '\0')
    {
        int start;
        int end;
        int pos;
        int octet;
        unsigned long address = 0;
        int valid = 1;
        int port = -1;

        // Only digits, periods, and colons are allowed
        if (!isdigit((unsigned char)str[i]) &&
            str[i] != '.' &&
            str[i] != ':')
        {
            i++;
            continue;
        }

        // Record the beginning of the candidate token
        start = i;

        // Find the end of the continuous sequence of allowed characters
        // The entire token must be validated
        while (str[i] != '\0' &&
               (isdigit((unsigned char)str[i]) ||
                str[i] == '.' ||
                str[i] == ':'))
        {
            i++;
        }

        end = i;
        pos = start;

        // Attempt to parse exactly four octets
        for (octet = 0; octet < 4 && valid; octet++)
        {
            int digitCount = 0;
            int value = 0;
            int firstDigitPos = pos;

            // Every octet must begin with a digit
            if (pos >= end ||
                !isdigit((unsigned char)str[pos]))
            {
                valid = 0;
                break;
            }

            // Read each digit and manually calculate the octet value
            while (pos < end &&
                   isdigit((unsigned char)str[pos]))
            {
                digitCount++;

                // Octets cannot contain more than three digits
                if (digitCount > 3)
                {
                    valid = 0;
                    break;
                }

                // Convert the current digit and accumulate its value
                value = value * 10 + (str[pos] - '0');

                // The maximum allowed octet value is 255
                if (value > 255)
                {
                    valid = 0;
                    break;
                }

                pos++;
            }

            // Stop parsing if current octet is invalid
            if (!valid)
            {
                break;
            }

            // Reject octets with multiple digits that begin with zero
            if (digitCount > 1 && str[firstDigitPos] == '0')
            {
                valid = 0;
                break;
            }

            // Build the 32-bit address
            // Shift the previous octets left one byte before adding the next octet
            address = address * 256UL + (unsigned long)value;

            // The first three octets must be followed by periods
            if (octet < 3)
            {
                if (pos >= end || str[pos] != '.')
                {
                    valid = 0;
                    break;
                }

                pos++;
            }
        }

        // After the fourth octet, check for an optional port
        // If additional characters exist, the next must be a colon
        if (valid && pos < end)
        {
            if (str[pos] == ':')
            {
                int digitCount = 0;
                int value = 0;
                int firstDigitPos;

                // Move past the colon and record the port's beginning
                pos++;
                firstDigitPos = pos;

                // A colon must be followed by at least one digit
                if (pos >= end ||
                    !isdigit((unsigned char)str[pos]))
                {
                    valid = 0;
                }

                // Read the port digits and calculate its numeric value
                while (valid &&
                       pos < end &&
                       isdigit((unsigned char)str[pos]))
                {
                    digitCount++;

                    // Ports cannot contain more than five digits
                    if (digitCount > 5)
                    {
                        valid = 0;
                        break;
                    }

                    // Accumulate the numeric port value
                    value = value * 10 + (str[pos] - '0');

                    // Reject ports exceeding 65535
                    if (value > 65535)
                    {
                        valid = 0;
                        break;
                    }

                    pos++;
                }

                // Reject leading zeros in ports with multiple digits
                if (valid &&
                    digitCount > 1 &&
                    str[firstDigitPos] == '0')
                {
                    valid = 0;
                }

                // The port must consume the remaining candidate
                if (valid && pos != end)
                {
                    valid = 0;
                }

                // Save the port only if every validation check passed
                if (valid)
                {
                    port = value;
                }
            }
            else
            {
                // If any character other than a colon immediately after the fourth octet, candidate invalid
                valid = 0;
            }
        }

        // Accept the address if the entire candidate is valid
        // Otherwise, keep searching after the rejected token
        if (valid && pos == end)
        {
            *outAddress = address;
            *outPort = port;
            return 1;
        }
    }

    // No valid candidate was found anywhere in the input
    *outAddress = 0;
    *outPort = -1;
    return 0;
}

int main(void)
{
    char input[1024];
    unsigned long address;
    int port;

    //runTests();

    // Continue reading input until the user enters END
    while (1)
    {
        unsigned int a;
        unsigned int b;
        unsigned int c;
        unsigned int d;
        size_t length;

        printf("Enter a string (or 'END' to quit):\n");

        // Read line, including spaces
        // Exit the loop if input cannot be read
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        // Get input length and remove trailing newline characters
        length = strlen(input);

        while (length > 0 &&
               (input[length - 1] == '\n' ||
                input[length - 1] == '\r'))
        {
            input[length - 1] = '\0';
            length--;
        }

        // Terminate if input is END
        if (strcmp(input, "END") == 0)
        {
            printf("Program terminated.\n");
            break;
        }

        // Attempt to extract the first valid IPv4 address
        if (extractIPv4(input, &address, &port))
        {
            // Recover the four octets from the numeric address
            a = (unsigned int)((address >> 24) & 255UL);
            b = (unsigned int)((address >> 16) & 255UL);
            c = (unsigned int)((address >> 8) & 255UL);
            d = (unsigned int)(address & 255UL);

            // Display the address without a port if none was found
            if (port == -1)
            {
                printf("Extracted IPv4 address: %u.%u.%u.%u "
                       "(decimal value: %lu, port: none)\n",
                       a, b, c, d, address);
            }
            else
            {
                // Otherwise, display the address and its numeric port
                printf("Extracted IPv4 address: %u.%u.%u.%u "
                       "(decimal value: %lu, port: %d)\n",
                       a, b, c, d, address, port);
            }
        }
        else
        {
            // Display the required message if extraction fails
            printf("Invalid input: no valid IPv4 address found\n");
        }
    }

    return 0;
}