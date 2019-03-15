 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>

#define MAX_QUEUE_SIZE 50
typedef struct Node *nodePointer;
typedef struct Node {
  char chr;
  int freq;
  nodePointer left;
  nodePointer right;
}n;
struct Encode {
    char data;
    char code[MAX_QUEUE_SIZE];
};
struct MINHeap {
    int size;
    int volume;
    nodePointer* arr;
};
//int MAX_QUEUE_SIZE;
nodePointer priorityQueue[MAX_QUEUE_SIZE];
struct Encode encode[MAX_QUEUE_SIZE];
int encode_index = 0;

int kind_count(char* input);
void freq_count(char* input,int kinds);
void Make_list(int kinds);
struct MINHeap* buildMINheap(int kinds);
void adjust(struct MINHeap* minheap, int parent);
void swapHeapNode(nodePointer* a,nodePointer* b);
void HuffmanEncode(nodePointer root,char a[],int code_count,FILE *ptr2);
void getresult(char a[],int n,FILE *ptr2,int encode_index);
nodePointer  buildHuffTree(int kinds);
nodePointer extractMin(struct MINHeap* minHeap);
void insertHeap(struct MINHeap* minheap,nodePointer node);
void decode(char* input,int count,FILE* ptr2);
 //struct Node* buildHuffTree(nodePointer start,int kinds);
 //nodePointer2 char_push(nodePointer2*, char );
 //char* move2(char*[] ,int );


int main(){
 	FILE *ptr1;
    FILE *ptr2;
    char *input=(char *)malloc(50*sizeof(char));
    ptr1 = fopen("text.txt","r+");
    ptr2 = fopen("code.txt","w+");
    //��text.txt�ɮפ@��@��Ū�J
    //fgets()����k�O����Ū�@��@�檺�A������size�O�nŪ���j�p
	//�Y����char�� > size-1 �A�hŪ��size-1�A�B����\0
	//�Y����char�� < size-1	�A�h������Ū���A�B����\0(�̦hsize-1)
    if(feof(ptr1) == 0){
            fgets(input, 50, ptr1);
            printf("input = %s\n",input);
            //printf("strlen = %d\n",strlen(input));
            int i,j,k,count;
            count = kind_count(input);  //�o��⧹�����X�إB�wmalloc�O���鵹priotyQueue
            printf("count = %d\n",count);
            //for (i=0;i<count;i++){
            //    printf("priorityQueue[%d] -> chr = %c\n",i,priorityQueue[i] -> chr);
            //}
            freq_count(input,count);  //��C��char���W�v
            //printf("FFFFFFF\n");
            //for(int j =0;j < count;j++)
            //    printf("priorityQueue[%d] -> freq = %d\n",j,priorityQueue[j] -> freq);
            //Make_list(count);
            //priorityQueue* = sort(&priorityQueue);
        	//�w��data�Ƨ�
            //struct MINHeap* heap_root = buildMINheap(count);
            //struct Node* root = buildHuffTree(priorityQueue[0], count);
			//��minheap
			nodePointer root =  buildHuffTree(count);

            char a[MAX_QUEUE_SIZE];
            int code_count = 0;
            HuffmanEncode(root,a,code_count,ptr2);
			//huffman�t��k && Encoding
			decode(input,count,ptr2);
    }
    fclose(ptr1);
    fclose(ptr2);
 	return 0;
 }
 //���`�@���X��
 int kind_count(char* input){
    int i,j,count = 0,q_index = 0;;
    for (i = 0; i <strlen(input) ;i++){
        for(j =0;j<i;j++)
            if(input[i]==input[j]){break;}

        if((i==j)&&(i!=0)){
            count++;
            priorityQueue[q_index] =(nodePointer)malloc(sizeof(struct Node));
            priorityQueue[q_index++] -> chr = input[i];}
        else if (i==0) {
            count++;
            priorityQueue[q_index] =(nodePointer)malloc(sizeof(struct Node));
            priorityQueue[q_index++] -> chr = input[i];}
        else{};
    }
    return count;
 }
 //��C�ت��W�v
 void freq_count(char* input,int kinds){
    for(int j =0;j < kinds;j++)
        priorityQueue[j] -> freq = 0;
        printf("GG\n");
    //printf("strlen(input) = %d\n",strlen(input));
    for(int i = 0;i < strlen(input);i++){
        for(int j =0;j < kinds;j++){
            if(input[i]==priorityQueue[j] -> chr){
                printf("priorityQueue[%d] -> chr = %c\n",j,priorityQueue[j] -> chr);
                priorityQueue[j] -> freq++;
                printf("GGC\n");
                printf("priorityQueue[%d] -> freq = %d\n\n",j,priorityQueue[j] -> freq);
                break;
            }
        }
    }
 }
 void Make_list(int kinds){
    for (int i = 0; i <kinds;i++){
        if(i<kinds-1)
            priorityQueue[i] -> right = priorityQueue[i+1];
        else{priorityQueue[i] -> right=NULL;}
    }
    //int index=0;
    //for (priorityQueue[index];priorityQueue[index] -> right!=NULL;priorityQueue[index]=priorityQueue[index] -> right,index++){
    //    printf("priorityQueue[%d] -> chr = %c\n",index,priorityQueue[index] -> chr);
    //}
    //printf("priorityQueue[%d] -> chr = %c\n",index,priorityQueue[index] -> chr);
 }
nodePointer  buildHuffTree(int kinds){
    nodePointer lchild,rchild,parent;
    struct MINHeap* minheap = buildMINheap(kinds);
    //���D�ѤUroot�A���M�Npop��ӳ̤p���X�ӥ[���A��^�h
    while (minheap->size != 1){
        lchild = extractMin(minheap);
        rchild = extractMin(minheap);
        parent =(nodePointer)malloc (sizeof(struct Node));
        parent -> chr = '\0';
        parent -> freq = lchild -> freq + rchild -> freq;
        parent -> left = lchild;parent -> right = rchild;
        insertHeap(minheap,parent);
    }
    return extractMin(minheap);
}
struct MINHeap* buildMINheap(int kinds){
    struct MINHeap* minheap = (struct MINHeap*)malloc(sizeof(struct MINHeap));
    minheap->size = 0;
    minheap->volume = kinds;
    minheap->arr = (nodePointer*)malloc(kinds * sizeof(nodePointer));
    //�N�C��Node�����Щ�Jminheap���A�o�˴N�i�H�ޱ��o��Node
    for (int i = 0; i < kinds; i++)
        minheap->arr[i] = priorityQueue[i];

    minheap->size = kinds;

    //int i;
    for (int i = (kinds - 1) / 2; i >= 0; i--)
        adjust(minheap, i);
    return minheap;
}

//insert Node����i��adjust
void adjust(struct MINHeap* minheap, int parent){
    int pos = parent;
    int left = 2 * parent + 1;
    int right = 2 * parent + 2;

    if (left < minheap->size && minheap->arr[left]->freq < minheap->arr[pos]->freq)
        pos = left;

    if (right < minheap->size && minheap->arr[right]->freq < minheap->arr[pos]->freq)
        pos = right;

    if (pos != parent) {
        swapHeapNode(&minheap->arr[pos],&minheap->arr[parent]);
        adjust(minheap, pos);
    }
}

//����I�u���v����C���ɭԡA�M�l�I�洫��m
void swapHeapNode(nodePointer* a,nodePointer* b){
    nodePointer t = *a;
    *a = *b;
    *b = t;
}


nodePointer extractMin(struct MINHeap* minheap){
    nodePointer temp = minheap->arr[0];
    minheap->arr[0] = minheap->arr[minheap->size - 1];
    minheap->size--;
    adjust(minheap, 0);
    return temp;

}
void insertHeap(struct MINHeap* minheap,nodePointer node){
    minheap->size++;
    int i = minheap->size - 1;

    while (i && (node->freq) < (minheap->arr[(i ) / 2]->freq)) {
        minheap->arr[i] = minheap->arr[(i ) / 2];
        i = (i ) / 2;
    }
    minheap->arr[i] = node;
}

void HuffmanEncode(nodePointer root,char a[],int code_count,FILE *ptr2){
    // ���l�I�s0
    if (root->left) {
        a[code_count] = '0';
        HuffmanEncode(root->left, a, code_count + 1,ptr2);
    }
    //�k�l�I�s1
    if (root->right) {
        a[code_count] = '1';
        HuffmanEncode(root->right, a, code_count + 1,ptr2);
    }
    //�p�G�Oleaf�N��L���s�X�L�X��
    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->chr);
        fprintf(ptr2,"%c: ", root->chr);
        fprintf(ptr2,"%d ", root->freq);
        encode[encode_index].data = root->chr;
        //printf("%c %d",encode[encode_index].data = root->chr,encode_index);
        getresult(a, code_count,ptr2,encode_index);
        //printf("\n");
        encode_index++;
    }
}
//�L�X�s�X
void getresult(char a[],int n,FILE *ptr2,int encode_index){
     for (int i = 0; i < n; i++){
        printf("%c", a[i]);
        fprintf(ptr2,"%c",a[i]);
        encode[encode_index].code[i] = a[i];
        //printf("encode[encode_index].code[i] = %d ",encode[encode_index].code[i]);
     }
     printf("\n");
     fprintf(ptr2,"\n");
}

void decode (char* input,int n,FILE* ptr2){
    //printf("%s\n",input);
    //for(int j = 0;j < n ;j++){
    //    printf("%c\n",encode[j].data);}
    //printf("%d\n",strlen(input));
    for (int i=0;i<strlen(input);i++){
        for(int j = 0;j < n ;j++){
            if (input[i] == encode[j].data){
               printf("%s",encode[j].code);
               fprintf(ptr2,"%s",encode[j].code);
            }
        }
    }
}




