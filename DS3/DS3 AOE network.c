 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
#define NODE_NUM 9 //�o�̭n�p�ߦ]���|�����Qassign��constant
                   //�ҥH�b�Q��parameter�ɡA�n�Υt�~���ܼƱ�
                   //�p int node_num = NODE_NUM;
typedef struct edgenode *edgeptr;
typedef struct edgenode{
    int  n_vex;
 	int  weight;
    edgeptr nextedge;//node���A�����СA�pint *
}e;
typedef struct vertexnode{
    int vertex;
    int id;
    int time;
    edgeptr eptr;
}v;
//fseek(ptr1, 0, SEEK_SET);(�^��}�Y)
//freopen�]�i�HŪ�g
void CreateEarlyGraph(int,int,v*,int*,int*,int*);
int CreateLateGraph(int,int,v*,FILE*);
e* mergesort(e*);
e* merge(e*,e*);
//void CreateLateGraph(int,int,v*,int*,int*,int*);
//SearchMapPath(graph,node_num,input_size);

int main(){
//Initialize
 	FILE *ptr1,*ptr2;
    int input_size;int i1,i2,i3,n_node,node_num=NODE_NUM;
    ptr1 = fopen("input.txt","r+");//��input.txt�ɮפ@��@��Ū�J
    ptr2 = fopen("output.txt","w+");
    fscanf(ptr1,"%ld ",&input_size);//�o�ӬOlong integer���O1d
    printf("input_size = %d\n",input_size);
    int n1[input_size],n2[input_size],w[input_size];
    if(feof(ptr1) == 0){
        for (int i = 1;i<=input_size;i++){
            fscanf(ptr1,"%d %d %ld",&i1,&i2,&i3);
            n1[i-1]=i1;n2[i-1]=i2;w[i-1]=i3;
            if (n1[i-1] <= n2[i-1])n_node=n2[i-1]=i2;
            else n_node=n1[i-1]=i1;//��node���X��
            printf("%1d %1d %ld\n",n1[i-1],n2[i-1],w[i-1]);
    	}
    }
    //printf("n_node = %d\n",n_node);

//Process
    v* graph=(v*)malloc(node_num * sizeof(v));
    CreateEarlyGraph(node_num,input_size,graph,n1,n2,w);
    //for(int i=0;i<node_num-1;i++){
    //    printf("graph[%d] n_vex = %d weight = %d id = %d \n",i,graph[i].eptr->n_vex ,graph[i].eptr->weight,graph[i].id);
	// } //���`�i�L
    //printf("graph[0] n_vex = %d weight = %d\n",graph[0].eptr->n_vex ,graph[0].eptr->weight);
    //CreateLateGraph(node_num,input_size,graph,n1,n2,w);
    CreateLateGraph(node_num,input_size,graph,ptr2);
    //SearchMapPath(graph,node_num,input_size);
    //Deviation()

//Result
    //PrintfResult()
    fclose(ptr1);
    fclose(ptr2);
 	return 0;
 }
void CreateEarlyGraph(int node_num,int edge_num,v* graph,
                 int* n1,int* n2,int* w){
     int i;edgeptr t;
//Initialize
     for(i=0;i<node_num;i++){
         graph[i].id = 0; //���X����s����o���I
         graph[i].eptr = NULL;  //vertex�s��edgenode��link
	 }
	 for(i=0;i<node_num;i++){
        graph[i].vertex = i; //�I���W�ٶi��R�W(�b�o�䪽�������assign)
	 }
//Process
    //�إ�edgenode��graph
    //data=vertex id=id firstedge=eptr
    //adjvex=n_vex w=weight nextedge=nextedge
    //vexnumber=node_num arcnumber=edge_num  Graph=graph
     for(i=0;i<edge_num;i++){
        t =(e*)malloc(sizeof(e));
        t->n_vex = n2[i];  //edge�s���쪺�t�@���I
        t->weight = w[i];  //edge weight
        graph[n2[i]].id++; //�s����o���I��edge��
        t->nextedge = graph[n1[i]].eptr;
        graph[n1[i]].eptr = t; //���@��Stack(�s�ت�t
                               //graph[n1[i]].eptr���Otop
                               //�ݳo���I���s������edge
        //printf("t n_vex = %d weight = %d\n",t->n_vex ,t->weight);
	 }
 //�b�ا��Ϥ���A��vertex edge�s���쪺�U�@�I�i��Ƨ�(���Pedge���t�@���I�p���ƫe��)
	 for(i=0;i<node_num;i++){
        //if(graph[i].eptr!=NULL)
            graph[i].eptr = mergesort(graph[i].eptr);
	 }
}
//�@��linked list��������h���Ƨ�(��⻼�j�U�h)
e* mergesort(e* head){
 //��k�Oslow = n/2 - 1��ceiling fast = n/2 ��ceiling
    if(!head||!head->nextedge) return head;
    e* fast = head->nextedge;
    e* slow = head;
    while(fast && fast->nextedge ){
        slow = slow->nextedge;
        fast = fast->nextedge->nextedge;
    }
    fast = slow ->nextedge;//�̫�fast�|����slow�᭱(��@l2��head�h���Ƨ�)
    slow->nextedge = NULL;
    e* l1 = mergesort(head);
    e* l2 = mergesort(fast);
    return merge(l1,l2);
}
e* merge(e* l1,e* l2){
    if(!l2)return l1; //�o��O�פ����A�����qmergesort�L�Ӫ�l1,l2���|�o��
    if(!l1)return l2; //��pmerge(NULL,l2)���ɭԴN�|����return l2
    if(l1->n_vex < l2->n_vex){ //�o�̪�n_vex�N�Ovalue��key���N��
        l1->nextedge = merge(l1->nextedge,l2);
        return l1; //l1�᭱�N�|��Ʀn��linked list
    }
    else{
        l2->nextedge = merge(l1,l2->nextedge);
        return l2; //�o�̪�nextedge�N�Onext���N�� �u�O�ŧinode���A���P
    }
}


int CreateLateGraph(int node_num,int edge_num,v* graph,FILE *ptr2){
    int totaltime=0;int i,j,k,a,cp_num=0,count=0;
    int cp[100];//char cp[100](���]���̦h100��node)
    int front=-1,rear=-1;int *topology_queue,*vl,*ve,*el,*ee;
    topology_queue = (int*)malloc(node_num * sizeof(int));
    vl = (int*)malloc(node_num * sizeof(int));
    ve = (int*)malloc(node_num * sizeof(int));
    el = (int*)malloc(edge_num * sizeof(int));
    ee = (int*)malloc(edge_num * sizeof(int));
    edgeptr t;
    for (i=0;i<node_num;i++)ve[i]=0;
    //�p�Gvertex�e���w�g�S���I�F�A�N�i�H�⥦��Jtopological queue
    for (i=0;i<node_num;i++){
        if (graph[i].id==0){
            topology_queue[++rear] = i;
            count++;
            break;
            //printf("i = %d\n",i);
        }
        //printf("topology_queue[rear]= %d\n",topology_queue[rear]);
    }
    //�p�Gtopological �٦�vertex
    //�p��Earliest time(�̱߮ɶ�)�]�N�OCreateEarlyGraph
    while (front!=rear){
        j = topology_queue[++front];//j�Otopological���Ĥ@��vertex
        t = graph[j].eptr; //p����vertex j���Ĥ@��edgenode�W
                           //p�Oedgepointer
        //printf("t = %d = %d\n",t,graph[j].eptr);//���`
        while(t){
            k = t -> n_vex;  //k�Oedge�s���쪺vertex
            //printf("k = %d\n",k);���`
            graph[k].id--;  //k�I���s��edge-1
            //printf("graph[k].id=%d\n",graph[k].id);
            if ((ve[j]+t->weight) > ve[k] ) //�p�G�W�@���I��time+edge weight >�U�@���I��time
                ve[k] = ve[j] + t -> weight;
            if (graph[k].id==0){ //�p�Gk�I�e���Sedge�A�N��J�ݾ�
                topology_queue[++rear] = k;
                count++;
            }
            t = t ->nextedge; //���Ik���U�@��edge
        }
    }
        if (count < node_num){
            printf("No Solution !\n ");
            fprintf(ptr2,"No Solution !");
            return 0;
        }
    //�p��latest time(�̱߮ɶ�)u�]�N�OCreateLateGraph
        totaltime = ve[node_num-1];//totaltime���b�̫�@���I
        for (i=0;i<node_num;i++) vl[i] = totaltime;
        for (i=node_num-2;i>=0;i--){
            j = topology_queue[i]; //�q�˼ƲĤG���I�}�l���_(�]���|�s��̫�@���I)
            t = graph[j].eptr;
            while(t){
                k = t -> n_vex;
                if ((vl[k] - t->weight) < vl[j])
                    vl[j] = vl[k] - t->weight; //�Yai-weight < ai-1
                t = t ->nextedge;
            }
        }
        printf("�_�I  ���I  �̦��}�l�ɶ�  �̱߶}�l�ɶ�  �t��  �O�_��������| \n");
  //�p��O�_��Critical Path
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
                    if(repeat>1)cp_num--; //�Y�P�_�I�A�N���[�J
                    cp[++cp_num] = graph[k].vertex;//�o��O�ݰ_�I�A�令���I�~���|����
                    printf("    graph[a].vertex=%d",graph[a].vertex);
                }
                else{printf("No");fprintf(ptr2,"No");}
                printf("\n");
                fprintf(ptr2,"\n");
                t = t -> nextedge;
            }
        }
    //��X������criitical path
        printf("Critical Path��node ���Ǥ��@�� :");
        cp[0] = graph[0].vertex;
        //cp[t] = graph[node_num-1].vertex;//�̫�@���I���bcritical path�W
        for (i=0;i<cp_num;i++){
            printf("%d",cp[i]);
            if (cp[i] != graph[node_num-1].vertex)printf("    ->    ");
        }
        printf("\n");
        printf("Critical path���׬��G%d\n",totaltime);
      	return 1;
}






