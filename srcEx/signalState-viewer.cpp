#include <stdio.h>
#include "SignalState.h"

int main(void)
{
    int c;
    printf("Hit any key (q:quit)\n");
    printf( "> ");
    while(1)
    {
        c = getchar();
        fflush(stdin);
        if( c == '\n' ){
            if(signalState())
                printf( "GO!!!\n" );
            else
                printf( "Stop\n");

            printf( "> " );
        }
        else if( c == 'q' || c == 27 )
            break;

    };
    printf("\n正常終了\n");

    return 0;
}
