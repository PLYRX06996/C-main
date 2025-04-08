#include<stdio.h>
#include<string.h>

void INPUT_arr(int n, int m, char arr[n][m][11]){
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%s", arr[i][j]);
        }
        
    }
    
}

void FIND_arr(int n, int m, char arr[n][m][11])
{
        int q;
        scanf("%d", &q);
        while (q>0) 
        {
            char temp_arr[1][2][11];
        scanf("%s %s", temp_arr[0][0][11], temp_arr[0][1][11]);
        int count=0, r1, r2;
        for (int i = 0; i < n; i++)
    {       
        for (int j = 0; j < m; j++)
        {
            if (strcmp(temp_arr[0][0][11], arr[i][j]) == 0)       
            {
                count++;
                    r1=i;
                break;
            }
            if (strcmp(temp_arr[0][1][11], arr[i][j]) == 0)       
            {
                count++;
                    r1=i;
                break;
            }
            
        }
        
    }
    if (count==2 && r1==r2)
    {
        printf("%d\n", r1);
    } else { printf("-1\n");}
            q--;
        }
    
}



int main(){
    int tc, r, c;
    scanf("%d ", &tc);

    while (tc>0)
    {
        scanf("%d %d", &r, &c);
        char arr[r][c][11];
        INPUT_arr(r, c, arr);
        FIND_arr(r, c, arr);
        tc--;
    }
    
    return 0;
}
