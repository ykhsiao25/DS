 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
#define NODE_NUM 9 //這裡要小心因為會直接被assign成constant
                   //所以在被當成parameter時，要用另外的變數接
                   //如 int node_num = NODE_NUM;
typedef struct edgenode *edgeptr;
typedef struct edgenode{
    int  n_vex;
 	int  weight;
    edgeptr nextedge;//node型態的指標，如int *
}e;
typedef struct vertexnode{
    int vertex;
    int id;
    int time;
    edgeptr eptr;
}v;
//fseek(ptr1, 0, SEEK_SET);(回到開頭)
//freopen也可以讀寫
void CreateEarlyGraph(int,int,v*,int*,int*,int*);
int CreateLateGraph(int,int,v*,FILE*);
e* mergesort(e*);
e* merge(e*,e*);


int main(){
//Initialize
 	FILE *ptr1,*ptr2;
    int input_size;int i1,i2,i3,n_node,node_num=NODE_NUM;
    ptr1 = fopen("input.txt","r+");//把input.txt檔案一行一行讀入
    ptr2 = fopen("output.txt","w+");
    fscanf(ptr1,"%ld ",&input_size);//這個是long integer不是1d
    printf("input_size = %d\n",input_size);
    int n1[input_size],n2[input_size],w[input_size];
    if(feof(ptr1) == 0){
        for (int i = 1;i<=input_size;i++){
            fscanf(ptr1,"%d %d %ld",&i1,&i2,&i3);
            n1[i-1]=i1;n2[i-1]=i2;w[i-1]=i3;
            if (n1[i-1] <= n2[i-1])n_node=n2[i-1]=i2;
            else n_node=n1[i-1]=i1;//算node有幾個
            printf("%1d %1d %ld\n",n1[i-1],n2[i-1],w[i-1]);
    	}
    }

//Process
    v* graph=(v*)malloc(node_num * sizeof(v));
    CreateEarlyGraph(node_num,input_size,graph,n1,n2,w);
    CreateLateGraph(node_num,input_size,graph,ptr2);

//Result
    fclose(ptr1);
    fclose(ptr2);
 	return 0;
 }
void CreateEarlyGraph(int node_num,int edge_num,v* graph,
                 int* n1,int* n2,int* w){
     int i;edgeptr t;
//Initialize
     for(i=0;i<node_num;i++){
         graph[i].id = 0; //有幾條邊連接到這個點
         graph[i].eptr = NULL;  //vertex連到edgenode的link
	 }
	 for(i=0;i<node_num;i++){
        graph[i].vertex = i; //點的名稱進行命名(在這邊直接當成整數assign)
	 }
//Process
    //建立edgenode的graph
    //data=vertex id=id firstedge=eptr
    //adjvex=n_vex w=weight nextedge=nextedge
    //vexnumber=node_num arcnumber=edge_num  Graph=graph
     for(i=0;i<edge_num;i++){
        t =(e*)malloc(sizeof(e));
        t->n_vex = n2[i];  //edge連接到的另一端點
        t->weight = w[i];  //edge weight
        graph[n2[i]].id++; //連接到這個點的edge數
        t->nextedge = graph[n1[i]].eptr;
        graph[n1[i]].eptr = t; //做一個Stack(新建的t
                               //graph[n1[i]].eptr像是top
                               //看這個點有連接哪些edge
	 }
 //在建完圖之後，把vertex edge連接到的下一點進行排序(不同edge的另一端點小的排前面)
	 for(i=0;i<node_num;i++){
            graph[i].eptr = mergesort(graph[i].eptr);
	 }
}
//一條linked list分成兩條去做排序(兩兩遞迴下去)
e* mergesort(e* head){
 //算法是slow = n/2 - 1取ceiling fast = n/2 取ceiling
    if(!head||!head->nextedge) return head;
    e* fast = head->nextedge;
    e* slow = head;
    while(fast && fast->nextedge ){
        slow = slow->nextedge;
        fast = fast->nextedge->nextedge;
    }
    fast = slow ->nextedge;//最後fast會移到slow後面(當作l2的head去做排序)
    slow->nextedge = NULL;
    e* l1 = mergesort(head);
    e* l2 = mergesort(fast);
    return merge(l1,l2);
}
e* merge(e* l1,e* l2){
    if(!l2)return l1; //這邊是終止條件，直接從mergesort過來的l1,l2不會發生
    if(!l1)return l2; //比如merge(NULL,l2)的時候就會直接return l2
    if(l1->n_vex < l2->n_vex){ //這裡的n_vex就是value或key的意思
        l1->nextedge = merge(l1->nextedge,l2);
        return l1; //l1後面就會串排好的linked list
    }
    else{
        l2->nextedge = merge(l1,l2->nextedge);
        return l2; //這裡的nextedge就是next的意思 只是宣告node型態不同
    }
}


int CreateLateGraph(int node_num,int edge_num,v* graph,FILE *ptr2){
    int totaltime=0;int i,j,k,a,cp_num=0,count=0;
    int cp[100];//char cp[100](假設有最多100個node)
    int front=-1,rear=-1;int *topology_queue,*vl,*ve,*el,*ee;
    topology_queue = (int*)malloc(node_num * sizeof(int));
    vl = (int*)malloc(node_num * sizeof(int));
    ve = (int*)malloc(node_num * sizeof(int));
    el = (int*)malloc(edge_num * sizeof(int));
    ee = (int*)malloc(edge_num * sizeof(int));
    edgeptr t;
    for (i=0;i<node_num;i++)ve[i]=0;
    //如果vertex前面已經沒有點了，就可以把它丟入topological queue
    for (i=0;i<node_num;i++){
        if (graph[i].id==0){
            topology_queue[++rear] = i;
            count++;
            break;
        }
    }
    //如果topological 還有vertex
    //計算Earliest time(最晚時間)也就是CreateEarlyGraph
    while (front!=rear){
        j = topology_queue[++front];//j是topological的第一個vertex
        t = graph[j].eptr; //p指到vertex j的第一個edgenode上
                           //p是edgepointer
        while(t){
            k = t -> n_vex;  //k是edge連接到的vertex
            graph[k].id--;  //k點的連接edge-1
            if ((ve[j]+t->weight) > ve[k] ) //如果上一個點的time+edge weight >下一個點的time
                ve[k] = ve[j] + t -> weight;
            if (graph[k].id==0){ //如果k點前面沒edge，就丟入拓樸
                topology_queue[++rear] = k;
                count++;
            }
            t = t ->nextedge; //看點k的下一條edge
        }
    }
        if (count < node_num){
            printf("No Solution !\n ");
            fprintf(ptr2,"No Solution !");
            return 0;
        }
    //計算latest time(最晚時間)u也就是CreateLateGraph
        totaltime = ve[node_num-1];//totaltime必在最後一個點
        for (i=0;i<node_num;i++) vl[i] = totaltime;
        for (i=node_num-2;i>=0;i--){
            j = topology_queue[i]; //從倒數第二個點開始拿起(因為會連到最後一個點)
            t = graph[j].eptr;
            while(t){
                k = t -> n_vex;
                if ((vl[k] - t->weight) < vl[j])
                    vl[j] = vl[k] - t->weight; //若ai-weight < ai-1
                t = t ->nextedge;
            }
        }
        printf("起點  終點  最早開始時間  最晚開始時間  差值  是否為關鍵路徑 \n");
  //計算是否為Critical Path
        i = 0;
        printf("%4d\n",edge_num);
        fprintf(ptr2,"%4d\n",edge_num);
        for(a=0;a<node_num;a++){
            t = graph[a].eptr;
            int repeat=0;
            while(t){
                k = t ->n_vex;
                ee[++i] = ve[a];
                el[i] = vl[k] - t->weight;
                printf("%4d  %4d  %4d  %4d  %4d     ",graph[a].vertex ,graph[k].vertex ,ee[i],el[i],el[i]-ee[i]);
                fprintf(ptr2,"%4d  %4d  %4d     ",ee[i],el[i],el[i]-ee[i]);
                if(el[i]==ee[i]){
                    repeat++;
                    printf("Yes");
                    fprintf(ptr2,"Yes");
                    if(repeat>1)cp_num--; //若同起點，就不加入
                    cp[++cp_num] = graph[k].vertex;//這邊是看起點，改成終點才不會重複
                    printf("    graph[a].vertex=%d",graph[a].vertex);
                }
                else{printf("No");fprintf(ptr2,"No");}
                printf("\n");
                fprintf(ptr2,"\n");
                t = t -> nextedge;
            }
        }
    //找出全部的criitical path
        printf("Critical Path的node 順序之一為 :");
        cp[0] = graph[0].vertex;
        //cp[t] = graph[node_num-1].vertex;//最後一個點必在critical path上
        for (i=0;i<cp_num;i++){
            printf("%d",cp[i]);
            if (cp[i] != graph[node_num-1].vertex)printf("    ->    ");
        }
        printf("\n");
        printf("Critical path長度為：%d\n",totaltime);
      	return 1;
}






