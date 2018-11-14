#include <cstdio>
#include <cstdlib>

#ifndef __DALY_BASE64
#define __DALY_BASE64
int base64Encode(const char *inStr, long long inStrLen, char **outStr, long long *outStrLen) {

    static char table[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    long long pos = 0, newLen = ((inStrLen + 2) / 3) << 2, newPos = 0;
    char *newStr = (char *)calloc(newLen, sizeof (char));
    int i1, i2, unit;

    while (1) {
        for (i1 = 2, unit = 0; i1 > -1; i1--, pos++) {
            unit |= (pos < inStrLen ? (unsigned char)inStr[pos] : 0) << (i1 << 3);
        }
        for (i1 = 3; i1 > -1; i1--, unit >>= 6) {
            newStr[newPos + i1] = table[unit & 0b00111111];
        }
        newPos += 4;
        if (pos >= inStrLen) {
            for (i1 = 0; i1 < pos - inStrLen; i1++) {
                newStr[newLen - i1 - 1] = '=';
            }
            *outStr = newStr;
            *outStrLen = newLen;
            return 0;
        }
    }

}

int base64Decode(const char *inStr, long long inStrLen, char **outStr, long long *outStrLen) {

    static char table[256] = {
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62,  0,  0,  0, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,
         0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,
         0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,  0,  0,  0,  0,  0
    };

    if (inStrLen & 3) {
        return 1;
    }

    long long pos = 0, newPos = 0;
    char *newStr = (char *)calloc(1, sizeof (char));
    int i1, i2, unit, back;

    while (1) {
        for (i1 = 3, back = 0, unit = 0; i1 > -1; i1--, pos++) {
            i2 = table[inStr[pos]];
            if (inStr[pos] == '=') {
                back++;
            } else if (i2 == 0 && inStr[pos] != 'A') {
                free(newStr);
                return 1;
            }
            unit |= i2 << (i1 * 6);
        }
        newStr = (char *)realloc(newStr, (newPos + 3) * sizeof (char));
        for (i1 = 2; i1 > -1; i1--, unit >>= 8) {
            newStr[newPos + i1] = unit & 0b11111111;
        }
        newPos += 3 - back;
        if (back > 0) {
            newStr = (char *)realloc(newStr, newPos * sizeof (char));
        }
        if (pos == inStrLen) {
            *outStr = newStr;
            *outStrLen = newPos;
            return 0;
        }
    }

}
#endif

#ifndef __DALY_FILE_STRING
#define __DALY_FILE_STRING
int fileGetContents(const char *fileName, char **contents, fpos_t *size) {
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL) {
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    fpos_t pos;
    if (fgetpos(fp, &pos)) {
        fclose(fp);
        return 1;
    }
    *size = pos;
    fseek(fp, 0, SEEK_SET);
    *contents = (char *)calloc(*size, sizeof (char));
    fread(*contents, sizeof (char), *size, fp);
    fclose(fp);
    return 0;
}

int filePutContents(const char *fileName, char *contents, fpos_t size) {
    FILE *fp = fopen(fileName, "wb");
    fwrite(contents, sizeof (char), size, fp);
    fclose(fp);
    return 0;
}

int existFile(const char *fileName) {
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL) {
        return 0;
    } else {
        fclose(fp);
        return 1;
    }
}
#endif