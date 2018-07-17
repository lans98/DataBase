//Basado en implementación de Ashraful Islam
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include "address.hpp"
using namespace std;
using namespace address;
using type_key = size_t; //hash es un size_t
using type_reg = Address<int>;
int nVal;
int nPointer;//P1 P2... Pn , n=nPointer
struct node {
    bool leaf;
    bool isRoot;
    node *par;//parent
    vector<type_key> value;
    vector<node*> child;
    vector<vector<type_reg>> regs; // si es hoja se usa este y no child. regs = vector de las posiciones de los registros
    node *last;
};
struct pNode{ // para imprimir b+
    node *tNode;//target node
    bool nl;
    pNode(node *Node, bool b){
        tNode = Node;
        nl = b;
    }
    pNode(){

    }
};
node *Root=NULL;
queue<pNode>q; //para imprimir

node* getTargetNode(node *tNode, type_key val); //tNode es un nodo tentativo, de partida para hallar el nodo real
node* getNewNode(bool isLeaf,bool isRoot); // se le dice si el nuevo nodo es hoja o raiz

void insertInParentNode(node *n, type_key kprime, node *nprime);
void insertInLeafNode(node *leafNode, type_key k, vector<type_reg> vreg);
void insert2(type_key k, type_reg reg);
void valueOfNodeInBox(node* tNode);
void bfsTraverse(node *tNode);
void phDelete(node* N, type_key k, node* p);
bool tooFewEntry(node *N);
void deleteEntry(node* N, type_key k, node* p);
void delet(type_key k, node* p);
bool isCoalesce(node *N, node *Nprime);



int main(){
    printf("How many values in each node?\n");
    FILE *fp;
    fp = fopen("input.txt","r");
    fscanf(fp,"%d",&nPointer);
    nVal = nPointer-1;


    printf("nPointer = %d\n",nPointer);
    int k; int x=1;
    while(fscanf(fp,"%d",&k)!=EOF){
        printf("val: %d\n",k);
        type_reg reg(x++,NULL);
        insert2(k,reg);
    }
    while(true){
        printf("Action: \npress 1 to insert\npress 2 to print in tree structure\npress 3 for delete\npress 0 for exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            type_key value;
            scanf("%d",&value);            
            type_reg reg2(x++,NULL);
            insert2(value,reg2);
        }else if(choice==2){
            printf("\n\n\n");
            bfsTraverse(Root);
            printf("\n\n\n");
        }else if(choice==3){
            type_key delV;
            scanf("%d",&delV);
            delet(delV,NULL);
        }else if(choice==0) break;
    }
    fclose(fp);
    return 0;
}
node* getTargetNode(node *tNode, type_key val){
    if(tNode->leaf) return tNode;
    int i;
    for(i=0;i<tNode->value.size();i++){
        if(tNode->value[i]>val) break;
    }
    return getTargetNode(tNode->child[i],val);
}
node* getNewNode(bool isLeaf,bool isRoot){
    node* tmp = new node;
    tmp->isRoot = isRoot;
    tmp->leaf = isLeaf;
    tmp->last = NULL;
    return tmp;
}
void insertInParentNode(node *n, type_key kprime, node *nprime){
   // printf("dbg: reached insertinparennode\n");
//
    if(n->isRoot){
        Root = getNewNode(false,true);
        n->isRoot=false;

        Root->child.push_back(n);
        Root->child.push_back(nprime);
        Root->value.push_back(kprime);
        n->par = Root;
        nprime->par = Root;
    }else{
        //printf("dbg: reached insertinparent node else\n");
        node *p = n->par;

        //printf("dbg: parent found\n");
        //printf("dbg: content of parent\n");
        //bfsTraverse(p);


        int i;
        for(i=0;i<p->value.size();i++){
            if(p->value[i]>kprime) break;
        }
        type_key tmpK;
        node *tmpP;

        for(int j = i; j<p->value.size(); j++){
            tmpK = p->value[j];
            tmpP = p->child[j+1];

            p->value[j] = kprime;
            p->child[j+1] = nprime;

            kprime = tmpK;
            nprime = tmpP;
        }
        p->value.push_back(kprime);
        p->child.push_back(nprime);
        nprime->par = p;

        //printf("dbg: reached before split\n");

        //printf("\n\np->child size: %d\n\n",p->child.size());

        if(p->child.size()>nPointer){
            node *pprime = getNewNode(false,false);
            int nbytwoceil = (nPointer+1)/2;
            type_key kdoubleprime = p->value[nbytwoceil-1];
            for(i = nbytwoceil; i<p->value.size();i++){
                pprime->child.push_back(p->child[i]);
                p->child[i]->par = pprime;
                pprime->value.push_back(p->value[i]);
            }
            pprime->child.push_back(p->child[i]);
            p->child[i]->par = pprime;


            p->value.erase(p->value.begin()+nbytwoceil-1,p->value.end());
            p->child.erase(p->child.begin()+nbytwoceil,p->child.end());

            insertInParentNode(p,kdoubleprime,pprime);
        }
    }

}


void insertInLeafNode(node *leafNode, type_key k, vector<type_reg> reg){ // se inserta un nuevo key, un nuevo vector. solo se hace push_back si el key es repetido(no entrara aca)
    int i;
    for(i=0;i<leafNode->value.size();i++){
        if(k<leafNode->value[i]) break;
    }
    type_key tmpK;
    vector<type_reg> tmpReg;

    for(int j = i; j<leafNode->value.size(); j++){
        tmpReg = leafNode->regs[j];
        tmpK = leafNode->value[j];

        leafNode->regs[j] = reg;
        leafNode->value[j] = k;

        reg = tmpReg;
        k = tmpK;
    }
    leafNode->regs.push_back(reg);
    leafNode->value.push_back(k);

}


//void insert2(int k, node *p){
void insert2(type_key k, type_reg reg){
    node *leafNode;
    if(Root==NULL){
        Root = getNewNode(true,true); //node* getNewNode(bool isLeaf,bool isRoot);
        leafNode = Root;
    }else leafNode = getTargetNode(Root,k);

    //printf("dbg: target node content:\n");
    //if(leafNode->value.size()>0) bfsTraverse(leafNode);

    type_key keyValueCount = leafNode->value.size();

    //no repetidos
    if(keyValueCount>=1){
	    int pos;
	    for(pos=0;pos<leafNode->value.size();pos++){
	        if(leafNode->value[pos]==k){
	        	//añadimos a vector de regs
	        	leafNode->regs[pos].push_back(reg);
	        	return;
	        }
    	}
    } 
    vector<type_reg> vreg={reg};
    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,vreg);
    else{

        //printf("dbg: reached in else1\n");
        node* leafNode2 = getNewNode(true,false);
        insertInLeafNode(leafNode,k,vreg);

        //printf("dbg: inserted in leaf node\n");
       // printf("dbg: content\n");
        //bfsTraverse(leafNode);

        leafNode2->last = leafNode->last;
        leafNode2->par = leafNode->par;

        leafNode->last=leafNode2;
        int nbytwoceil = (nPointer+1)/2;

        for(int i = nbytwoceil; i<nPointer ; i++){
            leafNode2->regs.push_back(leafNode->regs[i]);
            leafNode2->value.push_back(leafNode->value[i]);
        }
        leafNode->value.erase(leafNode->value.begin()+nbytwoceil,leafNode->value.end());
        leafNode->regs.erase(leafNode->regs.begin()+nbytwoceil,leafNode->regs.end());

        //printf("dbg: leafnode split complete\n");
        //printf("dbg: leafnode1 content:\n");
       // bfsTraverse(leafNode);
        //printf("dbg: leafnode2 content:\n");
        //bfsTraverse(leafNode2);

        type_key kprime = leafNode2->value[0];
        insertInParentNode(leafNode,kprime,leafNode2);
    }
}

void valueOfNodeInBox(node* tNode){
    printf(" [");
    int i=0 ;
    for(;i<tNode->value.size()-1;i++){
        if(tNode->leaf){
        	printf("%d -> ",tNode->value[i]);
        	for(int j=0; j<tNode->regs[i].size(); j++){
        		printf("%d ",tNode->regs[i][j].get_disk_address());
        	}
        	printf("|");
        }else
            printf("%d|",tNode->value[i]);
    }
    if(tNode->value.size()>0) 
        if(tNode->leaf){
        	printf("%d -> ",tNode->value[i]);
        	for(int j=0; j<tNode->regs[i].size(); j++){
        		printf("%d ",tNode->regs[i][j].get_disk_address());
        	}
        	printf("]");
        }
        else
            printf("%d]",tNode->value[i]);
    //printf(" ");
}



void bfsTraverse(node *tNode){

    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();
        //printf("  Got pNode ");
        node *temp = p.tNode;
        q.pop();
        valueOfNodeInBox(temp);
        //printf(" printed temp ");
        if(p.nl) printf("\n");
        int i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }
        //printf(" inserted second last child  ");
        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));
        //printf(" inserted  last child  ");
        }

    }
}

void phDelete(node* N, type_key k, node* p){
    int i;
    for(i=0;i<N->value.size();i++){
        if(N->value[i]==k) break;
    }
    N->value.erase(N->value.begin()+i);
    if (N->leaf) N->regs.erase(N->regs.begin()+i);
    if(!N->leaf) N->child.erase(N->child.begin()+i+1);
    printf("After ph delete contentent: \n");
    if(N->value.size()>0)bfsTraverse(N);
}


bool tooFewEntry(node *N){

    if(N->leaf){
        int minV = nPointer/2;
        if(N->value.size()<minV) return true;
    }else{
        int minC = (nPointer+1)/2;
        if(N->child.size()<minC) return true;
    }
    return false;
}

void deleteEntry(node* N, type_key k, node* p){
    phDelete(N,k,p);
    if(N->isRoot && N->child.size()==1){
      Root = N->child[0];
      Root->par = NULL;
      Root->isRoot = true;
    }
    if(tooFewEntry(N) && !N->isRoot){
        printf("too few entry\n");
        node *parN = N->par;
        int i;
        for(i=0;i<parN->child.size();i++){
            if(parN->child[i]==N) break;
        }
        printf("dbg: The node is %dth child of its parent\n",i);
        printf("dbg: parent has %d childs\n",parN->child.size());

        node *leftSib, *rightSib, *Nprime;
        bool left;
        if(i>0) leftSib = parN->child[i-1];
        else leftSib = NULL;

        if(i<parN->child.size()-1) rightSib = parN->child[i+1];
        else rightSib = NULL;

        if(leftSib==NULL) left=false;
        else if(rightSib==NULL) left = true;
        else if(leftSib->value.size()>rightSib->value.size()) left=false;
        else left = true;
        type_key kprime;

        if(left){
            Nprime = leftSib;
            kprime = parN->value[i-1];
            printf("Left sibling selected\n");
        }
        else {
            Nprime = rightSib;
            kprime = parN->value[i];
            printf("Right sibling selected\n");
        }
        printf("kprime %d\n",kprime);

        //int totalValue = N->child.size()+Nprime->child.size();
        //printf("dbg: totalValue: %d\n",totalValue);
        if(isCoalesce(N,Nprime)){
                printf("Coalesce\n");
                if(!left){
                    node *tmp = N;
                    N = Nprime;
                    Nprime = tmp;
                }
               // printf("reached after swap\n");
                if(!N->leaf){
                    Nprime->value.push_back(kprime);
                    int j;
                    for(j = 0; j<N->value.size();j++){
                        Nprime->child.push_back(N->child[j]);
                        Nprime->value.push_back(N->value[j]);
                        N->child[j]->par = Nprime;
                    }
                    Nprime->child.push_back(N->child[j]);
                    N->child[j]->par = Nprime;
                    //printf("reached not leaf complete\n");
                }else{
                     //printf("reached leaf start\n");
                    // printf("Content of N\n");
                     //bfsTraverse(N);
                     printf("Content of Nprime(before)\n");
                     bfsTraverse(Nprime);



                    for(int j = 0; j<N->value.size();j++){
                        Nprime->value.push_back(N->value[j]);
                        Nprime->regs.push_back(N->regs[j]);
                        ///Nprime->child.push_back(N->child[j]);
                        ///N->child[j]->par = Nprime;
                    }
                     //printf("reached leaf after loop\n");
                     Nprime->last = N->last;
                    //printf("reached leaf complete\n");
                }
                printf("Nprime content(after): \n");
                bfsTraverse(Nprime);
                deleteEntry(parN,kprime,N);

        }else{
                printf("dbg: Redistribute\n");

                printf("dbg: content of Nprime\n");
                bfsTraverse(Nprime);

                if(left){
                    printf("dbg: redistribute for left\n");

                    if(!N->leaf){
                        int m = Nprime->child.size()-1;
                        type_key tmpV = Nprime->value[m-1];
                        node *tmpP = Nprime->child[m];
                        Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin()+m-1);
                        N->child.insert(N->child.begin(),tmpP);
                        N->value.insert(N->value.begin(),kprime);
                        parN->value[i-1] = tmpV;//???
                      // parN->value[i-1] = N->
                    }else{
                        int m = Nprime->value.size()-1;
                        type_key tmpV = Nprime->value[m];
                        vector<type_reg> vreg = Nprime->regs[m];
                        ///node *tmpP = Nprime->child[m];
                        ///Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->regs.erase(Nprime->regs.begin()+m);
                        Nprime->value.erase(Nprime->value.begin()+m);
                        ///N->child.insert(N->child.begin(),tmpP);
                        N->regs.insert(N->regs.begin(),vreg);
                        N->value.insert(N->value.begin(),tmpV);
                        parN->value[i-1] = tmpV;
                    }
                }else{

                    if(!N->leaf){
                        int m = 0;
                        type_key tmpV = Nprime->value[m];
                        node *tmpP = Nprime->child[m];

                        Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin()+m);

                        N->child.push_back(tmpP);
                        N->value.push_back(kprime);

                       // N->child.insert(N->child.begin(),tmpP);
                        //N->value.insert(N->value.begin(),kprime);

                        parN->value[i-1] = tmpV;
                    }else{
                        int m = 0;
                        type_key tmpV = Nprime->value[m];
                        vector<type_reg> vregg = Nprime->regs[m];
                        ///node *tmpP = Nprime->child[m];
                        ///Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin());
                        Nprime->regs.erase(Nprime->regs.begin());
                        ///N->child.insert(N->child.begin(),tmpP);
                       // N->value.insert(N->value.begin(),tmpV);
                        N->value.push_back(tmpV);
                        N->regs.push_back(vregg);
                        parN->value[i] = Nprime->value[0];
                    }

                }
        }


    }else{
        printf("Enough Entry\n");
    }

}

void delet(type_key k, node* p){
    node *L = getTargetNode(Root,k);
    printf("content: \n");
    bfsTraverse(L);
    deleteEntry(L,k,p); // key k esta en nodo L, p es el puntero de esa key

}


bool isCoalesce(node *N, node *Nprime){
    if(N->leaf){
        if(nVal>=(N->value.size()+Nprime->value.size())) return true;
        return false;
    }
    if(nPointer>=(N->child.size()+Nprime->child.size())) return true;
    return false;
}
