#include <stdio.h>

int main(void)
{
   char buf[4096];
   int x;
   
   while (fgets(buf, sizeof(buf)-2, stdin) != NULL) {
        for (x = 0; x < 128; ) {
            printf("0x%c%c, ", buf[x], buf[x+1]);
            if (!((x += 2) & 31)) printf("\n");
        }
   }
}


/* ref:         HEAD -> master, origin/master, origin/HEAD */
/* git commit:  26edbfa0164e8efed06197be0dff225945834640 */
/* commit time: 2021-08-10 11:33:39 +0200 */
