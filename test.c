#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MODULE_DIR "/dev/MyModule"

int OpenCharDevice()
{
    int file = open(MODULE_DIR, O_RDONLY);

    if (file < 0)
    {
        printf("Can't open the device file to rand.\n");
        exit(0);
    }

    return file;
}

void CloseCharDevice(int file)
{
    close(file);
}

int main() {
    int file = OpenCharDevice(MODULE_DIR);
    int res = 0;
    read(file, &res, sizeof(res));
    printf("Random number: %d\n", res);
    return 0;
}
