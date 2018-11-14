#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "base64.h"

int main(int argc, char *argv[]) {
    
    const char *errMsg = NULL;
    const char *help = 
    "usage:  base64 help\n"
    "or      base64 enf|def <input-file-name> <output-file-name>\n"
    "or      base64 ens|des <input-string>";

    while (1) {
        
        if (argc == 1) {
            errMsg = "No command!";
            break;
        }

        if (strcmp(argv[1], "help") == 0) {
            puts(help);
            return 0;
        }

        if (strcmp(argv[1], "enf") == 0
        || strcmp(argv[1], "def") == 0) {

            if (argc != 4) {
                errMsg = "Two file names expected!";
                break;
            }
            if (existFile(argv[3])) {
                puts("File with the same name as output file exists!");
                return 1;
            }
            char *inStr = NULL, *outStr = NULL;
            long long inStrLen, outStrLen;
            if (fileGetContents(argv[2], &inStr, &inStrLen)) {
                puts("No such an input file!");
                return 1;
            }
            if (strcmp(argv[1], "enf") == 0) {
                base64Encode(inStr, inStrLen, &outStr, &outStrLen);
            } else {
                if (base64Decode(inStr, inStrLen, &outStr, &outStrLen)) {
                    puts("Shit is in your input file.");
                    return 1;
                }
            }
            filePutContents(argv[3], outStr, outStrLen);
            free(inStr);
            free(outStr);
            puts("File created successfully!");

        } else if (strcmp(argv[1], "ens") == 0
        || strcmp(argv[1], "des") == 0) {

            if (argc != 3) {
                errMsg = "One string expected!";
                break;
            }
            char *outStr = NULL;
            long long outStrLen;
            if (strcmp(argv[1], "ens") == 0) {
                base64Encode(argv[2], strlen(argv[2]), &outStr, &outStrLen);
            } else {
                if (base64Decode(argv[2], strlen(argv[2]), &outStr, &outStrLen)) {
                    puts("There is something wrong with the string.");
                    return 1;
                }
            }
            fwrite(outStr, sizeof (char), outStrLen, stdout);
            free(outStr);

        } else {

            errMsg = "Invalid sub command!";
            break;

        }
        break;

    }
    if (errMsg != NULL) {
        puts(errMsg);
        puts(help);
        return 1;
    }

}