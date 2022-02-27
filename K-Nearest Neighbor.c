#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
typedef struct data{
	int n;
	double distance;
	short int mat[28][28];
	struct data *next;
}num_struct;
void afficheList(num_struct *E){
	num_struct *nouveau=E;
	while(nouveau!=NULL){
		printf("- Number : %d\tDistance : %lf\n",nouveau->n,nouveau->distance);
		nouveau=nouveau->next;
	}
}
num_struct *ajoutListe(num_struct *E,num_struct S){
	num_struct *nouveau,*iter = E;
	nouveau=(num_struct*)malloc(sizeof(num_struct));
	nouveau->n = S.n;
	nouveau->distance = S.distance;
	int i,j;
	for(i=0;i<28;i++){
		for(j=0;j<28;j++){
			nouveau->mat[i][j] = S.mat[i][j];
		}
	}
	if(E == NULL){
		nouveau->next=E;
		E=nouveau;		
		return E;
	}
	else{
		if(iter->distance > nouveau->distance){
			nouveau->next = iter;
			iter = nouveau;
			return nouveau;
		}
		else{
			while(iter->next != NULL && iter->next->distance<nouveau->distance){
				iter=iter->next;
			}
			nouveau->next = iter->next;
			iter->next=nouveau;
			return E;
		}
	}	
}
const char* getfield(char* line, int num){
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
        if(!--num)
            return tok;
    return NULL;
}
void distance_euclidienne(num_struct *T,num_struct *S){
	int i,j;
	float Somme=0;
	for(i=0;i<28;i++){
		for(j=0;j<28;j++){
			Somme += pow(T->mat[i][j]-S->mat[i][j],2);
		}
	}
	T->distance = sqrt(Somme);
}
num_struct *Read(num_struct *E,num_struct *T){
	FILE* stream = fopen("DataSet.csv", "r");
    char line[10240];
    int ln=0,x = 0,i,j,l=0,c;
    while (fgets(line, 10240, stream)){
    	if(ln == 0){
    		ln++;
    		continue;
		}
		T[x].n = atoi(getfield(strdup(line), 1));
		for(j=2;j<=785;j++){
			T[x].mat[l][c] = atoi(getfield(strdup(line), j));
			c++;
			if((j-1)%28 == 0){
				c=0;
				l++;
			}
		}
		l=0;
		x++;
	}
	for(i=0;i<=999;i++){
		distance_euclidienne(&T[i],&T[1000]);
		E=ajoutListe(E,T[i]);
	}
	return E;
}
int KNeighborsClassifier(num_struct *E,int k){
	int numbers[10] = {0};
	int i=0,max=0,count=0;
	num_struct *nouveau=E;
	while(nouveau!=NULL){
		if(i<k){
			numbers[nouveau->n] += 1;
			i += 1;
			nouveau = nouveau->next;
		}
		else{
			nouveau = NULL;
		}
	}
	printf("\nThe number %d is repeated = %d times\n",0,numbers[0]);
	for(i=1;i<=9;i++){
		printf("The number %d is repeated = %d times\n",i,numbers[i]);
		if(numbers[i]>numbers[max]){
			max = i;
		}
	}
	for(i=0;i<=9;i++){
		if(numbers[i]==numbers[max]){
			count += 1;
		}
	}
	if(count==1){
		return max;
	}
	return -1;
}
void main(){
	int n,K=1;
	num_struct T[1001];
	num_struct *E=NULL;
	E=Read(E,T);
	afficheList(E);
	do{
		K+=2;
		n=KNeighborsClassifier(E,K);
	}while(n<0);
	printf("\n\n");
	printf("Best K : %d\n",K);
	printf("The closest value to the unknown point : %d",n);
}
