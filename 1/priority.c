#include<stdio.h>
void main() {
     int n, i, j, ct = 0, temp;
     double  avgtat = 0,avgwt =0;
     printf("Enter the Number of Processes: ");
     scanf("%d",&n);
     
     int bt[n], wt[n], tat[n] , p[n],priority[n];
     printf("Enter Burst Time and PRIORITY of");
     for(i = 0;i < n;i++) {
          p[i] = i + 1;
          printf("\n                      P%d : ",(i+1));
          scanf("%d%d",&bt[i],&priority[i]);
     }
     
     //SORTING
     for(i = 0; i < n-1; i++) {
        for(j = i + 1; j < n; j++) {
            if(priority[i] > priority[j]) {
                temp = bt[i];    //swap burst time
                bt[i] = bt[j];
                bt[j] = temp;
               
                temp = priority[i];    //swap priority
                priority[i] = priority[j];
                priority[j] = temp;
               
                temp = p[i];    //swap process IDs
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
     wt[0] = 0;
     for(i = 1;i < n;i++) {
          wt[i] = wt[i-1] + bt[i-1];
          avgwt = avgwt + wt[i];
     }
     avgwt = avgwt / n;
     
     for(i = 0;i < n;i++) {
          tat[i] = wt[i] + bt[i];
          avgtat = avgtat + tat[i];
     }
     avgtat = avgtat / n;
     
     
     printf("-----------------------------------------------------------------------------------------");
     printf("\n|\tProcess\t|\tBT\t|\tPRIORITY\t|\tWT\t|\tTAT\t|");
     printf("\n-----------------------------------------------------------------------------------------");
     for(i = 0;i < n; i++) {
          printf("\n|\tP%d\t|\t%d\t|\t    %d    \t|\t%d\t|\t%d\t|\n",p[i],bt[i],priority[i],wt[i],tat[i]);
     }
     printf("-----------------------------------------------------------------------------------------");
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
          printf("  P%d  |",p[i]);  
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
