#include<stdio.h>
void main() {
     int n, i, j, bt[n], wt[n], tat[n] ,ct = 0;
     double  avgtat = 0,avgwt =0;
     printf("Enter the Number of Processes: ");
     scanf("%d",&n);
     printf("Enter Burst Time of");
     for(i = 0;i < n;i++) {
          printf("\n                      P%d : ",(i+1));
          scanf("%d",&bt[i]);
     }
     
     wt[0] = 0;
     for(i = 0;i < n;i++) {
          wt[i] = wt[i-1] + bt[i-1];
          avgwt = avgwt + wt[i];
     }
     avgwt = avgwt / n;
     
     for(i = 0;i < n;i++) {
          tat[i] = wt[i] + bt[i];
          avgtat = avgtat + tat[i];
     }
     avgtat = avgtat / n;
     
     
     printf("-----------------------------------------------------------------");
     printf("\n|\tProcess\t|\tBT\t|\tWT\t|\tTAT\t|");
     printf("\n-----------------------------------------------------------------");
     for(i = 0;i < n; i++) {
          printf("\n|\tP%d\t|\t%d\t|\t%d\t|\t%d\t|\n",(i+1),bt[i],wt[i],tat[i]);
     }
     printf("-----------------------------------------------------------------");
     printf("\n\n");
     
     printf("Average Waiting Time = %.2fms\n",avgwt);
     printf("Average Turn Around Time = %.2fms\n",avgtat);
     printf("\n");
     
     printf("GANTT CHART\n");
     for(i = 0;i < n;i++) {
          printf("-------");
     }
     printf("\n|");
     for(i = 0;i < n;i++) {
          printf("  P%d  |",i+1);   
     }
     printf("\n");
     for(i = 0;i < n;i++) {
          printf("-------");
     }
     printf("\n0\t");
     for(i = 0;i < n;i++) {
          ct = ct + bt[i];
          printf("%d     ",ct);
     }
     printf("\n");
}
