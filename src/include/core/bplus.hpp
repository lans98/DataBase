#ifndef SCPPDB_BPLUS_HPP
#define SCPPDB_BPLUS_HPP

//Basado en implementación de Ashraful Islam Emon
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>

#include <fstream>
#include <string>
#include <functional>

#include <core/address.hpp>

namespace bplus {

	using namespace std;
	using namespace address;

	class BPlus {
	public:	
		using type_key = size_t; //hash es un size_t
		using type_reg = Address<size_t>;
		const string extension_arch_tabla = ".tabla";
		const int tam_bloque_de_disco = 4096;//bytes
		const int tam_key = 4;
		const int tam_address = 16;

		int nPointer = tam_bloque_de_disco/(tam_key+tam_address);//P1 P2... Pn , n=nPointer, 200
		int nVal=nPointer-1;
		struct node {
		    bool leaf;
		    bool isRoot;
		    Address<node> par;//parent
		    vector<type_key> value;
		    vector<Address<node>> child;
		    vector<vector<type_reg>> regs; // si es hoja se usa este y no child. regs = vector de las posiciones de los registros
		    Address<node> last;
		};
		struct pNode{ // para imprimir b+
		    Address<node> tNode;//target node
		    bool nl;    
		    pNode(Address<node> Node, bool b){
		        tNode.set_memory_address(Node.get_memory_address());
		        nl = b;
		    }
		    pNode(){

		    }
		};	
		Address<node> Root;
		queue<pNode>q; //para imprimir
	public:
		Address<node> getTargetNode(Address<node> tNode, type_key val){//tNode es un nodo tentativo, de partida para hallar el nodo real
		    if(tNode.get_memory_address()->leaf) return tNode;
		    int i;
		    for(i=0;i<tNode.get_memory_address()->value.size();i++){
		        if(tNode.get_memory_address()->value[i]>val) break;
		    }
		    return getTargetNode(tNode.get_memory_address()->child[i],val);
		}

		Address<node> getNewNode(bool isLeaf,bool isRoot){// se le dice si el nuevo nodo es hoja o raiz
		    Address<node> tmp;
		    tmp.set_memory_address(new node);
		    tmp.get_memory_address()->isRoot = isRoot;
		    tmp.get_memory_address()->leaf = isLeaf;
		    tmp.get_memory_address()->last.set_memory_address(NULL);
		    return tmp;/////////////////
		}

		void insertInParentNode(Address<node> n, type_key kprime, Address<node> nprime){
		   // printf("dbg: reached insertinparennode\n");
		    if(n.get_memory_address()->isRoot){
		        Root.set_memory_address(getNewNode(false,true).get_memory_address());
		        n.get_memory_address()->isRoot=false;

		        Root.get_memory_address()->child.push_back(n);
		        Root.get_memory_address()->child.push_back(nprime);
		        Root.get_memory_address()->value.push_back(kprime);
		        n.get_memory_address()->par.set_memory_address(Root.get_memory_address());
		        nprime.get_memory_address()->par.set_memory_address(Root.get_memory_address());
		    }else{
		        //printf("dbg: reached insertinparent node else\n");
		        Address<node> p;
		        p.set_memory_address(n.get_memory_address()->par.get_memory_address());

		        //printf("dbg: parent found\n");
		        //printf("dbg: content of parent\n");
		        //bfsTraverse(p);


		        int i;
		        for(i=0;i<p.get_memory_address()->value.size();i++){
		            if(p.get_memory_address()->value[i]>kprime) break;
		        }
		        type_key tmpK;
		        Address<node> tmpP;

		        for(int j = i; j<p.get_memory_address()->value.size(); j++){
		            tmpK = p.get_memory_address()->value[j];
		            tmpP = p.get_memory_address()->child[j+1];

		            p.get_memory_address()->value[j] = kprime;
		            p.get_memory_address()->child[j+1].set_memory_address(nprime.get_memory_address());

		            kprime = tmpK;
		            nprime.set_memory_address(tmpP.get_memory_address());
		        }
		        p.get_memory_address()->value.push_back(kprime);
		        p.get_memory_address()->child.push_back(nprime);
		        nprime.get_memory_address()->par.set_memory_address(p.get_memory_address());

		        //printf("dbg: reached before split\n");

		        //printf("\n\np.get_memory_address()->child size: %d\n\n",p.get_memory_address()->child.size());

		        if(p.get_memory_address()->child.size()>nPointer){
		            Address<node> pprime;
		            pprime.set_memory_address(getNewNode(false,false).get_memory_address());
		            int nbytwoceil = (nPointer+1)/2;
		            type_key kdoubleprime = p.get_memory_address()->value[nbytwoceil-1];
		            for(i = nbytwoceil; i<p.get_memory_address()->value.size();i++){
		                pprime.get_memory_address()->child.push_back(p.get_memory_address()->child[i]);
		                p.get_memory_address()->child[i].get_memory_address()->par.set_memory_address(pprime.get_memory_address());
		                pprime.get_memory_address()->value.push_back(p.get_memory_address()->value[i]);
		            }
		            pprime.get_memory_address()->child.push_back(p.get_memory_address()->child[i]);
		            p.get_memory_address()->child[i].get_memory_address()->par.set_memory_address(pprime.get_memory_address());


		            p.get_memory_address()->value.erase(p.get_memory_address()->value.begin()+nbytwoceil-1,p.get_memory_address()->value.end());
		            p.get_memory_address()->child.erase(p.get_memory_address()->child.begin()+nbytwoceil,p.get_memory_address()->child.end());

		            insertInParentNode(p,kdoubleprime,pprime);
		        }
		    }

		}


		void insertInLeafNode(Address<node> leafNode, type_key k, vector<type_reg> reg){ // se inserta un nuevo key, un nuevo vector. solo se hace push_back si el key es repetido(no entrara aca)
		    int i;
		    for(i=0;i<leafNode.get_memory_address()->value.size();i++){
		        if(k<leafNode.get_memory_address()->value[i]) break;
		    }
		    type_key tmpK;
		    vector<type_reg> tmpReg;

		    for(int j = i; j<leafNode.get_memory_address()->value.size(); j++){
		        tmpReg = leafNode.get_memory_address()->regs[j];
		        tmpK = leafNode.get_memory_address()->value[j];

		        leafNode.get_memory_address()->regs[j] = reg;
		        leafNode.get_memory_address()->value[j] = k;

		        reg = tmpReg;
		        k = tmpK;
		    }
		    leafNode.get_memory_address()->regs.push_back(reg);
		    leafNode.get_memory_address()->value.push_back(k);

		}


		//void insert2(int k, Address<node> p){
		void insert(type_key k, type_reg reg){
		    Address<node> leafNode;
		    if(Root.get_memory_address()==NULL){
		        Root.set_memory_address(getNewNode(true,true).get_memory_address()); //Address<node>  getNewNode(bool isLeaf,bool isRoot);
		        leafNode = Root;
		    }else leafNode = getTargetNode(Root,k);

		    //printf("dbg: target node content:\n");
		    //if(leafNode.get_memory_address()->value.size()>0) bfsTraverse(leafNode);

		    type_key keyValueCount = leafNode.get_memory_address()->value.size();

		    //no repetidos
		    if(keyValueCount>=1){
			    int pos;
			    for(pos=0;pos<leafNode.get_memory_address()->value.size();pos++){
			        if(leafNode.get_memory_address()->value[pos]==k){
			        	//añadimos a vector de regs
			        	leafNode.get_memory_address()->regs[pos].push_back(reg);
			        	return;
			        }
		    	}
		    } 
		    vector<type_reg> vreg={reg};
		    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,vreg);
		    else{

		        //printf("dbg: reached in else1\n");
		        Address<node> leafNode2;
		        leafNode2.set_memory_address(getNewNode(true,false).get_memory_address());
		        insertInLeafNode(leafNode,k,vreg);

		        //printf("dbg: inserted in leaf node\n");
		       // printf("dbg: content\n");
		        //bfsTraverse(leafNode);

		        leafNode2.get_memory_address()->last.set_memory_address(leafNode.get_memory_address()->last.get_memory_address());
		        leafNode2.get_memory_address()->par.set_memory_address(leafNode.get_memory_address()->par.get_memory_address());

		        leafNode.get_memory_address()->last.set_memory_address(leafNode2.get_memory_address());
		        int nbytwoceil = (nPointer+1)/2;

		        for(int i = nbytwoceil; i<nPointer ; i++){
		            leafNode2.get_memory_address()->regs.push_back(leafNode.get_memory_address()->regs[i]);
		            leafNode2.get_memory_address()->value.push_back(leafNode.get_memory_address()->value[i]);
		        }
		        leafNode.get_memory_address()->value.erase(leafNode.get_memory_address()->value.begin()+nbytwoceil,leafNode.get_memory_address()->value.end());
		        leafNode.get_memory_address()->regs.erase(leafNode.get_memory_address()->regs.begin()+nbytwoceil,leafNode.get_memory_address()->regs.end());

		        //printf("dbg: leafnode split complete\n");
		        //printf("dbg: leafnode1 content:\n");
		       // bfsTraverse(leafNode);
		        //printf("dbg: leafnode2 content:\n");
		        //bfsTraverse(leafNode2);

		        type_key kprime = leafNode2.get_memory_address()->value[0];
		        insertInParentNode(leafNode,kprime,leafNode2);
		    }
		}

		void valueOfNodeInBox(Address<node> tNode){
		    printf(" [");
		    int i=0 ;
		    for(;i<tNode.get_memory_address()->value.size()-1;i++){
		        if(tNode.get_memory_address()->leaf){
		        	printf("%d -> ",tNode.get_memory_address()->value[i]);
		        	for(int j=0; j<tNode.get_memory_address()->regs[i].size(); j++){
		        		printf("%d ",tNode.get_memory_address()->regs[i][j].get_disk_address());
		        	}
		        	printf("|");
		        }else
		            printf("%d|",tNode.get_memory_address()->value[i]);
		    }
		    if(tNode.get_memory_address()->value.size()>0)
		        if(tNode.get_memory_address()->leaf){
		        	printf("%d -> ",tNode.get_memory_address()->value[i]);
		        	for(int j=0; j<tNode.get_memory_address()->regs[i].size(); j++){
		        		printf("%d ",tNode.get_memory_address()->regs[i][j].get_disk_address());
		        	}
		        	printf("]");
		        }
		        else
		            printf("%d]",tNode.get_memory_address()->value[i]);
		    //printf(" ");
		}

		void bfsTraverse(Address<node> tNode){/////////////////
			if(tNode.get_memory_address() == NULL || tNode.get_memory_address()->value.size()==0){
				cout << "[]\n";
				return;
			}
		    q.push(pNode(tNode,true));
		    while(!q.empty()){
		        pNode p = q.front();
		        //printf("  Got pNode ");
		        Address<node> temp;
		        temp.set_memory_address(p.tNode.get_memory_address());
		        q.pop();
		        valueOfNodeInBox(temp);
		        //printf(" printed temp ");
		        if(p.nl) printf("\n");
		        int i;
		        if(!temp.get_memory_address()->leaf){
			        for(i=0;i<temp.get_memory_address()->child.size()-1;i++){
			            q.push(pNode(temp.get_memory_address()->child[i],false));
		        	}
			        //printf(" inserted second last child  ");
			        if(p.nl) q.push(pNode(temp.get_memory_address()->child[i],true));
			        else q.push(pNode(temp.get_memory_address()->child[i],false));
			        //printf(" inserted  last child  ");
		        }

		    }
		}

		void phDelete(Address<node> N, type_key k, Address<node>  p){
		    int i;
		    for(i=0;i<N.get_memory_address()->value.size();i++){
		        if(N.get_memory_address()->value[i]==k) break;
		    }
		    N.get_memory_address()->value.erase(N.get_memory_address()->value.begin()+i);
		    if (N.get_memory_address()->leaf) N.get_memory_address()->regs.erase(N.get_memory_address()->regs.begin()+i);
		    if(!N.get_memory_address()->leaf) N.get_memory_address()->child.erase(N.get_memory_address()->child.begin()+i+1);
		    printf("After ph delete contentent: \n");
		    if(N.get_memory_address()->value.size()>0)bfsTraverse(N);
		}


		bool tooFewEntry(Address<node> N){

		    if(N.get_memory_address()->leaf){
		        int minV = nPointer/2;
		        if(N.get_memory_address()->value.size()<minV) return true;
		    }else{
		        int minC = (nPointer+1)/2;
		        if(N.get_memory_address()->child.size()<minC) return true;
		    }
		    return false;
		}

		void deleteEntry(Address<node>  N, type_key k, Address<node>  p){
		    phDelete(N,k,p);
		    if(N.get_memory_address()->isRoot && N.get_memory_address()->child.size()==1){
		      Root = N.get_memory_address()->child[0];
		      Root.get_memory_address()->par = NULL;
		      Root.get_memory_address()->isRoot = true;
		    }
		    if(tooFewEntry(N) && !N.get_memory_address()->isRoot){
		        printf("too few entry\n");
		        Address<node> parN;
		        parN.set_memory_address(N.get_memory_address()->par.get_memory_address());
		        int i;
		        for(i=0;i<parN.get_memory_address()->child.size();i++){
		            if(parN.get_memory_address()->child[i].get_memory_address()==N.get_memory_address()) break;
		        }
		        printf("dbg: The node is %dth child of its parent\n",i);
		        printf("dbg: parent has %d childs\n",parN.get_memory_address()->child.size());

		        Address<node> leftSib, rightSib, Nprime;
		        bool left;
		        if(i>0) 
		        	leftSib.set_memory_address(parN.get_memory_address()->child[i-1].get_memory_address());
		        else leftSib.set_memory_address(NULL);

		        if(i<parN.get_memory_address()->child.size()-1) 
		        	rightSib.set_memory_address(parN.get_memory_address()->child[i+1].get_memory_address());
		        else rightSib.set_memory_address(NULL);

		        if(leftSib.get_memory_address()==NULL) left=false;
		        else if(rightSib.get_memory_address()==NULL) left = true;
		        else if(leftSib.get_memory_address()->value.size()>rightSib.get_memory_address()->value.size()) left=false;
		        else left = true;
		        type_key kprime;

		        if(left){
		            Nprime.set_memory_address(leftSib.get_memory_address());
		            kprime = parN.get_memory_address()->value[i-1];
		            printf("Left sibling selected\n");
		        }
		        else {
		            Nprime.set_memory_address(rightSib.get_memory_address());
		            kprime = parN.get_memory_address()->value[i];
		            printf("Right sibling selected\n");
		        }
		        printf("kprime %d\n",kprime);

		        //int totalValue = N.get_memory_address()->child.size()+Nprime.get_memory_address()->child.size();
		        //printf("dbg: totalValue: %d\n",totalValue);
		        if(isCoalesce(N,Nprime)){
		                printf("Coalesce\n");
		                if(!left){
		                    Address<node> tmp;
		                    tmp.set_memory_address(N.get_memory_address());
		                    N = Nprime;
		                    Nprime = tmp;
		                }
		               // printf("reached after swap\n");
		                if(!N.get_memory_address()->leaf){
		                    Nprime.get_memory_address()->value.push_back(kprime);
		                    int j;
		                    for(j = 0; j<N.get_memory_address()->value.size();j++){
		                        Nprime.get_memory_address()->child.push_back(N.get_memory_address()->child[j]);
		                        Nprime.get_memory_address()->value.push_back(N.get_memory_address()->value[j]);
		                        N.get_memory_address()->child[j].get_memory_address()->par.set_memory_address(Nprime.get_memory_address());
		                    }
		                    Nprime.get_memory_address()->child.push_back(N.get_memory_address()->child[j]);
		                    N.get_memory_address()->child[j].get_memory_address()->par.set_memory_address(Nprime.get_memory_address());
		                    //printf("reached not leaf complete\n");
		                }else{
		                     //printf("reached leaf start\n");
		                    // printf("Content of N\n");
		                     //bfsTraverse(N);
		                     printf("Content of Nprime(before)\n");
		                     bfsTraverse(Nprime);



		                    for(int j = 0; j<N.get_memory_address()->value.size();j++){
		                        Nprime.get_memory_address()->value.push_back(N.get_memory_address()->value[j]);
		                        Nprime.get_memory_address()->regs.push_back(N.get_memory_address()->regs[j]);
		                        ///Nprime.get_memory_address()->child.push_back(N.get_memory_address()->child[j]);
		                        ///N.get_memory_address()->child[j].get_memory_address()->par = Nprime;
		                    }
		                     //printf("reached leaf after loop\n");
		                     Nprime.get_memory_address()->last.set_memory_address(N.get_memory_address()->last.get_memory_address());
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

		                    if(!N.get_memory_address()->leaf){
		                        int m = Nprime.get_memory_address()->child.size()-1;
		                        type_key tmpV = Nprime.get_memory_address()->value[m-1];
		                        Address<node> tmpP;
		                        tmpP.set_memory_address(Nprime.get_memory_address()->child[m].get_memory_address());

		                        Nprime.get_memory_address()->child.erase(Nprime.get_memory_address()->child.begin()+m);

		                        Nprime.get_memory_address()->value.erase(Nprime.get_memory_address()->value.begin()+m-1);

		                        N.get_memory_address()->child.insert(N.get_memory_address()->child.begin(),tmpP);

		                        N.get_memory_address()->value.insert(N.get_memory_address()->value.begin(),kprime);

		                        parN.get_memory_address()->value[i-1] = tmpV;//???
		                      // parN.get_memory_address()->value[i-1] = N.get_memory_address()->
		                    }else{
		                        int m = Nprime.get_memory_address()->value.size()-1;
		                        type_key tmpV = Nprime.get_memory_address()->value[m];
		                        vector<type_reg> vreg = Nprime.get_memory_address()->regs[m];
		                        ///Address<node> tmpP = Nprime.get_memory_address()->child[m];
		                        ///Nprime.get_memory_address()->child.erase(Nprime.get_memory_address()->child.begin()+m);
		                        Nprime.get_memory_address()->regs.erase(Nprime.get_memory_address()->regs.begin()+m);
		                        Nprime.get_memory_address()->value.erase(Nprime.get_memory_address()->value.begin()+m);
		                        ///N.get_memory_address()->child.insert(N.get_memory_address()->child.begin(),tmpP);
		                        N.get_memory_address()->regs.insert(N.get_memory_address()->regs.begin(),vreg);
		                        N.get_memory_address()->value.insert(N.get_memory_address()->value.begin(),tmpV);
		                        parN.get_memory_address()->value[i-1] = tmpV;
		                    }
		                }else{

		                    if(!N.get_memory_address()->leaf){
		                        int m = 0;
		                        type_key tmpV = Nprime.get_memory_address()->value[m];
		                        Address<node> tmpP;
		                        tmpP.set_memory_address(Nprime.get_memory_address()->child[m].get_memory_address());

		                        Nprime.get_memory_address()->child.erase(Nprime.get_memory_address()->child.begin()+m);
		                        Nprime.get_memory_address()->value.erase(Nprime.get_memory_address()->value.begin()+m);

		                        N.get_memory_address()->child.push_back(tmpP);
		                        N.get_memory_address()->value.push_back(kprime);

		                       // N.get_memory_address()->child.insert(N.get_memory_address()->child.begin(),tmpP);
		                        //N.get_memory_address()->value.insert(N.get_memory_address()->value.begin(),kprime);

		                        parN.get_memory_address()->value[i-1] = tmpV;
		                    }else{
		                        int m = 0;
		                        type_key tmpV = Nprime.get_memory_address()->value[m];
		                        vector<type_reg> vregg = Nprime.get_memory_address()->regs[m];
		                        ///Address<node> tmpP = Nprime.get_memory_address()->child[m];
		                        ///Nprime.get_memory_address()->child.erase(Nprime.get_memory_address()->child.begin()+m);
		                        Nprime.get_memory_address()->value.erase(Nprime.get_memory_address()->value.begin());
		                        Nprime.get_memory_address()->regs.erase(Nprime.get_memory_address()->regs.begin());
		                        ///N.get_memory_address()->child.insert(N.get_memory_address()->child.begin(),tmpP);
		                       // N.get_memory_address()->value.insert(N.get_memory_address()->value.begin(),tmpV);
		                        N.get_memory_address()->value.push_back(tmpV);
		                        N.get_memory_address()->regs.push_back(vregg);
		                        parN.get_memory_address()->value[i] = Nprime.get_memory_address()->value[0];
		                    }

		                }
		        }


		    }else{
		        printf("Enough Entry\n");
		    }

		}

		void delet(type_key k, Address<node>  p){
		    Address<node> L;
		    L.set_memory_address(getTargetNode(Root,k).get_memory_address());
		    printf("content: \n");
		    bfsTraverse(L);
		    deleteEntry(L,k,p); // key k esta en nodo L, p es el puntero de esa key

		}


		bool isCoalesce(Address<node> N, Address<node> Nprime)
		{
		    if(N.get_memory_address()->leaf){
		        if(nVal>=(N.get_memory_address()->value.size()+Nprime.get_memory_address()->value.size())) return true;
		        return false;
		    }
		    if(nPointer>=(N.get_memory_address()->child.size()+Nprime.get_memory_address()->child.size())) return true;
		    return false;
		}

		/*
		bool indexar_campo(Table tabla, string field_name)
		{
		    //leo el archivo relacionado con esta tabla y voy insertando en índice, bajo hash
		    //se hace flush al terminar, ojo flush solo escribe en disco el árbol, pero este sigue estando en memoria

		    string tabla_path = tabla.get_name()+extension_arch_tabla;
		    fstream(archivo_tabla);
		    archivo_tabla.open(tabla_path, fstream::in);
		    //cerr << tabla.get_fields()[2].get_name() << "\n";
		    vector<Field> v(tabla.get_fields());
		    unsigned int posicion_campo = distance(v.begin(), find(v.begin(), v.end(), field_name));

		    string linea, string_campo;
		    int begin_valor_campo, end_valor_campo;
		    int posicion_de_inicio_de_linea=0;
		    while(archivo_tabla >> linea){
		        int posicion_comas=0, find_;
		        for(int i_=0; i_<= posicion_campo-1; i_++){
		            find_ = linea.find(",",posicion_comas,1);
		            posicion_comas = find_+1;
		        }
		        //ubicado justo en el primer caracter del campo a leer
		        begin_valor_campo = posicion_comas;
		        end_valor_campo = linea.find(",",posicion_comas,1)-1;
		        string_campo =  linea.substr(begin_valor_campo, end_valor_campo-begin_valor_campo+1);
		        //aplicamos hash
		        size_t valor_hash = hash<string>{}(string_campo);
		        //nPointers = 200
		        //insertamos
		        type_reg reg(posicion_de_inicio_de_linea,NULL); //type_reg: Address<T>
		        insert(valor_hash,reg);
		        posicion_de_inicio_de_linea += linea.length() +1;   
		    }  

		    archivo_tabla.close();
		    return true;
		}
		*/

	};
}

#endif