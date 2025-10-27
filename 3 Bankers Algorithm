#include<stdio.h>

int main() {
     int n, r, i, j, k;
     printf("Enter the Number of Process: ");
     scanf("%d",&n);
     printf("Enter the Number of Resources: ");
     scanf("%d",&r);
     
     int available[r], max[n][r], allocation[n][r], need[n][r], finish[n],safeseq[n];
     for(i = 0;i < n;i++) {
        finish[i] = 0;     //Initializing all columns finish array with 0
     }
     printf("\nEnter the AVAILABLE Resources:\n");
     for(i = 0;i < r;i++) {
          scanf("%d",&available[i]);
     }
     printf("\nEnter the ALLOCATION matrix\n");
     for(i = 0;i < n;i++) {
         for(j = 0;j < r;j++) {
          scanf("%d",&allocation[i][j]);
     } }
     printf("\nEnter the MAX matrix\n");
     for(i = 0;i < n;i++) {
          for(j = 0;j < r;j++) {
          scanf("%d",&max[i][j]);
     } }
     
     for(i = 0;i < n;i++) {
         for(j = 0;j < r;j++) {
             need[i][j] = max[i][j] - allocation[i][j];
     } }
     
     printf("\nNEED matrix =\n");
     for(i = 0;i < n;i++) {
         for(j = 0;j < r;j++) {
             printf("%d ", need[i][j]);
         }
         printf("\n");
     }
     
     int count = 0, z = 0;
     while(count < n) {
        for(i = 0; i < n;i++) {
           if(finish[i] == 0) {
           int can_allocate = 1;
              for(j = 0;j < r;j++) {
                 if(need[i][j] > available[j]) {
                    can_allocate = 0;
                        break;
                    }
             }
                
             if (can_allocate) {
                for (k = 0; k < r;k++) {
                   available[k] += allocation[i][k];
                }
                finish[i] = 1;
                safeseq[z++] = i;
                count++;
            }
         }
     }
  } 
     
    for(i = 0; i < n;i++) {
       if(finish[i] == 0) {
          printf("System is UnSAfe..!!!!!\n");
          return(0);
    } }
     
     printf("\n\nSystem is SAfe.......\nSAfe sequence =\n<");
     for(i = 0;i < n;i++) {
        printf("  P%d  ",safeseq[i]);
     }
     printf(">\n");
     return(0);
}
