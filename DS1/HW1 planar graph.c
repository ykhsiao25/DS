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
    //��node.txt�ɮפ@��@��Ū�J
    // double x[11] = {1.4142, 1.97 , 0.8823 , 1.9996 , 0.6301 , 0.3329 , 1.952  , 1.8181 , 0.4949 , 0.2079 , 0.1364 };
    //double y[11] = {1.1534, 1.85 , 1.3926 , 1.9484 , 1.9145 , 1.1756 , 0.5866 , 0.4043 , 1.946  , 1.4758 , 1.4084 };
        fscanf(ptr1,"%ld ",&A);
        n[0] = A;
        printf("%d\n",n[0]);
        for (int i =0;i<=10;i++){
            fscanf(ptr1,"%d %lf %lf",&A,&B,&C);
            n[i+1] = A;
            x[i] = B;
            y[i] = C;
            printf("%-2d         %.4f            %.4f\n",n[i+1],x[i],y[i]);
//            fscanf(ptr1,"%d %lf %lf",&n2[i],&x2[i],&y2[i]);
//            fgets(n,2,ptr1);
//            fgets(x,5,ptr1);
//            fgets(y,5,ptr1);
//            n2[i] = atof(n[i]);//����Ʒ|��Ū�X�Ӫ�����ରdouble���A
//            x2[i] = atof(x[i]);
//            y2[i] = atof(y[i]);
        }
    }
    fclose(ptr1);

    static int count;
    int c[11][11];
    int arr[11][11];
//    double brr[10][2];
    int nodes = 0;
    //��n���I�y�Х���l��
    for (int init1 =0 ; init1 <= 10 ;init1++){
        for (int init2 =0 ; init2 <= 10 ;init2++){
            arr[init1][init2] = 0;
//            brr[init1][init2] = 0;
        }
    }
    //if(u,v) <=1 �L�Xu,v�ݩ����node�Acount�h�O�N��ŦX���󪺲ĴX��link(�ھ�u,v�j�p�Ƨ�)
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

    //�Y����ۥ�A�H�䬰���|�e�@��Y���I�b�ꤺ�A�N����屼
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
