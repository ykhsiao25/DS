#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int main(){
    FILE *ptr1;
    FILE *ptr2;
    int n[12];
    double x[11];
    double  y[11];
    int A ;
    double B;
    double C;

    ptr1 = fopen("node.txt","r+");
    ptr2 = fopen("link.txt","w+");

    if(feof(ptr1) == 0){
    //把node.txt檔案一行一行讀入
        fscanf(ptr1,"%ld ",&A);
        n[0] = A;
        printf("%d\n",n[0]);
        for (int i =0;i<=10;i++){
            fscanf(ptr1,"%d %lf %lf",&A,&B,&C);
            n[i+1] = A;
            x[i] = B;
            y[i] = C;
            printf("%-2d         %.4f            %.4f\n",n[i+1],x[i],y[i]);
        }
    }
    fclose(ptr1);

    static int count;
    int c[11][11];
    int arr[11][11];
    int nodes = 0;
    //把要放的點座標先初始值
    for (int init1 =0 ; init1 <= 10 ;init1++){
        for (int init2 =0 ; init2 <= 10 ;init2++){
            arr[init1][init2] = 0;
        }
    }
    //if(u,v) <=1 印出u,v屬於哪個node，count則是代表符合條件的第幾條link(根據u,v大小排序)
    for (int i = 0; i <= 10 ;i++){
        for(int j = 0 ; j <= 10 ;j++){
            double delta_x  = x[i]-x[j];
            double delta_y = y[i]-y[j];
            if(  (sqrt( pow(delta_x,2) + pow(delta_y ,2) ) ) <=1){
                if (i!=j && i < j){
                  printf("%-2d          %2d         %2d                \n",count,i,j);
                  fprintf(ptr2,"%-2d            %2d         %2d                \n",count,i,j);
                  arr[i][j] = 1;
                  c[i][j] = count;
                  count +=1;
                  }
             }

         }
     }
    printf("%-2d\n",count);
    fprintf(ptr2,"%-2d\n",count);

    //若有邊相交，以邊為直徑畫一圓若有點在圓內，就把邊砍掉
    count = 0;
    for (int i = 0; i <= 10 ;i++){
        for(int j = 0 ; j <= 10 ;j++){
                double delta_x  = x[i]-x[j];
                double delta_y = y[i]-y[j];
                double c_x = (x[i] + x[j])/2;
                double c_y = (y[i] + y[j])/2;
                double r = sqrt( pow(c_x-x[i],2) + pow(c_y-y[i] ,2));
                if(  (sqrt( pow(delta_x,2) + pow(delta_y ,2) ) ) <=1){
                        for(int a = 0 ; a <= 10 ;a++){
                            double delta_iax  = x[i]-x[a];
                            double delta_iay = y[i]-y[a];
                            if((arr[i][j] ==1) && (a!=i)  &&(a!=j)){
                                if ((sqrt( pow((c_x - x[a]),2) + pow((c_y - y[a]) ,2) ) ) < r ){
                                        arr[i][j] = 0;
                                }
                            }
                        }
                }
        }
    }
    int b = 0;
    for (int i = 0; i <= 10 ;i++){
        for(int j = 0 ; j <= 10 ;j++){
                if (arr[i][j] !=0){
                    printf("%-2d            %2d         %2d\n",c[i][j],i,j);
                    fprintf(ptr2,"%-2d          %2d         %2d                \n",c[i][j],i,j);
                    b += 1;
                }
        }
    }
    printf("%d",b);
    fprintf(ptr2,"%d\n",b);
    fclose(ptr2);
    return 0;
}
