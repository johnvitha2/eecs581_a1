#include <stdio.h>
#include <string.h>
#include <ctype.h>

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort)
{
    int i = 0;

    *outAddress = 0;
    *outPort = -1;

    while (str[i] != '\0')
    {
        int start;
        int end;
        int pos;
        int octet;
        unsigned long address = 0;
        int valid = 1;
        int port = -1;

        if (!isdigit((unsigned char)str[i]) &&
            str[i] != '.' &&
            str[i] != ':')
        {
            i++;
            continue;
        }

        start = i;

        while (str[i] != '\0' &&
               (isdigit((unsigned char)str[i]) ||
                str[i] == '.' ||
                str[i] == ':'))
        {
            i++;
        }

        end = i;
        pos = start;

        for (octet = 0; octet < 4 && valid; octet++)
        {
            int digitCount = 0;
            int value = 0;
            int firstDigitPos = pos;

            if (pos >= end ||
                !isdigit((unsigned char)str[pos]))
            {
                valid = 0;
                break;
            }

            while (pos < end &&
                   isdigit((unsigned char)str[pos]))
            {
                digitCount++;

                if (digitCount > 3)
                {
                    valid = 0;
                    break;
                }

                value = value * 10 + (str[pos] - '0');

                if (value > 255)
                {
                    valid = 0;
                    break;
                }

                pos++;
            }

            if (!valid)
            {
                break;
            }

            if (digitCount > 1 && str[firstDigitPos] == '0')
            {
                valid = 0;
                break;
            }

            address = address * 256UL + (unsigned long)value;

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

        if (valid && pos < end)
        {
            if (str[pos] == ':')
            {
                int digitCount = 0;
                int value = 0;
                int firstDigitPos;

                pos++;
                firstDigitPos = pos;

                if (pos >= end ||
                    !isdigit((unsigned char)str[pos]))
                {
                    valid = 0;
                }

                while (valid &&
                       pos < end &&
                       isdigit((unsigned char)str[pos]))
                {
                    digitCount++;

                    if (digitCount > 5)
                    {
                        valid = 0;
                        break;
                    }

                    value = value * 10 + (str[pos] - '0');

                    if (value > 65535)
                    {
                        valid = 0;
                        break;
                    }

                    pos++;
                }

                if (valid &&
                    digitCount > 1 &&
                    str[firstDigitPos] == '0')
                {
                    valid = 0;
                }

                if (valid && pos != end)
                {
                    valid = 0;
                }

                if (valid)
                {
                    port = value;
                }
            }
            else
            {
                valid = 0;
            }
        }

        if (valid && pos == end)
        {
            *outAddress = address;
            *outPort = port;
            return 1;
        }
    }

    *outAddress = 0;
    *outPort = -1;
    return 0;
}

int main(void)
{
    char input[1024];
    unsigned long address;
    int port;

    while (1)
    {
        unsigned int a;
        unsigned int b;
        unsigned int c;
        unsigned int d;
        size_t length;

        printf("Enter a string (or 'END' to quit):\n");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        length = strlen(input);

        while (length > 0 &&
               (input[length - 1] == '\n' ||
                input[length - 1] == '\r'))
        {
            input[length - 1] = '\0';
            length--;
        }

        if (strcmp(input, "END") == 0)
        {
            printf("Program terminated.\n");
            break;
        }

        if (extractIPv4(input, &address, &port))
        {
            a = (unsigned int)((address >> 24) & 255UL);
            b = (unsigned int)((address >> 16) & 255UL);
            c = (unsigned int)((address >> 8) & 255UL);
            d = (unsigned int)(address & 255UL);

            if (port == -1)
            {
                printf("Extracted IPv4 address: %u.%u.%u.%u "
                       "(decimal value: %lu, port: none)\n",
                       a, b, c, d, address);
            }
            else
            {
                printf("Extracted IPv4 address: %u.%u.%u.%u "
                       "(decimal value: %lu, port: %d)\n",
                       a, b, c, d, address, port);
            }
        }
        else
        {
            printf("Invalid input: no valid IPv4 address found\n");
        }
    }

    return 0;
}
